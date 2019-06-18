function H = computeH(im1_pts, im2_pts)
% Computes a 3x3 2D homogeneous matrix (H) that indicates the
% transformation from set of 2D points to another.
% Inputs should be in the form of [x,y;x,y;x,y;...]
% im2_pts is roughly equal to (H*[im1_pts, ones(size(im2_pts,1),1)].').'
% 
% I got significant help with figuring out how to set up my linear
% equations from Sherry (Qiu) - I was having trouble figuring out the 3rd 
% row of H.

points = size(im1_pts,1); % Number of corresponding points


%H = [h_xx, h_xy, h_xw; h_yx, h_yy, h_wy; h_wx, h_wy, 1];
% 0 = x*h_xx + y*h_xy + h_xw + -x'*x*h_wx + -x'*y*h_wy -x'
A = zeros(2*points,8);
B = zeros(2*points,1);
for i = 1:points
    x = im1_pts(i,1);
    x2= im2_pts(i,1);
    y = im1_pts(i,2);
    y2= im2_pts(i,2);
    eqs = [
        x,y,1, 0,0,0, -x2*x,-x2*y; % -x2, but that goes elsewhere
        0,0,0, x,y,1, -y2*x,-y2*y  % -y2
    ];
    A(i*2-1:i*2, :) = eqs;
    B(i*2-1) = x2;
    B(i*2)   = y2;
end

h = A \ B;

H = [h(1), h(2), h(3); h(4),h(5),h(6); h(7),h(8),1];
end