function img = projectImage(im, dist)
% Projects image into cylindrical coordinates.
% Assuming that the images for the mosaic are taken from the same camera.

im = double(im);

if(size(im,3) == 3)
    im = cat(3, im, ones(size(im(:,:,1))));% add alpha channel
end

imsize = size(im);

[X,Y] = meshgrid(0:imsize(2)-1, 0:imsize(1)-1);

thetaRange = mapCylindrical(imsize(2),dist); % Technically should go from -theta/2 to theta/2
thetaPerPixel = thetaRange/imsize(2);
[x,y] = meshgrid(0:imsize(2)-1, 0:imsize(1)-1);

x = mapFromCylindrical(x*thetaPerPixel, dist);

img = zeros([size(x,1),size(x,2),4]);
for i=1:4
    img(:,:,i) = interp2(X,Y,im(:,:,i),x,y,'cubic',0);
end

end

% Take a point on a plane and map it into spherical/cylindrical coordinates
function theta = mapCylindrical(x,dist)
    theta = asin(x/dist);
end
function x = mapFromCylindrical(theta,dist)
    x = sin(theta)*dist;
end

function [theta,phi] = mapSpherical(x,y,dist)
    theta = asin(x/dist);
    phi = asin(y/dist);
end
function [x,y] = mapFromSpherical(theta,phi,dist)
    x = sin(theta)*dist;
    y = sin(phi)*dist;
end