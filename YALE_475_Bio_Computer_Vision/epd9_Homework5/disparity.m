function disp = disparity(leftimg, rightimg, halfpatchsize, maxdisp)
%DISPARITY Calculates the disparity for a pair of rectified stereo images.
%   DISP = DISPARITY(LEFTIMG, RIGHTIMG, HALFPATCHSIZE, MAXDISP) calculates
%   the pointwise disparity between a pair of rectified stereo images.
%
%   LEFTIMG is the left image of the stereo pair, while RIGHTIMG is the
%   right image.
%
%   HALFPATCHSIZE specifies half of the patch size (M in the problem
%   statement in the homework). The patch size actually used ends up being
%   2*HALFPATCHSIZE + 1.
%
%   MAXDISP is an optional parameter specifying the maximum absolute
%   disparity to be tested for. If omitted, it defaults to HALFPATCHSIZE.
%
%   The output argument DISP should contain the disparity between the two
%   images at every point where a valid patch comparison can be made. Due
%   to boundary effects, the size of DISP will be
%
%       size(leftimg) - 2*halfpatchsize - [0, 2*maxdisp]

if ~exist('maxdisp', 'var')
  maxdisp = halfpatchsize;
end

if ~exist('sod', 'var')
  sod = false;
end

nrows = size(leftimg, 1);
ncols = size(leftimg, 2);

if any([nrows, ncols] ~= size(rightimg))
  error('Left and right images aren''t of the same size');
end

disp = zeros(size(leftimg) - 2*halfpatchsize - [0, 2*maxdisp]);

% Your code here

onespatch = ones(2*halfpatchsize+1);
% norms = The ||patch|| functions
leftnorms = conv2(leftimg.^2,  onespatch, 'same').^0.5;
rightnorms= conv2(rightimg.^2, onespatch, 'same').^0.5;

% Find the dx that gives the maximum disparity for each pixel.
best_match = zeros(nrows,ncols) - 2*maxdisp; % Filled with some invalid value
best_match_vals = zeros(nrows,ncols) - inf;
for dx = -maxdisp:maxdisp
    similarities = conv2(leftimg.*circshift(rightimg,dx,2), onespatch, 'same');
    similarities = similarities./ (leftnorms.* circshift(rightnorms, dx, 2));
    new_maxes = similarities > best_match_vals;
    best_match(new_maxes) = dx;
    best_match_vals(new_maxes) = similarities(new_maxes);
end
% Now we trim off the invalid parts:
disp = best_match(halfpatchsize:end-halfpatchsize, halfpatchsize+maxdisp:end-(halfpatchsize+maxdisp));
