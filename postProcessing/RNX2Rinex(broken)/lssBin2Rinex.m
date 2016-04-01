function [] = lssBin2Rinex( fileName )
%% Function to convert an lssbinaries file to Rinex format for further analysis:
% Inputs:
%   - filename (name of the binary file being analyzed
% Ouputs: 
%   - generates a text file that is saved in the following format
%       MM_DD_YYYY_rinex.txt
%   - saves the file in the TestFiles directory
%% File settings
% fileName    = 'lssBinaries_11_1_2015.bin';    % file name 
myfile      = dir(fileName);        % file info
fileSize    = myfile.bytes;         % file size
fid         = fopen(fileName, 'rb');% FID
%% Set Truth or Mean for reference position
truthOrMeanRefStr = 't'; % 'm' for mean ref, 't' for truth ref
%% IF using TRUTH reference position, Set orz for truth reference position
% DLC antenna reference position: 
% - Lat/Lon/Hght: 40.00749295305 /-105.26167642616/1630.40495370706
% - XYZ: [-1288084.72435924,-4720848.72539722,4079671.03660523]
% orz = [-1288084.72435924,-4720848.72539722,4079671.03660523]; % DLC

% 1B81 antenna
orz = [-1288160.31484243,-4720790.09567223,4079712.76313084];   % 1B81

%% LSS file settings
nChannels       = 12;   % Number of channels
bytesInEpoch    = 1432; % How many bytes in 1 epoch data: 136 + 1248 + 48

%% skip the data
seek_ms	= 0;	% amount of the data to skip (unit: seconds)
fseek(fid, seek_ms*bytesInEpoch,'bof');

%% order for reading bytes: 'n' (default), 'b', 'l', 's', 'a'
orb = 'n';

%% scalar data type
scaType = { 'int32',   'int32',   'int32', ...  	% newKal, newMeas, valid pos
            'uint32',  'float64',  ...              % Week, TOW
            'float64', 'float64', 'float64', 'float64',...% position X/Y/Z, dt
            'float64', 'float64', 'float64', 'float64',...% velocity X/Y/Z, dt rate
            'float64', 'float64', 'float64', ...   	% HDOP, VDOP, TDOP
            'uint64',  'float32', ...               % Sample number: Integer, Fraction
            'uint32',  'uint32',   'uint32'};       % number of satellites, valid L1/L2
        
        
%% vector data type
vecType = { 'int32',  'uint32',...                      % validPRN, PRN
            'float64','float64','float64','float64',... % PR1, Carr1, Dopp1, CNo1
            'float64','float64','float64','float64',... % PR2, Carr2, Dopp2, CNo2
            'float64','float64','float64','float64'};   % Sat X/Y/Z/Clk

%% Pre-define the variable size
dataLen     = floor((fileSize - seek_ms*bytesInEpoch) / (bytesInEpoch));    % data length
lssScalar   = nan(dataLen, length(scaType)); % position data in ECEF frame
lssVector   = nan(32, length(vecType), dataLen);  % data for each PRN
pos_enu     = nan(dataLen, 5);      % position data in ENU frame
displayStep = round(dataLen / 10);  % Status display step
temppos = nan(dataLen, 3); 
disp('Start LSS decoding ...');

%% Read and save the data
for i = 1: dataLen%floor(fileSize/bytesInEpoch)
    
    % check the end of file
    if(feof(fid))
        break;
    end
 
    % skip the unnecessary data
    tempSkip = fread(fid,10,'int32'); % 40 for p_thread
    
    % read scalar varialbles
    for j = 1:length(scaType)
        lssScalar(i,j) = fread(fid,1, scaType{j}, orb);
    end
    
    % Read in position data
    if lssScalar(i,3) == 1 % if valid position solution
    pos_enu(i,1:2) = lssScalar(i,4:5); % time info
    temppos(i,1:3) = lssScalar(i,6:8); % set a temp matrix with RAW position data

    end
    % read channel data
    tempMat = nan(nChannels, length(vecType)); %initilize the temp
    for k = 1:length(vecType)
        for j = 1:nChannels
            tempMat(j,k) = fread(fid,1,vecType{k}, orb); % read 1 by 1
        end
    end
    % save the data into lssVector
    for j = 1:nChannels
        % checkt #satellites & PRN validity
        if j <= lssScalar(i,19) && tempMat(j,1) == 1
            prn                  = tempMat(j,2);  % PRN
            lssVector(prn, 1, i) = tempMat(j,3);  % PR1
            lssVector(prn, 2, i) = tempMat(j,4);  % Carr1
            lssVector(prn, 3, i) = tempMat(j,5);  % Dopp1
            lssVector(prn, 4, i) = tempMat(j,6);  % CNo1
            lssVector(prn, 5, i) = tempMat(j,7);  % PR2
            lssVector(prn, 6, i) = tempMat(j,8);  % Carr2
            lssVector(prn, 7, i) = tempMat(j,9);  % Dopp2
            lssVector(prn, 8, i) = tempMat(j,10);  % CNo2
            lssVector(prn, 9, i) = tempMat(j,11);  % Sat X
            lssVector(prn, 10, i) = tempMat(j,12);  % Sat Y
            lssVector(prn, 11, i) = tempMat(j,13);  % Sat Z
            lssVector(prn, 12, i) = tempMat(j,14);  % Sat Clk           
        end
    end
    % status display
    if rem(i, displayStep) == 0
        disp(['Processing ', num2str(i*bytesInEpoch/fileSize*100, '%2.0f'), ' %']);
    end
end
%% -------------------------Open New File--------------------------------
weekNumber = pos_enu(1,1);
TOW = pos_enu(1,2);
calcTime = calcDateTime(weekNumber,TOW);

fid = fopen(['TestFiles/' num2str(calcTime.month) '_' num2str(calcTime.day) '_' num2str(calcTime.year) '_rinex.txt'],'w');  
%% ---------------------------Header Data--------------------------------
% ------------ Set Rinex Version / Type ---------------------------------
versionStr = fprintf(fid,'%9.2f%-11s',2.11,''); %Format F9.2,11X 
fileTypeStr = fprintf(fid,'%-20s', 'O'); %Format A1,19X
satSysStr = fprintf(fid,'%-20s', 'M'); %Format A1,19X
headLabVersion = fprintf(fid,'%s\n', 'RINEX VERSION / TYPE'); %Format A20


commentLine = fprintf(fid,'%-60s', ' '); %Format A60 Place any comments here or leave blank
headLabComment = fprintf(fid,'%-20s\n', 'COMMENT '); %Format A20

% ------------ Set Program and who it was run by-------------------------
programStr = fprintf(fid,'%-20s', 'MATLAB'); %Format A20
runByStr = fprintf(fid,'%-20s', 'Space Sciences'); %Format A20
dateRun = datetime;
dateRunStr = fprintf(fid,'%-20s',datestr(dateRun)); %Format A20
headLabPgmRunDate = fprintf(fid,'%s\n','PGM / RUN BY / DATE ');

commentLine = fprintf(fid,'%-60s', ' '); %Format A60 Place any comments here or leave blank
headLabComment = fprintf(fid,'%-20s\n', 'COMMENT '); %Format A20

% -------------- Set name and number of antenna marker--------------------
markerNameStr = fprintf(fid,'%-60s', '|Unknown| '); %Format A60
headLabMarkerName = fprintf(fid,'%s\n','MARKER NAME         ');%Format A20

markerNumStr = fprintf(fid,'%-60s', '|Unknown| '); %Format A60
headLabMarkerNum = fprintf(fid,'%s\n','MARKER NUMBER       ');%Format A20

% ---------------Set Observer and Agency----------------------------------
observerStr = fprintf(fid,'%-20s', 'Space Sciences'); %Format A20
agencyStr = fprintf(fid,'%-40s','University of Colorado at Boulder'); %Format A40
headLabObsAgen = fprintf(fid,'%s\n','OBSERVER / AGENCY   ');

commentLine = fprintf(fid,'%-60s', ' '); %Format A60 Place any comments here or leave blank
headLabComment = fprintf(fid,'%-20s\n', 'COMMENT '); %Format A20

commentLine = fprintf(fid,'%-60s', ' '); %Format A60 Place any comments here or leave blank
headLabComment = fprintf(fid,'%-20s\n', 'COMMENT '); %Format A20

commentLine = fprintf(fid,'%-60s', ' '); %Format A60 Place any comments here or leave blank
headLabComment = fprintf(fid,'%-20s\n', 'COMMENT '); %Format A20

% --------------- Set Receiver number, type, and version --------------
receiverNumStr = fprintf(fid,'%-20s', '|Unknown| '); %Format A20
receiverTypeStr = fprintf(fid,'%-20s', '|Unknown| '); %Format A20
receiverVersionStr = fprintf(fid,'%-20s', '|Unknown| '); %Format A20
headLabRecNTV = fprintf(fid,'%s\n','REC # / TYPE / VERS ');

% -------------- Set Antenna Number and Type---------------------------
antennaNumStr = fprintf(fid,'%-20s', '|Unknown| '); %Format A20
antennaTypeStr = fprintf(fid,'%-40s', '|Unknown| '); %Format A40
headLabAntNT = fprintf(fid,'%s\n','ANT # / TYPE        ');

% ------------------ Get approximate position---------------------------
valIndex = ~isnan(temppos(:,1));
approxX = mean(temppos(valIndex,1));
approxY = mean(temppos(valIndex,2));
approxZ = mean(temppos(valIndex,3));

approxXStr = fprintf(fid,'%15.4f',approxX); %format F14.4
approxYStr = fprintf(fid,'%15.4f',approxY); %format F14.4
approxZStr = fprintf(fid,'%15.4f',approxZ); %format F14.4

fprintf(fid,'%-15s','');
headLabApproxPos = fprintf(fid,'%s\n','APPROX POSITION XYZ ');

% -----------------Get Antenna Height/Eccentricities---------------------
antennaHeightStr = fprintf(fid,'%-20s', '|Unknown| '); %format F14.4
antennaEastEccStr = fprintf(fid,'%-20s', '|Unknown| '); %format F14.4
antennaNorthEccStr = fprintf(fid,'%-20s', '|Unknown| '); %format F14.4
headLabApproxPos = fprintf(fid,'%s\n','ANTENNA: DELTA H/E/N');

% -----------Set L1 and L2 Wavelength Factors (GPS Only)-----------------
L1WaveFacStr = fprintf(fid,'%6d',1); %DEFAULT (Full Cycle Ambiguities) format I6
L2WaveFacStr = fprintf(fid,'%6d',1); %DEFAULT (Full Cycle Ambiguities) format I6
fprintf(fid,'%-48s',''); 
headLabWavFac = fprintf(fid,'%s\n','WAVELENGTH FACT L1/2');

% ----------Set Number / Types of Observations --------------------------
numObsStr = fprintf(fid,'%6d',8); %format I6
typeL1Str = fprintf(fid,'%6s','L1'); %format 4X, 2A1
typeL2Str = fprintf(fid,'%6s','L2'); %format 4X, 2A1
typeP1Str = fprintf(fid,'%6s','P1'); %format 4X, 2A1
typeP2Str = fprintf(fid,'%6s','P2'); %format 4X, 2A1
typeD1Str = fprintf(fid,'%6s','D1'); %format 4X, 2A1
typeD2Str = fprintf(fid,'%6s','D2'); %format 4X, 2A1
typeS1Str = fprintf(fid,'%6s','S1'); %format 4X, 2A1
typeS2Str = fprintf(fid,'%6s','S2'); %format 4X, 2A1
fprintf(fid,'%-6s','');
headLabTypeObs = fprintf(fid,'%s\n','# / TYPES OF OBSERV ');
% ----------Set Interval -----------------------------------------------
interval = diff(pos_enu(:,2));
intervalStr = fprintf(fid,'%10.3f',mean(interval)); %format F10.3
fprintf(fid,'%-50s','');
headLabInterval = fprintf(fid, '%s\n','INTERVAL            ');
% -----------Set Time of First Observ------------------------------------
weekNumber = pos_enu(1,1);
TOW = pos_enu(1,2);
calcTime = calcDateTime(weekNumber,TOW);
year = fprintf(fid,'%6d',calcTime.year);
month = fprintf(fid,'%6d',calcTime.month);
day = fprintf(fid,'%6d',calcTime.day);
hour = fprintf(fid,'%6d',calcTime.hour);
minute = fprintf(fid,'%6d',calcTime.minute);
second = fprintf(fid,'%14.7f',calcTime.second);
timeSys = fprintf(fid,'%8s','GPS');
fprintf(fid,'%8s','');
headLabTime1st = fprintf(fid,'%s\n','TIME OF FIRST OBS   ');

% --------------Set Time of Last Observ-----------------------------------
weekNumber = pos_enu(end,1);
TOW = pos_enu(end,2);
calcTime = calcDateTime(weekNumber,TOW);
year = fprintf(fid,'%6d',calcTime.year);
month = fprintf(fid,'%6d',calcTime.month);
day = fprintf(fid,'%6d',calcTime.day);
hour = fprintf(fid,'%6d',calcTime.hour);
minute = fprintf(fid,'%6d',calcTime.minute);
second = fprintf(fid,'%14.7f',calcTime.second);
timeSys = fprintf(fid,'%8s','GPS');
fprintf(fid,'%8s','');
headLabTime1st = fprintf(fid,'%s\n','TIME OF LAST OBS    ');
% ----Set num satellites and num observ for each prn --------------------
numSat = length(lssVector(:,1,1));
numSatStr = fprintf(fid,'%6d%-54s',numSat,''); %format I6
headLabNumSat = fprintf(fid,'%s\n','# OF SATELLITES     ');

L1count = zeros(numSat,1);
L2count = zeros(numSat,1);
P1count = zeros(numSat,1);
P2count = zeros(numSat,1);
D1count = zeros(numSat,1);
D2count = zeros(numSat,1);
S1count = zeros(numSat,1);
S2count = zeros(numSat,1);
for j = 1:numSat
    for i = 1:dataLen
    %---------------------L1 Count-------------------------------------
            if ~isnan(lssVector(j, 2, i)) && lssVector(j, 2, i) ~= 0
                L1count(j) = L1count(j) + 1;
            end
    %---------------------L2 Count-------------------------------------
            if ~isnan(lssVector(j, 6, i)) && lssVector(j, 6, i) ~= 0
                L2count(j) = L2count(j) + 1;
            end
    %---------------------P1 Count---------------------------------------
            if ~isnan(lssVector(j, 1, i)) && lssVector(j, 1, i) ~= 0
                P1count(j) = P1count(j) + 1;
            end
    %---------------------P2 Count---------------------------------------
            if ~isnan(lssVector(j, 5, i)) && lssVector(j, 5, i) ~= 0
                P2count(j) = P2count(j) + 1;
            end
    %---------------------D1 Count---------------------------------------
            if ~isnan(lssVector(j, 3, i)) && lssVector(j, 3, i) ~= 0
                D1count(j) = D1count(j) + 1;
            end
    %---------------------D2 Count---------------------------------------
            if ~isnan(lssVector(j, 7, i)) && lssVector(j, 7, i) ~= 0
                D2count(j) = D2count(j) + 1;
            end
    %---------------------S1 Count---------------------------------------
            if ~isnan(lssVector(j, 4, i)) && lssVector(j, 4, i) ~= 0
                S1count(j) = S1count(j) + 1;
            end
    %---------------------S2 Count---------------------------------------
            if ~isnan(lssVector(j, 8, i)) && lssVector(j, 8, i) ~= 0
                S2count(j) = S2count(j) + 1;
            end
    end
% ------------------Putting all together in Header-----------------------
    if j > 9
        prnNumStr = fprintf(fid,'%6s',['G' num2str(j)]);
    else
        prnNumStr = fprintf(fid,'%6s',['G0' num2str(j)]);
    end
    L1countStr = fprintf(fid,'%6d',L1count(j));
    L2countStr = fprintf(fid,'%6d',L2count(j));
    P1countStr = fprintf(fid,'%6d',P1count(j));
    P2countStr = fprintf(fid,'%6d',P2count(j));
    D1countStr = fprintf(fid,'%6d',D1count(j));
    D2countStr = fprintf(fid,'%6d',D2count(j));
    S1countStr = fprintf(fid,'%6d',S1count(j));
    S2countStr = fprintf(fid,'%6d',S2count(j));
    fprintf(fid,'%6s','');
    headLabNumObs = fprintf(fid,'%s\n','PRN / # OF OBS      ');
      
end
% ---------------- end of header line------------------------------------
endStr = fprintf(fid,'%60s','');
headLabEnd = fprintf(fid,'%s\n','END OF HEADER       ');



%% -----------------------TESTING DIFFERENT OBSERVABLES-------------------
% -----------------------(No effect on Rinex Output File) ----------------
satCount = zeros(1,dataLen);
carrier2 =  lssVector(3, 6, :);
dopp2 = lssVector(3, 7, :);
cno2 = lssVector(3, 8, :);
PR2 = lssVector(3, 5, :);
%% ------------------------Observation Data-------------------------------
disp('Start Rinex Processing ...');
for i = 1:dataLen
   allPRNStr = '';
%---------------------Calculate Date/Time---------------------------------
        weekNumber = pos_enu(i,1);
        TOW = pos_enu(i,2);
        calcTime = calcDateTime(weekNumber,TOW);
        year = num2str(calcTime.year);
        year = str2num(year(3:4));
        year = sprintf('%2d',year);
        month = sprintf('%2d',calcTime.month);
        day = sprintf('%2d',calcTime.day);
        hour = sprintf('%2d',calcTime.hour);
        minute = sprintf('%2d',calcTime.minute);
        second = sprintf('%11.7f',calcTime.second);
        epochflag = sprintf('%2s','0');
        dateStr = sprintf('%3s%3s%3s%3s%3s%11s%3s',year, month, day, hour, minute, second,epochflag);
   for j = 1:32  
%-------------------List of Epoch Satellites-----------------------------
        if ~isnan(lssVector(j, 1, i))
            satCount(i) = satCount(i)+1;
            if satCount(i) > 12
                allPRNStr = [allPRNStr blanks(12) sprintf('\n') blanks(32)];
            end
            allPRNStr = [allPRNStr sprintf('G%2d',j)];
        end
   end
   satPresentStr = sprintf('%3d',satCount(i));
%---------------------Putting All Together-------------------------------
   observationHeaderStr = sprintf('%s%3s%-36s\n',dateStr,satPresentStr,allPRNStr);
   
   
   observationStr = '';
   for j = 1:32
       if ~isnan(lssVector(j, 1, i))
%---------------------L1 Observable-------------------------------------
            if ~isnan(lssVector(j, 2, i)) && lssVector(j, 2, i) ~= 0
                L1carrierStr = sprintf('%14.3f',lssVector(j, 2, i));
                L1LoLindStr = sprintf(' ');
                L1sigStrengthindStr = sprintf(' ');
                L1Str = [L1carrierStr L1LoLindStr L1sigStrengthindStr];
            else
                L1Str = blanks(20);
            end
%---------------------L2 Observable-------------------------------------
            if ~isnan(lssVector(j, 6, i)) && lssVector(j, 6, i) ~= 0
                L2carrierStr = sprintf('%14.3f',lssVector(j, 6, i));
                L2LoLindStr = sprintf(' ');
                L2sigStrengthindStr = sprintf(' ');
                L2Str = [L2carrierStr L2LoLindStr L2sigStrengthindStr];
            else
                L2Str = blanks(16);
            end
%---------------------P1 Observable---------------------------------------
            if ~isnan(lssVector(j, 1, i)) && lssVector(j, 1, i) ~= 0
                P1carrierStr = sprintf('%14.3f',lssVector(j, 1, i));
                P1LoLindStr = sprintf(' ');
                P1sigStrengthindStr = sprintf(' ');
                P1Str = [P1carrierStr P1LoLindStr P1sigStrengthindStr];
            else
                P1Str = blanks(16);
            end
%---------------------P2 Observable---------------------------------------
            if ~isnan(lssVector(j, 5, i)) && lssVector(j, 5, i) ~= 0
                P2carrierStr = sprintf('%14.3f',lssVector(j, 5, i));
                P2LoLindStr = sprintf(' ');
                P2sigStrengthindStr = sprintf(' ');
                P2Str = [P2carrierStr P2LoLindStr P2sigStrengthindStr];
            else
                P2Str = blanks(16);
            end
%---------------------D1 Observable---------------------------------------
            if ~isnan(lssVector(j, 3, i)) && lssVector(j, 3, i) ~= 0
                D1carrierStr = sprintf('%14.3f',lssVector(j, 3, i));
                D1LoLindStr = sprintf(' ');
                D1sigStrengthindStr = sprintf(' ');
                D1Str = [D1carrierStr D1LoLindStr D1sigStrengthindStr];
            else
                D1Str = blanks(16);
            end
%---------------------D2 Observable---------------------------------------
            if ~isnan(lssVector(j, 7, i)) && lssVector(j, 7, i) ~= 0
                D2carrierStr = sprintf('%14.3f',lssVector(j, 7, i));
                D2LoLindStr = sprintf(' ');
                D2sigStrengthindStr = sprintf(' ');
                D2Str = [D2carrierStr D2LoLindStr D2sigStrengthindStr];
            else
                D2Str = blanks(16);
            end
%---------------------S1 Observable---------------------------------------
            if ~isnan(lssVector(j, 4, i)) && lssVector(j, 4, i) ~= 0
                S1carrierStr = sprintf('%14.3f',lssVector(j, 4, i));
                S1LoLindStr = sprintf(' ');
                S1sigStrengthindStr = sprintf(' ');
                S1Str = [S1carrierStr S1LoLindStr S1sigStrengthindStr];
            else
                S1Str = blanks(16);
            end
%---------------------S2 Observable---------------------------------------
            if ~isnan(lssVector(j, 8, i)) && lssVector(j, 8, i) ~= 0
                S2carrierStr = sprintf('%14.3f',lssVector(j, 8, i));
                S2LoLindStr = sprintf(' ');
                S2sigStrengthindStr = sprintf(' ');
                S2Str = [S2carrierStr S2LoLindStr S2sigStrengthindStr];
            else
                S2Str = blanks(16);
            end
%---------------------Putting All Together---------------------------------
            observationStr = [observationStr sprintf('%s%s%s%s%s\n%s%s%s\n',L1Str,L2Str,P1Str,P2Str,D1Str,D2Str,S1Str,S2Str)];
            
        end
   end
   fprintf(fid,'%s%s',observationHeaderStr,observationStr);
   if rem(i, displayStep) == 0
        disp(['Writing ', num2str(i*bytesInEpoch/fileSize*100, '%2.0f'), ' %']);
   end
end
disp(['Finished Writing Rinex File: ' num2str(calcTime.month) '_' num2str(calcTime.day) '_' num2str(calcTime.year) '_rinex.txt']);
