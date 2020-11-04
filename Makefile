CC=gcc
CFLAGS= -g -c
INCLUDES=-I ./include/
LIBS=-ll -ly -lstdc++ -lz3

pres: ./obj/rpcalc.o ./obj/lex.yy.o ./obj/printmodel.o ./obj/readKBDF.o \
	  ./obj/drivemenu.o ./obj/readUtility.o ./obj/writepres.o \
	  ./obj/searchUtility.o ./obj/readFile.o ./obj/findpath.o ./obj/visualize.o \
	  ./obj/pathequivalence.o ./obj/main.o
	$(CC) -g $(INCLUDES) -o pres ./obj/*.o $(LIBS) 

./obj/rpcalc.o: ./src/rpcalc.cxx ./include/types.h ./include/rpcalc.hxx
	$(CC) $(CFLAGS) $(INCLUDES) ./src/rpcalc.cxx -o ./obj/rpcalc.o

./src/rpcalc.cxx: ./src/rpcalc.yxx ./include/types.h ./include/Utility.h
	bison -dtvy -o ./src/rpcalc.cxx ./src/rpcalc.yxx
	mv ./src/rpcalc.hxx ./include/rpcalc.hxx

./obj/lex.yy.o: ./src/lex.yy.cxx ./include/types.h ./include/rpcalc.hxx
	$(CC) $(CFLAGS) $(INCLUDES) ./src/lex.yy.cxx -o ./obj/lex.yy.o

./src/lex.yy.cxx: ./include/types.h ./include/rpcalc.hxx
	flex -o ./src/lex.yy.cxx ./src/rpcalc.l

./obj/printmodel.o: ./src/printmodel.cxx ./include/Utility.h
	$(CC) $(CFLAGS) $(INCLUDES) ./src/printmodel.cxx -o ./obj/printmodel.o

./obj/readKBDF.o: ./src/readKBDF.cxx ./include/Utility.h
	$(CC) $(CFLAGS) $(INCLUDES) ./src/readKBDF.cxx -o ./obj/readKBDF.o

./obj/drivemenu.o: ./src/drivemenu.cxx ./include/Utility.h
	$(CC) $(CFLAGS) $(INCLUDES) ./src/drivemenu.cxx -o ./obj/drivemenu.o 

./obj/readUtility.o: ./src/readUtility.cxx ./include/Utility.h
	$(CC) $(CFLAGS) $(INCLUDES) ./src/readUtility.cxx -o ./obj/readUtility.o

./obj/writepres.o: ./src/writepres.cxx ./include/Utility.h
	$(CC) $(CFLAGS) $(INCLUDES) ./src/writepres.cxx -o ./obj/writepres.o

./obj/searchUtility.o: ./src/searchUtility.cxx ./include/Utility.h
	$(CC) $(CFLAGS) $(INCLUDES) ./src/searchUtility.cxx -o ./obj/searchUtility.o 

./obj/readFile.o: ./src/readFile.cxx ./include/Utility.h
	$(CC) $(CFLAGS) $(INCLUDES) ./src/readFile.cxx -o ./obj/readFile.o 

./obj/visualize.o: ./src/visualize.cxx ./include/Utility.h
	$(CC) $(CFLAGS) $(INCLUDES) ./src/visualize.cxx -o ./obj/visualize.o

./obj/findpath.o: ./src/findpath.cxx ./include/Utility.h ./include/findpath.h
	$(CC) $(CFLAGS) $(INCLUDES) ./src/findpath.cxx -o ./obj/findpath.o 

./obj/pathequivalence.o: ./src/pathequivalence.cxx ./include/Utility.h 
	$(CC) $(CFLAGS) $(INCLUDES) ./src/pathequivalence.cxx -o ./obj/pathequivalence.o 

./obj/main.o: ./src/main.cxx ./include/Utility.h
	$(CC) $(CFLAGS) $(INCLUDES) ./src/main.cxx -o ./obj/main.o

clean:
	rm -rf ./obj/*.o
	rm -rf ./pres	
	rm -rf ./include/rpcalc.hxx      
	rm -rf ./src/rpcalc.output
	rm -rf ./src/lex.yy.cxx
	rm -rf ./src/rpcalc.cxx
	rm -rf ./examples/fsmd/*.org
	clear

