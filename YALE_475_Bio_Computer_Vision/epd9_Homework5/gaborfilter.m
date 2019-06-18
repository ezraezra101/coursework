function gb = gaborfilter(theta, wavelength, phase, sigma, aspect, ksize)
%GABORFILTER Create a Gabor filter.
%   GB = GABORFILTER(THETA, WAVELENGTH, PHASE, SIGMA, ASPECT, KSIZE)
%   creates a Gabor filter GB with orientation THETA (in radians),
%   wavelength WAVELENGTH (in pixels), phase offset PHASE (in radians),
%   envelope standard deviation SIGMA, aspect ratio ASPECT, and dimensions
%   KSIZE x KSIZE. KSIZE is an optional parameter, and if omitted default
%   dimensions are selected.

if ~exist('ksize', 'var')
  ksize = 8*sigma*aspect;
end

if numel(ksize) == 1
  ksize = [ksize ksize];
end

xmax = floor(ksize(2)/2);
xmin = -xmax;
ymax = floor(ksize(1)/2);
ymin = -ymax;

[xs, ys] = meshgrid(xmin:xmax, ymax:-1:ymin);

% Your code here
% Following the formula in in lecture 11 (figure 17.1)
%x_hat = xs * cos(theta) + ys*sin(theta);
%y_hat = ys * cos(theta) - xs*sin(theta);
%gabor_even = exp(-(x_hat.^2 + aspect.^2*y_hat.^2)/(2*sigma.^2)) * cos(2 * pi * x_hat/wavelength + phase);

% The example from the homework handles y and x differently:
x_prime = xs * cos(theta) + ys*sin(theta); % Rotate x and y
y_prime = ys * cos(theta) - xs*sin(theta);
% Gabor filter is the multiplication of a gaussian (exp(...)) and a
% sinusoid.
gabor = sin(2*3.14159265358979*y_prime/wavelength+phase).* exp(-(x_prime.^2/aspect^2+y_prime.^2)/(2*sigma.^2));

gabor = gabor - mean(mean(gabor));
gabor = gabor/norm(gabor);
gb = gabor;