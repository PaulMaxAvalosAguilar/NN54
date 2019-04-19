include Makefile.config

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
BUILDDIR    := ../../$(BUILDDIRECTORY)/$(VERSION)
TARGETDIR   := ../../$(TARGETDIRECTORY)/$(VERSION)
LIBSDIR     := ../../$(LIBDIRECTORY)/$(VERSION)

SOURCES     := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS     := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))
DEPENDS     := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.d))
STATLIBS    := $(wildcard $(LIBSDIR)/*.$(SLEXT))
DYNLIBS     := $(wildcard $(LIBSDIR)/*.$(DLEXT))	

#---------------------------------------------------------------------------------

#COMPILE PART
$(TARGETDIR)/$(TARGET).$(HEXEXT): $(TARGETDIR)/$(TARGET).$(ELFEXT)
	rm -f $@
	$(OBJCOPY) -j .text -j .data -O ihex $(TARGETDIR)/$(TARGET).$(ELFEXT) $@

$(TARGETDIR)/$(TARGET).$(ELFEXT):  $(OBJECTS) $(STATLIBS) $(DYNLIBS)
	@mkdir -p $(dir $@)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

-include $(DEPENDS)

$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS)  $(MACROS) -MMD -MP -c $< $(INCDEP) -o $@  

