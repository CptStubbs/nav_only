function [gpsTimeOfWeek, gpsWeekNum]=ymds2gps(YMDS)
% ymds2gps Computes the calendar date to GPS time (sec) and GPS week.
% Iutput : GMT 협정시 (Greenwich Mean Time, 그리니치 표준시)
%          YMDS (1) : Year
%          YMDS (2) : Month (1~12)
%          YMDS (3) : Day
%          YMDS (4) : Sec (0~86400)
% 대한민국은 GMT + 9시간
% Output : gpsTimeOfWeek (GPS Sec)
%          gpsWeekNum (GPS Week)


% Constants
    SEC_PER_DAY     = 86400;

    % Extract YMDS
    year    = YMDS(1);
    month   = YMDS(2);
    day     = YMDS(3);
    sec     = YMDS(4);
    
    % Compute julian day number
    a = floor( (14 - month) / 12 );
    y = year + 4800 - a;
    m = month + 12*a - 3;
    
    % Assume the Gregorian calendar is used (leap seconds not included)
    JD = day...
       + floor( (153 * m + 2) / 5)...
       + 365 * y...
       + floor( y / 4)...
       - floor( y / 100)...
       + floor( y / 400)...
       - 32045 ...
       - 0.5 ...
       + (sec/SEC_PER_DAY);    
   
    % Constants
    SEC_PER_DAY     = 86400;
    NUM_GPS_WEEKS   = 1024;  % Number of GPS weeks before a week rollover
        
    
   % Julian day number of the birthday of GPS (Midnight of January 5, 1980)
   % Can compute using January 6, 1980, HMS = 00:00:00
   JD_GPS = 2444244.5;   
   
   % Compute the number of elapsed (decimal) days since start of GPS
   nDays = JD - JD_GPS;
   
   % Compute seconds after midnight (SAM)
   sam  = ( nDays - floor(nDays) ) * SEC_PER_DAY;
   
    % Initial GPS week number
    gpsWeekNum = floor(nDays/7);
    
    % Keep track of the number of week rollovers
    gpsCycle  = floor( gpsWeekNum / NUM_GPS_WEEKS );

    %  Report GPS week number modulo 1024
    gpsWeekNum = mod(gpsWeekNum,NUM_GPS_WEEKS)+NUM_GPS_WEEKS;

    % Number of days into the current week
    nDays = mod(floor(nDays),7);

    % Compute GPS time of week (seconds since Saturday/Sunday midnight)
    gpsTimeOfWeek = round(nDays * SEC_PER_DAY + sam);
    