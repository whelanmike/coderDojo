create or replace macro generate_table_from_csv(csv_file_name) as table 
	with c_table_sample as 
		(
		select * from read_csv_auto(csv_file_name::varchar) limit 1000
		)
		,c_col_metadata as
		(
		select 
			   row_number() over() as col_id
			  ,column_name
			  ,len(column_name)  as col_len
			  ,max(col_len) over()  as mx_len
			  ,repeat(' ', (mx_len - col_len) + 3)  as spacing
			  ,column_type
		from  (describe table c_table_sample)
		)
	select '--create table schema_name.' || lower(replace(csv_file_name, '.csv', '')) ||  ' as  -- **** RENAME AS REQUIRED ****' as definition
	union all
	select '    select '
	union all
	select 
	      case when col_id =1 then '           ' else '          ,' end  ||  lower(column_name) 
	from  c_col_metadata
	union all 
	select '    from  read_csv'
	union all 
	select '            (''' || current_setting('file_search_path') || '\' || csv_file_name || '''' 
	union all 
	select '            ,sep='',' || ''''
	union all 
	select '            --,quote=''"' || '''' || ' -- Include if csv data is quoted.'
	union all 
	select '            --,DATEFORMAT = ''' || '%m/%d/%Y' || ''''  ||    ' -- Convert Dates to ISO-8601 format.' 
	union all 
	select '            ,header = True' || ' -- comment out if no field names in file. Required names & types can be added below in columns section.'
	union all 
	select '            ,store_rejects = True' || '  -- select * FROM reject_errors;'
	union all
	select '            --,ignore_errors=True' || '  -- Mutually Exclusive with store_rejects option.'
	union all 
	select '            ,columns = ' 
	union all 
	select '                { '       
	union all
	select 
          case when col_id =1 then '                 ' else '                ,' end  ||  '''' || lower(column_name) || '''' 
		  || spacing || ':''' || column_type || ''''
		  as output
	from  c_col_metadata
	union all 
	select '                } '       
	union all 
	select '        )'      
	union all 
	select '    ;'      
;
