#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
LIBDIR	    := ../../$(LIBDIRECTORY)/$(VERSION)
BUILDDIR    := ../../$(BUILDDIRECTORY)/$(VERSION)
include Makefile.config
HEADDIR	    := ../../$(HEADERSDIRECTORY)/$(LIBNAME)

SOURCES     := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS     := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))
HEADERS	    := $(shell find $(SRCDIR) -type f -name *.$(HEADEXT))
COPYHEAD    := $(patsubst $(SRCDIR)/%,$(HEADDIR)/%,$(HEADERS))
#---------------------------------------------------------------------------------

all: $(COPYHEAD) $(PACKAGINGNAME)

$(COPYHEAD):
	@mkdir -p $(HEADDIR)
	rsync -r --delete $(HEADERS) $(HEADDIR)/

#COMPILE PART
$(PACKAGINGNAME):  $(OBJECTS) 
	@mkdir -p $(dir $@)
	$(PACKAGINGINSTRUCTIONS)  $@ $?

$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@) 
	$(COMPILATIONINSTURCTIONS) $< -o $@  

