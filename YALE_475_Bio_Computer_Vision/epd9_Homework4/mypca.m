function [components, variances] = mypca(X, m)
% Center X
X = X - mean(X,2);
% Construct covariance matrix
[n, ~] = size(X);
C = X.' * X / (n-1);
% Find its eigenvectors and values
[V, D] = eigs(C, m);

% Convert D to a vector (from diagonal matrix)
d = diag(D);

components = V;
variances = d;

end