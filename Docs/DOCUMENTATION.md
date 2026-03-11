# Fast and Efficient Fixed-Size Memory Pool

## Overview

This documentation explains a simple and fast memory pool allocator designed for **fixed-size blocks**, ideal for **real-time systems** like games or embedded devices. The key features are:

- **No loops**
- **No overhead**
- **O(1) allocation and deallocation time**
- **No fragmentation**

## Why Use a Memory Pool?

Default system allocators (like `malloc`) are slow and cause fragmentation over time. This pool allocator solves that by preallocating memory and reusing fixed-size blocks.

### Benefits:
- Much faster than `malloc`
- Low memory overhead
- Predictable performance
- Works well with frequently allocated small objects (e.g., particles, packets)

---

## How It Works

1. **Preallocate memory:** A large block is divided into fixed-sized chunks.
2. **Free blocks tracking:** Instead of using extra memory, unused blocks store a pointer (actually an index) to the next unused block.
3. **On Allocation:**
   - The block at the head of the free list is returned.
   - The head moves to the next block.
4. **On Deallocation:**
   - The block is inserted back at the head of the free list.

> 📌 The allocator does not loop during initialization. Blocks are only initialized as needed.

---

## Step-by-Step Example

For a pool with 4 blocks:
1. Create the pool.
2. Allocate 3 blocks — each time one is removed from the free list.
3. Deallocate a block — it gets added back to the free list.
4. Allocate again — the just-freed block is reused.

This mechanism is shown visually in the paper with diagrams (Fig. 1 & 2).
