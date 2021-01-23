Installation Notes for SystemC CCI 1.0.1
========================================

**Contents**
 * [Installation Notes for Unix][unix]
 * [Installation Notes for Windows][win]
 * [Library Configuration Switches][config]


System Requirements
-------------------

The SystemC CCI library can be installed on the following platforms:

  * Linux
    * Architectures
      - x86 (32-bit)
      - x86_64 (64-bit)
      - x86 (32-bit) application running on x86_64 (64-bit) kernel  
        (`../configure --host=i686-linux-gnu`)
    * Compilers
      - GNU C++ compiler
      - Clang C++ compiler
      - or compatible

  * Mac OS X
    * Architectures
      - x86 (32-bit)
      - x86_64 (64-bit)
    * Compilers
      - GNU C++ compiler
      - Clang C++ compiler
      - or compatible

  * Windows
    * Compatibility layer
      - Cygwin
      - MinGW / MSYS
    * Architectures
      - x86 (32-bit)
      - x86_64 (64-bit)
    * Compilers
      - GNU C++ compiler
      - or compatible

NOTE: _Not all combinations are equally well-tested and some combinations
      may not work as expected.  Please report your findings on the 
      [Accellera SystemC CCI forum](https://forums.accellera.org/forum/41-systemc-cci-configuration-control-inspection/).

The [RELEASENOTES](RELEASENOTES.md) file contains a list of detailed platforms,
architectures, and compiler versions that have been used for testing this release.

The following additional packages are required:

  * SystemC     https://accellera.org/downloads/standards/systemc
  * RapidJSON   https://github.com/Tencent/rapidjson
  * Boost       https://www.boost.org/


Installation Notes for Unix
===========================

Sources for Compilers and Related Tools
---------------------------------------

To build, install, and use SystemC on UNIX platforms, you need
the following tools:

  1. GNU C++11 compiler or later (version 4.8.1 or later), or
     Clang C++11 compiler or later (version 3.3 or later)
  2. GNU Make (gmake)

GCC, Clang, and gmake are free software that you can
obtain from the following sources:

  * GCC     http://www.gnu.org/software/gcc/gcc.html
  * Clang   http://clang.llvm.org/
  * gmake   http://www.gnu.org/software/make/make.html


Basic SystemC CCI Installation
------------------------------

To install SystemC on a UNIX system, do the following steps:

  1. Change to the top level directory (e.g. `cci-1.0.1`)

  2. Create a temporary build directory, e.g.,
     ```bash
     mkdir objdir
     ```
  3. Change to the temporary build directory, e.g.,
     ```bash
     cd objdir
     ```
  4. Choose your compiler by setting the `CXX` environment variable
     (the configure script tries to guess the default compiler, if
      this step is omitted).
     If you use a POSIX-compatible shell (e.g. bash):
     ```bash
     export CXX="<compiler>"
     ```
     e.g. for GCC compilers
     ```bash
     export CXX=g++
     ```
     The Clang compiler is usually named `clang++`, thus e.g.
     ```bash
     export CXX=clang++
     ```
     When using a C shell (e.g. `csh`/`tcsh`), the syntax to set the
     environment variable is different:
     ```sh
     setenv CXX g++
     ```
     You can also specify an absolute path to the compiler of your choice.
     See also the Section [Compilation and Linking Options][comp] below.

  5. **Configure the package** for your system, e.g.,
     (The `configure` script is explained below.)

     ```bash
     ../configure --help
     ```

     SystemC CCI relies on the availability of a SystemC installation
     directory, the configure script will automatically check this. In case
     the configure script cannot find SystemC, it will stop. You can specify
     the path to the SystemC installation directory as follows:

     ```
     ../configure --with-systemc=/path/to/systemc
     ```

     SystemC CCI also requires JSON serialization capabilties. Currently
     CCI requires the RapidJSON package for this, and the configure script 
     will automatically check the availability of this page. In case
     the configure script cannot find RapidJSON, it will stop. You can specify
     the path to the RapidJSON directory as follows:

     ```
     ../configure --with-json=/path/to/rapidjson
     ```

     While the 'configure' script is running, which takes a few moments,
     it prints messages to inform you of the features it is checking.
     It also detects the platform.

     _Note for System V users_: 
     If you are using 'csh' on an older version of System V, you might
     need to use the `sh ../configure` command instead of `../configure`.
     Otherwise, 'csh' will attempt to 'configure' itself.

     SystemC 2.3.x includes a fixed-point package that is always built.
     When compiling your applications with fixed-point types, you still have
     to use compiler flag `-DSC_INCLUDE_FX`. Note that compile times increase
     when using this compiler flag.

     In case you want to install the package in another place than the
     top level directory, configure the target directory, e.g. as follows:

     ```bash
     ../configure --prefix=/usr/local/cci-1.0.1
     ```

     NOTE: _Make sure you have created the target directory before installing
     the package. Do _not_ use `/usr/local` as a prefix, unless you
     follow the Unix/FHS directory layouts (see below)._

     A fine grained configuration of the installation directories can
     be achieved via additional options, given to the configure script.

     By default, the files are installed directly to the `PREFIX` directory
     root and the library is installed to `PREFIX/lib-<TARGETARCH>`,
     depending on the current target architecture.  This may be undesired
     in cases where the package is meant to be installed in a system-wide
     location as part of shared (default) library and include hierarchies
     (e.g. `/usr/local`, `/usr`, `/opt`, ...).  To follow the Unix/FHS directory
     standards, you can use the following options:

     ```
       --with-unix-layout     use Unix directory layout for installation
                              [default=no]
     ``` 
     when "yes", the following (fine-grained) settings will be used:
     ```
       --includedir=DIR       C++ header files      [PREFIX/include]
       --libdir=DIR           object code libraries [EPREFIX/lib]
       --docdir=DIR           documentation root    [DATAROOTDIR/doc/systemc]
     ```

      The library destination itself can be further and separately configured
      by using the following option:

      ```
       --with-arch-suffix     add suffix to library installation directory
                              [default=-<TARGETARCH>]
      ```

      With this option, one can easily follow e.g. the "multi-arch"
      conventions on some platforms:

      ```bash
      ../configure --with-arch-suffix=32                # lib32
      ../configure --with-arch-suffix=/x86_64-linux-gnu # lib/x86_64-linux-gnu
      ```
 
     Several options are available to the configure script to modify
     the compiler configuration and the selection of certain features:

     ```
       --disable-shared        do not build shared library (libsystemc.so)
       --enable-debug          include debugging symbols
       --disable-optimize      disable compiler optimization
     ```

     See the section on the general usage of the `configure` script and
     `../configure --help` for more information.

     Note: _If you change the configuration after having compiled the
            package already, you should run a `gmake clean` before
            recompiling._

  6. **Compile the package**

     ```bash
     gmake
     ```

     Note: _The explicit gmake targets `opt` and `debug`, etc. have
            been removed in this package.  Use the corresponding
            options to the configure script instead._

  7. At this point you may wish to verify the compiled package by
     testing the examples which are part of this package.

     ```bash
     gmake check
     ```

     This will compile and run the examples in the subdirectory
     examples.

  8. **Install the package**

     ```bash
     gmake install
     ```

  9. You can now remove the temporary build directory, .e.g,

     ```bash
     cd ..
     rm -rf objdir
     ```

     Alternatively, you can keep the temporary build directory to allow you to
     1) Experiment with the examples.
     2) Later uninstall the package.
     
     To clean up the temporary directory, enter:
     ```bash
     gmake clean
     ```

     To uninstall the package, enter:
     ```bash
     gmake uninstall
     ```

### Running the Examples in Unix

Copies of the examples reside in the temporary build directory. The sources
of the example code reside in the directory `examples` at the highest level
of the installation. The following build flows are supported:

 * Build and run all examples at once using the configure and gmake flow 
   described above, see instruction item 7.

 * Build and run all examples in the source directory directory 
   `examples/build-unix` with the command `make check`.

 * Build and run each example separately. For this, go to one of the examples
   subdirectories under `examples/cci` and use the command 
   `make -f Makefile.unix check`.

_HINT: Use the Makefiles provided in the `examples` directory as templates
for Makefiles you need for compiling your own examples._


### Using the Configure Script

The `configure` shell script tries to determine the correct values for
various system-dependent variables used during compilation. It uses
these values to create a `Makefile` in each directory of the package.
It also creates one or more `.h` files containing system-dependent
definitions if needed. Then, it creates the following files:

* `config.status`  
    A shell script that you can run at another time to
    recreate the current configuration.

* `config.cache`  
    A file in which the configure test results are
    saved to speed up reconfiguration.
    Data is appended to the `config.cache` file.
    You can remove unwanted data.

* `config.log`  
    A file in which compiler output is saved.
    This is used to debug the configure script.

If you need to use other commands to successfully compile the package
on your system, please try to determine if the configure script can be used
for these commands. Then, send either a diff file or instructions about
the commands you used to the e-mail address provided in the README file.
This information will be used to improve the installation process in
the next release.

The `configure.ac` file is provided in case you want to change or regenerate
the `configure` script, for example to use a newer version of `autoconf`.
The `configure.ac` file is used by the `autoconf` program to create the
`configure` script.

Note for (key) developers:

  In case you have changed the `configure.ac` file or one of the
  `Makefile.am` files:

  - Use 
    ```bash
    gmake distclean
    ```
    to remove the generated `configure` script, the generated `aclocal.m4`
   file and the generated `Makefile.in` files.

  - Use the `config/bootstrap` script to generate the `configure` script
    and the necessary `Makefile.in` files. This script makes use of the
    GNU auto-tools `aclocal`, `automake`, and `autoconf`.


Compilation and Linking Options
-------------------------------

Some systems require compilation or linking options that the `configure`
script does not define. You can define the initial values for these
options by setting them in your environment before running the
`configure` script.

Instead of passing the variables via the environment, it is preferred
to pass the values as options to the configure script:

```sh
../configure CXX=g++-4.8.1 LIBS=-lposix
```


### Specifying the System Type

Some features cannot be automatically determined by `configure` unless
it can detect the host type on which the package will run.
If it prints a message that it cannot determine the host type,
use the `--host=TYPE` option to define it. TYPE can either be a
short system name, such as `sun4`, or a canonical name with three fields:

     CPU-COMPANY-SYSTEM

See the `config.sub` file for details about the values of each field. If
the `config.sub` file is not included in the package, the package does not
need to know the host type.

If you are building compiler tools for cross-compiling, you can also
use the `--target=TYPE` option to select the type of system for which
the code is produced and the `--build=TYPE` option to select the type of
system on which you are compiling the package.


### Sharing Defaults

You can set the default values that `configure` scripts share by
creating a site shell script called `config.site`. This file contains the
default values for variables like `CC`, `cache_file`, and `prefix`.
The `configure` script looks for the `config.site` file in the following
search precedence:

  1. `PREFIX/share/config.site`
  2. `PREFIX/etc/config.site`

Alternatively, you can set the `CONFIG_SITE` environment variable to the
site script path.

Note: _The `configure` script for some systems does not look for a site script._


### Operation Controls

The `configure` script recognizes the following additional options to control
its operation:

`--cache-file=FILE`  
        Use and save the test results in FILE instead of
        `./config.cache`. Set FILE to `/dev/null` to disable caching
        when debugging `configure`.

`--help`  
        Print a summary of `configure` options and exit.

`--quiet`  
`--silent`  
`-q`  
        Do not print messages about checks being made.
        To suppress all normal output, redirect it to `/dev/null`.
        Error messages continue to print.

`--srcdir=DIR`  
        Look for the package's source code in directory DIR.
        Typically `configure` determines the directory automatically.

`--version`  
        Print the version of `autoconf` used to generate the `configure`
        script and exit.

Other options that are rarely used are available in the `configure` script.
Use the `--help` option to print a list.


--------------------------------------------------------------------------
Installation Notes for Windows
==========================================================================

This release has been tested on Visual C++ versions 2010 through 2015,
running on Windows 10. Older platforms before Windows 10 are not supported.

NOTE: _This section covers the installation based on Microsoft Visual C++.
      For Cygwin or MinGW-based installations, see Section 1._


NOTE: _If you experience spurious errors about missing files in the
      downloaded archive, please make sure to either download the
      ZIP archive from accellera.org or use a reliable archive software,
      fully supporting modern tar archive versions._


Microsoft Visual C++ 2010 (compiler version 10.0) or later
----------------------------------------------------------

The SystemC CCI package contains two subdirectories: `msvc10` and
`examples/build-msvc` to build the CCI library and the examples using
Microsoft Visual C++ (MSVC).

### Build the SystemC CCI library with MSVC

The `msvc10` directory contains the project and workspace files to
compile the `cci.lib` library. Double-click on the `cci.sln`
file to launch Visual C++ with the workspace file. The workspace file
will have the proper switches set to compile for Visual C++ 2010.
Select *Build Solution* under the *Build* menu or press Ctrl+Shift+B to 
build `cci.lib`.

### Build the SystemC CCI examples with MSVC

The `examples` directory contains the project and workspace files to 
compile the SystemC CCI examples. The following flows are supported:

 * Build all examples at once using Microsoft Visual C++ IDE. For this, 
   select the `cci_examples.sln` in the `examples/build-msvc` directory.
   Select *Build Solution* under the *Build* menu or press Ctrl+Shift+B to 
   build all examples.
  
 * Build each example separately using Microsoft Visual C++ IDE. For this,
   go to one of the examples subdirectories under `examples/cci` and 
   double-click on the .vcxproj file to launch Visual C++. The workspace 
   file will have the proper switches set to compile for Visual C++ 2010.
   Select *Build Solution* under the *Build* menu or press Ctrl+Shift+B to 
   build the examples.

 * Build all examples via the Windows Command Prompt using the Microsoft 
   Visual C++ NMAKE command. More information for this can be found in the
   file [READMEcommandlinemsvc](examples/build-msvc/READMEcommandlinemsvc.txt).

 * Build each examples separately via the Windows Command Prompt using the 
   Microsoft Visual C++ NMAKE command. More information for this can be found
   in the file [READMEcommandlinemsvc](examples/build-msvc/READMEcommandlinemsvc.txt).

The provided project files are prepared for both the 32-bit 'Win32' and
64-bit 'x64' configurations.  Please refer to the Microsoft Visual Studio
documentation for details about 64-bit builds.


Creating SystemC CCI Applications
---------------------------------

1. Start Visual Studio C++. From the Start Page select New Project and Win32
   Console Project. Type the project name and select a suitable location
   then click OK.

2. Select the Application Settings page of the Win32 Application Wizard
   and make sure the 'Empty project' box is ticked. Click 'Finish' to
   complete the wizard.

3. Add new/existing C++ files to the project and edit code.

4. Display the project Property Pages by selecting 'Properties...' from
   the Project menu.

5. Select in Configuration to set the properties for Debug or Release
   
6. From the C/C++ tab, select the General properties and set
   'Detect 64-bit Portability Issues' to No

7. From the C/C++ tab, select the General properties and add to
   'Additional Include Directories' the path 
   `$(CCI_HOME)\src;$(SYSTEMC_HOME)\src;$(RAPIDJSON_HOME)\include;$(BOOST_HOME);`
   
8. From the C/C++ tab, select the Code Generation and set
   'Runtime Library' to 'Multi-threaded DLL (/MD)' in case of a Release 
   Configuration or 'Multi-threaded Debug DLL (/MDd)' in case of a Debug 
   Configuration

9. From the C/C++ tab, select the Language properties and set
   'Enable Run-Time Type Info' to Yes (/GR)

10. From the C/C++ tab, select the Command Line properties and add /vmg
    to the 'Additional Options:' box.

11. From the Linker tab, select the General properties and add to 
    'Additional Library Directories' the path 
    `$(SYSTEMC_HOME)\$(SYSTEMC_MSVC)\SystemC\$(IntDir);$(CCI_HOME)\$(CCI_MSVC)\cci\$(IntDir);`

12. From the Linker tab, select the Input properties and add to 
    'Additional Dependencies' the files 'systemc.lib;uvm-systemc.lib;'

13. From the Linker tab, select the System properties and set 
    'Subsystem' to 'Console (/SUBSYSTEM:CONSOLE)'

14. Click OK. If necessary repeat the same steps for Configuration Debug 
    or Release

Also make sure that the Windows environment variables `CCI_HOME`, `SYSTEMC_HOME`, 
`RAPIDJSON_HOME` and `BOOST_HOME` are set and point to the installation directory for each package.


Building against a SystemC DLL
------------------------------

In order to link your application against a DLL-build of SystemC (build
configurations `DebugDLL`, `ReleaseDLL` in the SystemC library build),
several changes are needed.

1. Adjust the linker library directory settings to reference `DebugDLL`
   (or `ReleaseDLL`) instead of `Debug` or `Release`, respecitvely:  
      `...\systemc-2.3.3\msvc10\systemc\DebugDLL`

2. Add the preprocessor switch `SC_WIN_DLL` to your project's properties  
   (C/C++ -> Preprocessor -> Preprocessor Definitions).

3. When running the simulation, you need to add the location of the
   SystemC DLL to your `PATH` variable.

--------------------------------------------------------------------------


[unix]: #installation-notes-for-unix
[comp]: #compilation-and-linking-options
[win]: #installation-notes-for-windows
[config]: #systemc-library-configuration-switches
[cmake]: cmake/INSTALL_USING_CMAKE
