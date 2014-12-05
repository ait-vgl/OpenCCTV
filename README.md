OpenCCTV
========
OpenCCTV project is an middleware application that enables integration of Video Management Systems (VMSs) such as Milestone, Zoneminder, OpenVSS etc. with video analytics executed on a distributed platform.
The main goal of the project is to provide a video analytic platform for CCTV camera base security solutions.

Why did we start this project?
------------------------------
Security & surveillance cameras are everywhere. There are many different VMSs are being deployed for management and storage purposes of video from these cameras.
Most VMSs have limited video analytic capabilities.
It is unable to deploy new/customized video analytics on the videos from cameras connected to VMSs.
We started this project to develop open bus/plugin architecture to integrate VMSs with video analytic.

How it works?
-------------
OpenCCTV allows users to input VMS information into the system. Currently it supports only the Milestone XProtect® VMS but we will develop the system to support other popular VMSs in near future.
Then system automatically connects to the Cameras configured with the VMSs.
Users can create Streams with different resolutions from these Cameras.
Users can upload their own video analytic modules as shared library files. Currently system supports only the Linux shared library files (.so) but we will develop it to support Microsoft Windows® shared library files (.dll) very soon.

Then users can create instances of uploaded video analutic modules, and direct the Streams to these video analytic instances.

##### OpenCCTV Web
User have to use the Web application to input VMS information, upload video analytics
##### OpenCCTV Server
