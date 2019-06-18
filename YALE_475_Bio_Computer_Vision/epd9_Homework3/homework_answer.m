%% Homework 3
% Ezra Davis (epd9)
%% 1 Linear Independence and basis
%% 1a
% $\alpha \vec u + \beta \vec v = [6.5, 0.5]^T$ is a linear combination of
% $\vec u$ and $\vec b$ by definition because it is the sum of scalar
% multiplications of $\vec u$ and $\vec b$.
alpha = 2;
beta = 1.5;
u = [1, 1].';
v = [3, -1].';
alpha * u + beta * v

%% 1b
% $\alpha' = 7, \beta' = -1$.
alpha = 7;
beta = -1;
alpha * u + beta * v

%% 1c
M = [u,v];
M_inverse = inv(M);
M_inverse * [4, 8].'

%% 1d
% They are independent - $\vec x$ is not a multiple of $\vec y$, they have
% the same 1st value, but different 2nd value, so they must be linearly
% independent.
%% 1e
% $\vec x$ and $\vec y$ *cannot* form a basis for $\mathbb{R}^3$ because
% two vectors can at most define a plane.
% Incidently, $[1, 0, 2]^T$ cannot be a multiple of those two vectors
% because, even leaving out the 2nd value, $\alpha [1,-1] + \beta [1, -1] \neq [1, 2]$
x = [1,1,-1].';
y = [1,-1,-1].';

%% 1f
% Those vectors are all linearly independent and thus cover $\mathbb{R}^3$
% Despite this, they are not orthogonal.
%% 1g
% The identity matrix's columns is the *classic* orthogonal basis.
% All of them are independent (so it's a basis), and all of them are
% orthogonal (easily shown).
% If $[\vec i, \vec j, \vec k] = I_3$, then $[2,1,7]^T = 2\vec i + 1 \vec j + 7 \vec k$

%% 1h
% They form an orthogonal basis, and we can normalize them to be
% $[1/\sqrt(2), 1/\sqrt(2)]^T$ and $[1/\sqrt(2), -1/\sqrt(2)]^T$
%% 1i
% $[\alpha, \beta]^T = [1, -2]^T$ (which I can solve for in my head.
%% 2 Intro to Fourier series
%% 2a
t = linspace(0, 2*pi);
y1 = sin(t);
plot(t, y1);
%% 2b
% Oddly, I didn't expect the multiplied values to look so similar.
% Both are mirrored around $\pi$.
figure();
hold on;
y2 = sin(2*t);
plot(t, y1);
plot(t, y2);
plot(t, y1.*y2);

figure();
hold on;
plot(t, y1);
plot(t, sin(3*t));
plot(t, y1.*sin(3*t));
hold off;
%% 2c
% The dot product is almost 0, so they are orthogonal.
% I'd consider y1 and y2 to be vectors primarily that approximate the sin
% function (though mathematically it's easy enough to convert a vector into
% a function with a very small domain).
dot(y1, y2)
%% 2d
% These are also orthogonal.
dot(cos(t), y1)
dot(sin(3*t), y1)
dot(sin(3*t), y2)
%% 2e
% The locations of the peaks in the FFT correspond to the frequencies of
% the cosines, and the height of the peaks to roughly 5 fimes the
% amplitude.
figure();
w = 3*cos(5*t) + 7*cos(15*t)+1.2*cos(30*t);
f = -50:49; % t has length 100
W = abs(fftshift(fft(w)));
plot(f, W);
%% 2f
% The sawtooth isn't very periodic, having peaks on odd frequencies,
% decreasing exponentially from frequency=1.
figure()
w = sawtooth(t, 0.5);
W = abs(fftshift(fft(w)));
plot(f, W);
%% 2g
% This is smooth - it's shaped much like the previous sawtooth(t, 0.5), but
% has non-zero values for even frequencies.
figure();
w = sawtooth(t);
W = abs(fftshift(fft(w)));
% plot(w); % Looks like a straight line except has a value of 0 at the last
% index
plot(f, W);
%% 3 Discrete convolution in 1-D
%% 3a
figure();
squares_convolved = [0,0,0,1,2,3,4,5,4,3,2,1,0,0,0] * 4;
stem(-7:7, squares_convolved);
axis equal;
grid on;
%%
bumps_convolved = [1,3,5,7,12,14,11,4,1,0,0,0,0];
stem(-6:6, bumps_convolved);
axis equal;
grid on;
%%
spikes_convolved = [0,0,4,7,5,3,1,4,7,5,3,1,0,0,0,0];
stem(spikes_convolved);
axis equal;
grid on;

%% 3b
% $[0,0,-1,-1,5,-1,-1,0,0]$ is a pretty good lateral inhibitory network for
% limulus.
%% 4 Fourier Transform and convolutions in 2-D
%% 4a
% $$ \cos(\theta) = 0.5 * (e^{i\theta} + e^{-i\theta}) $$
%
% $$ = 0.5 * (\cos(\theta) + i\sin(\theta) + (\cos(-\theta) + i\sin(-\theta))) $$
%
% $$ = 0.5 * (2\cos(\theta) + i\sin(\theta) - i\sin(-\theta)) $$
%
% Because $\sin(-\theta) = -\sin(\theta)$:
%
% $$ = \cos(\theta) $$
%
%
% $$ \sin(\theta) = 0.5i * (e^{i\theta} - e^{-i\theta}) $$
%
% $$ = 0.5i * (\cos(\theta) + i\sin(\theta) - (\cos(-\theta) + i\sin(-\theta))) $$
%
% $$ = 0.5i * (\cos(\theta) - \cos(-\theta) + i\sin(\theta) - i\sin(-\theta)) $$
%
% $$ = 0.5i * 2i \sin (\theta) $$
%
% $$ \sin(\theta)$$

%% 4b

axis auto
x = 0:0.5:10;
f = x.*(10-x);
g = x.*(10-x).*(5-x);

fft_1 = fft(conv(f, g), 50);
plot( 0:49, real(fft_1), 0:49, imag(fft_1));

%%

fft_2 = fft(f, 50).* fft(g, 50);
plot(0:49, real(fft_2),0:49, imag(fft_2));

%% 5
%% 5a
% Code in blurfilter.m
%% 5b
% This is another gaussian function!
filt = blurfilter(1, 20);
% surf(filt); % <- a gaussian
% figure()

surf(abs(fftshift(fft2(filt, 100, 100))), 'EdgeColor','none');


%% 5c
% The inverse fft of the pointwise products of the fft are almost identical
% (there's a little offset) to the convolution.
paolina = imread('Paolina.tiff');
paolina_fft = fft2(paolina);
surf(abs(paolina_fft), 'EdgeColor','none');

imagesc(ifft2(fft2(filt, 480, 512).*paolina_fft));
figure;
imagesc(filter2(filt,paolina))

%% 5d
% This filter highlights edges!
% This could be considered a band pass because it permits frequencies that
% are roughly a certain distance from the origin in frequency space.
filt_d = blurfilter(2, 13)- blurfilter(1, 13);
filt_d = filt_d-mean(mean(filt_d)); % which is tiny... maybe we should scale it instead?

loki = imread('Loki.tiff');
steve = imread('Steve.tiff');
imshow([filter2(filt_d, loki),filter2(filt_d, steve)],[]);
figure()
surf(abs(fftshift(fft2(filt_d))), 'EdgeColor','none');


%% 5e
% This appears to apply a 'glow' effect to objects in the scene by making
% pixels less like their neighbors (with influence of a gaussian function).
figure();
imshow(unsharpmask(paolina, 5, 5));
figure();
imshow(unsharpmask(loki, 5, 5));
figure();
imshow(unsharpmask(steve, 5, 5));

%% 5f

sigma = 5;
amount = 5;
n = 6*sigma + 1;
img = im2double(loki);
filter = blurfilter(sigma, n);

% new stuff:
c = n/2 + 0.5; % center 'pixel' in filter
filter = -amount * filter; % -amount * blurred
filter(c,c) = filter(c,c) + amount + 1; % amount*mask+img = -amount*blurred + amount*img + img

filtered = filter2(filter, img);
imshow(filtered);