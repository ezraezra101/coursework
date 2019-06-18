function features = findFeatures(im)

% Make Gaussian pyramid
g1 = gaussianPyramid(im);
corners = cell(numel(g1),1);
for i = 1:numel(g1)
    % (Harris) corner detection
    corners{i} = harrisCornerDetector(g1{i});
end

% I probably shouldn't do this yet...
cornerMap = collapseLaplacianPyramid(corners);

% It looks really pretty at this point, so I print it out...
imagesc(cornerMap); 

% Eliminate close weak corners
% I'm not following the methods described in the paper, but I'm still doing
% some kind of non-maximal suppression.
% http://inst.eecs.berkeley.edu/~cs194-26/fa17/Papers/MOPS.pdf
mask = fspecial('Gaussian', 10, 1)-fspecial('Gaussian', 10, 2);
cornerMap = conv2(cornerMap, mask);

% imagesc(cornerMap);

features = cornerMap;
end

function [im1_pts, im2_pts] = findCorrespondences(im1, im2)
% This function is incomplete!


% Make Gaussian pyramid
% (Harris) corner detection
% Eliminate close weak corners
% get SIFT features
f1 = findFeatures(double(sum(im1(:,:,1:3), 3)));
f2 = findFeatures(double(sum(im2(:,:,1:3), 3)));

% Feature matching using RANSAC & geometric constraints
% - Fast Approximate Nearest Neighbours using Wavelet Indexing

% <code goes here!>

end


function G = gaussianPyramid(im)
G = cell(ceil(log2(size(im,1)))+1,1);
G{1} = im;
for i = 2:numel(G)
G{i} = impyramid(G{i-1}, 'reduce');
end
end

function L = laplacianPyramid(im)
% https://www.mathworks.com/matlabcentral/fileexchange/30790-image-pyramid-gaussian-and-laplacian-
% Was incredibly helpful
% I also read a few other resources, including
% https://en.wikipedia.org/wiki/Pyramid_(image_processing)#Laplacian_pyramid
% and http://graphics.cs.cmu.edu/courses/15-463/2005_fall/www/Lectures/Pyramids.pdf
G = gaussianPyramid(im);
L = cell(numel(G),1);
L{end} = G{end};
for i = 1:numel(L)-1
    % I use imresize because impyramid's expand function is inconveniently implemented :-(
    s = size(G{end-i});
    L{end-i} = G{end-i}-imresize(G{end-i+1}, s(1:2));
end
end

function img = collapseLaplacianPyramid(L)
img = zeros(size(L{1}));
for i=1:numel(L)
   img = img + imresize(L{i},[size(img,1),size(img,2)]);
end
end



function corners = harrisCornerDetector(im)
% There is a harris corner detection function in the CV toolkit in matlab,
% but I'm implementing my own simple version.
dx = conv2(im,[-1,0,1], 'same');
dy = conv2(im,[-1,0,1].', 'same');

windowingKernel = ones(3);
xx = conv2(dx.*dx, windowingKernel,'same');
yy = conv2(dy.*dy, windowingKernel,'same');
xy = conv2(dx.*dy, windowingKernel,'same');

k = 0.04; % Empirically determined
trace = xx + yy;
determinant = xx.*yy + xy.*xy;
detected = determinant - (k*trace);
corners = detected;
end