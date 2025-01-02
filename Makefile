# Default target: Build the project
all:
	cmake -B build -S . -G "MinGW Makefiles"
	cmake --build build

# Run target: Build and execute the program
run: all
	./build/bin/ran-art.exe

# Clean target: Remove build artifacts
clean:
	rm -rf build

# Debug target: Build with debug symbols
debug:
	cmake -B build -S . -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
	cmake --build build

# Release target: Build with optimizations
release:
	cmake -B build -S . -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
	cmake --build build
