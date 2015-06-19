function [grad,grb_result] = OptimalSamplingSingleSubset(gradAll, param, grbParam)
% get several subsets of gradients from a given gradient set, such that
% gradients in each subset are evenly distributed.
%
% USAGE:
%   [grad,grb_result] = OptimalSamplingSingleSubset(gradAll, param, grbParam)
%
% INPUT
%   gradAll            :  Nx3 gradient matrix, where each row is a point in sphere.
%   params.numSamples  :  the number of samples in the subset
%
%   grbParam.start     :  a warm start if provided
%   grbParam.TimeLimit :  time limit to terminate the program
%   grbParam.MIPFocus  :  MIPFocus
%
% OUTPUT
%   grad               :  output gradient matrix with numSamples samples.
%   grb_result         :  output from GUROBI
%
% Copyright (c) 2013, Jian Cheng <jian.cheng.1983@gmail.com>
%

%%
[N, c] = size(gradAll);
if c~=3
    error('wrong size of input gradients!');
end

if isfield(param,'numSamples')==0
    error('should give a number of samples!');
end
numSamples=param.numSamples;

    
% model
clear model 


% MIPFocus=1 gives better results than other values
if ~isfield(grbParam,'MIPFocus')
    grbParam.MIPFocus=1; 
end
if ~isfield(grbParam,'OutputFlag') || grbParam.OutputFlag
    fprintf('MIPFocus = %d\n', grbParam.MIPFocus);
end


%% MILP

% calculate the distances between points
distAll = real(acos(abs(gradAll*gradAll')));

tmp = distAll(triu(distAll,1)>0);
M = max(tmp)-min(tmp);

distAllM = distAll+2*M;

% http://mathworld.wolfram.com/SphericalCode.html
% 2*numSamples points in sphere
upperBuondEuc = sqrt(  4-csc( pi*2*numSamples/(6*(2*numSamples-2)) )^2 );
upperBuondSpherical = acos((2.0-upperBuondEuc*upperBuondEuc)/2.0);

Nrows = nnz(tmp<=upperBuondSpherical);


% objective
model.obj = [1 zeros(1,N)];
model.modelsense = 'max';

% inequality constraint
rhs = zeros(Nrows,1);
A = zeros(Nrows, N+1);
A0 = sparse(Nrows, N+1);
A0(:,1) = 1;

kk=1;
for i = 2 : N
    for j = 1 : i-1
        %         fprintf('i=%d, j=%d\n', i, j);
        if distAll(i,j) <= upperBuondSpherical
            rhs(kk) = distAllM(i,j);
            A(kk,i+1) = M;
            A(kk,j+1) = M;
            kk = kk+1;
        end
    end
end

A = sparse(A) + A0;
% equality constraint
model.A = sparse([ [0, ones(1,N) ]; A]);
model.rhs = [numSamples; rhs];
model.sense = ['=', repmat('<',[1,Nrows])];
if (isfield(param,'lbCost')~=0)
    model.lb = [max(param.lbCost, min(tmp)), zeros( 1, N) ];
else
    model.lb = [min(tmp), zeros( 1, N) ];
end
if (isfield(param,'ubCost')~=0)
    model.ub = [min(param.ubCost, upperBuondSpherical), ones(1,N) ];
else
    model.ub = [upperBuondSpherical, ones(1,N) ];
end
model.vtype = ['C', repmat('B',[1,N])];

%%
grb_result = gurobi(model, grbParam);

% grb_result.x
grad=gradAll(grb_result.x(2:end)>1e-4,:);