rng(1234);
block_size = 16;
adpcm_bits = 2;
[~,~,~] = mkdir('out_matlab');
[~,~,~] = mkdir('out_debug');
[~,~,~] = mkdir('export');

% This has a problem with bass-crop
files = dir('in/*.wav');
for file = files'
    filename = file.name;
    disp(['Processing ' filename]);
    process_file(['in/' filename], ['out_matlab/' filename], ['out_debug/' filename '/'], block_size, adpcm_bits);
end


function process_file(infile, outfile, checkdir, block_size, adpcm_bits)
    samples_per_byte = 8 / adpcm_bits;
    [~,~,~] = mkdir(checkdir);
    [data_in, data_in_norm, sample_rate] = load_input(infile);
    save_output([checkdir 'orig.wav'], data_in, sample_rate);
    
    if (contains(infile, 'amen', 'IgnoreCase', true)) % do not normalize the amen break beats    
        data_in_treb = data_in
        data_in_bass = mean([data_in, data_in_norm], 2);
    else
        data_in_treb = data_in_norm
        data_in_bass = data_in_norm
    end
    [bass] = analyze_bass(data_in_bass, sample_rate, block_size, samples_per_byte, checkdir);
    [treble_env_fit, treble_color_b, treble_color_a] = analyze_treble(data_in_treb, sample_rate, block_size, checkdir);

    orig_len = length(data_in);
    %short_len = max(ceil(treble_cut_point/block_size)*block_size, length(bass)*block_size);
    %long_len = orig_len + min(ceil(orig_len/2), ceil(sample_rate/2));
        
    % write stats
    fid = fopen([checkdir 'stats.txt'],'w');
    size1 = 4 + 7 + 2; % treble_env, treble_filter, bass_length
    size2 = length(bass) / samples_per_byte; % encoded bass size in bytes (one sample in this array is one block, and 2 bits per block)
    size = size1 + size2;
    fprintf(fid, [ 'Original length: ' num2str(orig_len) '\r\n']);
    %fprintf(fid, [ 'Treble length: ' num2str(treble_cut_point) '\r\n']);
    fprintf(fid, [ 'Bass length: ' num2str(length(bass)*block_size) '\r\n']);
    %fprintf(fid, [ 'Length: ' num2str(short_len) '\r\n']);
    %fprintf(fid, [ 'Elongated length: ' num2str(long_len) '\r\n']);
    fprintf(fid, [ 'Final Size: ' num2str(size1) ' + ' num2str(size2) ' = ' num2str(size) '\r\n']);
    fprintf(fid, [ 'Kbps: ' num2str(size*8 / (orig_len/sample_rate) / 1024) '\r\n']);
    fprintf(fid, [ 'Compression Ratio: ' num2str(orig_len/size) '\r\n']);
    fprintf(fid, [ 'Bits Per Sample: ' num2str(size*8 / orig_len) '\r\n']);
    fclose(fid);
    
    % Output debug stuff
    if (~isempty(bass))
        save_output([checkdir 'bass_cut.wav'], resample(bass, block_size, 1), sample_rate);
    end
    
    [bass_adpcm_data] = compress_adpcm(bass*128);
    clear volume_adj;
    
    % Reconstruct bass
    bass_recon = decompress_adpcm(bass_adpcm_data);
    if (~isempty(bass_recon))
        save_output([checkdir 'bass_recon.wav'], resample(bass_recon/128, block_size, 1), sample_rate);
        bass_recon_upsampled = resample(bass_recon, block_size, 1);
    else
        bass_recon_upsampled = [];
    end

%    figure();
%    subplot(4,1,1)
%    plot(bass);
%    subplot(4,1,2)
%    plot(bass_norm);
%    subplot(4,1,3)
%    plot(bass_norm_recon);
%    subplot(4,1,4)
%    plot(bass_recon);
    
    % Make short mix
    treble_recon = reconstruct_treble(orig_len, treble_env_fit, treble_color_b, treble_color_a, checkdir, sample_rate);
    mix = pad_to(bass_recon_upsampled, orig_len, 0) + treble_recon;
    save_output([checkdir 'treble_recon.wav'], treble_recon / 128, sample_rate);
    save_output([checkdir 'result.wav'], mix / 128, sample_rate);

    % This is the final mix
    save_output(outfile, mix / 128, sample_rate);
    
    % Export to C
    export(infile, bass_adpcm_data, treble_color_b, treble_color_a, treble_env_fit, block_size);
end


% Load wav-file
function [data, data_normalized, sample_rate] = load_input(filename)
	[data, sample_rate] = audioread(filename);
    data = sum(data, 2); % combine channels
    scaler = max(abs(data)); % normalize
    data_normalized = data / scaler; % normalize
end


% Save wav-file
function save_output(filename, data, sample_rate)
    audiowrite(filename, data, sample_rate);
end

% Prepare data for compression
function [env_fit, color_b, color_a] = analyze_treble(data_in, sample_rate, block_size, checkdir)
    smooth_window = 50;
    nyquist = sample_rate/2.0;
	separation_freq = nyquist/block_size/2.0;

    % Separate treble
    data = pad(data_in, block_size);
	[sep_b, sep_a] = butter(6, separation_freq / nyquist, 'high');
	treble = filter(sep_b, sep_a, data);
    save_output([checkdir 'treble.wav'], treble, sample_rate);
 
	% Find volume envelopes
    env = get_env(treble);
    env_smooth = smooth(env, smooth_window); 

    % clear samples after volume drops too low
    limit = 0.1 / 2^8; % 0.1 is good value
    cut_point = 0;
    for i = 1:length(treble)
        vol_real = env_smooth(i);
        if (abs(vol_real) > limit)
            cut_point = i;
        end
    end
    treble = treble(1:cut_point);      
    env_smooth = env_smooth(1:cut_point);
    env = env(1:cut_point);
    save_output([checkdir 'treble_cut.wav'], treble, sample_rate);
    
    % fit envelope
    env_fit = fit((0:length(treble)-1)', env, 'exp1');
    
    % Extract treble color
    num_freqs = 2^18;
    color_h = fft(treble, num_freqs);
    color_h = mps(color_h);
    color_h = color_h(1:num_freqs/2+1);
    color_w = (0:(num_freqs/2)) * (2*pi)/num_freqs;
    [color_b, color_a] = invfreqz(color_h, color_w, 2, 2, [], 100, 0.01);
    %figure; zplane(color_b, color_a);
    
    % Adjust filter coeffs so output is normalized to int8 range
    white = rand(60*44100, 1) * 256 - 128;
    colored = filter(color_b, color_a, white);
    color_b = color_b * 128 / max(abs(colored));
    white = rand(100*44100, 1) * 256 - 128; % debug check
    colored = filter(color_b, color_a, white); % debug check
    
    % Adjust envelope so volume is correct in the end
    treble_norm = treble ./ env_smooth;
    volume_adj = (128 / mean(abs(hilbert(colored)))) * mean(abs(hilbert(treble_norm)));
    env_fit.a = env_fit.a * volume_adj;
end

%function ret = my_fitoptions()
    %ac_limit = 2;
    %ret = fitoptions('exp1','upper',[ac_limit 0],'lower',[-ac_limit -Inf]);
%end

function [env] = get_env(data_in)
    padding = 1000;
    tmp = [zeros(padding, 1); data_in; zeros(padding, 1)];
    tmp = abs(hilbert(tmp));
    env = tmp(1+padding:end-padding);
    assert(length(env)==length(data_in));
end

% Prepare data for compression
function [bass] = analyze_bass(data_in, sample_rate, block_size, samples_per_byte, checkdir)
    smooth_window = 50;
    
	% extract and resample bass
    data = pad(data_in, block_size);
    bass = resample(data, 1, block_size);
    save_output([checkdir 'bass.wav'], resample(bass, block_size, 1), sample_rate);
    
    % calculate and fit volume envelope
    env = get_env(bass);
    env_smooth = smooth(env, smooth_window);
    clear env;
    
    % clear samples after volume drops too low
    limit = 10. / 2^8; % 10 is good value
    cut_point = 0;
    for i = 1:length(bass)
        vol_real = env_smooth(i);
        if (abs(vol_real) > limit)
            cut_point = i;
        end
    end
    bass = bass(1:cut_point);
    bass = pad(bass, samples_per_byte);
end




function [new_modifier] = get_modifier(last_modifier, index)
    if index==1
        new_modifier = last_modifier;
    elseif index==2
        new_modifier = -last_modifier;
    elseif index==3
        if last_modifier==0
            new_modifier = 1;
        else
            new_modifier = last_modifier * 2;
        end
    elseif index==4
        if last_modifier==0
            new_modifier = -1;
        else
            new_modifier = round(last_modifier / 2);
        end
    else
        assert(false);
    end
end


function data_out = decompress_adpcm(data_in)
    data_out = zeros(length(data_in), 1);
    recon_1 = 0;
    recon_2 = 0;
    modifier = 0;
    for i = 1:length(data_in)
        index = data_in(i);
        prediction = recon_1 + recon_1 - recon_2;
        modifier = wrap8(get_modifier(modifier, index));
        recon = wrap8(prediction + modifier);
        data_out(i) = recon;
        recon_2 = recon_1;
        recon_1 = recon;
    end
end


function data_out = reconstruct_treble(num_samples, env_fit, color_b, color_a, checkdir, sample_rate)
    white = rand(num_samples, 1) * 256 - 128;
    colored = filter(color_b, color_a, white);
    save_output([checkdir 'treble_colored.wav'], colored / 128, sample_rate);
    env_fit_x = (0:num_samples-1)';
    env_values = feval(env_fit, env_fit_x);
    %env_values = max(0, min(1, env_values));
    firstIndex = find(env_values<0, 1);
    env_values(firstIndex:end) = 0;
    data_out = colored .* env_values;
end


function data = pad(data_in, block_size)
    num = ceil(length(data_in) / block_size) * block_size - length(data_in);
    padding = zeros(num, 1);
    data = [data_in; padding];
end

function data = pad_to(data_in, len, val)
    if (len > length(data_in))
        if (val == 'clamp') 
            val = data_in(end);
        end
        padding = ones(len - length(data_in), 1) .* val;
        data = [data_in; padding];
    else
        data = data_in(1:len);
    end
end

function [data_out, volume_adj] = normalize8(data_in)
    bitdepth = 8;
    scale = 2^(bitdepth-1);
    minval = -scale;
    maxval =  scale-1;
    volume_adj = [minval-.49 maxval+.49] ./ [min(data_in) max(data_in)]; 
    volume_adj = min(volume_adj(volume_adj > 0));    
    data_out = data_in .* volume_adj;
end


function [val] = wrap8(val)
    val = mod(val+128, 256) - 128;
end

