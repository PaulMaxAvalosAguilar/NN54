include Makefile.config

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------

export

#DEFAULT PART
all: Application 

#DIRECTORIES PART
directories: 
	@mkdir -p $(TARGETDIRECTORY)/$(VERSION)
	@mkdir -p $(BUILDDIRECTORY)/$(VERSION)
	@mkdir -p $(LIBDIRECTORY)/$(VERSION)
	@mkdir -p $(SRC_MODULES_DIR)
	@mkdir -p $(HEADERSDIRECTORY)

	@for module in $(DEPMODULES); do\
		mkdir -p $(SRC_MODULES_DIR)/$$module/$(SRCDIR);\
	done

	@mkdir -p $(SRC_MODULES_DIR)/$(MAINMODULE)/$(SRCDIR)

size:
	$(SIZE) $(TARGETDIRECTORY)/$(VERSION)/$(TARGET).$(ELFEXT)

Application: 

	@for module in $(DEPMODULES); do\
		make -C ./$(SRC_MODULES_DIR)/$$module;\
	done

	@make -C ./$(SRC_MODULES_DIR)/$(MAINMODULE)


flash: Application
	$(STFLASH) write $(TARGETDIRECTORY)/$(VERSION)/$(TARGET).bin 0x8000000

FILECOMAND := 'file ./$(TARGETDIRECTORY)/$(VERSION)/$(TARGET).elf'

debug:
	openocd -f OpenOCD/stlink-v2.cfg -c "transport select hla_swd" -f OpenOCD/stm32f1x.cfg \
	-c "init" &
	arm-none-eabi-gdb -ex 'target remote localhost:3333' \
	-ex $(FILECOMAND) -ex 'monitor arm semihosting enable' -ex 'monitor reset halt' 




#Clean PART
clean: 
	@$(RM) -rf $(BUILDDIRECTORY)/$(VERSION)/
	@$(RM) -rf $(TARGETDIRECTORY)/$(VERSION)/
