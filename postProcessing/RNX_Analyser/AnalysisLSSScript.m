%% Script to analyze an lssbinaries.bin file and produce plots to visualize the data.
% Inputs:
%   - fileName = name of binary file to analyze set under File settings
%% Clear Workspace
clear all;
close all;
clc;
%% Set File Details
% Set File String
fileStr = 'rnxBinaries_0_0.bin';
fileStrTitles = 'nfstimingrnxBinaries\_0\_0.bin';
% Set DateVector
dateVector = [0 0];
% Set Path for Plots and Files
parentpath = '../../output/';
plotpath = '../../output/RNX_Analysis/';





%% Add Path to get Helper Functions
currentpathinitial = pwd();
currentpath = cd('..');
currentpath = cd('..');
addpath(genpath(currentpath));
cd(currentpathinitial);


%% get screensize
scrsz = get(0,'ScreenSize');


%% set default to full screen
set(0,'defaultfigureposition', scrsz);


%% File settings
fileName    =[parentpath fileStr];    % file name 
myfile    = dir(fileName);        % file info
if isempty(myfile)
    error(['Wrong Directory: Current Directory -' parentpath ' does not contain the file.']);
else
    fileSize    = myfile.bytes;         % file size
end
fid         = fopen(fileName, 'rb');% FID


%% Read in Header Data
metadata = fgets(fid);
metadata = strsplit(metadata, '\t');
variableNames = fgets(fid);
variableNames = strsplit(variableNames, '\t');
variableSizes = fgets(fid);
variableSizes = strsplit(variableSizes, '\t');
numel(variableSizes);
scaIndex = 1;
vecIndex = 1;
variableSizes = cellstr(variableSizes);
for i = 1:numel(variableSizes)
    if isempty(strfind(variableSizes{i},'*'))
        scaType{scaIndex} = variableSizes{i};
        scaNames{scaIndex} = variableNames{i};
        scaIndex = scaIndex + 1;
    else
        startIndex = strfind(variableSizes{i},'*');
        numChannels = str2double(variableSizes{i}(1:startIndex-1));
        vecType{vecIndex} = variableSizes{i}(startIndex+1:end);
        vecNames{vecIndex} = variableNames{i};
        vecIndex = vecIndex + 1;
    end

end
scaType = cellstr(scaType);
vecType = cellstr(vecType);


%% Set Truth or Mean for reference position
truthOrMeanRefStr = 't'; % 'm' for mean ref, 't' for truth ref


%% IF using TRUTH reference position, Set orz for truth reference position
% DLC antenna reference position: 
% - Lat/Lon/Hght: 40.00749295305 /-105.26167642616/1630.40495370706
% - XYZ: [-1288084.72435924,-4720848.72539722,4079671.03660523]
% orz = [-1288084.72435924,-4720848.72539722,4079671.03660523]; % DLC

% 1B81 antenna
orz = [-1288160.31484243,-4720790.09567223,4079712.76313084];   % 1B81


%% RNX file settings
nChannels       = numChannels;   % Number of channels
bytesInEpoch    = 1432; % How many bytes in 1 epoch data: 136 + 1248 + 48


%% Plot Settings 
subplotWindow       = 3600;	% Time window for position solution within 1 subplot (s)
subCol              = 3;   	% Number of columns within 1 figure 
subRow              = 2;    % Number of rows within 1 figure
samplingFrequency   = 6.864e6;	% Sampling frequency
baseError           = 100; 	% Threshold to check the error in dt


%% Pre-define the variable size
dataLen     = floor((fileSize) / (bytesInEpoch));    % data length
lssVector   = nan(32, length(vecType), dataLen);  % data for each PRN
pos_enu     = nan(dataLen, 5);      % position data in ENU frame
displayStep = round(dataLen / 10);  % Status display step
temppos = nan(dataLen, 3); 
disp('Start LSS decoding ...');


%% Read and save the data
% Variable names:
%
% SCALAR: rtow	X	Y	Z	dt	Vx	Vy	Vz	dtRate	HDOP	VDOP	
% TDOP	smp	frc	numSat	valid1	valid2	newKal	newMea	validPos	week
%
% VECTOR: pr1	cp1	dp1	cno1	pr2	cp2	dp2	cno2	Xsv	Ysv	Zsv	svClk	validPRN	prn	
for i = 1:dataLen 
    scaIndex = 1;
    vecIndex = 1;
    for l = 1:length(scaNames)-1
        eval([scaNames{scaIndex}, '(i)=fread(fid,1, scaType{scaIndex});']);
        scaIndex = scaIndex + 1;
    end
    for k = 1:length(vecNames)
        for j = 1:numChannels
            eval([vecNames{vecIndex}, '(i,j)=fread(fid,1, vecType{vecIndex});']);
        end
        vecIndex = vecIndex + 1;
    end
    for j = 1:numChannels
        % checkt #satellites & PRN validity
        if j <= numSat(i) && validPRN(i,j) == 1
            lssVector(prn(i,j), 1, i) = pr1(i,j);  % PR1
            lssVector(prn(i,j), 2, i) = cp1(i,j);  % Carr1
            lssVector(prn(i,j), 3, i) = dp1(i,j);  % Dopp1
            lssVector(prn(i,j), 4, i) = cno1(i,j);  % CNo1
            lssVector(prn(i,j), 5, i) = pr2(i,j);  % PR2
            lssVector(prn(i,j), 6, i) = cp2(i,j);  % Carr2
            lssVector(prn(i,j), 7, i) = dp2(i,j);  % Dopp2
            lssVector(prn(i,j), 8, i) = cno2(i,j);  % CNo2
            lssVector(prn(i,j), 9, i) = Xsv(i,j);  % Sat X
            lssVector(prn(i,j), 10, i) = Ysv(i,j);  % Sat Y
            lssVector(prn(i,j), 11, i) = Zsv(i,j);  % Sat Z
            lssVector(prn(i,j), 12, i) = svClk(i,j);  % Sat Clk            
        end
    end
%    status display
    if rem(i, displayStep) == 0
        disp(['Processing ', num2str(i*bytesInEpoch/fileSize*100, '%2.0f'), ' %']);
    end
end
lssScalar = [rtow',	X',	Y',	Z',	dt',	Vx',	Vy',	Vz',	dtRate',	HDOP',	VDOP',	TDOP' ...
     ,smp',	frc',	numSat',	valid1',	valid2',	newKal',	newMea',	validPos',	week'];
 
 
%% Find indexes where there is a valid position solution
dataIndex = find(validPos == 1); % if valid position solution
pos_enu(dataIndex,1:2) = [lssScalar(dataIndex,21) lssScalar(dataIndex,1)]; % time info
temppos(dataIndex,1:3) = lssScalar(dataIndex,2:4); % set a temp matrix with RAW position data


%% Calculate Mean reference position 
meanorz = [mean(temppos(dataIndex,1)), mean(temppos(dataIndex,2)), mean(temppos(dataIndex,3))];


%% Convert to ENU reference frame based on truth or mean reference position
if strcmp(truthOrMeanRefStr, 'm')==1 %------if using mean ref position-------
    for i = 1:sum(dataLen)
        if lssScalar(i,20) == 1
            pos_enu(i,3:5) = xyz2enu(lssScalar(i,2:4), meanorz);% ENU position
        end
    end
    %% Set Latitude/Longitude of Reference Position
    llh = xyz2llh(meanorz);
    lat = llh(1);
    lon = llh(2);
    alt = llh(3);
else %------if using truth ref position-----------------
    for i = 1:sum(dataLen)
        if lssScalar(i,20) == 1
            pos_enu(i,3:5) = xyz2enu(lssScalar(i,2:4), orz);% ENU position
        end
    end
    %% Set Latitude/Longitude of Reference Position
    llh = xyz2llh(orz);
    lat = llh(1);
    lon = llh(2);
    alt = llh(3);
end

fclose all;
disp('---  LSS Decoding finished ---');


%% Get total length of time
totTime = lssScalar(end,1)-lssScalar(1,1); %End time of file minus beg time of file
Time = lssScalar(:,1); %Time vector
timeHours = Time.*(1/3600); %Time in hours vector
%-----Taking into account week rollover (Time resets at end of week)--------
if (find((timeHours >= 167.9999),1))
    timeHours(find((timeHours >= 167.9999),1)+1:end) = timeHours(find((timeHours >= 167.9999),1)+1:end) + 168;
    Time = timeHours*3600;
    totTime = Time(end)-Time(1);
end


%% Availability
dataIndex = ~isnan(pos_enu(:,3)); % Exclude the NaN. 
timePosAvail = length(Time(dataIndex)); % finds number of indexes (seconds) that a position solution is available
percentAvail = timePosAvail/totTime; % Percentage of time that a position solution is available over entire file
percentAvailStr = sprintf('%.1f',100*percentAvail);


%% Max 2D and 3D Error (and epoch/time)
% Max 2D
error2D = sqrt(pos_enu(dataIndex,3).^2 + pos_enu(dataIndex,4).^2);
maxError2D = max(error2D); %m
timeMax2D = timeHours(error2D == maxError2D)-timeHours(1); %hr
% Max 3D 
error3D = sqrt(pos_enu(dataIndex,3).^2 + pos_enu(dataIndex,4).^2 + pos_enu(dataIndex,5).^2);
maxError3D = max(error3D); %m
timeMax3D = timeHours(error3D == maxError3D)-timeHours(1); %hr


%% Max Height Error (and epoch/time)
maxHeightError = max(pos_enu(:,5)); %m
timeMaxHeight = timeHours(pos_enu(:,5) == maxHeightError)-timeHours(1); %hr


%% Mean and std for 2D and 3D Error
% Mean and std 2D
mean2D = mean(error2D);
std2D = std(error2D);
% Mean and std 3D
mean3D = mean(error3D);
std3D = std(error3D);


%% Whole position solutions
dataIndex = ~isnan(pos_enu(:,3)); % Exclude the NaN.
PosSol = figure(100);
h = scatter(pos_enu(dataIndex,3), pos_enu(dataIndex,4),24, timeHours(dataIndex)-timeHours(1),'filled');
cdata = get(h, 'CData');
caxis([timeHours(1)-timeHours(1) timeHours(end)-timeHours(1)]); %Set the colorbar axis to be over the entire time frame of the file
l = colorbar;
ylabel(l, 'Time (Hours)');
%---------Calculate the sigma radius circle for 1,2, and 3 stdeviations-----
[xunit, yunit, sigma_radius] = posError(pos_enu(dataIndex,3),pos_enu(dataIndex,4));
hold on;
plot(xunit(:,1), yunit(:,1),'g', 'linewidth', 2);       % 1 sigma
plot(xunit(:,2), yunit(:,2),'b', 'linewidth', 2);   % 2 sigma
plot(xunit(:,3), yunit(:,3),'r', 'linewidth', 2);   % 3 sigma
title(['Filename: ' fileStrTitles ', East-North 2D Position Solution (', num2str(length(pos_enu)), ' samples); 1 \sigma Radius =',num2str(sigma_radius(1),4),'m','; av: ',percentAvailStr,'%']);
xlabel('East (m)');
ylabel('North (m)');
axis equal;
grid on;
%------Include text in the plot for what the radius of the circles are--------------
text(sigma_radius(1), 0,   num2str(sigma_radius(1),'%0.1f'), 'fontsize', 12, 'fontweight', 'bold', 'color', 'k');
text(sigma_radius(2), -5, num2str(sigma_radius(2),'%0.1f'), 'fontsize', 12, 'fontweight', 'bold', 'color', 'k');
text(sigma_radius(3), -10, num2str(sigma_radius(3),'%0.1f'), 'fontsize', 12, 'fontweight', 'bold', 'color', 'k');
%--------Include version number, git, compiler in plot--------------------
ylimit=get(gca,'YLim');
xlimit=get(gca,'XLim');
info = sprintf('%s\n%s\n%s',metadata{2},metadata{3},metadata{4});
text(xlimit(2),ylimit(1),info,...
   'VerticalAlignment','bottom',...
   'HorizontalAlignment','right');
legend('Samples', '1 \sigma (39.3%)', '2 \sigma (86.5%)', '3 \sigma (98.9%)')


%% Plot the results over smaller time intervals

% Calculate number of subplots needed to cover the entire dataset over the
% required subplot window
numSubFig = ceil((Time(end)-Time(1))/subplotWindow);
subplotCount = 1;
figCount = 1;

disp(['Plotting ', num2str(numSubFig), ' sub-figures']);

for j = 1:numSubFig
    if rem(j,subRow*subCol) == 1
    NAV(figCount) = figure(100+figCount);
        subplotCount = 1;
        figCount = figCount + 1;
    end
%-----Find the index indicating the end of the current hour----------------
    dataIndexEnd = find(((Time-Time(1))>= subplotWindow + subplotWindow*(j-1)),1);
%-----Find the index indicating the beginning of the current hour----------
    dataIndexBeg = find(((Time-Time(1))>= subplotWindow*(j-1)),1);
%-----Update the data index------------------------------------------------    
    dataIndex = dataIndexBeg:dataIndexEnd;
%-----If near the end of the file, and the final subplot is not a full
%hour, set the index so that it just goes to the end of the file-----------
    if isempty(dataIndexEnd) || isempty(dataIndex)
        dataIndex = dataIndexBeg:length(pos_enu);
        dataIndexEnd = length(pos_enu);
    end
    
    dataIndex = dataIndexBeg+find(lssScalar(dataIndex,20)==1)-1; % Exclude the NaN.

%-----Calculate availability over just this time interval------------------
    timeAv = Time(dataIndex); % assumes 1 second intervals
    tPA = length(timeAv);
%-----Check to make sure there is at least two position solutions in the
%time interval-------------------------------------------------------------
    if dataIndexEnd ~= dataIndexBeg
        pA = tPA/(Time(dataIndexEnd)-Time(dataIndexBeg));
        pA = pA*100;
    else
        pA = 0; 
    end
    pAstr = sprintf('%.1f',pA);
%-------plot-------------------
    subplot(subRow,subCol,subplotCount);
    if tPA >= 2 % Checks again for at least two position solutions
        scatter(pos_enu(dataIndex,3), pos_enu(dataIndex,4),24,timeHours(dataIndex)-timeHours(1), 'filled');
        caxis([timeHours(dataIndexBeg)-timeHours(1) timeHours(dataIndexEnd)-timeHours(1)]);
        [xunit, yunit, CEP_radius] = CEP(pos_enu(dataIndex,3),pos_enu(dataIndex,4));
        hold on;plot(xunit,yunit,'r.');
        title([ 't-', num2str((subplotWindow*(j-1))/3600),'-',num2str((subplotWindow + subplotWindow*(j-1))/3600) ,'hr; av:',pAstr,'%; 90%C:',num2str(CEP_radius,4),'m' ],'FontSize',10);
        xlabel('East (m)');
        ylabel('North (m)');
        axis equal;
        grid on;
    else
        title([ 't-', num2str((subplotWindow*(j-1))/3600),'-',num2str((subplotWindow + subplotWindow*(j-1))/3600) ,'hr; av:',pAstr,'%'],'FontSize',10);
        xlabel('East (m)');
        ylabel('North (m)');
        axis equal;
        grid on;
    end
%------------ update the index for the subplot-------------------------
    subplotCount = subplotCount + 1;   
end


%% Find large position error
largeIndex = (abs(lssScalar(:,5)) > baseError); 
dataIndex  = 1:length(lssScalar);

if sum(largeIndex) > 1
    disp('There are large position error!!');
    disp('Absolute sample number:');
    disp(num2str(lssScalar(largeIndex(11:end),19)'/samplingFrequency));
    disp('Data index:');
    disp(num2str(dataIndex(largeIndex(1:end))));
else
    disp('The position solution looks good!');
end


%% dt / #SVs / HDOP
%-----Get indexes where there is a position solution----------------------
dataIndex = (lssScalar(:,20) ~= 0);
%-----Get indexes where there is outages----------------------------------
dataIndex1 = (lssScalar(:,20) == 0);
outages = timeHours(dataIndex1)-timeHours(1);
x = zeros(1,length(outages));
DT = figure(200);
%-------Plot height error and outages on same subplot----------------------
subplot(4,1,1)
maxThreshold = 150;
minThreshold = -150;
hold on
plot(timeHours(dataIndex)-timeHours(1), pos_enu(dataIndex,5),'.b','MarkerSize',18);
indexAboveThres = find(pos_enu(dataIndex,5) >= maxThreshold);
if ~isempty(indexAboveThres)
    maxThreshold = maxThreshold*ones(length(indexAboveThres),1);
    plot(timeHours(indexAboveThres)-timeHours(1),maxThreshold,'.r','MarkerSize',18);
    ylim([minThreshold(1)-1 maxThreshold(1)+1]);
end
indexBelowThres = find(pos_enu(dataIndex,5) <= minThreshold);
if ~isempty(indexBelowThres)
    minThreshold = minThreshold*ones(length(indexBelowThres),1);
    plot(timeHours(indexBelowThres)-timeHours(1),minThreshold,'.r','MarkerSize',18);
    ylim([minThreshold(1)-1 maxThreshold(1)+1]);
end
if ~isempty(outages)
outageHandle = plot(outages,x, 'xr','MarkerSize',18);
legend(outageHandle, 'Outages');
end

%ylim([-100 100]);
xlabel('Time (h)');
xlim([0 (timeHours(end) - timeHours(1))]);
y=ylabel('Height Error (m)');
set(y, 'Units', 'Normalized', 'Position', [-0.1, 0.5, 0]);
grid on;
hold off
title(['Filename: ' fileStrTitles]);
%------------plot dt in next subplot--------------------------------------
subplot(4,1,2)
maxThreshold = 400;
minThreshold = -400;
hold on;
plot(timeHours(dataIndex)-timeHours(1), lssScalar(dataIndex,5),'.g','MarkerSize',18);
% axis tight 
%ylim([-100 100]);
indexAboveThres = find(lssScalar(dataIndex,5) >= maxThreshold);
if ~isempty(indexAboveThres)
    maxThreshold = maxThreshold*ones(length(indexAboveThres),1);
    plot(timeHours(indexAboveThres)-timeHours(1),maxThreshold,'.r','MarkerSize',18)
    ylim([minThreshold(1)-1 maxThreshold(1)+1]);
end
indexBelowThres = find(lssScalar(dataIndex,5) <= minThreshold);
if ~isempty(indexBelowThres)
    minThreshold = minThreshold*ones(length(indexBelowThres),1);
    plot(timeHours(indexBelowThres)-timeHours(1),minThreshold,'.r','MarkerSize',18)
    ylim([minThreshold(1)-1 maxThreshold(1)+1]);
end
xlabel('Time (h)');
xlim([0 (timeHours(end) - timeHours(1))]);
y=ylabel('dt (m)');
set(y, 'Units', 'Normalized', 'Position', [-0.1, 0.5, 0]);
grid on;
hold off;
%-----------plot # of satellites in next subplot--------------------------
subplot(4,1,3)
plot(timeHours(dataIndex)-timeHours(1), lssScalar(dataIndex,15),'k*');
axis tight; %ylim([0 14]);
xlabel('Time (h)');
xlim([0 (timeHours(end) - timeHours(1))]);
ylabel('#SVs');
grid on;
%-----------plot HDOP in next subplot -----------------------------------
subplot(4,1,4)
plot(timeHours(dataIndex)-timeHours(1), lssScalar(dataIndex,10), '.r','MarkerSize',18);
% axis tight;%ylim([0 10]);
xlabel('Time (h)');
xlim([0 (timeHours(end) - timeHours(1))]);
y=ylabel('HDOP');
set(y, 'Units', 'Normalized', 'Position', [-0.1, 0.5, 0]);
grid on;

%--------Include version number, git, compiler in plot--------------------
ylimit=get(gca,'YLim');
xlimit=get(gca,'XLim');
info = sprintf('%s\n%s\n%s',metadata{2},metadata{3},metadata{4});
text(xlimit(2),ylimit(1),info,...
   'VerticalAlignment','bottom',...
   'HorizontalAlignment','right');
%----------JOIN subplots and set the ylabels closer to the plots to see
%them better-------------------------------------------------------------
samexaxis('join','yld',0.75);


%% Velocity X,Y,Z and DT Rate
%-----Get indexes where there is a position solution----------------------
dataIndex = (lssScalar(:,20) ~= 0);
%-----Get indexes where there is outages----------------------------------
dataIndex1 = (lssScalar(:,20) == 0);
outages = timeHours(dataIndex1)-timeHours(1);
x = zeros(1,length(outages));
Vel = figure(250);
%-------Plot X vel and outages on same subplot----------------------
subplot(4,1,1)
maxThreshold = 150;
minThreshold = -150;
hold on
plot(timeHours(dataIndex)-timeHours(1), lssScalar(dataIndex,6),'.b','MarkerSize',18);
indexAboveThres = find(lssScalar(dataIndex,6) >= maxThreshold);
if ~isempty(indexAboveThres)
    maxThreshold = maxThreshold*ones(length(indexAboveThres),1);
    plot(timeHours(indexAboveThres)-timeHours(1),maxThreshold,'.r','MarkerSize',18);
    ylim([minThreshold(1)-1 maxThreshold(1)+1]);
end
indexBelowThres = find(lssScalar(dataIndex,6) <= minThreshold);
if ~isempty(indexBelowThres)
    minThreshold = minThreshold*ones(length(indexBelowThres),1);
    plot(timeHours(indexBelowThres)-timeHours(1),minThreshold,'.r','MarkerSize',18);
    ylim([minThreshold(1)-1 maxThreshold(1)+1]);
end
if ~isempty(outages)
outageHandle = plot(outages,x, 'xr','MarkerSize',18);
legend(outageHandle, 'Outages');
end

title(['Filename: ' fileStrTitles]);
%ylim([-100 100]);
xlabel('Time (h)');
xlim([0 (timeHours(end) - timeHours(1))]);
y=ylabel('X Vel [m/s]');
set(y, 'Units', 'Normalized', 'Position', [-0.1, 0.5, 0]);
grid on;
hold off
%------------plot Y Vel in next subplot--------------------------------------
subplot(4,1,2)
maxThreshold =150;
minThreshold = -150;
hold on;
plot(timeHours(dataIndex)-timeHours(1), lssScalar(dataIndex,7),'.g','MarkerSize',18);
% axis tight 
%ylim([-100 100]);
indexAboveThres = find(lssScalar(dataIndex,7) >= maxThreshold);
if ~isempty(indexAboveThres)
    maxThreshold = maxThreshold*ones(length(indexAboveThres),1);
    plot(timeHours(indexAboveThres)-timeHours(1),maxThreshold,'.r','MarkerSize',18)
    ylim([minThreshold(1)-1 maxThreshold(1)+1]);
end
indexBelowThres = find(lssScalar(dataIndex,7) <= minThreshold);
if ~isempty(indexBelowThres)
    minThreshold = minThreshold*ones(length(indexBelowThres),1);
    plot(timeHours(indexBelowThres)-timeHours(1),minThreshold,'.r','MarkerSize',18)
    ylim([minThreshold(1)-1 maxThreshold(1)+1]);
end
xlabel('Time (h)');
xlim([0 (timeHours(end) - timeHours(1))]);
y=ylabel('Y Vel [m/s]');
set(y, 'Units', 'Normalized', 'Position', [-0.1, 0.5, 0]);
grid on;
hold off;
%-----------plot Z Vel in next subplot--------------------------
subplot(4,1,3)
maxThreshold = 150;
minThreshold = -150;
hold on;
plot(timeHours(dataIndex)-timeHours(1), lssScalar(dataIndex,8),'.k','MarkerSize',18);
% axis tight 
%ylim([-100 100]);
indexAboveThres = find(lssScalar(dataIndex,8) >= maxThreshold);
if ~isempty(indexAboveThres)
    maxThreshold = maxThreshold*ones(length(indexAboveThres),1);
    plot(timeHours(indexAboveThres)-timeHours(1),maxThreshold,'.r','MarkerSize',18)
    ylim([minThreshold(1)-1 maxThreshold(1)+1]);
end
indexBelowThres = find(lssScalar(dataIndex,8) <= minThreshold);
if ~isempty(indexBelowThres)
    minThreshold = minThreshold*ones(length(indexBelowThres),1);
    plot(timeHours(indexBelowThres)-timeHours(1),minThreshold,'.r','MarkerSize',18)
    ylim([minThreshold(1)-1 maxThreshold(1)+1]);
end
xlabel('Time (h)');
xlim([0 (timeHours(end) - timeHours(1))]);
y=ylabel('Z Vel [m/s]');
set(y, 'Units', 'Normalized', 'Position', [-0.1, 0.5, 0]);
grid on;
%-----------plot DT Rate in next subplot -----------------------------------
subplot(4,1,4)
maxThreshold = 300;
minThreshold = -300;
hold on;
plot(timeHours(dataIndex)-timeHours(1), lssScalar(dataIndex,9),'.r','MarkerSize',18);
% axis tight 
%ylim([-100 100]);
indexAboveThres = find(lssScalar(dataIndex,9) >= maxThreshold);
if ~isempty(indexAboveThres)
    maxThreshold = maxThreshold*ones(length(indexAboveThres),1);
    plot(timeHours(indexAboveThres)-timeHours(1),maxThreshold,'.r','MarkerSize',18)
    ylim([minThreshold(1)-1 maxThreshold(1)+1]);
end
indexBelowThres = find(lssScalar(dataIndex,9) <= minThreshold);
if ~isempty(indexBelowThres)
    minThreshold = minThreshold*ones(length(indexBelowThres),1);
    plot(timeHours(indexBelowThres)-timeHours(1),minThreshold,'.r','MarkerSize',18)
    ylim([minThreshold(1)-1 maxThreshold(1)+1]);
end
xlabel('Time (h)');
xlim([0 (timeHours(end) - timeHours(1))]);
y=ylabel('dt Rate');
set(y, 'Units', 'Normalized', 'Position', [-0.1, 0.5, 0]);
grid on;

%--------Include version number, git, compiler in plot--------------------
ylimit=get(gca,'YLim');
xlimit=get(gca,'XLim');
info = sprintf('%s\n%s\n%s',metadata{2},metadata{3},metadata{4});
text(xlimit(2),ylimit(1),info,...
   'VerticalAlignment','bottom',...
   'HorizontalAlignment','right');
%----------JOIN subplots and set the ylabels closer to the plots to see
%them better-------------------------------------------------------------
samexaxis('join','yld',0.75);


%% Histogram X,Y,Z Velocities
nbins = 100;
%-----Get indexes where there is a position solution----------------------
dataIndex = (lssScalar(:,3) ~= 0);
%-----Get indexes where there is outages----------------------------------
dataIndex1 = (lssScalar(:,3) == 0);
outages = timeHours(dataIndex1)-timeHours(1);
x = zeros(1,length(outages));
VelHist = figure(251);
%-------Plot X vel and outages on same subplot----------------------
subplot(3,1,1)
maxThreshold = 15;
minThreshold = -15;
edges = [minThreshold:((abs(maxThreshold)+abs(minThreshold))/nbins):maxThreshold];
hold on
histogram(lssScalar(dataIndex,6),edges,'FaceColor','b');
indexAboveThres = find(lssScalar(dataIndex,6) >= maxThreshold);
if ~isempty(indexAboveThres)
    maxThreshold = maxThreshold*ones(length(indexAboveThres),1);
    histogram(maxThreshold,edges,'FaceColor','r');
end
indexBelowThres = find(lssScalar(dataIndex,6) <= minThreshold);
if ~isempty(indexBelowThres)
    minThreshold = minThreshold*ones(length(indexBelowThres),1);
    histogram(minThreshold,edges,'FaceColor','r');
end

%ylim([-100 100]);
y=ylabel('X Vel Samples');
set(y, 'Units', 'Normalized', 'Position', [-0.1, 0.5, 0]);
grid on;
hold off
title(['Filename: ' fileStrTitles ', Histogram of X,Y,Z,dt Rate Velocities for Static Data']);
%------------plot Y Vel in next subplot--------------------------------------
subplot(3,1,2)
maxThreshold = 15;
minThreshold = -15;
edges = [minThreshold:((abs(maxThreshold)+abs(minThreshold))/nbins):maxThreshold];
hold on
histogram(lssScalar(dataIndex,7),edges,'FaceColor','b');
indexAboveThres = find(lssScalar(dataIndex,7) >= maxThreshold);
if ~isempty(indexAboveThres)
    maxThreshold = maxThreshold*ones(length(indexAboveThres),1);
    histogram(maxThreshold,edges,'FaceColor','r');
end
indexBelowThres = find(lssScalar(dataIndex,7) <= minThreshold);
if ~isempty(indexBelowThres)
    minThreshold = minThreshold*ones(length(indexBelowThres),1);
    histogram(minThreshold,edges,'FaceColor','r');
end

y=ylabel('Y Vel Samples');
set(y, 'Units', 'Normalized', 'Position', [-0.1, 0.5, 0]);
grid on;
hold off
%-----------plot Z Vel in next subplot--------------------------
subplot(3,1,3)
maxThreshold = 15;
minThreshold = -15;
edges = [minThreshold:((abs(maxThreshold)+abs(minThreshold))/nbins):maxThreshold];
hold on
histogram(lssScalar(dataIndex,8),edges,'FaceColor','b');
indexAboveThres = find(lssScalar(dataIndex,8) >= maxThreshold);
if ~isempty(indexAboveThres)
    maxThreshold = maxThreshold*ones(length(indexAboveThres),1);
    histogram(maxThreshold,edges,'FaceColor','r');
end
indexBelowThres = find(lssScalar(dataIndex,8) <= minThreshold);
if ~isempty(indexBelowThres)
    minThreshold = minThreshold*ones(length(indexBelowThres),1);
    histogram(minThreshold,edges,'FaceColor','r');
end

y=ylabel('Z Vel Samples');
set(y, 'Units', 'Normalized', 'Position', [-0.1, 0.5, 0]);
xlabel('Velocity [m/s]');
grid on;
hold off

%--------Include version number, git, compiler in plot--------------------
ylimit=get(gca,'YLim');
xlimit=get(gca,'XLim');
info = sprintf('%s\n%s\n%s',metadata{2},metadata{3},metadata{4});
text(xlimit(2),ylimit(1),info,...
   'VerticalAlignment','bottom',...
   'HorizontalAlignment','right');

samexaxis('join','yld',0.75);
%----------JOIN subplots and set the ylabels closer to the plots to see
%them better-------------------------------------------------------------




%% Plot the available PRN
PRN = figure(300);
colors = {'.k','.b','.g','.r','.c','.m','.y'};
s =subplot(2,1,1);
hold on;grid on;

for i = 1:32
    % check the valid PRNs
    checkPRN = reshape(lssVector(i,1,:), 1, length(lssVector));
    data = ~isnan(checkPRN)*i;
    idx = (data==0);
    data(idx) = nan;
    
    %plot PRNs with time
    j = mod(i,length(colors));
    if j == 0
        j = length(colors);
    end
    plot(timeHours-timeHours(1), data, colors{j});
end
ax = gca;
ax.YTick = 0:1:32;
ylim([0 32]);
xlabel('Time (h)');
xlim([0 (timeHours(end) - timeHours(1))]);
y=ylabel('PRN');
set(y, 'Units', 'Normalized', 'Position', [-0.1, 0.5, 0]);
title(['Filename: ' fileStrTitles ', Available PRNs: ', num2str(lssScalar(1,1),'%0.0f'), ' - ' , num2str(lssScalar(end,1),'%0.0f'), ' sec']);


%% Plot the Elevation Angle as a function of time over all PRN's
subplot(2,1,2);
hold on;grid on;
minThreshold = 0;
for i = 1:32
%    check the valid PRNs
    checkPRN = reshape(lssVector(i,1,:), 1, length(lssVector));
    data = ~isnan(checkPRN)*i;
    idx = (data==0);
    data(idx) = nan;
    
    dataIndex = ~isnan(data);
%   Get Position data for indexes where the current PRN is available
    satPos =  [reshape(lssVector(i,9,dataIndex), length(lssVector(dataIndex)),1) reshape(lssVector(i,10,dataIndex),length(lssVector(dataIndex)),1) reshape(lssVector(i,11,dataIndex),length(lssVector(dataIndex)),1)];  % Pos Sat
    rcvPos = lssScalar(dataIndex,2:4);
%   Set Variables
    Elev = nan(length(satPos),1);
    Azumith = nan(length(satPos),1);
%   Get Elevation and Azumith
    for j = 1:length(satPos)
        [Elev(j),Azumith(j)] = Calc_Azimuth_Elevation(rcvPos(j,:),satPos(j,:));
    end
    Elev = Elev*180/pi; %convert to degrees
%%%%%%%%%%%%%%%%%%%%%% Plot Data and Legend %%%%%%%%%%%%%%%%%%%%%%%%%%%%
    j = mod(i,length(colors));
    if j == 0
        j = length(colors);
    end
    plot(timeHours(dataIndex)-timeHours(1), Elev, colors{j});
    if ~isempty(Elev)
        indexBelowThres = find(Elev <= minThreshold);
        if ~isempty(indexBelowThres)
            minThresholdvector = i*ones(length(indexBelowThres),1);
            outageHandle = plot(s,timeHours(indexBelowThres)-timeHours(1),minThresholdvector,'xr','MarkerSize',18);
%             legend(outageHandle, 'Below 0 degrees');
        end
    end
%     Legend{i}=strcat('PRN Number', num2str(i));
end
 xlabel('Time (h)');
 xlim([0 (timeHours(end) - timeHours(1))]);
 ylim([0 90]);
 y=ylabel('Elevation Angle [degrees]');
 set(y, 'Units', 'Normalized', 'Position', [-0.1, 0.5, 0]);
 
 %--------Include version number, git, compiler in plot--------------------
ylimit=get(gca,'YLim');
xlimit=get(gca,'XLim');
info = sprintf('%s\n%s\n%s',metadata{2},metadata{3},metadata{4});
text(xlimit(2),ylimit(1),info,...
   'VerticalAlignment','bottom',...
   'HorizontalAlignment','right');
samexaxis('join','yld',0.75);
 
 
%% 3D Skyplot 
sky3D = figure(400);
hold on;grid on;
minThreshold = 0;
firstvalidPRN = 1;
for i = 1:32
%    check the valid PRNs
    checkPRN = reshape(lssVector(i,1,:), 1, length(lssVector));
    data = ~isnan(checkPRN)*i;
    idx = (data==0);
    data(idx) = nan;
    
    dataIndex = ~isnan(data);
%   Get Position data for indexes where the current PRN is available
    satPos =  [reshape(lssVector(i,9,dataIndex), length(lssVector(dataIndex)),1) reshape(lssVector(i,10,dataIndex),length(lssVector(dataIndex)),1) reshape(lssVector(i,11,dataIndex),length(lssVector(dataIndex)),1)];  % Pos Sat
    rcvPos = lssScalar(dataIndex,2:4);
%   Set Variables
    Elev = nan(length(satPos),1);
    Azumith = nan(length(satPos),1);
%   Get Elevation and Azumith
    for j = 1:length(satPos)
        [Elev(j),Azumith(j)] = Calc_Azimuth_Elevation(rcvPos(j,:),satPos(j,:));
    end
    Elev = Elev*180/pi; %convert to degrees
    Azumith = Azumith*180/pi;
%%%%%%%%%%%%%%%%%%%%%% Plot Data and Legend %%%%%%%%%%%%%%%%%%%%%%%%%%%%
    w = mod(i,length(colors));
    if w == 0
        w = length(colors);
    end
    if ~isempty(Elev)
        indexBelowThres = find(Elev <= minThreshold);
        if ~isempty(indexBelowThres)
            Elev(indexBelowThres) = [];
            Azumith(indexBelowThres) = [];
        end
        if i == firstvalidPRN
            skyPlot3d(Azumith, Elev, i*ones(length(Azumith)),colors{w});
        else
            [x, y, z] = skyPlot3d(Azumith, Elev, i*ones(length(Azumith)),colors{w});
        end
    else
        firstvalidPRN = firstvalidPRN + 1;
    end
    
end
%--------Include version number, git, compiler in plot--------------------
ylimit=get(gca,'YLim');
xlimit=get(gca,'XLim');
info = sprintf('%s\n%s\n%s',metadata{2},metadata{3},metadata{4});
text(xlimit(2),ylimit(1),info,...
   'VerticalAlignment','bottom',...
   'HorizontalAlignment','right');
title(['Filename: ' fileStrTitles ]); 


%% Sky 2D
firstvalidPRN = 1;
sky2D = figure(500);
hold on;
for i = 1:32
%    check the valid PRNs
    checkPRN = reshape(lssVector(i,1,:), 1, length(lssVector));
    data = ~isnan(checkPRN)*i;
    idx = (data==0);
    data(idx) = nan;
    
    dataIndex = ~isnan(data);
%   Get Position data for indexes where the current PRN is available
    satPos =  [reshape(lssVector(i,9,dataIndex), length(lssVector(dataIndex)),1) reshape(lssVector(i,10,dataIndex),length(lssVector(dataIndex)),1) reshape(lssVector(i,11,dataIndex),length(lssVector(dataIndex)),1)];  % Pos Sat
    rcvPos = lssScalar(dataIndex,2:4);
%   Set Variables
    Elev = nan(length(satPos),1);
    Azumith = nan(length(satPos),1);
%   Get Elevation and Azumith
    for j = 1:length(satPos)
        [Elev(j),Azumith(j)] = Calc_Azimuth_Elevation(rcvPos(j,:),satPos(j,:));
    end
    Elev = Elev*180/pi; %convert to degrees
    Azumith = Azumith*180/pi;
%%%%%%%%%%%%%%%%%%%%%% Plot Data and Legend %%%%%%%%%%%%%%%%%%%%%%%%%%%%
    w = mod(i,length(colors));
    if w == 0
        w = length(colors);
    end
    if ~isempty(Elev)
        indexBelowThres = find(Elev <= minThreshold);
        if ~isempty(indexBelowThres)
            Elev(indexBelowThres) = [];
            Azumith(indexBelowThres) = [];
        end
        if i == firstvalidPRN
            skyPlot(Azumith, Elev, i*ones(length(Azumith)),colors{w});
        else
            hpol = skyPlot(Azumith, Elev, i*ones(length(Azumith)),colors{w});
        end
    else
        firstvalidPRN = firstvalidPRN + 1;
    end
    
end
%--------Include version number, git, compiler in plot--------------------
ylimit=get(gca,'YLim');
xlimit=get(gca,'XLim');
info = sprintf('%s\n%s\n%s',metadata{2},metadata{3},metadata{4});
text(xlimit(2),ylimit(1),info,...
   'VerticalAlignment','bottom',...
   'HorizontalAlignment','right');
title(['Filename: ' fileStrTitles ]); 


%% Check Where and When outages occur
dataIndex = ~isnan(pos_enu(:,3)); 
currentlyOutage = 0; %bool to keep track of if in an outage or not
outageIndex = 1; %index to keep track of number of outages
numSecondsOutage = nan(length(dataIndex),1);
startOfOutage = nan(length(dataIndex),1);
startOfOutageS = nan(length(dataIndex),1);
for i = 1:length(dataIndex) %loop through entire data set
%--------------The start of a new outage----------------------------------
    if dataIndex(i) == 0 && currentlyOutage == 0 
        numSecondsOutage(outageIndex,1) = 0; %in time
        startOfOutage(outageIndex,1) = Time(i)-Time(1); %in time
        startOfOutageS(outageIndex,1) = lssScalar(i,13); %in samples
        currentlyOutage = 1;
%-------------Currently in an outage--------------------------------------
    else if dataIndex(i) == 0 && currentlyOutage == 1
            numSecondsOutage(outageIndex,1) = numSecondsOutage(outageIndex,1) + 1;
%-------------The end of an outage-----------------------------------------
        else if dataIndex(i) == 1 && currentlyOutage == 1
                currentlyOutage = 0;
                outageIndex = outageIndex + 1;
            end
        end
    end
end
%-----------Get rid of any NaN entries in the outage variables------------
numSecondsOutage = numSecondsOutage(~isnan(numSecondsOutage(:,1)));
startOfOutage = startOfOutage(~isnan(startOfOutage(:,1)));
startOfOutageS = startOfOutageS(~isnan(startOfOutageS(:,1)));
% string format for email
Outagestr = '';
for i = 1:length(numSecondsOutage)
%     Outagestr = strcat(Outagestr, 
    Outagestr = [Outagestr sprintf('%s%.1f%s%d%s%s%.1f%s\n\n','Start: ', startOfOutage(i),' (sec), ',startOfOutageS(i), ' (samples)', ' Lasts for: ', numSecondsOutage(i), ' (sec)')];
end


%% Save Plots to send as attachments
plotFolder = plotpath;
dateStr = ['_' num2str(dateVector(1)) '_' num2str(dateVector(2))];
plotName1 = ['PosSol_' fileStr '.png'];
plotName2 = ['DT_' fileStr '.png'];
plotName3 = ['PRN_' fileStr '.png'];
plotName4 = ['Skyplot2D_' fileStr '.png'];
plotName5 = ['Skyplot3D_' fileStr '.png'];
plotName6 = ['Velocity_' fileStr '.png'];
plotName7 = ['VelocityHist_' fileStr '.png'];
plotName1fig = ['PosSol_' fileStr '.fig'];
plotName2fig = ['DT_' fileStr '.fig'];
plotName3fig = ['PRN_' fileStr '.fig'];
plotName4fig = ['Skyplot2D_' fileStr '.fig'];
plotName5fig = ['Skyplot3D_' fileStr '.fig'];
plotName6fig = ['Velocity_' fileStr '.fig'];
plotName7fig = ['VelocityHist_' fileStr '.fig'];
saveas(PosSol, [plotFolder plotName1]);
saveas(DT, [plotFolder plotName2]);
saveas(PRN, [plotFolder plotName3]);
saveas(sky2D, [plotFolder plotName4]);
saveas(sky3D, [plotFolder plotName5]);
saveas(Vel, [plotFolder plotName6]);
saveas(VelHist, [plotFolder plotName7]);
saveas(PosSol, [plotFolder plotName1fig]);
saveas(DT, [plotFolder plotName2fig]);
saveas(PRN, [plotFolder plotName3fig]);
saveas(sky2D, [plotFolder plotName4fig]);
saveas(sky3D, [plotFolder plotName5fig]);
saveas(Vel, [plotFolder plotName6fig]);
saveas(VelHist, [plotFolder plotName7fig]);
attachments = {[plotFolder plotName1],[plotFolder plotName2],[plotFolder plotName3],[plotFolder plotName4],[plotFolder plotName5],[plotFolder plotName6],[plotFolder plotName7]};
for i = 1:length(NAV)
      saveas(NAV(i), [plotFolder 'NAV' num2str(i) dateStr  '.png']);
      saveas(NAV(i), [plotFolder 'NAV' num2str(i) dateStr  '.fig']);
      attachments{7+i} = [plotFolder 'NAV' num2str(i) dateStr '.png'];
end


%% Setup Output Variables
newLine = sprintf('\n');
subject = ['Filename: ' fileStr ' Lat:' num2str(lat*180/pi) ' Lon:' num2str(lon*180/pi) ' Week:' num2str(dateVector(1)) ' TOW:' num2str(dateVector(2)) ' Av:' percentAvailStr '% ' 'Sigma 3D = ' num2str(std3D) 'm'];
bodyStr = ['Max 2D Error: ', num2str(maxError2D), ' meters ', ...
        '(', num2str(timeMax2D), ' hours)', newLine, newLine ...
        'Max 3D Error: ', num2str(maxError3D), ' meters ',...
        '(', num2str(timeMax3D), ' hours)', newLine, newLine ...
        'Max Height Error: ', num2str(maxHeightError), ' meters ' ...
        '(', num2str(timeMaxHeight), ' hours)', newLine, newLine ...
        'The mean and standard deviation of error in 2D:', newLine ...
        'Mean 2D = ', num2str(mean2D), ' meters', newLine ...
        '1 Std 2D = ', num2str(std2D), ' meters', newLine, newLine ...
        'The mean and standard deviation of error in 3D:', newLine ...
        'Mean 3D = ', num2str(mean3D), ' meters', newLine ...
        '1 Std 3D = ', num2str(std3D), ' meters', newLine, newLine ...
        'Total Availability of position solution was found to be:  ', percentAvailStr, ' %', newLine, newLine ...
        'Outages:', newLine ...
        Outagestr];
        
body = bodyStr;
fid = fopen([plotFolder 'results' dateStr '.txt'], 'w');
fprintf(fid, '%s', body);
