# Custom C Hash Table Implementation

A high-performance Hash Table implementation in C using **Open Addressing** with **Double Hashing** for collision resolution and **Tombstone markers** for key deletion.

## Features

- **Double Hashing**: Uses two distinct hash algorithms to minimize collisions during probing.
- **Dynamic Resizing**: Automatically handles resizing (up and down) based on low/high load factor thresholds to optimize memory and operations ($O(1)$ average time complexity).
- **Tombstone Deletion**: Safe key deletion (`&HT_DELETED_ITEM`) ensuring open addressing search chains remain unbroken.
- **Automated Build System**: Easy compilation and management via `Makefile`.

## Project Structure

```text
.
├── Makefile          # Build instructions
├── .gitignore        # Git ignore patterns (build artifacts, OS files)
└── src/
    ├── hash_table.h  # Public interface and data structures
    ├── hash_table.c  # Core hash table operations & rehash logic
    ├── prime.h       # Utility declarations for prime calculations
    ├── prime.c       # Functions to compute prime sizes for resizing
    └── main.c        # Integration and verification test suite
```
**Requirements:**
gcc compiler
make utility

Compile and execute the project using the included Makefile commands:
```bash
make
make run
```
