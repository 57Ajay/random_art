# ran-art

A modern C project created with project-generator.

## Requirements

- CMake 3.20 or higher
- C compiler with C23 support
- MinGW-w64 (for Windows)

## Building

This project uses CMake for building. Various build options are available:

```bash
# Regular build
make

# Build and run
make run

# Debug build
make debug

# Release build
make release

# Clean build files
make clean
```

## Project Structure

- `src/`: Source files
- `include/`: Header files
- `build/`: Build artifacts (created during build)
  - `bin/`: Compiled executable
  - `compile_commands.json`: Compilation database for tooling
