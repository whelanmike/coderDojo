/*
	get the maximum temperature for each 
	weather station for the month of march 2019.
	Return the day on which this occurred.
*/
with c_wetaher as 
    (
    SELECT 
         stn.name                                                            AS station_name
        ,concat ( wth.year , wth.mo , wth.da )                               AS date
        ,row_number() over ( partition by stn.name order by wth.max desc )   AS temperature_rank
        ,wth.max                                                             AS max_temperature
    
    FROM `bigquery-public-data.noaa_gsod.gsod2019`        AS wth
    inner join `bigquery-public-data.noaa_gsod.stations`  AS stn on wth.stn = stn.usaf 
    where  
        stn.country  = 'EI'
        and wth.year = '2019'
        and wth.mo   = '03'
    )
  
    select 
        * 
    from c_wetaher w
    where 
        w.temperature_rank = 1
    order by 
        w.station_name 
