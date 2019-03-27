/*
    Get the maximun daily temperature reading
    in Shannon each day during March 2019.
*/
SELECT
     concat(wth.YEAR, '-', wth.mo, '-', wth.da)                       AS DATE
    ,wth.MAX AS max_temp  -- Maximum temperature reported during the day in Fahrenheit
    ,wth.prcp             -- Total precipitation (rain and/or melted snow) reported during the day in inches and hundredths
    ,stn.name             -- Station name
    ,concat( CAST(stn.lat AS string), ',', CAST(stn.lon AS string))   AS lat_lon
FROM       `bigquery-public-data.noaa_gsod.gsod2019`  AS wth
INNER JOIN `bigquery-public-data.noaa_gsod.stations`  AS stn ON wth.stn = stn.usaf
WHERE  stn.country = 'EI'
  AND wth.YEAR = '2019'
  AND wth.mo = '03'
--  and wth.da = '25'
  AND stn.name = 'SHANNON'
ORDER BY
   stn.name
  ,wth.da
LIMIT 100