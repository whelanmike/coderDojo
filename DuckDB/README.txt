Usage:
- Requires DuckDB v1.1 or later
- Set file path to csv location e.g.
    set file_search_path = 'C:\Users\user_name\Downloads\';
- Generate DDL for table from csv file, e.g.
    select definition from generate_table_from_csv('some_interesting_file.csv');

- Copy the generated sql into a new editor window.
- Review/amend the following before executing
    - table name (+ schema name)
    - columns names
    - data types
    - field separator (normally comma, but often pipe)
    - file headers
    - store reject data (invalid e.g. datatype, number of columns)
    - ignore errors (use store_rejects option instead for DQ)
    - formatting options, e.g. date & time

        