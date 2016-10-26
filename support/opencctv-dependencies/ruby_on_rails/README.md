# Installing Ruby on Rails
## Ruby 2.x and Rails 4.y
### Linux

#### 1. Install dependencies
``` sh
$ sudo apt-get update
$ sudo apt-get install git-core curl zlib1g-dev build-essential libssl-dev libreadline-dev libyaml-dev libsqlite3-dev sqlite3 libxml2-dev libxslt-dev libpq-dev
```

#### 2. Install Ruby environment (rbenv)
``` sh
$ cd ~
$ git clone git://github.com/sstephenson/rbenv.git .rbenv
$ echo 'export PATH="$HOME/.rbenv/bin:$PATH"' >> ~/.bashrc
$ echo 'eval "$(rbenv init -)"' >> ~/.bashrc
$ exec $SHELL
```
Installing the ruby-build plugin for rbenv
``` sh
$ git clone git://github.com/sstephenson/ruby-build.git ~/.rbenv/plugins/ruby-build
$ echo 'export PATH="$HOME/.rbenv/plugins/ruby-build/bin:$PATH"' >> ~/.bashrc
$ exec $SHELL
```

#### 3. Install Ruby
Checking released Ruby versions
``` sh
$ rbenv install --list
```
Select latest version and install it.
``` sh
$ rbenv install 2.2.2
$ rbenv global 2.2.2
```
Checking Ruby version
``` sh
$ ruby -v
````
The following step is to tell Ruby Gems not to install the documentation for each package locally.
``` sh
$ echo "gem: --no-ri --no-rdoc" > ~/.gemrc
```
Updating Ruby Gem version
``` sh
$ gem update --system
```

#### 4. Install Rails
``` sh
$ sudo add-apt-repository ppa:chris-lea/node.js
$ sudo apt-get update
$ sudo apt-get install nodejs
$ gem install rails
$ rbenv rehash
```
Checking rails version
``` sh
$ rails -v
```

Reference: https://cis.ait.asia/course_offerings/224/rails-companion
