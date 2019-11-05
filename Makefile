.PHONY: mainServer
mainServer: mainServer
	./mainServer

.PHONY: dbServer
dbServer: dbServer
	./dbServer

.PHONY: calcServer
calcServer: calcServer
	./calcServer

 

all: mainServer.cpp dbServer.cpp 
	g++ -std=c++0x -o dbServer dbServer.cpp
	g++ -std=c++0x -o mainServer mainServer.cpp
	g++ -std=c++0x -o calcServer calcServer.cpp
	 
	 


clean:
	rm client sever
