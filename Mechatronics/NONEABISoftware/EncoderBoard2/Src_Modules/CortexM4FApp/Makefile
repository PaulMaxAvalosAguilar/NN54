#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
BUILDDIR    := ../../$(BUILDDIRECTORY)/$(VERSION)
TARGETDIR   := ../../$(TARGETDIRECTORY)/$(VERSION)
LIBSDIR     := ../../$(LIBDIRECTORY)/$(VERSION)
HEADERDIR   := ../../$(HEADERSDIRECTORY)

CCSOURCES   := $(shell find $(SRCDIR) -type f -name *.$(CCEXT))
CPPSOURCES  := $(shell find $(SRCDIR) -type f -name *.$(CPPEXT))
ASMSOURCES  := $(shell find $(SRCDIR) -type f -name *.$(ASMEXT))

CCOBJECTS   := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(CCSOURCES:.$(CCEXT)=.$(OBJEXT)))
CPPOBJECTS  := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(CPPSOURCES:.$(CPPEXT)=.$(OBJEXT)))
ASMOBJECTS  := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(ASMSOURCES:.$(ASMEXT)=.$(OBJEXT)))

CCDEPENDS   := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(CCSOURCES:.$(CCEXT)=.d))
CPPDEPENDS  := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(CPPSOURCES:.$(CPPEXT)=.d))

STATLIBS    := $(wildcard $(LIBSDIR)/*.$(SLEXT))
DYNLIBS     := $(wildcard $(LIBSDIR)/*.$(DLEXT))	

#---------------------------------------------------------------------------------

include Makefile.config


#COMPILE PART
$(TARGETDIR)/$(TARGET).$(BINEXT): $(TARGETDIR)/$(TARGET).$(ELFEXT)
	rm -f $@
	$(OBJCOPY) -Obinary $(TARGETDIR)/$(TARGET).$(ELFEXT) $@

$(TARGETDIR)/$(TARGET).$(ELFEXT): $(CCOBJECTS) $(CPPOBJECTS) $(ASMOBJECTS) $(STATLIBS) $(DYNLIBS)
	@mkdir -p $(dir $@)
	$(LD) -o $@ $(CCOBJECTS) $(CPPOBJECTS) $(ASMOBJECTS) $(LDFLAGS)
	$(SIZE) $(TARGETDIR)/$(TARGET).$(ELFEXT)

-include $(CCDEPENDS)
-include $(CPPDEPENDS)

$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(CCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< $(INCDEP) -o $@ 

$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(CPPEXT)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< $(INCDEP) -o $@

$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(ASMEXT)
	@mkdir -p $(dir $@)	
	$(AS) $(ASFLAGS) -c $< $(INCDEP) -o $@

