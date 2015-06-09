m = [919, 997, 1061, 1093, 1129, 1151, 1171, 1187, 1213, 1237, 1259, 1283, 1303, 1319, 1327, 1361];
fs = 44100;
for i = 1:16
    kp(i) = 10^(-1*m(i)/fs);  
    q = 1.25/3;
    bp(i) = 20* log10(kp(i))*log(10)/80*(1-1/(q*q));
    %bp(i) = 1/bp(i);
end



BufferSize = 1024;
Freq = 100;
DCInput = ones(1,BufferSize);

%DC just for the first lpfilter
f_dc = fopen('DC.txt','w');
y = lowpass(DCInput,kp(1),bp(1));
for i = 1:length(y)
    fprintf(f_dc,'%f ',y(i));
end
fprintf(f_dc,'\n');

%DC test for all pass 
y = allpass(DCInput,0.55,37);
for i = 1:length(y)
    fprintf(f_dc,'%f ',y(i));
end
fprintf(f_dc,'\n');

%DC test for tone correction 
y = tonecorrect(DCInput);
for i = 1:length(y)
    fprintf(f_dc,'%f ',y(i));
end
fclose(f_dc);

Impulse = zeros(1,BufferSize);
Impulse(1) = 1;
% calculate impulse response for lp

f_im_lp = fopen('lp_impulse.txt','w');
for i = 1:16
    y = lowpass(Impulse,kp(i),bp(i));
    for i = 1:length(y)
        fprintf(f_im_lp,'%f ',y(i));
    end
    if i ~= 16
        fprintf(f_im_lp,'\n');
    end
end

fclose(f_im_lp);

% calculate impulse response for ap

f_im_ap = fopen('ap_impulse.txt','w');
g = 0.55;

y = allpass(Impulse,g,37);
for i = 1:length(y)
    fprintf(f_im_ap,'%f ',y(i));
end
fprintf(f_im_ap,'\n');

y = allpass(Impulse,g,113);
for i = 1:length(y)
    fprintf(f_im_ap,'%f ',y(i));
end
fclose(f_im_ap);

% calculate impulse response for tc

f_im_tc = fopen('tc_impulse.txt','w');

y = tonecorrect(Impulse);
for i = 1:length(y)
    fprintf(f_im_tc,'%f ',y(i));
end

fclose(f_im_tc);












