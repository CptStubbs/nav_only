#
# Top level makefile for Pyxis project. Calls the other makefiles as needed
# Compilers Gcc gnu-arm-linux-gcc arm-linux-gnueabi-gcc
# Edits should be done primarily to the other makefiles. 
# The nested makefiles should not reset any global variables defined here. 
# Update the subdirs when new ones are added

CFLAGS=-std=gnu99
LFLAGS=-std=gnu99

#Include the custome make rules 
include conf/conf.mk
include conf/rules.mk


#Compilers
export ARMCC=arm-linux-gnueabi-gcc
export GCC=gcc
export ARMAR=arm-linux-gnueabi-ar 
export GAR=ar

export INCDIR=../../src/include
export CONFDIR=../../src/conf
export INCLIB=../../bin/include
export DOCNAME=refman.pdf
#Optimization and debuggin
export DEBUGFLAGS=-g -pg #-fprofile-arcs -ftest-coverage
#export DEBUGFLAGS=-pg -g -DDEBUG 
#export OPTFLAGS= -O0  
export OPTFLAGS= -O3  -funroll-loops -funit-at-a-time 
export ARFLAGS=rcs
export CFLAGS
export LFLAGS

#Default
CC=$(GCC)
AR=$(GAR)
#Run the code for arm
arm: CC=$(ARMCC)
arm: AR=$(ARMAR)
arm: OPTFLAGS +=  -mcpu=cortex-a9 -mfpu=neon -mvectorize-with-neon-quad 
arm: OPTFLAGS +=  -mfloat-abi=softfp 
all: CC=$(GCC)
clean: CC=$(GCC)

export CC
CFLAGS+= -DCCNAME=\"$(CC)\"
#Dependicies (conf files)
DEPSDIR = ../src/conf
export DEPS = $(realpath $(wildcard $(DEPSDIR)/*.h)) 
#Targets
SUBDIRS= nav
.PHONY: check clean all arm subdirs $(SUBDIRS)

all: subdirs

arm: subdirs

check: 
	for d in $(SUBDIRS); do ($(MAKE) -C $$d DIR=$$d check); done

subdirs: $(SUBDIRS) 

nav: include

$(SUBDIRS): 
	$(MAKE) -C $@ DIR=$@

clean :
	for d in $(SUBDIRS); do (cd $$d; $(MAKE) clean DIR=$$d ); done

