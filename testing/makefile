## SOURCE
AUTOBUILDS  = speed_test
BUILDS      =
MODULES     =
STDMODS     = 
LIBS        =
LIBPATH     = $(HOME)/lib/cpp/
STDMODPATH  = .

## SECONDARY
UPFILES     = 
PATHFILE    = .structure

## FlAGS
BUILD_PATH_FILE=0
WITH_PATH_ARGS=0


## CUSTOM PATHS
CURRENT_DIR :=  $(dir $(abspath $(MAKEFILE_LIST)))
CUSTOMPATHS =   PATH_FIG=\"$(CURRENT_DIR)fig/\"            \
                PATH_OUTPUT=\"$(CURRENT_DIR)output/\"
PATHARGS    =   $(CUSTOMPATHS:%=-DPRE_%)


## COMMANDS
CC      = g++
CFLAGS  = -Wall -std=c++11 -O2
LDFLAGS =
RM      = rm -f


## FILES
GARBAGE = *.o *.mod *.pyc
EXEC    = *.out *.exe
OUTPUT  = .out
SOURCE  = .cpp
OBJECT  = .o


## UPLOAD
UPHOST  = jangu@babel.nt.ntnu.no:~/web/
UPC     = scp
UPFLAGS =


## DEFS
AUTOBUILDS  := $(AUTOBUILDS:%=%$(OUTPUT))
BUILDS      := $(BUILDS:%=%$(OUTPUT))
MODULES     := $(MODULES:%=%$(OBJECT))
STDMODS     := $(STDMODS:%=%$(OBJECT))

LIBARGS     = -L$(LIBPATH) $(LIBS:%=-l%)
STDMODARGS  = $(STDMODS:%=$(STDMODPATH)/%)

define LINK
$(CC) $(LDFLAGS) -o $@ $^ $(LIBARGS) $(STDMODARGS)
endef

define COMPILE
$(CC) $(CFLAGS) -c $<
endef

define INITARGS

endef


## FLAG SWITCH
ifeq ($(BUILD_PATH_FILE), 1)
INITARGS += $(PATHFILE)
endif

ifeq ($(WITH_PATH_ARGS), 1)
COMPILE += $(PATHARGS)
endif


## GENERAL
all: init builds

builds: $(BUILDS) $(AUTOBUILDS)

modules: $(MODULES)

stdmods: $(STDMODS)


## DEPENDENCIES



## PATTERNS
$(AUTOBUILDS): %$(OUTPUT): %$(OBJECT)
	$(LINK)

$(MODULES): %$(OBJECT): %$(SOURCE) $(STDMODS)
	$(COMPILE)

$(STDMODS): %$(OBJECT): %$(SOURCE)
	$(COMPILE)

%$(OBJECT): %$(SOURCE) $(STDMODS)
	$(COMPILE)


## UTILS
.INTERMEDIATE: $(AUTOBUILDS:%$(OUTPUT)=%$(OBJECT)) $(BUILDS:%$(OUTPUT)=%$(OBJECT))
.PHONY: all remake clean vclean init builds modules stdmods upload

init: $(INITARGS)

remake: vclean
	$(MAKE) --always-make all

upload:
	$(UPC) $(UPFLAGS) $(UPFILES) $(UPHOST)

clean:
	$(RM) $(GARBAGE)

vclean: clean
	$(RM) $(EXEC)
	$(RM) $(PATHFILE)


## PATH FILE
$(PATHFILE):
	@touch $(PATHFILE)
	@$(foreach PATH, $(CUSTOMPATHS), echo "$(PATH)" >> $(PATHFILE);) 
	@echo >> $(PATHFILE)
	@echo "Building path file: $(PATHFILE)"

