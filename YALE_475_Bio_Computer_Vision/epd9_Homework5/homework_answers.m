%% 1a Edge detection in 1D
% The derivative is highest at it's peak at the center of the 'edge' and lowest far from the
% edge, allowing this to act as an edge detector.
n = 100;
x=linspace(-5, 5, n);
sigmoid = sigmf(x, [2 0]);
grad = gradient(sigmoid);
plot(sigmoid)
hold on;
plot(grad)
hold off;

%% 1b
peaks = (circshift(grad,-1) <= grad) & (circshift(grad,1) < grad);
peaks(end) = 0;
peaks(1) = 0;
plot(grad)
hold on;
plot(peaks/10)
hold off;
%% 1c
xs = 2:n-1;
xpeaks = grad(1,xs-1) <= grad(1,xs) & grad(1,xs) > grad(1,xs+1);
isequal(peaks, [0,xpeaks,0])

%% 1d
% The zero value of the 2nd derivative indicates a local minimum/maximum in
% the 1st derivative, which is what we were trying to find in 1b and 1c.
g2 = gradient(grad);
plot(grad)
hold on;
plot(g2)
hold off;

%% 1e
% The new method provides the same result as from 1b. It indicates that the
% next value will contain the crossing (or simply reaching) zero in the 2nd derivative.

% current != 0 and either (next==0) or (next's sign != current's sign).
peaks2 = (g2 ~= 0) & (circshift(g2, -1) == 0) | ((circshift(g2,-1) > 0) ~= (g2 > 0));
peaks2(end) = 0;
peaks2(1) = 0;
plot(g2)
hold on;
plot(peaks2/100)
hold off;
isequal(peaks,peaks2)

%% 2a Edge detection in 2D
% These images convey an approximation of the edges in the x and y
% directions, respectively. Dark (negative) values represent a light->dark
% transition in the original image.
% Each image is related to the gradient in 1a, and should be (nearly)
% equivalent to twice the gradient in 1a for each row/column plus the gradient
% of the previous and next row/columns.
s_x = [-1,0,1;-2,0,2;-1,0,1]/8;
s_y = s_x';

paolina = im2double(imread('Paolina.tiff'));
subplot(1,2,1)
% Adding 0.5 to move range from -0.5:0.5 to 0:1.
d_x = conv2(paolina,s_x,'same');
d_y = conv2(paolina,s_y,'same');
imshow(0.5+d_x)
subplot(1,2,2)
imshow(0.5+d_y)

%% 2b
figure()
d=(d_x.^2 + d_y.^2).^0.5;
d = d >= mean(mean(d)); % Honestly, it was cleaner before this step...
% d = d/mean(mean(d))/2 % I'd probably do this instead, though I guess it isn't binary
imshow(d)

%% 2c
% This step thins the edges to be one pixel wide, which is vital for any
% sort of processing on them. Programmatically, it is just the union of the
% result of applying 1c to the image row by row, then column by column, and
% then getting the intersection of their union and the original edges.
% Processing on edges (e.g. object segmentation) works best when actually
% using single pixel edges instead of areas where it's unclear if it is
% in or out of the object.
im = d;
[m, n] = size(im);
xs = 2:n-1;
ys = 2:m-1;
xpeaks = im(ys,xs-1) <= im(ys,xs) & im(ys,xs) > im(ys,xs+1);
ypeaks = im(ys-1,xs) <= im(ys,xs) & im(ys,xs) > im(ys+1,xs);
im(ys,xs)= im(ys,xs) & ( xpeaks | ypeaks );
imshow(im);

%% 2d
% The logical 1 values indicate edges.
% This method gives a more contiguous line around the object, which makes
% it far more useful than using the Sobel detector.
% The gaussian removes the highest frequency noise in the image before the
% laplacian starts looking for high frequency changes to label as edges.

laplacian = edge(d, 'log');
imshow(laplacian);


%% 3a
% Very low thresholds give excellent results on this image.
circles = im2double(imread('Circles.tiff'));
subplot(2,1,1);
imshow(edge(circles,'sobel', 0.0001));
subplot(2,1,2);
imshow(edge(circles,'canny',0.0001));



%% 3b
% The noisy version of the circles requires much larger thresholds, and the
% sobel filter cannot detect any but the outermost circle no matter the
% threshold.
circles_noisy = imnoise(circles,'gaussian');
circles = im2double(imread('Circles.tiff'));
subplot(2,1,1);
imshow(edge(circles_noisy,'sobel', 0.18));
subplot(2,1,2);
imshow(edge(circles_noisy,'canny',0.25));

%% 3c
sigma = 4;
filtersize = 6*sigma;
blurred = imfilter(circles_noisy, fspecial('gaussian', filtersize, sigma), 'replicate');
subplot(2,1,1);
imshow(edge(blurred,'sobel', 0.005));
subplot(2,1,2);
sigma = 3;
filtersize = 6*sigma;
blurred = imfilter(circles_noisy, fspecial('gaussian', filtersize, sigma), 'replicate');
imshow(edge(blurred,'canny',0.19));


%% 4a Oriented Gabor Filters
%% 4a
% The bright line enhancer is the negative version of the dark line enhancer (e.g.
% bright + dark = 0), so by convolving by one of these, I can invert the
% result to get the result of the other. Similarly, for the edge enhancer,
% adding pi to theta reverses the sign.

% Some of the code is in `gaborfilter.m`.
sigma = 2; % Sigma 10 doesn't make interesting pictures....
aspect = 2;
wavelength =4*sigma;
ksize = 8*sigma*aspect;

%%
% Edge enhancers
figure()
phase = 0;
for i =1:4
    theta = i*3.1415926535/4; % Writing out pi seems to give better results
    gb = gaborfilter(theta, wavelength,phase,sigma,aspect);
    subplot(2,8,i);
    imshow(0.5+(gb)*0.5/max(max(abs(gb)))); % Who needs imagesc? Apparently not me... (I scaled it myself)
    subplot(2,8,i+4);
    imshow(0.5+(-gb)*0.5/max(max(abs(gb))));
end

for i =1:4
    theta = i*3.1415926535/4;
    gb = gaborfilter(theta, wavelength,phase,sigma,aspect);
    filtered = filter2(gb, paolina);
    subplot(2,8,8+i); imshow(0.5+filtered/2);
    subplot(2,8,8+i+4); imshow(0.5-filtered/2);
end
%%
% Bright/dark line enhancers
phase = 3.1415926/2;
for i =1:4
    theta = i*3.1415926535/4;
    gb = gaborfilter(theta, wavelength,phase,sigma,aspect);
    subplot(2,8,i);
    imshow(0.5+(gb)*0.5/max(max(abs(gb))));
    subplot(2,8,i+4);
    imshow(0.5+(-gb)*0.5/max(max(abs(gb))));
end

for i =1:4
    theta = i*3.1415926535/4;
    gb = gaborfilter(theta, wavelength,phase,sigma,aspect);
    filtered = filter2(gb, paolina);
    subplot(2,8,8+i); imshow(0.5+filtered/2);
    subplot(2,8,8+i+4); imshow(0.5-filtered/2);
end

%% 4b
figure()
theta = 5*3.1415926535/4;
phase=0*3.1415926535;
subplot(1,3,1);
imshow(0.5+filter2(gaborfilter(theta, wavelength,phase,sigma,aspect), paolina)/2);
title("Edge enhancement filter");

phase=0.5*3.1415926535;
subplot(1,3,2);
imshow(0.5+filter2(gaborfilter(theta, wavelength,phase,sigma,aspect), paolina)/2);
title("Bright line enhancement filter");

phase=1.5*3.1415926535;
subplot(1,3,3);
imshow(0.5+filter2(gaborfilter(theta, wavelength,phase,sigma,aspect), paolina)/2);
title("Dark line enhancement filter");

%%
figure()
theta = 5*3.1415926535/4;
phase=0*3.1415926535;
f1 = filter2(gaborfilter(theta, wavelength,phase,sigma,aspect), paolina);
theta = 3*3.14159265/4;
f2 = filter2(gaborfilter(theta, wavelength,phase,sigma,aspect),paolina);

imshow(0.5+(f1+f2)/4);
title("Average of edge enhancement filters");

%% 4c
% Gabor filters are similar to the 'simple' cells in the visual cortex
% - the receptive fields of the simple cells are (roughly) the same shape
% as gabor filters, to the point where there are even (plus or minus edge
% detectors) or odd (edge detectors), just like in these gabor filters.
% Multiple orientations of edges (instead of simply line detection) gives
% some information about the orientation of the object or texture - if
% there are a lot of lines going in the same direction, it tells you a lot
% about the material (e.g. is it straight or curly hair) which helps in object recognition.
%% 4d
% The gabor filter in the frequency domain is the convolution of its
% gaussian and sine waves (once converted into their own frequency domain
% representations).
% This means that it should look like two gaussians (gaussian's frequency
% domain equivalent is a gaussian, and the sine wave's frequency domain
% version is two delta functions, and the convolution of the two is two
% gaussians).
meshc(abs(fftshift(fft2(gaborfilter(theta, wavelength,phase,4,aspect)))));

%% Here is the problem more exhaustively demonstrated in 1D: (feel free to skip this part)

x = -10:.1:10;
norm = normpdf(x,0,2);
sine = sin(10*x);
gabor = norm.*sine; % 1D gabor filter

plot(gabor)
%%
% Ploting frequency domain versions of the functions that go into the gabor
% filter.
sine_f = fft(sine);
sine_f = sine_f/max(abs(sine_f));
norm_f = fft(norm);
plot(abs(fftshift(norm_f)));
hold on;
plot(abs(fftshift(sine_f)));
hold off;

%%
% Now showing that the convolution of the sine wave and gaussian in the
% frequency domain can be converted back into (nearly) the original gabor
% filter.
norm_f_convolved_with_sine_f = fftshift(conv((fftshift(norm_f)),(fftshift(sine_f)), 'same'));
plot(-real(ifft(norm_f_convolved_with_sine_f)));
hold on; plot(gabor); hold off;

%% 5 Stereo

leftscene = im2double(imread('Pentagon-Left.tiff'));
rightscene = im2double(imread('Pentagon-Right.tiff'));
halfpatchsize = 5;
maxdisp = 5;
disp = disparity(leftscene,rightscene, halfpatchsize, maxdisp);
%disp = abs(disp); % This helped for Scene-*.tif...

img = leftscene(halfpatchsize:end-halfpatchsize, halfpatchsize+maxdisp:end-(halfpatchsize+maxdisp));
disp_norm = (disp-min(min(disp))) / (max(max(disp))-min(min(disp)));
hsv = ones([size(disp),3]);
hsv(:,:,3) = disp_norm;
hsv(:,:,2) = img/10 + 0.1;
subplot(1,3,1);
imshow(hsv2rgb(hsv)); title("Displacement over hue from original image");
subplot(1,3,2);imshow(img); title("Original image (cropped)")
subplot(1,3,3);imagesc(disp); title("Displacement");

%%
figure()
loki = im2double(imread('Loki.tiff'));
halfpatchsize = 5;
maxdisp = 10;
disp = disparity(loki,loki, halfpatchsize, maxdisp);
imagesc(disp);
title("Making sure a single image doesn't have any disparity");


%%
% Both the pentagon and the scene have roughly correct planes - though I
% needed to adjust the patch size up to get Scene to have good (and
% smoother) segmentation.
leftscene = im2double(imread('Pentagon-Left.tiff'));
rightscene = im2double(imread('Pentagon-Right.tiff'));
disp = disparity(leftscene,rightscene, halfpatchsize, maxdisp);
imwrite(disp, 'Pentagon-disparity.png');


leftscene = im2double(imread('Scene-Left.tiff'));
rightscene = im2double(imread('Scene-Right.tiff'));
disp = disparity(leftscene,rightscene, 10, 10);
imwrite(disp, gray(20), 'Scene-disparity.png');
imagesc(disp);
