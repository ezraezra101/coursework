%% Ezra Davis Homework 1

%% Problem 1
%% 1a
filename = 'Paolina.tiff';
file = imread(filename);
img = im2double(file);
imshow(img);

%% 1b
% The contour function clearly plots isolines - curves of equal intensity
% on the image.
contour(img);

axis ij;

%% 1c
% Gradient is determining the direction that the value (brightness) of the
% image increases the most. A longer arrow indicates that the image's
% brighness is changing a lot at that position.
[U, V] = gradient(img); %, 10);
quiver(U,V);
axis ij;

%% 1d
% This image tells us where the image is changing the most, and performs
% simple edge detection - the larger the gradient, the more likely that the
% pixel is at an edge.

gmag = sqrt((U.^ 2) + (V.^ 2));
imagesc(gmag);

axis ij;
%% 1e
% Multiplying the image by the gradient magnitude brightens the edge of the
% statue, highlighting the areas where the shading changes, especially the
% brighter ones.

beta = 1;
g = gmag * beta + img;
imagesc(g);

colormap(gray); % It really makes sense for this image to be gray...
axis ij;
%%
figure

beta = 10;
g = gmag * beta + img;
imagesc(g);

colormap(gray);
axis ij;
%%
figure

beta = 100;
g = gmag * beta + img;
imagesc(g);

colormap(gray);

%% Problem 2
%% 2a
% -- If both transitions are unlikely:
% A. Coli will generally not change between traveling straight and spinning
% in place, causing it to overshoot its mark and not turn often or
% reasonably.
% -- If both are likely:
% The organism behaves far more reasonably, circling around the area of
% high food concentration.
% -- If one is high:
% If tumble transitioning to run is high, then A. coli will travel long
% straight lines before turning, if the reverse is true, then A. coli will
% spend most of its time spinning, with occasional movements.
% -- If either is 0:
% It will never change from one state to another - and will either never
% move, or never turn.
rttp = 0.99;
ttrp = 0.99;
[hist, food] = acoli_hist([0,0], 1, rttp, ttrp, 1, 100, 20, true);

%% 2b
% Higher values close to one for both rttp and ttrp causes the A. Coli to
% take small steps (but still logical turns), leading to small circles
% around the local (and in our case, global) maximum.

%% 2c
% I've done a fair amount of experimentation, but high chances of changing
% from one state to another still appears to be best.

% I assumed that different values would be better because I assumed that
% the 0.99 probabilities would cause A. coli to circle the closer
% concentation of food, whereas longer straightaways (lower ttrp) would
% allow it to occasionally find the higher concentrations of food farther
% away.

% Old version
rttp = 0.99;
ttrp = 0.99;
[hist, food] = acoli_hist([0,0], 2, rttp, ttrp, 50, 300, 500, false);
sum(sum(food.*hist))

% Experimental (generally worse) version
rttp = 0.8;
ttrp = 0.8;
[hist, food] = acoli_hist([0,0], 2, rttp, ttrp, 50, 300, 500, false);
sum(sum(food.*hist))

%% 2d
% With 50 trials and 300 steps (15000 points of data), in general, more
% bins is better - you can get a much clearer idea of how A. coli circles
% the maxima with more than 10 bins, and get a clearer picture of where
% it's been in general with more bins. By the point of 500 bins, the
% utility has somewhat run out, at least for our food function.
% If we decrease the trials and steps enough (to around 10 and 100
% respectively), lower numbers of bins become much more useful because with
% too many bins, most bins are visited only once or twice.
trials = 50;
steps = 300;

figure
axis xy;
bins = 10;
[hist, food] = acoli_hist([0,0], 2, rttp, ttrp, trials, steps, bins, false);
imagesc(hist)
%%
figure(); axis xy;
bins = 50;
[hist, food] = acoli_hist([0,0], 2, rttp, ttrp, trials, steps, bins, false);
imagesc(hist)
%%
figure(); axis xy
bins = 100;
[hist, food] = acoli_hist([0,0], 2, rttp, ttrp, trials, steps, bins, false);
imagesc(hist)
%%
figure(); axis xy
bins = 500;
[hist, food] = acoli_hist([0,0], 2, rttp, ttrp, trials, steps, bins, false);
imagesc(hist)


%%
% cleanup
close all;
