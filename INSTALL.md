# Installation Notes for SystemC CCI

## Table of Contents

 1. System Requirements
 2. Installation Notes for CMake
 3. Installation Notes for Windows using Visual Studio C++
 4. Known Problems
 5. Resources


## 1. System Requirements

The SystemC CCI library can be installed on the following platforms:

 * Linux
   * Architectures
     - x86_64 (64-bit)
   * Compilers
     - GNU C++ compiler
     - Clang C++ compiler
     - or compatible

 * macOS
   * Architectures
     - Apple Silicon
     - x86_64 (64-bit)
   * Compilers
     - GNU C++ compiler
     - Clang C++ compiler
     - or compatible

 * Windows
   * Compatibility layer
     - MinGW / MSYS
   * Architectures
     - x86_64 (64-bit)
   * Compilers
     - GNU C++ compiler
     - Clang C++ compiler
     - or compatible
   * Visual Studio C++
     -  Win32 (32-bit)
     -  x64 (64-bit)

NOTE: _Not all combinations are equally well-tested and some combinations
      may not work as expected.  Please report your findings on the 
      [Accellera SystemC CCI forum](https://forums.accellera.org/forum/41-systemc-cci-configuration-control-inspection/).

The [RELEASENOTES](RELEASENOTES.md) file contains a list of detailed platforms,
architectures, and compiler versions that have been used for testing this release.

The following additional packages are required:

  * SystemC     https://accellera.org/downloads/standards/systemc

## 2. Installation Notes for CMake

### 2.1 Sources for Compilers and Related Tools

To build, install, and use SystemC CCI, you need the following tools:

  1. GNU C++ compiler (version 9.3 or later), or
     Clang C++ compiler (version 13.0 or later), or
     Visual Studio C++ (version msvc16 (2019) or later)
  2. GNU Make (make) or Ninja
  3. CMake

GCC, Clang, make, Ninja, and CMake are free software that you can
obtain from the following sources:

  * GCC     https://www.gnu.org/software/gcc/index.html
  * Clang   https://clang.llvm.org/
  * make    https://www.gnu.org/software/make/make.html
  * Ninja   https://ninja-build.org/
  * CMake   https://cmake.org/

The CMake build scripts are compatible with CMake >=3.5 and have been
tested on the following OS/processor/compiler platforms:

  * macOS >= 13.6 until 15.4 (Xcode/AppleClang): x86_64, Apple Silicon, universal binary
  * Linux (GCC, Clang): i386, x86_64
  * Linux (GCC): aarch64
  * Windows (MSYS2 with the MinGW-W32 or MinGW-W64 GCC toolchains, Visual C++):
    x86 (Win32), AMD64 (x64)

### 2.2 Installation Steps

Typically, the following steps need to be followed to compile the
sources using CMake:

  1. Download and install CMake from [this website][1]
     (Linux distributions provide often a package).

  2. Create a build subdirectory:

         $ pwd
         .../cci/
         $ mkdir build

  3. Launch the interactive cmake console program `ccmake` (on Unix) or
     the CMake GUI (Unix, macOS, Windows) and specify the source
     (`.../cci`) and build directory (`.../cci/build`).  For
     the console based `ccmake`, this is done by changing into the build
     directory and launching `ccmake` with the relative path to the source
     root directory as an argument:

         $ cd build/
         $ ccmake ..

     For cmake-gui, the same approach works:

         $ cd build/
         $ cmake-gui ..

     Alternatively, the CMake GUI allows to directly enter the paths to the
     sources and build directory in the top of its main window.

  4. Configure the sources by pressing 'c' or clicking on
     "Configure".  Depending on the operating system, CMake will ask the user
     what it should generate Makefiles or IDE project files (Eclipse,
     Xcode, Kdevelop, Microsoft Visual Studio C++).  CMake will parse the file
     `CMakeLists.txt` in the source root directory and configure the sources
     accordingly carrying out all necessary platform-specific checks.

     The CMake build scripts detect the actual features provided by the
     target (Unix or Windows) platform.  On Unix (including macOS), shared
     libraries can be built.  If the QuickThreads library provides support
     for the target processor, it will be automatically used.  Otherwise, we
     rely on Pthreads on Unix and Fiber on Windows.  By default, the SystemC
     library installation will follow the GNU standard installation layout
     so that also other SystemC libraries (SystemC, SCV, TLM, AMS
     extensions) can be installed into the same directory hierarchy (Unix:
     `/opt/systemc/`; Windows: `$ENV{ProgramFiles}/SystemC/`).  The target
     platform's conventions are respected meaning usually `include/` for the
     headers and `lib/`, `lib64/`, or `lib/<multiarch-tuple>/` for the
     libraries.  The `lib-${SYSTEMC_TARGET_ARCH})/` convention is not used
     by default, as `${SYSTEMC_TARGET_ARCH}` does not reliably encode the
     OS/processor/compiler tuple.

     The build process can be configured through the configuration
     variables offered to the user in the CMake console and GUI (`ccmake` and
     `cmake-gui`, respectively).  Each variable is annotated with a brief
     documentation string.  The most important variables are:

     * `BUILD_SHARED_LIBS`
       Build shared instead of static libraries
       (default: `ON` if not targeting Windows).

     * `BUILD_SOURCE_DOCUMENTATION`
       Build source code documentation using Doxygen (default: `OFF`).

     * `CMAKE_BUILD_TYPE`
       Specifies the build type on single-configuration generators.
       (default: `Release`).

     * `CMAKE_INSTALL_PREFIX`
       Root directory of the SystemC libraries installation (defaults to
       `$ENV{SYSTEMC_HOME}` if set to an absolute path and otherwise to either
       `/opt/systemc/` (Unix-like platforms including CYGWIN),
       `$ENV{ProgramFiles}/SystemC/` (on Windows systems), or
       `${CMAKE_INSTALL_PREFIX}/systemc`.

     * `SYSTEMCCCI_BUILD_TESTS`
       Add build targets for all examples under the `examples/` folder,
       add composite targets `all-examples` and `check-examples` to
       build and run all of them (default: `ON`).

     * `SYSTEMC_INCLUDE_DIRS`
       Path to SystemC include files.

     * `SYSTEMC_LIBRARIES`
       Path to SystemC compiled library.                        

     * `SystemC_TARGET_ARCH`
       Target architecture according to the Accellera SystemC conventions set
       either from `$ENV{SYSTEMC_TARGET_ARCH}`, `$ENV{SYSTEMC_ARCH}`, or
       detected by CMake.

     Other configuration variables will be proposed by CMake depending on the
     OS and detected processor, compiler, and libraries.  For more information,
     please refer to the [CMake documentation][1].

     The configuration step needs to be repeated until no new configuration
     variables appear.

  5. Generate the Makefiles or IDE project files by pressing 'g' or
     clicking on "Generate".

  6. Compile, test, and install the libraries using make or the IDE:

         $ make
         $ make check
         $ sudo make install

More documentation on using CMake is available from the the [CMake Wiki][2].

### 2.3 Cross Compilation

CMake supports cross compilation on a host system for a different
target architecture.  Depending on the scenario, different approaches
can be used.

#### Cross Compilation for 32-bit on a 64-bit platform using GCC

This section is primarily relevant for users of 64-bit versions of
Linux and Windows.

On 64-bit platforms like Linux x86_64 and Windows AMD64, the OS
provides usually for backward-compatibility the capability to execute
64-bit and 32-bit applications.  To this end, it provides 32-bit and
64-bit versions of all runtime libraries (on Linux, they can be
usually found in the lib/ and lib64/ directories).  On these platforms,
the GCC toolchain is usually installed in a multi-lib variant, which
supports the compilation and linking of 64-bit and 32-bit applications
and libraries.  The word width of the target architecture is simply
chosen with the `-m64` or `-m32` option, which has to be consistently
passed at each compiler and linker call.

Therefore, it is in principle sufficient to add `-m32` to the
corresponding CMake variables containing the flags to be passed to the
linker and compiler:

       CMAKE_C_FLAGS
       CMAKE_CXX_FLAGS
       CMAKE_EXE_LINKER_FLAGS
       CMAKE_MODULE_LINKER_FLAGS
       CMAKE_SHARED_LINKER_FLAGS
       CMAKE_STATIC_LINKER_FLAGS

These flags should be already set upon the first CMake invocation in a
new build directory, as most system introspection results are cached
over multiple CMake runs.  You can do so via the `-D<VAR>=<VALUE>`
option of `cmake`, `ccmake`, and `cmake-gui`.

In a similar way, the particular C or C++ compiler can be chosen by
providing the name (optionally preceded by the path) of the compiler
executables:

       CMAKE_C_COMPILER
       CMAKE_CXX_COMPILER

However, it is very handy to know that CMake honors the standard
environment variables `CC` (C compiler), `CXX` (C++ compiler), `CFLAGS`
(C compiler flags), `CXXFLAGS` (C++ compiler flags), and `LDFLAGS` (linker
flags), which are also used by other tools such as GNU Make, to
initialize the above variables upon its first call in a new build
directory. Note, that `CPPFLAGS` is currently ignored by CMake, see
[this issue][3].

On certain platforms, CMake might have trouble to find the right
library for the target processor architecture when using
`find_package()` or `find_library()` in the CMake scripts.  In that case, it
can help to explicitly set the following global property in your CMake
file before issuing any `find_package()` or `find_library()` in your CMake
script:

       set_property (GLOBAL FIND_LIBRARY_USE_LIB64_PATHS <TRUE|FALSE>)

If set to `TRUE`, it will look for the library in `lib64/` instead of `lib/`.
This property is usually automatically set on platforms known to need
it, but depending on the chosen compiler options, it may need adjustment.

#### Cross Compilation on macOS

On macOS, a similar approach as in
[Section 3.1](#31-microsoft-visual-studio-c-2017-compiler-version-150-or-later)
can be adopted.  However,
it has to be noted that Apple's versions of GCC and Clang don't support
`-m32` and `-m64`, but the more flexible `-arch <arm64|x86_64>`
parameter.  The latter can be even repeated several times on the command
line to create universal binaries able to run on several processor
architectures.  CMake has native support for this command line option,
which value can be set centrally in the CMake scripts and will be then
passed on to the compilers and linker.
It is sufficient to set the variable `CMAKE_OSX_ARCHITECTURES` to the
desired target architecture(s).  Multiple architectures are separated
with a `;`. You can do so already on the command line

      $ cmake .. -DCMAKE_OSX_ARCHITECTURES='x86_64;arm64'

or in variable editors of `ccmake` and `cmake-gui`.

If you are using MacPorts or Homebrew versions of GCC, you will have
to make sure that you install universal variants of the compiler
toolchain.  Then, you can follow the instruction in
[Section 3.1](#31-microsoft-visual-studio-c-2017-compiler-version-150-or-later).

#### Generic Cross Compilation to a Target System

As CMake cannot guess the target system, a number of variables have to
be preset so that the correct compiler toolchain and libraries are
found.  It is recommended to do this with a so-called toolchain file,
which is passed as an argument to `cmake`, `ccmake` or `cmake-gui` using the
option `-DCMAKE_TOOLCHAIN_FILE=<path-to-file>`. The file in CMake
syntax has to define the following variables:

 * `CMAKE_SYSTEM_NAME` (mandatory), e.g., "Linux", "Windows", "Darwin"
 * `CMAKE_SYSTEM_VERSION` (optional)`
 * `CMAKE_SYSTEM_PROCESSOR` (optional), e.g., "i386", "x86_64"
 * `CMAKE_C_COMPILER` (mandatory), name of C compiler executable
 * `CMAKE_CXX_COMPILER` (mandatory), name of C compiler executable
 * `CMAKE_FIND_ROOT_PATH` (optional), root to target include and library directories
 * `CMAKE_FIND_ROOT_PATH_MODE_PROGRAM`, either "NEVER", "ONLY", or "BOTH" (default)
 * `CMAKE_FIND_ROOT_PATH_MODE_LIBRARY`, either "NEVER", "ONLY", or "BOTH" (default)
 * `CMAKE_FIND_ROOT_PATH_MODE_INCLUDE`, either "NEVER", "ONLY", or "BOTH" (default)

The necessary steps are discussed in detail on the CMake [Cross Compiling][4] page.


## 3. Installation Notes for Windows using Visual Studio C++

This release has been tested on Visual C++ version 2019 running Windows 11.  Older platforms before Windows 11 are not supported.

The following environment variables are required:

 * SYSTEMC_HOME    - path to SystemC installation root directory
 * SYSTEMC_MSVC    - name of the SystemC MSVC directory (e.g. msvc16)
 * CCI_HOME        - path to CCI installation
 * CCI_MSVC        - name of the CCI MSVC directory (e.g. msvc16)
 * RAPIDJSON_HOME  - path to RapidJSON library

Note: _This section covers the installation based on Microsoft Visual C++.
      For Cygwin or MinGW-based installations, see Section 1._

NOTE: _If you experience spurious errors about missing files in the
      downloaded archive, please make sure to download the latest
      archive from `github.com/accellera-official`._

### 3.1 Microsoft Visual Studio C++ 2017 (compiler version 15.0) or later

The download directory contains two subdirectories: `msvc16` and
`examples/build-msvc`.

#### Build the SystemC CCI library with MSVC

The `msvc16` directory contains the project and workspace files to
compile the `cci.lib` library. Double-click on the `cci.sln`
file to launch Visual C++ with the workspace file. The workspace file
will have the proper switches set to compile for Visual C++ 2010.
Select *Build Solution* under the *Build* menu or press Ctrl+Shift+B to 
build `cci.lib`.

#### Build the SystemC CCI examples with MSVC

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

The provided project files are prepared for both the 32-bit 'Win32' and
64-bit 'x64' configurations.  Please refer to the Microsoft Visual Studio
documentation for details about 64-bit builds.

### 3.2 Creating SystemC CCI Applications

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
   `$(CCI_HOME)\src;$(SYSTEMC_HOME)\src;$(RAPIDJSON_HOME)\include;`
   
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

Also make sure that the Windows environment variables `CCI_HOME`, `SYSTEMC_HOME`, and
`RAPIDJSON_HOME` are set and point to the installation directory for each package.


### 3.3 Building against a SystemC DLL

In order to link your application against a DLL-build of SystemC (build
configurations `DebugDLL`, `ReleaseDLL` in the SystemC library build),
several changes are needed.

 1. Adjust the linker library directory settings to reference `DebugDLL`
    (or `ReleaseDLL`) instead of `Debug` or `Release`, respectively:  
    `...\systemc\msvc16\systemc\DebugDLL`

 2. Add the preprocessor switch `SC_WIN_DLL` to your project's properties  
    (C/C++ -> Preprocessor -> Preprocessor Definitions).

 3. When running the simulation, you need to add the location of the
    SystemC DLL to your `PATH` variable.


## 4. Known Problems

 - The CMake build scripts require more thorough testing of the various build
   configurations on as many platforms as possible.  For the moment, testing
   has been primarily done on macOS (x86_64 and i686), Linux (x86_64 and
   i686) and Windows 10 using Visual Studio 2019 (16) (Win32 and x64).

## 5. Resources

 * [CMake][1]
 * [CMake Wiki][2]
 * [Mastering CMake][5]
 * [The MacPorts Project Homepage][6]
 * [Homebrew - The missing package manager for macOS][7]


[1]: https://cmake.org/ "Cross Platform Make Homepage"
[2]: https://gitlab.kitware.com/cmake/community/-/wikis/Home
[3]: https://gitlab.kitware.com/cmake/cmake/-/issues/12928
[4]: https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html#cross-compiling
[5]: https://cmake.org/cmake/help/book/mastering-cmake/
[6]: https://www.macports.org/
[7]: https://brew.sh/
