ALL:
	mkdir -p bin
	$(CXX) -o bin/test test.cpp Buffer.cpp Bank.cpp Set.cpp
