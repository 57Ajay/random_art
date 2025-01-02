# ran-art

A Random art generator in c.
For understanding of the full implementation visit -> https://www.youtube.com/watch?v=3D_h2RE0o0E&list=PPSV


## Requirements

- CMake 3.20 or higher
- C compiler with C23 support
- MinGW-w64 (for Windows)
- nob.h (can get Tsoding dailt yt channel)
- stb_image_write.h

## Building

This project uses CMake for building. Various build options are available:

```bash
# Regular build
make

# Build and run
make run

# Debug build
make debug (Not added for now)

# Release build
make release (currently not added)

# Clean build files
make clean
```

## Project Structure

- `src/`: Source files
- `include/`: Header files
- `build/`: Build artifacts (created during build)
  - `bin/`: Compiled executable
  - `compile_commands.json`: Compilation database for tooling
