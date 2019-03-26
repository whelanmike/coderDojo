/*
Part 1:
    Use Google BigQuery.
    Query to get temperature and weather station names
    and geo locations for Ireland for a specific day
    in March 2019.
Part 2:
    Export data to geo visualization in Google Data Studio
*/
SELECT 
     concat(wth.year, '-', wth.mo, '-', wth.da)                       AS date
--  ,wth.temp -- temperature Fahrenheit
    ,round( ( cast( wth.temp as float64 ) -32 ) * 5/9, 2 )            AS temp_celcius
--  ,wth.max -- Maximum temperature reported during the day in Fahrenheit
    ,round( ( cast( wth.max as float64 ) -32 ) * 5/9, 2 )             AS max_temp_celcius
    ,wth.prcp -- Total precipitation (rain and/or melted snow) reported during the day in inches and hundredths
    ,wth.slp   -- sea level pressure
    ,stn.name  -- Station name
    ,concat( cast(stn.lat as string), ',', cast(stn.lon as string))   AS lat_lon
    ,stn.elev
FROM       `bigquery-public-data.noaa_gsod.gsod2019`  AS wth
inner join `bigquery-public-data.noaa_gsod.stations`  AS stn on wth.stn = stn.usaf 
where  stn.country = 'EI'
  and wth.year = '2019'
  and wth.mo = '03'
  and wth.da = '25'
order by 
   stn.name
  ,wth.temp
LIMIT 100