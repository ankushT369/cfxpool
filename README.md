# cfxpool
 
 cfxpool is a **fast and lightweight fixed-size memory pool allocator** written in C, ideal for high-performance and real-time applications such as:
 
 - Games and game engines
 - Graphical asset systems
 - Particle systems
 - Network packet processing
 - Embedded systems
 
 ## Features
 
 - O(1) allocation and deallocation
 - Zero fragmentation
 - No initialization loops
 - Minimal memory overhead
 - Easy to integrate into existing codebases
 
 ## Documentation
 
 Detailed technical explanation and internal design:  
 [Documentation](./Docs/DOCUMENTATION.md)
 
 Original paper reference:  
 [Fast Efficient Fixed-Size Memory Pool (PDF)](https://github.com/ankushT369/FxPool/blob/main/Docs/fast_efficient_fixed-size_memory_pool.pdf)
 
 ## Benchmarking
 
 To benchmark the performance of the memory pool, you can use the provided benchmark code. The benchmark measures the performance of the memory pool's allocation and deallocation in comparison to other techniques.
 
 ### Image
 Below is a graphical representation of the benchmarking results:
 ![Benchmark Results](https://github.com/ankushT369/cfxpool/blob/main/Docs/benchmark.png)
 
 ### How to Run the Benchmark 
 
 1. **Build the Benchmark:**
 
    First, you need to compile the benchmark code. You can do this with the following commands:
    
    *all build setups are currently desgined for linux later it will be for every platform*
 
    ```bash
    make benchmark

