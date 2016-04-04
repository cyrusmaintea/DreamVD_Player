BUILD = bin
SRC = src

INCLUDES = -Iinclude
LIBS = -lkosext2fs -ljpeg -lz -lkmg -lm

TARGET = $(BUILD)/dreamvd.elf
BINTARGET = $(BUILD)/1ST_READ.BIN

OBJS = $(BUILD)/dreamvd.o romdisk.o

KOS_ROMDISK_DIR = romdisk

include $(KOS_BASE)/Makefile.rules

all: dreamvd.o $(TARGET)

dbg:
	$(KOS_LOADER) -b 1500000 -t COM2 -x $(TARGET)

run:
	$(KOS_LOADER) -b 1500000 -t COM2 -x $(BINTARGET)

clean:
	-rm -f $(TARGET) $(BINTARGET) $(OBJS) romdisk.* $(BUILD)/*.o

dreamvd.o: $(SRC)/dreamvd.c
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) $(INCLUDES) -c $(SRC)/dreamvd.c -o $(BUILD)/dreamvd.o

genromfs:
	genromfs.exe -f romdisk.img -d romdisk -v -x .svn

$(TARGET): $(OBJS)
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) -o $@ $(KOS_START) $(OBJS) $(DATAOBJS) $(OBJEXTRA) $(LIBS) $(KOS_LIBS)
	$(KOS_STRIP)	$@
	$(KOS_OBJCOPY) -O binary -R .stack $@ bin/1ST_READ.BIN
