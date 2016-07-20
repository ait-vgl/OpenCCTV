# Installing Java
## Oracle Java 7 (JDK + JRE)
### Linux

```sh
$ sudo add-apt-repository ppa:webupd8team/java
$ sudo apt-get update
$ sudo apt-get install oracle-java7-installer
```

Checking currently running Java version
``` sh
$ update-alternatives --display java
```

Selecting a Java version
``` sh
$ update-alternatives --config java
```