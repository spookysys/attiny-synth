

% ref. https://ccrma.stanford.edu/~jos/fp/Creating_Minimum_Phase_Filters.html
function [sm] = mps(s) 
   % [sm] = mps(s) 
   % create minimum-phase spectrum sm from complex spectrum s 
   sm = exp( fft( fold( ifft( log( clipdb(s,-100) )))));
end


% ref. https://ccrma.stanford.edu/~jos/fp/Creating_Minimum_Phase_Filters.html
function [clipped] = clipdb(s,cutoff)
    % [clipped] = clipdb(s,cutoff)
    % Clip magnitude of s at its maximum + cutoff in dB.
    % Example: clip(s,-100) makes sure the minimum magnitude
    % of s is not more than 100dB below its maximum magnitude.
    % If s is zero, nothing is done.

    clipped = s;
    as = abs(s);
    mas = max(as(:));
    if mas==0, return; end
    if cutoff >= 0, return; end
    thresh = mas*10^(cutoff/20); % db to linear
    toosmall = find(as < thresh);
    clipped = s;
    clipped(toosmall) = thresh;
end

% ref. https://ccrma.stanford.edu/~jos/fp/Creating_Minimum_Phase_Filters.html
function [rw] = fold(r) 
% [rw] = fold(r) 
% Fold left wing of vector in "FFT buffer format" 
% onto right wing 
% J.O. Smith, 1982-2002
  
   [m,n] = size(r);
   if m*n ~= m+n-1
     error('fold.m: input must be a vector'); 
   end
   flipped = 0;
   if (m > n)
     n = m;
     r = r.';
     flipped = 1;
   end
   if n < 3, rw = r; return; 
   elseif mod(n,2)==1, 
       nt = (n+1)/2; 
       rw = [ r(1), r(2:nt) + conj(r(n:-1:nt+1)), ...
             0*ones(1,n-nt) ]; 
   else 
       nt = n/2; 
       rf = [r(2:nt),0]; 
       rf = rf + conj(r(n:-1:nt+1)); 
       rw = [ r(1) , rf , 0*ones(1,n-nt-1) ]; 
   end; 

   if flipped
     rw = rw.';
   end
end

% ref . https://ccrma.stanford.edu/~jos/fp/Matlab_diary_tmps_d.html
function test_mps()
    spec = [1 1 1 0 0 0 1 1]'; % Lowpass cutting off at fs*3/8
    format short;
    mps(spec)
    abs(mps(spec))
    ifft(spec)
    ifft(mps(spec))
end
