EduDist
=======

This project is an attempt to create a cross-platform general-purpose 
distribution system. A great example of distribution systems are
GNU/Linux packaging systems. You might be familiar with APT, RPM or YUM.

Instead of focussing on distributing software, this project aims to create
a distribution system that can distribute data. There won't be scripts to
run upon installation or removal, instead only files related to the package
will be extracted or removed.

What this project should provide:
* A lightweight repository system.
* A cross-platform tool to distribute data (not limited to software).


Dependencies
------------

Make sure you have the following libraries, development packages and build
tools installed:
* GCC or CLANG
* Autotools
* Make
* cURL development library

The code is normally compiled and tested with GCC.

Build instructions
------------------

For GNU/Linux distributions:
<pre>
autoreconf -i
./configure
make
</pre>

Contributing
------------

At this point there's no specific feature to contribute to, however, ideas
and discussions are always welcome.

License
-------

This project's code is licensed under the GPLv3 or any later version.
