.PHONY: all clean

#Jim Wen
#Changed to be able to build(ARM) and run the library on BlackBerry
CC=qcc -Vgcc_ntoarmv7le -fPIC

CPPFLAGS = -I../include -I../include/id3v2lib

OBJS = frame.o \
       header.o \
       id3v2lib.o \
       types.o \
       utils.o

LIBID3V2=libid3v2.a

all .DEFAULT: $(LIBID3V2)

frame.o:

$(LIBID3V2): $(OBJS)
	ar -rcs $(LIBID3V2) $(OBJS)

clean:
	rm -rf $(LIBID3V2) $(OBJS) *~
