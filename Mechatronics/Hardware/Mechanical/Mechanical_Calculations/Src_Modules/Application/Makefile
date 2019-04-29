#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
BUILDDIR    := $(TOP_DIR)/$(BUILDDIRECTORY)/$(VERSION)
TARGETDIR   := $(TOP_DIR)/$(TARGETDIRECTORY)/$(VERSION)
LIBSDIR     := $(TOP_DIR)/$(LIBDIRECTORY)/$(VERSION)
HEADERDIR   := $(TOP_DIR)/$(HEADERSDIRECTORY)

CCSOURCES   := $(shell find $(SRCDIR) -type f -name *.$(CCEXT))
CPPSOURCES  := $(shell find $(SRCDIR) -type f -name *.$(CPPEXT))

CCOBJECTS   := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(CCSOURCES:.$(CCEXT)=.$(OBJEXT)))
CPPOBJECTS  := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(CPPSOURCES:.$(CPPEXT)=.$(OBJEXT)))

CCDEPENDS   := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(CCSOURCES:.$(CCEXT)=.d))
CPPDEPENDS  := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(CPPSOURCES:.$(CPPEXT)=.d))

STATLIBS    := $(wildcard $(LIBSDIR)/*.$(SLEXT))
DYNLIBS     := $(wildcard $(LIBSDIR)/*.$(DLEXT))	

#---------------------------------------------------------------------------------

include Makefile.config

#COMPILE PART
$(TARGETDIR)/$(TARGET): $(CCOBJECTS) $(CPPOBJECTS)  $(STATLIBS) $(DYNLIBS)
	@mkdir -p $(dir $@)
	$(LD) -o $@ $(CCOBJECTS) $(CPPOBJECTS) $(LDFLAGS)
	$(SIZE) $(TARGETDIR)/$(TARGET)

-include $(CCDEPENDS)
-include $(CPPDEPENDS)

$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(CCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< $(INCDEP) -o $@ 

$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(CPPEXT)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< $(INCDEP) -o $@  
