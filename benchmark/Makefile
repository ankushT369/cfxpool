CXX = g++
CXXFLAGS = -std=c++11 -O3
LDFLAGS = -lbenchmark -pthread

SRC_DIR = ..
COMMON_SRC = $(SRC_DIR)/fxpool.c $(SRC_DIR)/fxerror.c $(SRC_DIR)/fxlog.c $(SRC_DIR)/fxsys.c

# List of benchmark source files
TARGETS = benchmark_alloc benchmark_create_pool

# Build rule for each benchmark target
all: $(TARGETS)

# benchmark: benchmark.cpp $(COMMON_SRC)
#	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

benchmark_alloc: benchmark_alloc.cpp $(COMMON_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

benchmark_create_pool: benchmark_create_pool.cpp $(COMMON_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Save section: Run each benchmark and save output to a text file
save: $(TARGETS)
	./benchmark_alloc > benchmark_alloc_results.txt
	./benchmark_create_pool > benchmark_create_pool_results.txt

# Clean all binaries and result files
clean:
	rm -f $(TARGETS) benchmark_alloc_results.txt benchmark_create_pool_results.txt
