### **Usage:**
(Requires [DuckDB v1.1](https://duckdb.org/2024/09/09/announcing-duckdb-110.html) or later)
1. Set file path to csv location e.g. <br>
    `set file_search_path = 'C:\Users\user_name\Downloads\';`
1. Run create macro script (`generate_table_from_csv.sql`).
1. Generate DDL for table from csv file, e.g. <br>
    `select definition from generate_table_from_csv('some_interesting_file.csv');`

1. Copy the generated sql into a new editor window.
1. Review/amend the following before executing
    - table name (+ schema name)
    - columns names
    - data types
    - field separator (normally comma, but often pipe)
    - file headers
    - store reject data (invalid e.g. datatype, number of columns)
    - ignore errors (use store_rejects option instead for DQ)
    - formatting options, e.g. date & time

### **Note**
To DROP MACRO <br>
`drop macro table get_csv_file_schema;`


### **Sample execution & output**

```sql
set file_search_path = 'C:\downloads';

select definition from generate_table_from_csv('test_result_2022.csv');
```

```sql
--create table schema_name.test_result_2022 as  -- **** RENAME AS REQUIRED ****                                                  
    select                                                                                                                       
           test_id                                                                                                               
          ,vehicle_id                                                                                                            
          ,test_date                                                                                                             
          ,test_class_id                                                                                                         
          ,test_type                                                                                                             
          ,test_result                                                                                                           
          ,test_mileage                                                                                                          
          ,postcode_area                                                                                                         
          ,make                                                                                                                  
          ,model                                                                                                                 
          ,colour                                                                                                                
          ,fuel_type                                                                                                             
          ,cylinder_capacity                                                                                                     
          ,first_use_date                                                                                                        
    from  read_csv                                                                                                               
            ('C:\downloads\test_result_2022.csv'                                                                                 
            ,sep=','                                                                                                             
            --,quote='"' -- Include if csv data is quoted.                                                                       
            --,DATEFORMAT = '%m/%d/%Y' -- Convert Dates to ISO-8601 format.                                                      
            ,header = True -- comment out if no field names in file. Required names & types can be added below in columns section
            ,store_rejects = True  -- select * FROM reject_errors;                                                               
            --,ignore_errors=True  -- Mutually Exclusive with store_rejects option.                                              
            ,columns =                                                                                                           
                {                                                                                                                
                 'test_id'             :'BIGINT'                                                                                 
                ,'vehicle_id'          :'BIGINT'                                                                                 
                ,'test_date'           :'DATE'                                                                                   
                ,'test_class_id'       :'BIGINT'                                                                                 
                ,'test_type'           :'VARCHAR'                                                                                
                ,'test_result'         :'VARCHAR'                                                                                
                ,'test_mileage'        :'BIGINT'                                                                                 
                ,'postcode_area'       :'VARCHAR'                                                                                
                ,'make'                :'VARCHAR'                                                                                
                ,'model'               :'VARCHAR'                                                                                
                ,'colour'              :'VARCHAR'                                                                                
                ,'fuel_type'           :'VARCHAR'                                                                                
                ,'cylinder_capacity'   :'BIGINT'                                                                                 
                ,'first_use_date'      :'DATE'                                                                                   
                }                                                                                                                
        )                                                                                                                        
    ;
```
