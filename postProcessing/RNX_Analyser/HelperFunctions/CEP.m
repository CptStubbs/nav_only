function [xunit, yunit, CEP_radius] = CEP(E,N)
r = sqrt(E.^2+N.^2);
rad = sort(r);
sN = sort(abs(N));
len = length(E);
lenH = ceil(len*0.9);
CEP_radius = rad(lenH);
%CEP_radius = 1.18*std(r);
th = 0:pi/50:2*pi;
xunit = CEP_radius * cos(th) + 0;
yunit = CEP_radius * sin(th) + 0;

% xunit = CEP_radius * cos(th) + mean(E);
% yunit = CEP_radius * sin(th) + mean(N);