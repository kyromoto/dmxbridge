CC=g++
CFLAGS=-g `pkg-config --cflags --libs libftdi`

dmxbridger: dmxbridger.o
	$(CC) -o dmxbridger dmxbridger.cpp $(CFLAGS)