FROM opencctv:web


# throw errors if Gemfile has been modified since Gemfile.lock
#RUN bundle config --global frozen 1

ENV InstallPath /myapp
RUN mkdir -p ${InstallPath}
WORKDIR ${InstallPath}


RUN rm Gemfile && rm Gemfile.lock 
ADD Gemfile ${InstallPath}

#RUN rm -f /myapp/tmp/pids/server.pid
RUN bundle install

ADD . ${InstallPath}

VOLUME ["/usr/local/opencctv/analytics","/usr/local/opencctv/vms_connectors"]

EXPOSE 3000

CMD rails s -p 3000 -b 0.0.0.0
