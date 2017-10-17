ifneq ($(windir),)
SHELL  := cmd.exe
endif
TARGET ?= LAB_B3
ARCH   ?= Cortex-M3
SCATTER = stm32f103rbt6.sct

AR      = armar.exe
AS      = armasm.exe
CC      = armcc.exe
LINK    = armlink.exe
OBJCOPY = fromelf.exe
ifeq ($(SHELL), cmd.exe)
RM      = del /q
else
RM      = rm -f
endif

scanfile=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call scanfile,$d/,$2))

asmfile := $(call scanfile,,*.S)
asmfile := $(wildcard *.S)
srcfile := $(call scanfile,,*.c)
srcfile := $(srcfile:%ucos_ii.c=)
objc := $(srcfile:%.c=%.o)
obja := $(asmfile:%.s=%.o)

AFLAGS := --cpu=$(ARCH) --apcs=interwork
CFLAGS := --cpu=$(ARCH) --apcs=interwork --multibyte_chars -O3
CFLAGS += -I. $(addprefix -I,$(filter-out ./,$(sort $(dir $(call scanfile,,*.h)))))
CFLAGS += -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DMACH_MPTEII

ifeq ($(DEBUG),1)
CFLAGS += -DDEBUG -Ddbgout"(level,...)"="(level?printf(__VA_ARGS__):0)"
else

ifeq ($(findstring -DDEBUG,$(OPT)), )
CFLAGS += -DNDEBUG -Ddbgout"(level,...)"="do{}while(0)"
else
CFLAGS += -DDEBUG -Ddbgout"(level,...)"="(level?printf(__VA_ARGS__):0)"
endif #($(findstring -DDEBUG,$(OPT)), )

endif #($(DEBUG),1)

ifeq (fb.txt,$(wildcard fb.txt))
CFLAGS += --feedback=fb.txt
endif #(fb.txt,$(wildcard fb.txt))

LFLAGS := --cpu=$(ARCH)  --scatter=$(SCATTER) --feedback=fb.txt --strict  --callgraph --list=$(TARGET).map #--map ./print/mark_Cortex-M3.lib

.PHONY : all clean distclean
all:  $(TARGET).bin $(TARGET).hex#$(TARGET).dfu

#$(TARGET).dfu : $(TARGET).bin
#	bin2dfu $< $@ 1

$(TARGET).bin : $(TARGET).axf
	$(OBJCOPY) --bin $< --output $@
#	-$(RM) firmware.bin
#	-ren $(TARGET).bin firmware.bin
#	bin2crcbin.exe HM-E200_BOOT.bin firmware.bin HM-E200_FACTEST_V1.00.00.bin $(TARGET).bin


$(TARGET).hex : $(TARGET).axf
	$(OBJCOPY) --i32 $< --output $@

$(TARGET).axf : $(obja) $(objc) $(SCATTER)
	$(LINK) $(LFLAGS) $(^:$(SCATTER)=) --output=$@
	$(OBJCOPY) -z $@
	@$(OBJCOPY) -z $(^:$(SCATTER)=)

$(objc):%.o:%.c Makefile
	$(CC) $(CFLAGS) --depend=$(@:%.o=%.d) -o $@ -c $<

$(obja):%.o:%.S Makefile
	$(AS) $(AFLAGS) --depend=$(@:%.o=%.d) -o $@ $<

clean:
	-$(RM) *.o $(subst /,\,$(strip $(call scanfile,,*.o)))
	-$(RM) *.d $(subst /,\,$(strip $(call scanfile,,*.d)))

distclean: clean
	-$(RM) $(TARGET).dfu
	-$(RM) $(TARGET).axf
	-$(RM) $(TARGET).bin
	-$(RM) $(TARGET).hex
	-$(RM) $(TARGET).map
	-$(RM) $(TARGET).htm
	-$(RM) fb.txt

-include $(call scanfile,,*.d)
