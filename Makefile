# OpenDarkEden Client - Makefile
# Wrapper around CMake build system

.PHONY: all debug release test clean fmt fmt-check help

# Default target
all: debug

# Build directories
BUILD_DIR_DEBUG = build/debug
BUILD_DIR_RELEASE = build/release

# Number of parallel jobs
NPROCS ?= 1
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	NPROCS := $(shell nproc)
endif
ifeq ($(UNAME_S),Darwin)
	NPROCS := $(shell sysctl -n hw.ncpu)
endif

# Debug build
debug:
	@echo "Building Debug configuration..."
	cmake -S . -B $(BUILD_DIR_DEBUG) -DCMAKE_BUILD_TYPE=Debug
	cmake --build $(BUILD_DIR_DEBUG) -j$(NPROCS)

# Release build
release:
	@echo "Building Release configuration..."
	cmake -S . -B $(BUILD_DIR_RELEASE) -DCMAKE_BUILD_TYPE=Release
	cmake --build $(BUILD_DIR_RELEASE) -j$(NPROCS)

# Run tests
test:
	@echo "TODO: Run tests"
	@echo "cd $(BUILD_DIR_DEBUG) && ctest --output-on-failure"

# Code formatting
fmt:
	@echo "TODO: Format code"
	@echo "This will format all C++ code using clang-format"

# Check formatting
fmt-check:
	@echo "TODO: Check code formatting"
	@echo "This will check if code is properly formatted"

# Clean build directories
clean:
	@echo "Cleaning build directories..."
	rm -rf build/debug
	rm -rf build/release
	@echo "Clean complete"

# Show help
help:
	@echo "OpenDarkEden Client - Available targets:"
	@echo ""
	@echo "  make          - Build debug version (default)"
	@echo "  make debug    - Build debug version"
	@echo "  make release  - Build release version (optimized)"
	@echo "  make test     - Run tests (TODO)"
	@echo "  make fmt      - Format code (TODO)"
	@echo "  make fmt-check- Check code formatting (TODO)"
	@echo "  make clean    - Clean build directories"
	@echo "  make help     - Show this help message"
	@echo ""
	@echo "Build directories:"
	@echo "  Debug:   $(BUILD_DIR_DEBUG)"
	@echo "  Release: $(BUILD_DIR_RELEASE)"
	@echo ""
	@echo "Examples:"
	@echo "  make              # Build debug"
	@echo "  make release      # Build release"
	@echo "  make clean        # Clean all builds"
	@echo "  make NPROCS=8     # Build with 8 parallel jobs"
