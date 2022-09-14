clear
clc
%n = 100000; d = 2; % works for any d

%X = rand(n,d);

X = importdata('./points.txt');
n = size(X, 1); m = size(X, 2);

dfile ='matlab_output.txt';
if exist(dfile, 'file') ; delete(dfile); end
diary(dfile)
diary on
fprintf('Points:\n');
for i = 1:n
    for j = 1:m
        fprintf('%.2f ', X(i, j));
    end
    fprintf('\n');
end
fprintf('\n');

% assume vantage point is the last one
% get squares of distances from it
d = sqrt( sum((X(1:n-1,:) - X(n,:)).^2,2) );
% and find the median distance
medianDistance = median(d);

% plot them to confirm
clf; hold on; axis equal

plot(X(d <= medianDistance,1), X(d <= medianDistance,2), 'r.')
plot(X(d >  medianDistance,1), X(d >  medianDistance,2), 'b.')
plot(X(n,1), X(n,2), 'bo') % vantage point

T = vpTree(X);
traversePreOrder(T);
diary off;

function T = vpTree(X)
% function T = vpTree(X)
% computes the vantage point tree structure with
%   T.vp : the vantage point
%   T.md : the median distance of the vantage point to the other points
%   T.idx : the index of the vantage point in the original set
%   T.inner and T.outer : vantage-point subtrees
% of a set of n points in d dimensions with coordinates X(1:n,1:d)
%
  T = vpt(X, 1:size(X,1));

  function T = vpt(X, idx)

    n = size(X,1); % number of points
    if n == 0
      T = [];
    else
      T.vp = X(n,:);
      T.idx = idx(n);
      
      d = sqrt( sum((X(1:n-1,:) - X(n,:)).^2,2) );
      
      medianDistance = median(d);
      T.md = medianDistance;
      
      % split and recurse
      inner = d <= medianDistance;
      T.inner = vpt(X( inner,:), idx( inner));
      T.outer = vpt(X(~inner,:), idx(~inner));
    end
  end
end


function [] = traversePreOrder(T)
    if(~isempty(T))
        for j = 1:size(T.vp, 2)
            fprintf('%.2f ', T.vp(j));
        end
        fprintf('\n');
        traversePreOrder(T.inner);
        traversePreOrder(T.outer);
    end
end