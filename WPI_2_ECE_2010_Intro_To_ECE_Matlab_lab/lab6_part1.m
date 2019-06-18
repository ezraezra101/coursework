clear all
t = [0:1e-4:0.1];       %   time array, s
f = 60;                 %   frequency, Hz
Vin = 1*sin(2*pi*f*t) + 1.0*(rand(1, length(t))-0.5);   %   input voltage signal plus some noise
% Vin = 1*sin(2*pi*f*t);

Vout1 = zeros(1, length(t));         %   initialize output voltage for the simple zero-level-detector
Vout2 = zeros(1, length(t));         %   initialize output voltage for the Schmidt trigger.

F    = figure;                      %   initialize MATLAB figure                            


for m = 2:length(t)
    % Schmidt trigger
    if Vin(m-1)-Vout2(m-1)*1/11<0 
        Vout2(m) = +12.0;     %   power rail 
    else
        Vout2(m) = -12.0;     %   power rail 
    end

    % Simpler circuit
    if Vin(m-1)-0<0 
        Vout1(m) = +12.0;     %   power rail 
    else
        Vout(m) = -12.0;     %   power rail 
    end
    
    %   graphics
    subplot(2,1,1)

    plot(t, Vin, 'b', t, Vout1, 'r'); 
    axis([min(t) max(t) -14 +14]);    %   define axes
    grid on;
    
    title('Simple amplifier trigger with a noisy input');
    xlabel('Time (seconds)');
    ylabel('Voltage (in and out of the circuit)');
    
    subplot(2,1,2)
    plot(t, Vin, 'b', t, Vout2, 'r'); 
    axis([min(t) max(t) -14 +14]);    %   define axes
    
    title('Schmidt trigger trigger with a noisy input');
    xlabel('Time (seconds)');
    ylabel('Voltage (in and out of the circuit)');
    grid on;     
 
    drawnow 
end

