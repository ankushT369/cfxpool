CXXFLAGS += -std=c++11
LDFLAGS += -lbenchmark -pthread

# The benchmark target
benchmark: benchmark.cpp ../fxpool.c ../fxerror.c ../fxlog.c ../fxsys.c
	g++ $(CXXFLAGS) -O3 -o benchmark benchmark.cpp ../fxpool.c ../fxerror.c ../fxlog.c ../fxsys.c $(LDFLAGS)

# Clean build files
clean:
	rm -f benchmark

