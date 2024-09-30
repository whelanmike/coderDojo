**Usage:**
<br>(Requires DuckDB v1.1 or later)
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

**Note**
To DROP MACRO <br>
`drop macro table get_csv_file_schema;`
