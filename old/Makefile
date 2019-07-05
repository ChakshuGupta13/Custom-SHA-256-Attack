CXXFLAGS = -std=c++11 -O3
all: sha
sha: main.o carry_graph.o diff_chars.o difference_propagation_tables.o SHA-256.o
	g++ $(CXXFLAGS) main.o carry_graph.o diff_chars.o difference_propagation_tables.o SHA-256.o -o SHA256
main.o: main.cpp
	g++ $(CXXFLAGS) -c main.cpp
carry_graph.o: carry_graph.cpp carry_graph.h
	g++ $(CXXFLAGS) -c carry_graph.cpp
diff_chars.o: diff_chars.cpp diff_chars.h
	g++ $(CXXFLAGS) -c diff_chars.cpp
difference_propagation_tables.o: difference_propagation_tables.cpp difference_propagation_tables.h
	g++ $(CXXFLAGS) -c difference_propagation_tables.cpp
SHA-256.o: SHA-256.cpp SHA-256.h GLOBAL_CONSTANTS.h
	g++ $(CXXFLAGS) -c SHA-256.cpp
clean:
	rm *.o SHA256