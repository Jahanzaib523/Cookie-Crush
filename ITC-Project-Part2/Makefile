CXXFLAGS =	-g3 -Wall -fmessage-length=0

OBJS =		ascii-art.o

LIBS = -L/usr/X11R6/lib -lm -lpthread -lX11

TARGET =	ascii-art

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
