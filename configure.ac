##                            -*- Autoconf -*-
##****************************************************************************
##
##  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
##  more contributor license agreements.  See the NOTICE file distributed
##  with this work for additional information regarding copyright ownership.
##  Accellera licenses this file to you under the Apache License, Version 2.0
##  (the "License"); you may not use this file except in compliance with the
##  License.  You may obtain a copy of the License at
##
##    http://www.apache.org/licenses/LICENSE-2.0
##
##  Unless required by applicable law or agreed to in writing, software
##  distributed under the License is distributed on an "AS IS" BASIS,
##  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
##  implied.  See the License for the specific language governing
##  permissions and limitations under the License.
##
##****************************************************************************

AC_PREREQ(2.61)

AC_INIT([CCI],
        [1.0],
        [https://forums.accellera.org/forum/41-systemc-cci-configuration-control-inspection],
        [cci],
        [http://accellera.org],
)

AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([config])
AC_CONFIG_SRCDIR([src/cci_configuration])
AC_CONFIG_HEADERS([src/config.h])

AC_DEFINE_UNQUOTED(CONFIGURE_DATE, "`sh -c date`", [Date of configure])
AC_SUBST(CONFIGURE_DATE)

AC_DEFINE_UNQUOTED(CCI_VERSION, "${PACKAGE_VERSION}", [CCI Version])
AC_DEFINE_UNQUOTED(CCI_RELEASE_DATE, "2018-06-20", [CCI Release Date])
AC_SUBST(CCI_VERSION)
AC_SUBST(CCI_RELEASE_DATE)

AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([1.9.4 tar-pax -Werror -Wno-portability no-define subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES(yes)])

# ignore GNU defaults for CFLAGS and CXXFLAGS
: ${CFLAGS=""}
: ${CXXFLAGS=""}

AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S


# figure out architecture dependent variables

CXX_COMP=`set $CXX; basename $1`

EXTRA_CXXFLAGS="-Wall -pedantic -Wno-long-long -Wno-variadic-macros"
DEBUG_CXXFLAGS="-g"
OPT_CXXFLAGS="-O3"
LDFLAG_RPATH="-Wl,-rpath="

case "$depmode" in
    gcc3)
        EXTRA_CXXFLAGS="${EXTRA_CXXFLAGS} -fmessage-length=0"
        ;;
    *)
        ;;
esac

case "$target" in
    *-apple-*)
        case "$target_cpu" in
            x86_64)
                TARGET_ARCH="macosx64"
                CPU_ARCH="x86_64"
                ;;
            x386 | i386)
                TARGET_ARCH="macosx"
                CPU_ARCH="i386"
                ;;
            powerpc)
                TARGET_ARCH="macosxppc"
                CPU_ARCH="ppc"
                ;;
            powerpc64)
                TARGET_ARCH="macosxppc64"
                CPU_ARCH="ppc64"
                ;;
            *)
                AC_MSG_ERROR("sorry... architecture not supported.")
                ;;
        esac
        if test "$GXX" = "yes"; then
                EXTRA_CXXFLAGS="${EXTRA_CXXFLAGS} -arch ${CPU_ARCH}"
                LDFLAG_RPATH="-Wl,-rpath -Wl,"
        else
                AC_MSG_ERROR("sorry... compiler not supported.")
        fi
        ;;
    sparc-sun-solaris*)
        if test "$GXX" = "yes"; then
                TARGET_ARCH="gccsparcOS5"
                LDFLAG_RPATH="-Wl,-R"
        else
            if test "$CXX_COMP" = "CC"; then
                EXTRA_CXXFLAGS=""
                OPT_CXXFLAGS="-fast -xO4 -xnolibmil -mc"
                TARGET_ARCH="sparcOS5"
                LDFLAG_RPATH="-Wl,-R"
            else
                AC_MSG_ERROR("sorry... compiler not supported.")
            fi
        fi
        ;;
    *linux*)
        case "$target_cpu" in
            x86_64 | amd64)
                TARGET_ARCH="linux64"
                CPU_ARCH=64
                ;;
            i*86)
                TARGET_ARCH="linux"
                CPU_ARCH=32
                ;;
            *)
                AC_MSG_ERROR("sorry... architecture not supported.")
            ;;
        esac
        if test "$GXX" = "yes"; then
                EXTRA_CXXFLAGS="${EXTRA_CXXFLAGS} -m${CPU_ARCH}"
        else
                AC_MSG_ERROR("sorry... compiler not supported.")
        fi
        ;;
    *freebsd* | dragonfly* | *netbsd* | openbsd*)
        case "$target_cpu" in
            x86_64 | amd64)
                TARGET_ARCH="bsd64"
                CPU_ARCH=64
                ;;
            i*86)
                TARGET_ARCH="bsd"
                CPU_ARCH=32
                ;;
            *)
                AC_MSG_ERROR("sorry... architecture not supported.")
            ;;
        esac
        if test "$GXX" = "yes"; then
                EXTRA_CXXFLAGS="${EXTRA_CXXFLAGS} -m${CPU_ARCH}"
        else
                AC_MSG_ERROR("sorry... compiler not supported.")
        fi
        ;;
    *cygwin*)
        case "$target_cpu" in
            x86_64 | amd64)
                TARGET_ARCH="cygwin64"
                CPU_ARCH=64
                ;;
            i*86)
                TARGET_ARCH="cygwin"
                CPU_ARCH=32
                ;;
            *)
                AC_MSG_ERROR("sorry... architecture not supported.")
            ;;
        esac
        if test "$GXX" = "yes"; then
                EXTRA_CXXFLAGS="${EXTRA_CXXFLAGS} -m${CPU_ARCH}"
        else
                AC_MSG_ERROR("sorry... compiler not supported.")
        fi
        ;;
    *mingw*)
        case "$target_cpu" in
            x86_64 | amd64)
                TARGET_ARCH="mingw64"
                CPU_ARCH=64
                ;;
            i*86)
                TARGET_ARCH="mingw"
                CPU_ARCH=32
                ;;
            *)
                AC_MSG_ERROR("sorry... architecture not supported.")
            ;;
        esac
        if test "$GXX" = "yes"; then
                EXTRA_CXXFLAGS="${EXTRA_CXXFLAGS} -m${CPU_ARCH}"
        else
                AC_MSG_ERROR("sorry... compiler not supported.")
        fi
        ;;
esac

AC_SUBST(LDFLAG_RPATH)
AC_SUBST(TARGET_ARCH)


# append additional flags to configure script

AC_MSG_CHECKING(whether to include debug symbols)
AC_ARG_ENABLE([debug],
  AS_HELP_STRING([--enable-debug],
                 [include debug symbols]),
  [case "$enableval" in
    "no" | "yes")  enable_debug=$enableval;;
    *)             AC_MSG_ERROR(bad value "$enableval" for --enable-debug);;
  esac],
  [enable_debug="not defined"])
AC_MSG_RESULT($enable_debug)


AC_MSG_CHECKING(whether to enable code profiling)
AC_ARG_ENABLE([profiling],
  [AS_HELP_STRING([--enable-profiling@<:@=PROGRAM@:>@],
                  [generate instrumentation calls for code
                   profiling, supported profilers: prof,
                   gprof, instrument (-finstrument-functions)
                   @<:@default=no@:>@])],
  [case "$enableval" in
    "no" | "prof" | "gprof" | "instrument")  enable_profiling=$enableval;;
    "yes")  AC_MSG_ERROR(no profiler defined with --enable-profiling);;
    *)      AC_MSG_ERROR(bad value "$enableval" for --enable-profiling);;
  esac],
  [enable_profiling="no"]
)
AC_MSG_RESULT($enable_profiling)

if test "$enable_profiling" = "no"; then
  if test "$enable_debug" = "not defined"; then
    enable_debug="no"
  fi
else
  case "$enable_debug" in
    "not defined")  enable_debug="yes";;
    "no")           AC_MSG_ERROR(contradicting options: 
           Cannot have --disable-debug with --enable-profiling.
           Please re-run configure with only one of these options 
           at the same time.);;
  esac
  case "$enable_profiling" in
    "prof")        DEBUG_CXXFLAGS="${DEBUG_CXXFLAGS} -p";;
    "gprof")       DEBUG_CXXFLAGS="${DEBUG_CXXFLAGS} -pg";;
    "instrument")  DEBUG_CXXFLAGS="${DEBUG_CXXFLAGS} -finstrument-functions";;
  esac
fi
AM_CONDITIONAL([WANT_DEBUG], [test "$enable_debug" = "yes"])


AC_MSG_CHECKING(whether to enable compiler optimizations)
AC_ARG_ENABLE([optimize],
  AS_HELP_STRING([--disable-optimize],
                 [disable compiler optimizations]),
  [case "$enableval" in
    "no" | "yes")  enable_optimize=$enableval;;
    *)             AC_MSG_ERROR(bad value "$enableval" for --enable-optimize);;
  esac],
  [enable_optimize="yes"])
AC_MSG_RESULT($enable_optimize)
AM_CONDITIONAL([WANT_OPTIMIZE], [test "$enable_optimize" = "yes"])

AC_SUBST(EXTRA_CXXFLAGS)
AC_SUBST(DEBUG_CXXFLAGS)
AC_SUBST(OPT_CXXFLAGS)


AC_MSG_CHECKING(how installed files will be laid out)
AC_ARG_WITH([layout],
  [AS_HELP_STRING([--with-layout@<:@=TYPE@:>@],
                  [sets how installed files will be laid out,
                   possible directory layouts: systemc, unix
                   @<:@default=systemc@:>@])],
  [case "$withval" in
    "systemc")  want_install_layout="SystemC";;
    "unix")     want_install_layout="UNIX";;
    *)          want_install_layout="$withval";;
  esac],
  [want_install_layout="SystemC"]
)

abs_builddir=`pwd`
abs_srcdir=`cd "$srcdir"; pwd`

# set default prefix: $srcdir
# preset SystemC directory layout if installing to $srcdir
if test "$prefix" = "NONE"; then
  prefix="$abs_srcdir"
  want_install_layout="SystemC"
fi
AC_MSG_RESULT($want_install_layout)

# strip trailing slash unless $prefix="/"
if test "$prefix" != "/"; then
  prefix=`echo "$prefix" | $AWK '{sub(/\/$/, "")};1'`
fi

if test "$abs_srcdir" != "$abs_builddir"; then
  if test "$prefix" = "$abs_builddir"; then
    AC_MSG_ERROR(Installation to build directory not supported.)
  fi
fi

case "$want_install_layout" in
  "SystemC")
    if test "$datarootdir" = '${prefix}/share'; then
      datarootdir="$prefix"
    fi
    if test "$docdir" = '${datarootdir}/doc/${PACKAGE_TARNAME}'; then
      docdir="$prefix/docs"
      rootdocdir="$prefix"
    else
      rootdocdir="$docdir"
    fi
    have_arch_suffix="default";
    LIB_ARCH_SUFFIX="-${TARGET_ARCH}"
    ;;
  "UNIX")
    # keep Automake/GNU/UNIX defaults
    rootdocdir="$docdir"
    have_arch_suffix="ignored";
    LIB_ARCH_SUFFIX=""
    ;;
  *)
    AC_MSG_ERROR(Unknown directory layout requested.)
    ;;
esac


AC_MSG_CHECKING([for SystemC library arch suffix])
AC_ARG_WITH([arch-suffix],
  [AS_HELP_STRING([--with-arch-suffix@<:@=SUFFIX@:>@],
                  [append SUFFIX to library installation directory
                   @<:@default=-$TARGET_ARCH@:>@])],
  [case "$withval" in
     "no" | "")  have_arch_suffix="no";  LIB_ARCH_SUFFIX="";;
     "yes")      have_arch_suffix="yes"; LIB_ARCH_SUFFIX="-${TARGET_ARCH}";;
     *)          have_arch_suffix="yes"; LIB_ARCH_SUFFIX="$withval";;
  esac],
  [ac_dummy=""]
)
AC_MSG_RESULT($have_arch_suffix (TARGET_ARCH=${TARGET_ARCH}))
AC_SUBST(rootdocdir)
AC_SUBST(examplesdir, [${rootdocdir}/examples])
AC_SUBST([libarchdir],  ['${libdir}${LIB_ARCH_SUFFIX}'] )

AM_CONDITIONAL([SEPARATE_INSTALL_TREE],dnl
  [ test "x${prefix}" != "x${abs_srcdir}" ])
dnl Check for out-of-tree build
AM_CONDITIONAL([SEPARATE_BUILD_TREE],dnl
  [ test "x${abs_srcdir}" != "x${abs_builddir}" ])

#AM_COND_IF([SEPARATE_BUILD_TREE],[
if test -z "${SEPARATE_BUILD_TREE_TRUE}" ; then
  if test "x${prefix}" = "x${abs_builddir}" ; then
    AC_MSG_ERROR([Installation to build directory not supported.])
  fi
fi
#])

AC_ARG_WITH([systemc],
  [AS_HELP_STRING([--with-systemc=DIR],
                  [define path to Accellera SystemC library home])],
  [case "$withval" in
    "no")   have_systemc="no";    systemc_home="";;
    "yes")  have_systemc="yes";   systemc_home="";;
    "")     have_systemc="maybe"; systemc_home="";;
    *)      have_systemc="yes";   systemc_home="$withval";;
  esac],
  [have_systemc="maybe"; systemc_home=""]
)

PKG_CHECK_MODULES(SYSTEMC, [systemc >= 2.3.0],
  [have_systemc="yes"],
  [if test -z "$systemc_home"; then
    AC_MSG_CHECKING(for SystemC library home)
    if test -d "${SYSTEMC_HOME}"; then
      builddir=`pwd`;
      systemc_home=`cd ${SYSTEMC_HOME}; pwd`
      AC_MSG_RESULT(found in \$SYSTEMC_HOME)
    elif test -d "`pwd`/systemc-2.3.1"; then
      builddir=`pwd`;
      systemc_home=`cd $builddir/systemc-2.3.1; pwd`
    elif test -d "`pwd`/systemc-2.3.0"; then
      builddir=`pwd`;
      systemc_home=`cd $builddir/systemc-2.3.0; pwd`
    elif test -d "`pwd`/../systemc-2.3.1"; then
      builddir=`pwd`;
      systemc_home=`cd $builddir/../systemc-2.3.1; pwd`
    elif test -d "`pwd`/../systemc-2.3.0"; then
      builddir=`pwd`;
      systemc_home=`cd $builddir/../systemc-2.3.0; pwd`
    fi
    if test -z "$systemc_home"; then
      have_systemc="no"
      AC_MSG_RESULT(not found)
      AC_MSG_ERROR(SystemC library home not found.
           Specify --with-systemc=/path/to/systemc_home pointing to the installed location.)
    else
      have_systemc="yes"
      AC_MSG_RESULT($systemc_home)
    fi
  fi
  if test "$have_systemc" = "yes"; then
    if test -d "$systemc_home"; then
      systemc_home=`cd $systemc_home; pwd`;
    else
      AC_MSG_ERROR(SystemC library home $systemc_home does not exist.
           SystemC might not have been installed correctly.)
    fi
    SYSTEMC_INC_PREFIX="$systemc_home/include"
    SYSTEMC_LIB_PREFIX="$systemc_home/lib${LIB_ARCH_SUFFIX}"
    AC_MSG_CHECKING(for $SYSTEMC_INC_PREFIX/systemc.h)
    if test -r "$SYSTEMC_INC_PREFIX/systemc.h"; then
      AC_MSG_RESULT(yes)
      SYSTEMC_CFLAGS="-I$SYSTEMC_INC_PREFIX"
      SYSTEMC_LIBS="-L$SYSTEMC_LIB_PREFIX -lsystemc"
    else
      AC_MSG_RESULT(no)
      AC_MSG_ERROR(SystemC header files missing.
           SystemC might not have been installed correctly.)
    fi
  fi]
)

AC_ARG_WITH([json],
  [AS_HELP_STRING([--with-json=DIR],
                  [define path to RapidJSON home])],
  [case "$withval" in
    "no")   have_json="no";    json_home="";;
    "yes")  have_json="yes";   json_home="";;
    "")     have_json="maybe"; json_home="";;
    *)      have_json"yes";    json_home="$withval";;
  esac],
  [have_json="maybe"; json_home=""]
)

PKG_CHECK_MODULES(JSON, [rapidjson >= 1.0.0],
  [have_json="yes"],
  [if test -z "$json_home"; then
    AC_MSG_CHECKING(for RapidJSON home)
    if test -d "$RAPIDJSON_HOME}"; then
      builddir=`pwd`;
      json_home=`cd ${RAPIDJSON_HOME}; pwd`
      AC_MSG_RESULT(found in \$RAPIDJSON_HOME)
     elif test -d "`pwd`/rapidjson"; then
      builddir=`pwd`;
      json_home=`cd $builddir/rapidjson; pwd`
     elif test -d "`pwd`/../rapidjson"; then
      builddir=`pwd`;
      json_home=`cd $builddir/../rapidjson; pwd`
     elif test -d "`pwd`/../../rapidjson"; then
      builddir=`pwd`;
      json_home=`cd $builddir/../../rapidjson; pwd`
    fi
    if test -z "$json_home"; then
      have_json="no"
      AC_MSG_RESULT(not found)
      AC_MSG_ERROR(RapidJSON home not found.
           Specify --with-json=/path/to/rapidjson pointing to the installed location.)
    else
      have_json="yes"
      AC_MSG_RESULT($json_home)
    fi
  fi
  if test "$have_json" = "yes"; then
    if test -d "$json_home"; then
      json_home=`cd $json_home; pwd`;
    else
      AC_MSG_ERROR(RapidJSON home $json_home does not exist.
           RapidJSON might not have been installed correctly.)
    fi
    JSON_INC_PREFIX="$json_home/include"
    AC_MSG_CHECKING(for $JSON_INC_PREFIX/rapidjson/document.h)
    if test -r "$JSON_INC_PREFIX/rapidjson/document.h"; then
      AC_MSG_RESULT(yes)
      JSON_CFLAGS="-I$JSON_INC_PREFIX"
    else
      AC_MSG_RESULT(no)
      AC_MSG_ERROR(RapidJSON header files missing.
           RapidJSON might not have been installed correctly.)
    fi
  fi]
)


# Libtool initialisation (always AFTER all the CC options have been set)
# * explicitly use C++ for libtool initialization since we are always
#   compiling C++ sources
AC_LANG([C++])
# * need to pass EXTRA_CXXFLAGS to Libtool via CXXFLAGS to correctly
#   detect C++ library dependencies, esp. for cross-compilation
stored_CXXFLAGS="${CXXFLAGS}"
CXXFLAGS="${EXTRA_CXXFLAGS} ${CXXFLAGS}"
AC_PROG_LIBTOOL
# * restore CXXFLAGS
CXXFLAGS="${stored_CXXFLAGS}"

#check SystemC Version by building an example program
stored_CXXFLAGS="${CXXFLAGS}"
stored_LIBS="${LIBS}"
CXXFLAGS="${JSON_CFLAGS} ${SYSTEMC_CFLAGS} ${CXXFLAGS}"
LIBS="${LIBS} ${SYSTEMC_LIBS}"
AC_MSG_CHECKING( SystemC Version IEEE compliant)
AC_RUN_IFELSE( 
	[AC_LANG_SOURCE([[#include <systemc>
	int sc_main(int argc, char* argv[]) {
	#if (IEEE_1666_SYSTEMC >= 201101L)
	return 0;
	#endif
	return 1;
	}
	]])]
	,  
	[AC_MSG_RESULT([IEEE 1666 compatible SystemC version found ])],
	[AC_MSG_FAILURE([Non IEEE 1666 compatible SystemC version found, make sure that SystemC can be found and you build with the same compile options as building SystemC])]
	)
CXXFLAGS="${stored_CXXFLAGS}"
LIBS="${stored_LIBS}"

AC_SUBST(LIB_ARCH_SUFFIX)

AC_OUTPUT( Makefile
           src/Makefile
           src/cci/Makefile
           src/cci/cci.pc
           examples/Makefile
           examples/cci/Makefile
           docs/Makefile
           docs/cci/doxygen/Doxyfile )

# resolve path variables
CCI_HOME=$prefix
eval "install_prefix=\"${CCI_HOME}\""
eval "install_includedir=\"${includedir}\""
eval "install_includedir=\"${install_includedir}\""
eval "install_libdir=\"${libdir}\""
eval "install_libdir=\"${install_libdir}\""
eval "install_docdir=\"${docdir}\""
eval "install_docdir=\"${install_docdir}\""
eval "install_examplesdir=\"${examplesdir}\""
eval "install_examplesdir=\"${install_examplesdir}\""
eval "install_examplesdir=\"${install_examplesdir}\""
eval "install_examplesdir=\"${install_examplesdir}\""
prefix=$CCI_SYSTEMC_HOME

cat <<EOF | tee -a config.log

------------------------------------------------------------------------------
Configuration summary of $PACKAGE_STRING for $target
------------------------------------------------------------------------------

 Directory setup (based on $want_install_layout layout):
   Installation prefix: ${install_prefix}
   Header files:        ${install_includedir}
   Libraries:           ${install_libdir}
   Documentation:       ${install_docdir}
   Examples:            ${install_examplesdir}

 Target architecture:   ${TARGET_ARCH}

 Build settings:
   Compiler (flags):    ${CXX}
                        ${CXXFLAGS}
   Preprocessor flags:  ${CPPFLAGS}
   SystemC CFLAGS:      ${SYSTEMC_CFLAGS}
   SystemC LIBS:        ${SYSTEMC_LIBS}
   JSON CFLAGS:         ${JSON_CFLAGS}
   Additional LIBS:     ${LIBS}
   Enable compiler optimizations:   $enable_optimize
   Include debug symbols:           $enable_debug
   Generate instrumentation calls:  $enable_profiling
------------------------------------------------------------------------------
EOF


## Taf!
