function y = lowpass(x,kp,bp)
    y = zeros(1,length(x));
%     y(1) = x(1);
    for i = 1:length(x)
        if i <= 1
            y(i) = (kp-kp*bp) * x(i);
        else
            y(i) = (kp-kp*bp) * x(i) + bp * y(i-1);
        end
    end
end