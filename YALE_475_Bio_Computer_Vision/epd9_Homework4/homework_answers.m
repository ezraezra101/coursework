%% Homework 4 - Ezra Davis
%% 1a
% The last 10 pixels have a bright center pixel
load('tinypics.mat')
imagesc(data);
axis equal tight;
colormap gray
%% 1b
% There are very clearly at least 2 clusters... and there may be 3, but
% that's much harder to see.
scatter3(data(:,1),data(:,2),data(:,3), 'filled');
xlabel('Pixel 1');
ylabel('Pixel 2');
zlabel('Pixel 3');

%% 1c
% Code in `mykmeans.m`.

%% 1d
% The clusters were exactly as I expected, grouping the first 10 together.
% Occasionally, my code errors and one of the centroids is farther from all
% the points than the other (and becomes NaN), and sometimes what is in
% group 1 swaps with group 2, but there are no real surprises.

[clusters, centroids] = mykmeans(data,2);
scatter3([data(:,1);centroids(:,1)],[data(:,2);centroids(:,2)],[data(:,3);centroids(:,3)],10*ones(20+2,1),[clusters;3;3], 'filled');
colormap([1,0,0;0,1,0;0,0,1]);
xlabel('Pixel 1');
ylabel('Pixel 2');
zlabel('Pixel 3');



%% 2a
load('logos.mat');
for i = 1:10
   subplot(5,2,i);
   imshow(data(:,:,i));
end
%% 2b
% We can't show all of the dimensions at once, but we can perform
% dimensionality reduction of some kind (in this case PCA) and get a pretty
% good idea of how it's going to be grouped.
% (Note: I needed to install the statistical & ML tools package to get this
% to work. Also... I didn't realize we were going to implement PCA next section)
X = ones(10,4800);
for i = 1:10
    X(i,:) = reshape(data(:,:,i),1,4800);
end
c = pca(X.').'; % Row 1 corresponds to X's row 1
figure()
scatter3(c(:,1), c(:,2), c(:,3));

%% 2c
% These are *not* the clusters I was expecting
% There seems to be a high degree of randomness in the final groupings too.
% (incidently, there's still an intermittant issue with NaNs and missing centroids)
[clusters, centroids] = mykmeans(X,2);
sizes = 10*ones(10,1);
scatter3(c(:,1), c(:,2), c(:,3),sizes, clusters, 'filled');
colormap([1,0,0;0,1,0;0,0,1]);
%%
% Images correspond to the given centroid (top image in each column)
range = 1:10;
[~,n] = mode(clusters);
subplot(n+1,2,1);
imshow(reshape(centroids(1,:),48,100));
title("Centroid 1")
j=range(clusters==1);
for i = 1:size(j.')
   subplot(n+1,2,2*i+1);
   imshow(data(:,:,j(i)));
end

subplot(n+1,2,2);
imshow(reshape(centroids(2,:),48,100));
title("Centroid 2")
j=range(clusters==2);
for i = 1:size(j.')
   subplot(n+1,2,2*(i+1));
   imshow(data(:,:,j(i)));
end

%% 3a
% Code for this part is in `mypca.m`

%% 3b
figure();
load('gaussian.mat');
scatter(gaussian(:,1), gaussian(:,2));
hold on;
[components, variances] = mypca(gaussian,2);
quiver([0;0],[0;0],components(:,1), components(:,2),1, 'Linewidth',3);
coeff = components * gaussian.';
hold off;

%% 3c
% An eigenvector is a vector that when multiplied by a matrix, results in
% the same vector multiplied by a scalar (the eigenvector's associated
% eigenvalue).
%% 3d
% The covariance between two vectors represent how much they
% influence each other, and a covariance matrix is a matrix of these
% covariances with position i,j corresponding to the influence i has on j
% (given we're just using vectors withoug causal relationships, a
% covariance matrix is symmetrical)
% If we didn't set the mean of all our data points to 0, we would have
% difficulty comparing brighter and darker images, and almost all of our
% data would be in a few directions from the origin, leaving us with a very
% uninteresting component analysis.

%% 4a
% The variances appear to decrease exponentially (e.g. the most important
% eigenvector is about twice the variance of the 2nd most important)
% Honestly, 
% Entertainingly, this occasionally reverses image values (light/dark)
load('logos.mat')
X = ones(10,4800);
for i = 1:10
    X(i,:) = reshape(data(:,:,i),1,4800);
end
[components, variances] =mypca(X,4);
for i=1:4
    subplot(2,2,i);
    imshow(variances(i)*reshape(components(:,i),48,100));
end
%%
% Variances:
figure()
plot(variances)

%% 4b
% If you think of an image as a 4800 dimensional vector, each pixel
% represents a different direction, and each PCA component represents a
% vector (or direction) in this space.
%% 4c
% The dimensionality of data projected onto 1 1xN vector will always be one
% dimensional. The components matrix represents an embedding of the image
% space - it's own dimensionally reduced basis for a coordinate system.
plot(components(:,1).'*X.');
%% 4d
% This is exactly what I expected from my clustering!
% The result is also very reliable and doesn't change significantly unless
% one of the clusters is invalidated...
% depending on the k-means starting positions.

% Plot according to previous PCA coordinates
coords=components(:,1:2).'*X.'; % Why would I want to use just the 1st eigenvector when I can use 2?

[clusters2, centroids] = mykmeans(coords.',2);

scatter(coords(1,:), coords(2,:),20*ones(1,size(coords,2)), clusters2);
colormap summer ;
%%
% Here are the centroids of the clustering above:
subplot(2,1,1)
imshow(reshape(components(:,1:2)*centroids(1,:).',48,100));
subplot(2,1,2)
imshow(reshape(components(:,1:2)*centroids(2,:).',48,100));


%% 4e
% The cluster assignment using the PCA coordinates is much more like what I
% expected compared to performing `mykmeans` on the original image space.
% k-means clustering evidently doesn't work terribly well in extremely high
% dimensional space (because Euclidian distance isn't very meaningful in
% high dimensional spaces).
% Here is the result of clustering using kmeans on the original images:
[clusters, centroids] = mykmeans(X,2);
sizes = 10*ones(10,1);

coords=components(:,1:2).' *X.';
% Scatter plot of categories plotted according to the 1st 2 principal
% components
figure();
scatter(coords(1,:), coords(2,:),20*ones(1,size(coords,2)), clusters);

%% 5a
% These "eigenfaces" show us the values that change the images of our faces
% the most - faces can be approximated by a linear combination of these
% images. They imply things like hair style, face shape, and shadows around
% the eyes are important for differentiating faces.
load('faces.mat')
faces = double(Data);
faces = faces./ max(faces,[],2);
% To show a face:
% imshow(reshape(faces(10,:),96,96).',[])
[components, variances] = mypca(faces,9);

for i=1:9
   subplot(3,3,i)
   imshow(reshape(components(:,i),96,96).',[])
end