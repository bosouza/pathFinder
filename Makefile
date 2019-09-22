.PHONY: testWorldGraph
testWorldGraph: testWorldGraph.exe
	testWorldGraph.exe

testWorldGraph.exe: Test/TestWorldGraph/main.cpp Src/worldGraph.cpp
	g++ Test/TestWorldGraph/main.cpp Src/worldGraph.cpp -I./Inc -std=c++11 -o testWorldGraph.exe