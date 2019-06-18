clear all
t = [0:5e-4:0.1];     %
Vin     = zeros(size(t));    %   Initialize input voltage
Vout    = zeros(size(t));    %   Initialize output voltage
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
n = rand(size(t))-0.5;
a = 5;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Vin(1) = 12.0;       %   Write operation at the very first time moment:
                   %   raise input high (12 V) -  write  bit value 0 (out. inverted)
                   %   set input low (-12 V) -  write  bit value 1 (out. inverted)
F    = figure;     %   Initialize figure and observe Vin/Vout when time progresses
for m = 2:length(t)
    if Vin(m-1)-0.5*Vout(m-1)<a * n(m)
        Vout(m) = +5.0;     %   power rail
else
        Vout(m) = -5.0;     %   power rail
    end
% graphics
    plot(t(1:m), Vin(1:m), 'b', t(1:m), Vout(1:m), 'r');
    axis([min(t) max(t) -6 +6]);    %   define axes
    grid on;
    string1 = 'Blue - input voltage; red - output voltage; stored bit value =';
    string2 = num2str(Vout(m)==12);
    title(strcat(string1, string2));
drawnow
end