function [imwarped, xoffset, yoffset] = warpImage(im, H)
% Given a 2D homogenous transformation matrix, convert im into imwarped.

if(size(im,3) == 3)
    im(:,:,4) = ones(size(im(:,:,1)));
end

% The general process of this is:
% 1. Determine bounds of warped image
% 2. List indices of final warped image
% 3. List warped indices of `im`
% 4. For each pixel of the final image, interpolate its values of `im`
%    according to the indices we've created

% Figure out the size (and indices) of the image we're creating
x=size(im,2)-1; % Dumb column major ordering...
y=size(im,1)-1;
bounds = H*[0,0,1; 0,y,1; x,0,1; x,y,1].';
bounds=bounds./bounds(3,:); % Get rid of those pesky homogeneous coordinates.
minBounds = floor(min(bounds.'));
maxBounds = ceil(max(bounds.'));

xoffset = minBounds(1);
yoffset = minBounds(2);

[warpedX,warpedY] = meshgrid(minBounds(1):1:maxBounds(1), minBounds(2):1:maxBounds(2));

% Create XY coordinates for im
[X,Y] = meshgrid(0:1:size(im(:,:,1),2)-1,0:1:size(im(:,:,1),1)-1);

% Map our final warped image into coordinates that match X and Y.
Hi = inv(H); % Now im2 = Hi*im1. Hopefully H is invertible...
ws = Hi(3,1)*warpedX+Hi(3,2)*warpedY + Hi(3,3);
xs = (Hi(1,1)*warpedX+Hi(1,2)*warpedY + Hi(1,3))./ws;
ys = (Hi(2,1)*warpedX+Hi(2,2)*warpedY + Hi(2,3))./ws;

im = double(im);
r = interp2(X,Y,im(:,:,1),xs,ys, 'cubic', 0);
g = interp2(X,Y,im(:,:,2),xs,ys, 'cubic', 0);
b = interp2(X,Y,im(:,:,3),xs,ys, 'cubic', 0);
a = interp2(X,Y,im(:,:,4),xs,ys, 'cubic', 0);

imwarped = cat(3,r,g,b,a);
% imshow(imwarped(:,:,1:3)/255);
end