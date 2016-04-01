function [xunit, yunit,errRadius] = posError(E,N)
r = sqrt(E.^2+N.^2);
rad = sort(r);
len = length(E);
lenH = ceil(len*0.393);
errRadius1 = rad(lenH);
lenH = ceil(len*0.865);
errRadius2 = rad(lenH);
lenH = ceil(len*0.989);
errRadius3 = rad(lenH);
% errRadius = std(r);
th = 0:pi/50:2*pi;
errRadius = [errRadius1, errRadius2, errRadius3];
% 1 sigma boundary
xunit(:,1) = errRadius1 * cos(th);
yunit(:,1) = errRadius1 * sin(th);
% 2 sigma boundary
xunit(:,2) = errRadius2 * cos(th);
yunit(:,2) = errRadius2 * sin(th);
% 3 sigma boundary
xunit(:,3) = errRadius3 * cos(th);
yunit(:,3) = errRadius3 * sin(th);

% For 2D position solution,
% Sigma   Probability (%)
%  1.0       39.3
%  1.18      50.0
%  spqr(2)   63.2 (DRMS)
%  2.0       86.5
%  2.45      95.0
%  2*sprt(2) 98.2 (2DRMS)
%  3.0       98.9