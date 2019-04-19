include Makefile.config

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------

export

#DEFAULT PART
all: Application 

#DIRECTORIES PART
directories: 
	@mkdir -p $(TARGETDIRECTORY)
	@mkdir -p $(BUILDDIRECTORY)
	@mkdir -p $(LIBDIRECTORY)
	@mkdir -p $(SRC_MODULES_DIR)
	@mkdir -p $(HEADERSDIRECTORY)

	@for module in $(DEPMODULES); do\
		mkdir -p $(SRC_MODULES_DIR)/$$module/$(SRCDIR);\
	done

	@mkdir -p $(SRC_MODULES_DIR)/$(MAINMODULE)/$(SRCDIR)

Application: 

	@for module in $(DEPMODULES); do\
		make -C ./$(SRC_MODULES_DIR)/$$module;\
	done

	@make -C ./$(SRC_MODULES_DIR)/$(MAINMODULE)


flash: Application
	$(AVRDUDE) -U flash:w:$(TARGETDIRECTORY)/\
$(VERSION)/$(TARGET).$(HEXEXT):i

fuse:
	$(AVRDUDE) $(FUSES)

install: flash fuse


#Clean PART
clean: 
	@$(RM) -rf $(BUILDDIRECTORY)/$(VERSION)/
	@$(RM) -rf $(TARGETDIRECTORY)/$(VERSION)/
