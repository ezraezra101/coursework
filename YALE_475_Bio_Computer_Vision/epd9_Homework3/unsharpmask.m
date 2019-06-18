function filtered = unsharpmask(img, sigma, amount)
%UNSHARPMASK Perform unsharp masking on an image.
%   FILTERED = UNSHARPMASK(IMG, SIGMA, AMOUNT) performs unsharp masking on
%   the image IMG, using a blur parameter SIGMA and a strength AMOUNT.
%   Unsharp masking consists of:
%   
%     1. Blurring the input image;
%     2. Forming the "mask" by substracting the blurred image from the
%        input image;
%     3. Adding the mask, scaled by some amount, back to the input image.
%
%   The output of this function is the filtered image FILTERED.

% Your code here

% step 1:
img = im2double(img);
blurred = filter2(blurfilter(sigma), img);
% step 2:
mask = img + -1* blurred;
% step 3:
filtered = img + amount * mask;