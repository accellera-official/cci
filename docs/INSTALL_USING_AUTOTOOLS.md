# Installation using GNU Autotools

Note 1: _Usage of the GNU Autotools build flow will be deprecated as part of SystemC CCI 1.1.
        It it recommended to use the CMake build system instead, see [INSTALL.md](../INSTALL.md)._

Note 2: _IEEE Std. 1666-2023 mandates C++17 as the baseline for SystemC implementations,
        see [RELEASENOTES.md](../RELEASENOTES.md).  Make sure to configure your compiler
        accordingly (see below)._

## Basic SystemC CCI Installation

To install SystemC CCI on a UNIX system, do the following steps:

  1. Change to the top level directory (e.g., `cci`)

  2. Choose your compiler by setting the `CXX` environment variable
     (the configure script tries to guess the default compiler, if
      this step is omitted).
     If you use a POSIX-compatible shell (e.g., bash):
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
     When using a C shell (e.g., `csh`/`tcsh`), the syntax to set the
     environment variable is different:
     ```sh
     setenv CXX g++
     ```
     You can also specify an absolute path to the compiler of your choice.
     See also the Section [Compilation and Linking Options][comp] below.

  3. Generate the `configure` script and the necessary `Makefile.in` 
     files (only for git clone)

     Note: _This step is required when using a git clone.  It requires the
            GNU Autotools `libtoolize`, `aclocal`, `automake`, and `autoconf`.
            A SystemC CCI release already contains the configuration executable
            so this step can be ignored._

     ```bash
     ./config/bootstrap
     ```

  4. Create a temporary directory, e.g.,
     ```bash
     mkdir objdir
     ```

  5. Change to the temporary directory, e.g.,
     ```bash
     cd objdir
     ```

  6. Configure the package for your system, e.g.,
     (The `configure` script is explained below.)

     ```bash
     ../configure --help
     ```

     SystemC CCI relies on the availability of a SystemC installation
     directory, the configure script will automatically check this. In case
     the configure script cannot find SystemC, it will stop. You can specify
     the path to the SystemC installation directory as follows:

     ```bash
     ../configure --with-systemc=/path/to/systemc
     ```

     SystemC CCI also requires JSON serialization capabilties. Currently
     CCI requires the RapidJSON package for this, and the configure script 
     will automatically check the availability of this page. In case
     the configure script cannot find RapidJSON, it will stop. You can specify
     the path to the RapidJSON directory as follows:

     ```bash
     ../configure --with-json=/path/to/rapidjson
     ```

     While the 'configure' script is running, which takes a few moments,
     it prints messages to inform you of the features it is checking.
     It also detects the platform.

     Note: _As IEEE Std. 1666-2023 mandates C++17 as the baseline for SystemC
           implementations, make sure you enable the compiler flag to
           select C++17, e.g._:

     ```bash
     ../configure 'CXXFLAGS=-std=c++17'
     ```

     Compiling this implementation for newer C++ standards is generally
     possible and tested at a best effort basis.  Prefer C++17 for full
     IEEE Std. 1666-2023 conformance, see also `SC_CPLUSPLUS` macro
     documentation below.

     SystemC includes a fixed-point package that is always built.
     When compiling your applications with fixed-point types, you still have
     to use compiler flag `-DSC_INCLUDE_FX`.  Note that compile times might
     increase when using this compiler flag.

     In case you want to install the package in another place than the
     top level directory, configure the package e.g. as follows:

     ```bash
     ../configure --prefix=/usr/local/cci
     ```

     Note: _Make sure you have created the target directory before installing
     the package.  Do _not_ use `/usr/local` as a prefix, unless you
     follow the Unix/FHS directory layouts (see below)._

     A fine grained configuration of the installation directories can
     be achieved via additional options, given to the configure script.

     By default, the files are installed directly to the `PREFIX` directory
     root and the library is installed to `PREFIX/lib-<TARGETARCH>`,
     depending on the current target architecture.  This may be undesired
     in cases where the package is meant to be installed in a system-wide
     location as part of shared (default) library and include hierarchies
     (e.g. `/usr/local`, `/usr`, `/opt`, ...).  To follow the Unix/FHS
     directory standards, you can use the following options:

     ```
       --with-unix-layout     use Unix directory layout for installation
                              [default=no]
     ```
     when "yes", the following (fine-grained) settings will be used:
     ```
       --includedir=DIR       C++ header files      [PREFIX/include]
       --libdir=DIR           object code libraries [EPREFIX/lib]
       --docdir=DIR           documentation root    [DATAROOTDIR/doc/cci]
     ```

      The library destination itself can be further and separately configured
      by using the following option:

      ```
        --with-arch-suffix    add suffix to library installation directory
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
       --disable-shared       do not build shared library (libcci.so)
       --enable-debug         include debugging symbols
       --disable-optimize     disable compiler optimization
     ```

     See the section on the general usage of the `configure` script and
     `../configure --help` for more information.

     Note: _If you change the configuration after having compiled the
            package already, you should run a `make clean` before
            recompiling._

  7. Compile the package

     ```bash
     make
     ```

     Note: _The explicit make targets `opt` and `debug`, etc. have
            been removed in this package.  Use the corresponding
            options to the configure script instead._

  8. At this point you may wish to verify the compiled package by
     testing the example suite.

     ```bash
     make check
     ```

     This will compile and run the examples in the subdirectory
     examples.

  9. Install the package

     ```bash
     make install
     ```

  10. You can now remove the temporary directory, .e.g,

      ```bash
      cd ..
      rm -rf objdir
      ```

      Alternatively, you can keep the temporary directory to allow you to
      1) Experiment with the examples.
      2) Later uninstall the package.

      To clean up the temporary directory, enter:
      ```bash
      make clean
      ```

      To uninstall the package, enter:
      ```bash
      make uninstall
      ```

### Running the Examples

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
various system-dependent variables used during compilation.  It uses
these values to create a `Makefile` in each directory of the package.
It also creates one or more `.h` files containing system-dependent
definitions if needed.  Then, it creates the following files:

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
for these commands.  Then, send either a diff file or instructions about
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
    make distclean
    ```
    to remove the generated `configure` script, the generated `aclocal.m4`
   file and the generated `Makefile.in` files.

  - Use the `config/bootstrap` script to generate the `configure` script
    and the necessary `Makefile.in` files.  This script makes use of the
    GNU Autotools `aclocal`, `automake`, and `autoconf`.

## Compilation and Linking Options

Some systems require compilation or linking options that the `configure`
script does not define.  You can define the initial values for these
options by setting them in your environment before running the
`configure` script.

Instead of passing the variables via the environment, it is preferred
to pass the values as options to the configure script, e.g.,:

```sh
../configure CXX=g++-9.3 LIBS=-lposix
```

### Specifying the System Type

Some features cannot be automatically determined by `configure` unless
it can detect the host type on which the package will run.
If it prints a message that it cannot determine the host type,
use the `--host=TYPE` option to define it.  `TYPE` can either be a
short system name or a canonical name with three fields:

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
        `./config.cache`.  Set FILE to `/dev/null` to disable caching
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

[comp]: #compilation-and-linking-options
