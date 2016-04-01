#Check if we are inheriting variables
ifndef CFLAGS
	CFLAGS=-std=gnu99
	include ../conf/*.mk
endif
ifndef LFLAGS 
	LFLAGS=-std=gnu99
endif 

ifndef DIR
	DIR=include
endif
ifndef INCDIR
	INCDIR=../../src/include
endif
ifndef CONFDIR
	CONFDIR=../../src/conf
endif
ifndef DOCNAME 
	DOCNAME=refman.pdf
endif
ifndef CC
	CC=gcc
endif
ifeq ($(CC), cc)
	CC=gcc
endif

ifndef AR 
	AR=ar
endif
ifndef ARFLAGS 
	ARFLAGS=rcs
endif
#File Locations
SRCDIR = ../../src/$(DIR)/
TSTDIR = ../../test/$(DIR)/
DOCDIR = ../../doc/$(DIR)/codedocs/latex
BINDIR = ../../bin/$(DIR)

#Documentation 
DOCGEN = doxygen 
DOCFLAGS = 
DOCFILE = ./pyxis_$(DIR).doxyfile 
DOCOUTPUT = $(DOCDIR)/$(DOCNAME) 

#Compile flags
CFLAGS += -c -Wall -Wextra -pedantic -fpic $(OPTFLAGS) $(DEBUGFLAGS)  -I$(INCDIR) 
CFLAGS += -I$(CONFDIR) 
LFLAGS += -Wall -Wextra -pedantic -L$(BINDIR) -static $(OPTFLAGS)  $(DEBUGFLAGS)
LIBFLAGS = -l$(DIR) -lm 

#Compile Targets
SOURCES = $(wildcard $(SRCDIR)/*.c) $(INCDIR)/utils.c
OBJECTS = $(SOURCES:.c=.o) 
	LIBOBJECTS = $(filter-out %/main.o, $(OBJECTS))
	MAINOBJECT = $(filter %/main.o, $(OBJECTS))
	LIBRARY = $(BINDIR)/lib$(DIR).a

#Unit testing
ifneq ($(strip $(CC)), gcc)
	TESTLIBFLAGS = 
	TESTSOURCES =  
else
	TESTLIBFLAGS = -lcheck -lrt -l$(DIR) -lpthread -lm
	TESTSOURCES = $(wildcard $(TSTDIR)/*.c)  
endif
check_PROGRAMS = $(TESTSOURCES:.c=)
check_OBJECTS =  $(TESTSOURCES:.c=.o) 

.PHONY: all clean check docs

all: $(SOURCES) $(EXECUTABLE) $(LIBRARY) docs check


$(EXECUTABLE): $(LIBRARY) $(MAINOBJECT) 
	$(CC) $(LFLAGS) $^ -o $@ $(LIBFLAGS)

$(LIBRARY): $(LIBOBJECTS) $(DEPS)
	$(AR) $(ARFLAGS) $@ -o $^

.c.o: 
	$(CC) $(CFLAGS) $< -o $@

$(check_PROGRAMS): $(TSTDIR)/%: $(LIBRARY) $(TSTDIR)/%.o
	$(CC) $(LFLAGS) $^ -o $@ $(TESTLIBFLAGS)

check: $(check_PROGRAMS)
	for test in $^; do ./$$test; done

docs: $(DOCOUTPUT)

$(DOCOUTPUT):$(EXECUTABLE)
	cd $(SRCDIR) && $(DOCGEN) $(DOCFLAGS) $(DOCFILE) 
	cd $(DOCDIR) && $(MAKE) >/dev/null 

clean:
	rm -r -f $(OBJECTS) $(EXECUTABLE) $(LIBRARY)
	cd  $(DOCDIR) && $(MAKE) clean
	rm -r -f $(check_PROGRAMS) $(check_OBJECTS)
