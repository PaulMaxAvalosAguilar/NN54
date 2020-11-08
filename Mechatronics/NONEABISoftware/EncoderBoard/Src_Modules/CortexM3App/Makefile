#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
BUILDDIR    := ../../$(BUILDDIRECTORY)/$(VERSION)
TARGETDIR   := ../../$(TARGETDIRECTORY)/$(VERSION)
LIBSDIR     := ../../$(LIBDIRECTORY)/$(VERSION)
HEADERDIR   := ../../$(HEADERSDIRECTORY)

#CCSOURCES   := $(shell find $(SRCDIR) -type f -name *.$(CCEXT))
#CPPSOURCES  := $(shell find $(SRCDIR) -type f -name *.$(CPPEXT))
rwildcard    = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

CCSOURCES    := $(call rwildcard,$(SRCDIR)/,*.$(CCEXT))
CPPSOURCES   := $(call rwildcard,$(SRCDIR),*.(CCPEXT))

CCOBJECTS   := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(CCSOURCES:.$(CCEXT)=.$(OBJEXT)))
CPPOBJECTS  := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(CPPSOURCES:.$(CPPEXT)=.$(OBJEXT)))

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

$(TARGETDIR)/$(TARGET).$(ELFEXT): $(CCOBJECTS) $(CPPOBJECTS)  $(STATLIBS) $(DYNLIBS)
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
