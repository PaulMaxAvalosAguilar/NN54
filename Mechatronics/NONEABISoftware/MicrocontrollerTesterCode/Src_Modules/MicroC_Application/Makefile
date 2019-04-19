include Makefile.config

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
BUILDDIR    := ../../$(BUILDDIRECTORY)/$(VERSION)
TARGETDIR   := ../../$(TARGETDIRECTORY)/$(VERSION)

SOURCES     := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS     := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))


#---------------------------------------------------------------------------------

#COMPILE PART
$(TARGETDIR)/$(TARGET).$(HEXEXT): $(TARGETDIR)/$(TARGET).$(ELFEXT)
	rm -f $@
	$(OBJCOPY) -j .text -j .data -O ihex $(TARGETDIR)/$(TARGET).$(ELFEXT) $@

$(TARGETDIR)/$(TARGET).$(ELFEXT):  $(OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS)  $(MACROS) -c $< $(INCDEP) -o $@  

