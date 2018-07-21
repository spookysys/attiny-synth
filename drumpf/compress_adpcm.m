



function [indices] = compress_adpcm(data_in)
    if (isempty(data_in))
        indices = [];
    else
        % output the thing
        disp('Compressing');
        [error, indices] = compress(data_in, 5);
        disp(['Final error: ', num2str(error)]);
        disp('Done');
    end
end



% Run a simplified (for now) compression and return data
function [avg_error, data_out] = compress(data_in, lookahead)
    data_out = zeros(length(data_in), 1);
    recon_1 = 0;
    recon_2 = 0;
    modifier = 0;
    total_error = 0;
    for i = 1:length(data_in)
        [local_error, ~, recon, index, modifier] = compress_step(data_in, i, modifier, recon_1, recon_2, lookahead);
        total_error = total_error + local_error;
        data_out(i) = index;
        recon_2 = recon_1;
        recon_1 = recon;
    end
    avg_error = total_error / length(data_in);
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

function [local_error, total_error, recon, index, modifier] = compress_step(data_in, pos, last_modifier, recon_1, recon_2, lookahead)
    val = round(data_in(pos));
    prediction = recon_1 + recon_1 - recon_2;
    local_error_opts = [0 0 0 0];
    total_error_opts = [0 0 0 0];
    recon_opts = [0 0 0 0];
    modifier_opts = [0 0 0 0];
    for index = 1:4
        modifier = wrap8(get_modifier(last_modifier, index));
        recon = wrap8(prediction + modifier);
        if lookahead > 0 && pos < length(data_in)
            [~, tail_error, ~, ~] = compress_step(data_in, pos+1, modifier, recon, recon_1, lookahead-1);
        else
            tail_error = 0;
        end
        local_error = abs(recon - val);
        local_error_opts(index) = local_error;
        total_error_opts(index) = tail_error + local_error;
        recon_opts(index) = recon;
        modifier_opts(index) = modifier;
    end
    [total_error, index] = min(total_error_opts);
    local_error = local_error_opts(index);
    modifier = modifier_opts(index);
    recon = recon_opts(index);
end


function [val] = wrap8(val)
    val = mod(val+128, 256) - 128;
end

