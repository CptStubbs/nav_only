# Front End Source */
ifeq ($(FRONTENDTYPE), FILE)
	CFLAGS += -D USEFILE 
	ifeq ($(USEL2C), TRUE)
		CFLAGS += -D USEL2C
	endif 
endif

ifeq ($(FRONTENDTYPE), USB)
	CFLAGS += -D USEUSB
endif 

# File type
ifeq ($(FILETYPE), 2) 
	CFLAGS += -D USECOMP
endif 

ifeq ($(FILETYPE), 1) 
	CFLAGS += -D USEREAL
endif 

####Header 
#Git info (Thank you staffan) 
GIT_VERSION :=$(shell git describe --abbrev=4 --dirty --always --tags)
GIT_BRANCH :=$(shell git symbolic-ref --short HEAD)
#Hardware info 
HARDWARE_BUILD :=3
BASEBAND_VERSION :=.14

CFLAGS+= -DGITVERSION=\"$(GIT_VERSION)\"
CFLAGS+= -DGITBRANCH=\"$(GIT_BRANCH)\"
CFLAGS+= -DHARDWAREBUILD=\"$(HARDWARE_BUILD)\"
CFLAGS+= -DBASEBANDVERSION=\"$(BASEBAND_VERSION)\"
