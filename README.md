The main goal of the OpenCCTV project is to provide a Video Analytic platform for CCTV camera base security solutions. OpenCCTV platform consists of a middleware and a Web application that enables integration of Video Management Systems (VMSes) with Video Analytics. We anticipate develop the system to support VMSes such as Milestone®, Zoneminder, OpenVSS etc. With this platform users will be able to execute their own Video Analytics on a distributed server apart from the VMSes and send Video Analytic results back to VMSes or write them to a user preferred database.

Why did we start this project?
------------------------------
Security & surveillance cameras are everywhere. There are many different VMSs are being deployed for management and storage purposes of video from these cameras.
Most VMSs have limited video analytic capabilities.
It is unable to deploy new/customized video analytics on the videos from cameras connected to VMSs.
We started this project to develop open bus/plugin architecture to integrate VMSs with video analytic.

How it works?
-------------
OpenCCTV allows users to input VMS information into the system. Currently it supports only the Milestone XProtect® VMS but we will develop the system to support other popular VMSes in near future.
Then system automatically connects to the Cameras configured with the VMSes.
Users can create Streams with different resolutions from these Cameras.
Users can upload their own video analytic modules as shared library files. Currently system supports only the Linux shared library files (.so) but we will develop it to support Microsoft Windows® shared library files (.dll) very soon.
Then users can create instances of uploaded video analutic modules, and direct the Streams to these video analytic instances.

##### OpenCCTV Web
User have to use the Web application to input VMS information, create video Streams from the VMS Cameras, upload Video Analytics, create Video Analytic Instances and direct video Stream to created Video Analytic Instances.The information of VMSes, cameras, streams, analytic modules and analytic processes stored in the database through the Web application will be used by the middleware (OpenCCTV Server and Analytic Server).

##### OpenCCTV Server
This server application uses the data written by the Web application to the databse to connect to the VMSes and grab the video Streams from the VMS Cameras. These video Streams are routed to Video Analytic Instances that are running on the Analytic Servers by this server application.

##### Analytic Server
The Analytics server runs video analytic instances in separate processes. Since different analytics will have different processing speeds, each analytic process has its own input image queue and also output results queue. OpenCCTV server routes video Streams as individual image objects (with metadata) to these input image queues of analytic processes. And retrieves analytic results from the output results queues as XML texts.

##### OpenCCTV Videos
[OpenCCTV Video part 1](http://youtu.be/xRuYp1DhHBs)

[OpenCCTV Video part 2](http://youtu.be/NPrm8g5uqm8)

Required packages for OpenCCTV
------------------------------
* [Boost C++ Libraries](http://www.boost.org/)

![Boost logo](http://www.boost.org/doc/libs/1_57_0/boost.png)

Installation guide to Boost: http://particlephysicsandcode.com/2013/03/11/installing-boost-1-52-ubuntu-12-04-fedora/

* [ZeroMQ](http://zeromq.org/)

![ZeroMQ logo](http://travlr.github.io/zmqirclog/images/zeromq1-300x115.png)

Installation guide to ZeroMQ: https://maddigitiser.wordpress.com/2013/05/02/installing-zeromq-on-ubuntu-13-04/

* [OpenCV](http://opencv.org/)

![OpenCV logo](http://opencv.org/wp-content/themes/opencv/images/logo.png)

Installation guide to OpenCV: http://docs.opencv.org/trunk/doc/tutorials/introduction/linux_install/linux_install.html

* [MySQL](http://www.mysql.com/)

![MySQL logo](http://www.mysql.com/common/logos/logo-mysql-110x57.png)

Installation guide to MySQL: http://www.tocker.ca/2014/04/21/installing-mysql-5-6-on-ubuntu-14-04-trusty-tahr.html

To install MySQL C++ connector API using terminal: <code>sudo apt-get install libmysqlcppconn-dev</code>

Supported By
-------------
This project is developed by [Vision and Graphics Lab (VGL)](http://vgl-ait.org/wiki/doku.php) at [Asian Institute of Technology](http://www.ait.asia) and the [National Electronics and Computer Technology Center (NECTEC)](http://www.nectec.or.th/en/).

![AIT logo](http://www.ait.ac.th/media/documents/logo_ait_08_ver3.jpg)

![NECTEC logo](http://www.nstda.or.th/images/stories/resource/corporate-band-nstda/Color-400/20110613-nectec-400px.gif)

This project was funded by the [Nation Science and Technology Development Agency (NSTDA)](http://www.nstda.or.th/eng/).

![NSTDA logo](http://www.nstda.or.th/images/stories/resource/corporate-band-nstda/Color-400/20110613-nstda-400px.gif)
