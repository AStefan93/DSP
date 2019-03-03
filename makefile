CC = g++
CFLAGS = -std=gnu++11 -I$(IDIR) -lpthread `pkg-config --libs --cflags opencv`
EXECUTABLE = DSP
ODIR = build
SDIR = src
IDIR = include
DEPS = $(IDIR)/*.h
_OBJ = dsp.o LaneDetection.o perspective.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
$(EXECUTABLE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
.PHONY:	clean
clean :
	rm -f $(ODIR)/*.o $(EXECUTABLE)
