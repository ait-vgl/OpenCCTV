# Creating Analytic Results Database
## Store OpenCCTV analytic results
### MySQL

#### Run SQL script to create results database and table
```sh
$ mysql -u username -p
Enter password: <Enter MySQL password>
mysql> source /path_to_sql_script/opencctv_results_db.sql;
```

Example
```sh
$ mysql -u root -p
Enter password: <Enter MySQL root password>
mysql> source ~/Downloads/OpenCCTV/opencctv-dependencies/analytic_results_database/opencctv_results_db.sql;
```

Analytic results table:-

![analytic results table diagram comes here.](opencctv_results_table.JPG "Analytic Runner design")