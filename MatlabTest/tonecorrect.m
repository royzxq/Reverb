function y = tonecorrect(x)
y = zeros(1,length(x));
a = 1.25/3;
g = (1-a)/(1+a);
% y(1) = x(1);
for i = 1:length(x)
    if i <= 1
        y(i) = x(i) / (1-g);
    else
        y(i) = (x(i) - g * x(i-1)) / (1-g);
    end
end
end