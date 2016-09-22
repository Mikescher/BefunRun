![](https://raw.githubusercontent.com/Mikescher/BefunUtils/master/README-FILES/icon_BefunRun.png) BefunRun  [![Build status](https://ci.appveyor.com/api/projects/status/7x53ceio5fa6bsbv/branch/master?svg=true)](https://ci.appveyor.com/project/Mikescher/befunrun/branch/master)
========

A simple command line program to execute befunge-93 code.
The first parameter is expected to be a file with valid befunge-93 code.
All output is redirected to stdout. *(Errors end up in stderr)*

Like all programs in my [BefunUtils](https://github.com/Mikescher/BefunUtils) collection this supports befunge-93 code with extended grid size (bigger than 80x25, see [BefunExec](https://github.com/Mikescher/BefunExec))

Download
========

You can download the binaries from my website [www.mikescher.com](http://www.mikescher.com/programs/view/BefunUtils)

Or you can download the latest (nightly) version from the **[AppVeyor build server](https://ci.appveyor.com/project/Mikescher/BefunRun/build/artifacts)**

Set Up
======

*This program was developed under Windows with Visual Studio.*

You don't need other [BefunUtils](https://github.com/Mikescher/BefunUtils) projects to use this.  
Theoretically you can only clone this repository and start using it.  
But it could be useful to get the whole BefunUtils solution like described [here](https://github.com/Mikescher/BefunUtils/blob/master/README.md)

Usage
=====

Simply call the exe with `BefunRun code.b93` to execute a program and output its result.

There are a few optional commandline options:

 - `-limit=X`: Abort execution after X steps
 - `-info`: Show information about the program execution (max number, max stack size, etc)
 - `-errorlevel=0`: default error level, execute program befunge-compliant and never abort.
 - `-errorlevel=1`: Abort execution when 
	 - reading from an empty stack
	 - reading/writing to invalid coordinates
	 - dividing (or modulo-ing) by zero
 - `-errorlevel=2`: Abort execution on these conditions: (and on all conditions of lower error levels)
	 - Moving the PC over the program edge
 - `-errorlevel=2`: Abort execution on these conditions: (and on all conditions of lower error levels)
	 - executing (= entering with the PC) a previously modified cell


Limitations
===========

BefunRun is **not** intended for non-terminating programs or programs with user input. You can use [BefunExec](https://github.com/Mikescher/BefunExec) to run these kind of programs.


Contributions
=============

Yes, please