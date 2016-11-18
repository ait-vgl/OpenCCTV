# Installing MySQL
## Version 5.6
### Linux

#### 1. Install MySQL Server
``` sh
$ sudo apt-get update
$ sudo apt-get install mysql-server
$ sudo apt-get install mysql-client

$ mysql -u root -p
[Enter root password]
```
If you want to create a new user and grant permissions, follow instructions given in this [digitalocean tutorial](https://www.digitalocean.com/community/tutorials/how-to-create-a-new-user-and-grant-permissions-in-mysql).

#### 2. Install MySQL C++ Connector
``` sh
$ sudo apt-get install libmysqlcppconn-dev
$ sudo apt-get install libmysqlclient-dev
```

#### Grant permission for test ####
CREATE USER 'username'@'%' IDENTIFIED BY 'password';
GRANT ALL PRIVILEGES ON *.* TO 'username'@'%';
FLUSH PRIVILEGES;
