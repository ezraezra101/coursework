function img = blendImage(ims, offsets)

% Acquire sizes of our images:
sizes = zeros(size(offsets));
for i = 1:numel(ims)
   s = size(ims{i});
   sizes(i,:) = s(1:2);
end

% Figure out the corners of our final canvas:
leftTop = min(offsets);
rightBottom = max([sizes(:,2),sizes(:,1)]+offsets);

% Create our canvas (rgba):
img = zeros(rightBottom(2)-leftTop(2), rightBottom(1)-leftTop(1), 4, 'double');

% Add each image (in order) to `img` according to their alpha value
for i= 1:numel(ims)
    oset = offsets(i,:)-leftTop; % offset
    ix = (oset(1)+1:oset(1)+sizes(i,2)); % Indexes that ims{i} is in canvas
    iy = (oset(2)+1:oset(2)+sizes(i,1));
    
    blurred = laplacianPyramidBlend(img(iy,ix,:), double(ims{i}));

    img(iy,ix,:) = fixLaplacianBlur(blurred, double(ims{i}(:,:,4)), img(iy,ix,4));
end

end

function destination = simpleBlend(im1, im2)
% Simple dumb blending based on summing alphas

alphaMask = im1(:,:,4); % canvas' alpha (for some reason its 0-1... not 0-255)

% Lazy feathering:
nza = alphaMask ~= 0; % Non-zero alpha
alphaMask(nza) = alphaMask(nza)./(alphaMask(nza)+alphaMask(nza));

destination = alphaMask.*im1 + (1-alphaMask).*im2;
end

function img = fixLaplacianBlur(img, alpha1, alpha2)
    % In our images (which have transparency) Laplacian Pyramid blending
    % causes some bleedover - blurring one image into another where it
    % should have alpha=0.
    % Knowing that we have alpha 1 or 0, and when alpha==0, color=[0,0,0],
    % I can rectify this to get an image that is appropriately opaque (and
    % not darkened where images meet)

    nza = alpha1~=0 | alpha2~=0;
    alpha = img(:,:,4);
    for i=1:4 % dumb... I would really rather not use a for loop
        color = img(:,:,i);
        color(nza) = color(nza)./alpha(nza);
        img(:,:,i) = color;
    end
end

function destination = laplacianPyramidBlend(im1, im2)
    L1 = laplacianPyramid(im1);
    L2 = laplacianPyramid(im2);
    
    im1Alpha = gaussianPyramid(im1(:,:,4));
    im2Alpha = gaussianPyramid(im2(:,:,4));
    
    L3 = cell(numel(L1),1);
    
    for i=1:numel(L3)
        alpha = im1Alpha{i};
        nza = alpha ~= 0; %nza = non-zero alpha
        alpha(nza) = im1Alpha{i}(nza)./(im1Alpha{i}(nza)+im2Alpha{i}(nza));
        L3{i} = L1{i}.*alpha + L2{i}.*(1-alpha);
    end
    
    destination = collapseLaplacianPyramid(L3);    
end


function corners = harrisCornerDetector(im)
% There is a harris corner detection function in the CV toolkit in matlab,
% but I'm implementing my own simple version.

% Derivatives in x and y directions
dx = conv2(im,[-1,0,1], 'same');
dy = conv2(im,[-1,0,1].', 'same');

% Smooth over a window
windowingKernel = ones(10); % Could probably be done faster
xx = conv2(dx.*dx, windowingKernel,'same');
yy = conv2(dy.*dy, windowingKernel,'same');
xy = conv2(dx.*dy, windowingKernel,'same');

k = 0.04; % Empirically determined
trace = xx + yy;
determinant = xx.*yy + xy.*xy;
response = determinant - (k*trace);

corners = response;
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