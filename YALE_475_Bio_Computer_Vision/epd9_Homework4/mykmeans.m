function [clusters centroids] = mykmeans(X, k, init)
%MYKMEANS implements the k-means algorithm.
%   [CLUSTERS CENTROIDS] = MYKMEANS(X, K, INITS) partitions the data points
%   in the N-by-P data matrix X into K distinct clusters, using Euclidean
%   distance. This is a simple implementation of the k-means algorithm with
%   random initialization.
%
%   Optionally, it takes the argument INIT, a K-by-P matrix with a fixed
%   initial position for the cluster centroids.  
%
%   MYKMEANS returns an N-by-1 vector CLUSTERS containing the cluster
%   indices of each data point, as well as CENTROIDS, a K-by-P matrix with 
%   the final cluster centroids' locations.


[n p] = size(X);

if ~exist('init', 'var')
  %choose initial centroids by picking K points uniformly at random from the range of X
  init = min(X(:)) + rand(k,p)*(max(X(:))-min(X(:)));
end
%centroids is a k-by-p random matrix
%its i^th row contains the coordinates of the cluster with index i
centroids = init;

%initialize cluster assignment vector
clusters = zeros(n,1);

MAXITER = 1000;


for iter=1:MAXITER
    
    %create a new clusters vector to fill in with updated assignments
    new_clusters = zeros(n,1);
    

    %for each data point x_i
    for i=1:n
        
        x_i = X(i,:);
        
        %find closest cluster
        closest = findClosestCluster(x_i,centroids);%%%IMPLEMENT THIS FUNCTION AT THE END OF THIS FILE
        %reassign x_i to the index of the closest centroid found
        new_clusters(i) = closest;

    end
    
    if hasConverged(clusters,new_clusters)%%%IMPLEMENT THIS FUNCTION AT THE END OF THIS FILE
        %exit loop
        break 
    end
    
    %otherwise, update assignment
    clusters = new_clusters;
    %and recompute centroids
    centroids = recomputeCentroids(X,clusters,k);%%%IMPLEMENT THIS FUNCTION AT THE END OF THIS FILE

end

if iter == MAXITER
    disp('Maximum number of iterations reached!');
end

end

function closest = findClosestCluster(x_i,centroids)
% Compute Euclidean distance from x_i to each cluster centroid and return 
% the index of the closest one.

%%% Replace the following line with your own code
% This looks soo simple... but I've gone through so many versions (with
% arrayfun and friends)
[~, closest] = min(sum((centroids - x_i).^2, 2));

end

function converged = hasConverged(old_assignment, new_assignment)
% Check if algorithm has converged, i.e., cluster assignments haven't
% changed since last iteration.

%%% Replace the following line with your own code
converged = isequal(old_assignment, new_assignment);

end

function centroids = recomputeCentroids(X,clusters,k)
% Recompute centroids based on current cluster assignment.

%%% Replace the following line with your own code
% It would make so much sense to use splitapply here... but it doesn't
% work :-(... as far as I can tell.
[~, p] = size(X);
centroids = zeros(k, p);
for i=1:k
   centroids(i,:) = mean(X(clusters==i,:));
end
centroids(isnan(centroids)) = 0;
end
    