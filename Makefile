OBJ = constructor.o create.o createBuffer.o createFile.o filesystem.o format.o getStatus.o handleInput.o listDirectoryContents.o readFile.o readSector.o split.o updateStatus.o writeSector.o createDir.o changeDir.o printWorkingDir.o deleteFile.o undeleteFile.o info.o tree.o findFreeSectors.o

DEP = filesystem.h
CC  = g++
CFLAGS = -std=c++11

all: $(OBJ) $(DEP)
	$(CC) $(CFLAGS) -o filesystem $(OBJ)

%.o: %.cpp $(DEP)
	g++ $(CFLAGS) -g -c -o $@ $<

clean:
	@rm -f *.o
	@rm -f *.gch
	@rm -f *.bin
	@rm filesystem