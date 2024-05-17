### makefile for findlib

SYS_VERS  := r$(shell uname -r)
OBJ_DIR := obj.$(SYS_VERS)
DEP_DIR := depends




OPTIMIZER		=	-O3

CC		     	=	gcc

# This is needed to be able to compile on either nightlies, or beta4.
# See https://dev.haiku-os.org/ticket/11818 (fixed for good in hrev57497).
# We can simplify this once the haikuports buildmasters switch to beta5.

HAIKU_HREV_NUM = $(shell uname -v | cut -d " " -f 1 | cut -d "+" -f 1 | sed s/hrev//)
HAS_FIXED_FEATURES_H = $(shell if [ $(strip $(HAIKU_HREV_NUM)) -ge 57497 ]; then echo "YES"; else echo "NO"; fi)

ifeq ($(strip $(HAS_FIXED_FEATURES_H)), YES)
	CFLAGS	=	$(OPTIMIZER) -Wall -Wno-multichar -Wno-ctor-dtor-privacy
else
	CFLAGS	=	$(OPTIMIZER) -Wall -Wno-multichar -Wno-ctor-dtor-privacy -D__USE_GNU -DSTDC_HEADERS
endif

LD				=	gcc
LDFLAGS			=	
SHAREDLIB_FLAGS	=	-shared -Xlinker -soname=$(notdir $@)


MAKE_DEPEND		 = ../../etc/MakeDepend.sh $(CFLAGS) -c $< -dep $@
MAKE_CC			 = $(CC) $(CFLAGS) -c $< -o $@


FINDLIB_DIR := ..
FREGEX_DIR  := $(FINDLIB_DIR)/../FRegex

include $(FREGEX_DIR)/etc/makefile.defs
include makefile.defs

all: $(FINDLIB_TGT)

include makefile.rules
include $(FREGEX_DIR)/etc/makefile.rules

%$(DEP_DIR)::
	@[ -d $@ ] || mkdir $@ > /dev/null 2>&1

%$(OBJ_DIR)::
	@[ -d $@ ] || mkdir $@ > /dev/null 2>&1
