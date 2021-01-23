## ****************************************************************************
##
##  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
##  more contributor license agreements.  See the NOTICE file distributed
##  with this work for additional information regarding copyright ownership.
##  Accellera licenses this file to you under the Apache License, Version 2.0
##  (the "License"); you may not use this file except in compliance with the
##  License.  You may obtain a copy of the License at
##
##   http://www.apache.org/licenses/LICENSE-2.0
##
##  Unless required by applicable law or agreed to in writing, software
##  distributed under the License is distributed on an "AS IS" BASIS,
##  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
##  implied.  See the License for the specific language governing
##  permissions and limitations under the License.
##
## ****************************************************************************

include $(top_srcdir)/config/Make-rules

DOXYGEN=doxygen

## Core documentation (see also root directory)

DOC_FILES =

##
## SystemC CCI documentation
##   find cci -type f -a -not -name "*.in" -printf "\t%p \\\\\n"
DOC_FILES += \
	cci/README \
	cci/archived/0909_analysis_reject_param_access.doc \
	cci/archived/100224 CCI API proposal 1.3.ppt \
	cci/archived/broker_accessor_concept.pdf \
	cci/archived/CCI_Instructions_for_MSVS2015.pdf \
	cci/archived/CODING_STYLE.txt \
	cci/archived/discussions.txt \
	cci/archived/example_review_checklist \
	cci/archived/GreenControl_GreenConfig_CODES09_Schroeder_p81.pdf \
	cci/archived/guidelines.txt \
	cci/archived/notes.txt \
	cci/archived/cci_param_concept.jpg


## ****************************************************************************

## install documentation?
if SEPARATE_INSTALL_TREE

nobase_doc_DATA = \
	$(DOC_FILES)

## Install, but do not distribute Doxygen configuration
nobase_doc_DATA += \
	cci/doxygen/Doxyfile

DOCDIRS = \
	cci/archived \
	cci/doxygen \
	cci


uninstall-hook:
	test ! -d "$(docdir)" || ( set -e ; cd "$(docdir)" ; \
	  for dir in $(DOCDIRS) ; do test ! -d $$dir || rmdir $$dir ; done ; \
	  cd - ; rmdir --ignore-fail-on-non-empty "$(docdir)" )

endif # SEPARATE_INSTALL_TREE

## add documentation to distribution
EXTRA_DIST += \
	$(DOC_FILES)

# TODO:
# EXTRA_DIST += CMakeLists.txt

doxygen: cci/doxygen/Doxyfile
	@(cd cci/doxygen && $(DOXYGEN))

%/Doxyfile: %/Doxyfile.in
	(cd $(top_builddir) && $(SHELL) config.status docs/$@)

## Taf!
