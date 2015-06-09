function y = allpass(x,g,N)
y = zeros(1,length(x));
for i = 1:length(x)
    if i <= N
        y(i) = -g * x(i);
    else
        y(i) = -g * x(i) + x(i-N) + g*y(i-N);
    end
end

end