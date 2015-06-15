# http://blog.jgc.org/2007/01/what-makefile-am-i-in.html
# Note: Do not reuse variable names from other make files!
libmaer_thisFile:= $(abspath $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))
libmaer_thisDir := $(shell dirname $(libmaer_thisFile))



# directory which contains libraries
export LIBMAER_DIR   = ${libmaer_thisDir}

# include directory
#export LIBMAER_INC_DIR   = ${libmaer_thisDir}

## all boost libs except libLIBMAER_python because this needs pthyon lib
## as described here: http://mail-index.netbsd.org/pkgsrc-bugs/2010/05/14/msg038006.html
#export LIBMAER_LDXXLIBS  := $(shell \
#				find ${LIBMAER_LIB_DIR} -name \*.so -type f -exec basename {} \;   \
#				| sed 's/^lib/-l/'                                                 \
#				| sed 's/\.so$$//'                                                 \
#				| sed ':a;N;$$!ba;s/\n/ /g'                                        \
#			)


## linker flags
#export LIBMAER_LDXXFLAGS = -L${LIBMAER_DIR}

## compiler flags
#export LIBMAER_CPPFLAGS  = -I${LIBMAER_INC_DIR}



#libMaEr_show:
#	@echo "libMaEr_show:"
#	@echo "==========="
#	@echo "libmaer_thisFile  = $(libmaer_thisFile)";
#	@echo "libmaer_thisDir   = $(libmaer_thisDir)";
#	@echo "LIBMAER_LIB_DIR   = $(LIBMAER_LIB_DIR)";
#	@echo "LIBMAER_INC_DIR   = $(LIBMAER_INC_DIR)";
#	@echo "LIBMAER_LDXXLIBS  = $(LIBMAER_LDXXLIBS)";
#	@echo "LIBMAER_LDXXFLAGS = $(LIBMAER_LDXXFLAGS)";
#	@echo "LIBMAER_CPPFLAGS  = $(LIBMAER_CPPFLAGS)";
#	@echo "==========="
#	@#$(abspath $(filter %libMaEr.mak, $(MAKEFILE_LIST)))


