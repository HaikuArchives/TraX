### makefile for findlib

OBJ_DIR := obj
DEP_DIR := depends




OPTIMIZER		=	-O0

CC		     	=	gcc
CFLAGS			=	$(OPTIMIZER) -Wall -Wno-multichar -Wno-ctor-dtor-privacy

LD				=	gcc
LDFLAGS			=	
SHAREDLIB_FLAGS	=	-nostart -Xlinker -soname=$(notdir $@)




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
