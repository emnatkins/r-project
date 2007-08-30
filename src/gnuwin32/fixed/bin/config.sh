## config -- Simple shell script to get the values of basic R configure
## variables, or the header and library flags necessary for linking
## against R.
##
## Usage:
##   R CMD config [options] [VAR]

## Copyright (C) 2002-6 The R Core Development Team
##
## This document is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
##
## This program is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## A copy of the GNU General Public License is available at
## http://www.r-project.org/Licenses/

revision='$Revision: 42314 $'
version=`set - ${revision}; echo ${2}`
version="R configuration information retrieval script ${version}

Copyright (C) 2002-6 The R Core Development Team.
This is free software; see the GNU General Public License version 2
or later for copying conditions.  There is NO warranty."

usage="Usage: R CMD config [options] [VAR]

Get the value of a basic R configure variable VAR which must be among
those listed in the 'Variables' section below, or the header and
library flags necessary for linking against R.

Options:
  -h, --help            print short help message and exit
  -v, --version         print 'config' version info and exit
      --cppflags        print pre-processor flags required to compile
                        a program using R as a shared library
      --ldflags         print linker flags needed for linking against
                        the R shared library

Variables:
  BLAS_LIBS     flags needed for linking against external BLAS libraries 
  CC            C compiler command
  CFLAGS        C compiler flags
  CPICFLAGS     special flags for compiling C code to be turned into a
                shared library 
  CPP           C preprocessor
  CPPFLAGS      C/C++ preprocessor flags, e.g. -I<dir> if you have
                headers in a nonstandard directory <dir> 
  CXX           C++ compiler command
  CXXCPP        C++ preprocessor
  CXXFLAGS      C++ compiler flags
  CXXPICFLAGS   special flags for compiling C++ code to be turned into a
                shared library
  DYLIB_LD      command for linking dynamic libraries which contain
                object files from a C or Fortran compiler only
  DYLIB_LDFLAGS
                special flags used by DYLIB_LD
  F77           Fortran 77 compiler command
  FFLAGS        Fortran 77 compiler flags
  FLIBS         linker flags needed to link Fortran code
  FPICFLAGS     special flags for compiling Fortran code to be turned
                into a shared library 
  JAR           Java archive tool command
  JAVA          Java interpreter command
  JAVAC         Java compiler command
  JAVAH         Java header and stub generator command
  JAVA_HOME     path to the home of Java distribution
  JAVA_LIBS     flags needed for linking against Java libraries
  JAVA_CPPFLAGS C preprocessor flags needed for compiling JNI programs
  LAPACK_LIBS   flags needed for linking against external LAPACK libraries
  LIBnn         location for libraries, e.g. 'lib' or 'lib64' on this platform
  LDFLAGS       linker flags, e.g. -L<dir> if you have libraries in a
                nonstandard directory <dir> 
  OBJC          Objective C compiler command
  OBJCFLAGS     Objective C compiler flags
  MAKE          Make command
  SAFE_FFLAGS   Safe (as conformant as possible) Fortran 77 compiler flags
  SHLIB_CFLAGS  additional CFLAGS used when building shared libraries
  SHLIB_CXXLD   command for linking shared libraries which contain
                object files from a C++ compiler 
  SHLIB_CXXLDFLAGS
                special flags used by SHLIB_CXXLD
  SHLIB_FFLAGS  additional FFLAGS used when building shared libraries 
  SHLIB_LD      command for linking shared libraries which contain
                object files from a C or Fortran compiler only 
  SHLIB_LDFLAGS
                special flags used by SHLIB_LD
  TCLTK_CPPFLAGS
                flags needed for finding the tcl.h and tk.h headers
  TCLTK_LIBS    flags needed for linking against the Tcl and Tk libraries

Report bugs to <r-bugs@r-project.org>."

## <NOTE>
## The variables are basically the precious configure variables (with
## the R_* and MAIN_* ones removed), plus FLIBS and BLAS_LIBS.
## One could use
##   precious_configure_vars=`~/src/R/configure --help \
##     | sed -n '/^Some influential/,/^[^ ]/p' \
##     | sed '/^[^ ]/d' \
##     | sed 's/^  //' \
##     | cut -f1 -d ' ' \
##     | grep -v '^MAIN_' \
##     | grep -v '^R_' \
##     | sort \
##     | uniq`
## to obtain the configure vars and hence create most of the above usage
## info as well as the list of accepted variables below automatically.
## </NOTE>

if test $# = 0; then
  echo "${usage}"
  exit 1
fi

MAKE=make
R_HOME=${R_HOME}
R_DOC_DIR=${R_HOME}/doc
R_INCLUDE_DIR=${R_HOME}/include
R_SHARE_DIR=${R_HOME}/share
R_ARCH=
makefiles="-f ${R_HOME}/etc${R_ARCH}/Makeconf -f ${R_SHARE_DIR}/make/config.mk"
query="${MAKE} -s ${makefiles} print R_HOME=${R_HOME}"

LIBR=`eval $query VAR=LIBR`

var=
while test -n "${1}"; do
  case "${1}" in
    -h|--help)
      echo "${usage}"; exit 0 ;;
    -v|--version)
      echo "${version}"; exit 0 ;;
    --cppflags)
      if test -z "${LIBR}"; then
        echo "R was not built as a shared library" >&2
      else
        echo "-I${R_INCLUDE_DIR}"
      fi
      exit 0
      ;;
    --ldflags)
      if test -z "${LIBR}"; then
        echo "R was not built as a shared library" >&2
      else
        echo "${LIBR}"
      fi
      exit 0
      ;;
    *)
      if test -z "${var}"; then
        var="${1}"
      else
        echo "ERROR: cannot query more than one variable" >&2
        exit 1
      fi
      ;;
  esac
  shift
done

ok_c_vars="CC CFLAGS CPICFLAGS CPP CPPFLAGS"
ok_cxx_vars="CXX CXXCPP CXXFLAGS CXXPICFLAGS"
ok_dylib_vars="DYLIB_LD DYLIB_LDFLAGS"
ok_objc_vars="OBJC OBJCFLAGS"
ok_java_vars="JAVA JAVAC JAVAH JAR JAVA_HOME JAVA_LIBS JAVA_CPPFLAGS"
ok_f77_vars="F77 FFLAGS FPICFLAGS FLIBS SAFE_FFLAGS FC FCFLAGS"
ok_ld_vars="LDFLAGS"
ok_shlib_vars="SHLIB_CFLAGS SHLIB_CXXLD SHLIB_CXXLDFLAGS SHLIB_FFLAGS SHLIB_LD SHLIB_LDFLAGS"
ok_tcltk_vars="TCLTK_CPPFLAGS TCLTK_LIBS"
ok_other_vars="BLAS_LIBS LAPACK_LIBS MAKE LIBnn"

## Can we do this elegantly using case?

var_ok=no
for v in ${ok_c_vars} ${ok_cxx_vars} ${ok_dylib_vars} ${ok_f77_vars} \
         ${ok_objc_vars} ${ok_java_vars} \
         ${ok_ld_vars} ${ok_shlib_vars} ${ok_tcltk_vars} \
	 ${ok_other_vars}; do
  if test "${var}" = "${v}"; then
    var_ok=yes
    break
  fi
done

if test "${var_ok}" = yes; then
  eval "${query} VAR=${var}"
else
  echo "ERROR: no information for variable '${var}'"
  exit 1
fi

### Local Variables: ***
### mode: sh ***
### sh-indentation: 2 ***
### End: ***
