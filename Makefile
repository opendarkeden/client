# OpenDarkEden Client - Makefile
# Wrapper around CMake build system

.PHONY: all debug release test clean fmt fmt-check help
.PHONY: check-resources extract-resources clean-resources
.PHONY: sprite-viewer creature-viewer item-viewer map-viewer zone-parser
.PHONY: debug-asan debug-tsan run-asan run-tsan

# Default target
all: debug

# Build directories
BUILD_DIR_DEBUG = build/debug
BUILD_DIR_RELEASE = build/release
BUILD_DIR_DEBUG_ASAN = build/debug-asan
BUILD_DIR_DEBUG_TSAN = build/debug-tsan

# DarkEden data directory (can be overridden)
DARKEDEN_DIR ?= DarkEden

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

# ============================================================
# Sanitizer Builds
# ============================================================

# Debug build with AddressSanitizer
debug-asan:
	@echo "Building Debug with AddressSanitizer..."
	cmake -S . -B $(BUILD_DIR_DEBUG_ASAN) -DCMAKE_BUILD_TYPE=Debug -DUSE_ASAN=ON
	cmake --build $(BUILD_DIR_DEBUG_ASAN) -j$(NPROCS)

# Debug build with ThreadSanitizer
debug-tsan:
	@echo "Building Debug with ThreadSanitizer..."
	cmake -S . -B $(BUILD_DIR_DEBUG_TSAN) -DCMAKE_BUILD_TYPE=Debug -DUSE_TSAN=ON
	cmake --build $(BUILD_DIR_DEBUG_TSAN) -j$(NPROCS)

# Run with AddressSanitizer
run-asan: debug-asan
	@echo "Running with AddressSanitizer..."
	@echo "Usage: make run-asan MODE=0000000001"

# Run with ThreadSanitizer
run-tsan: debug-tsan
	@echo "Running with ThreadSanitizer..."
	@echo "Usage: make run-tsan MODE=0000000001"

# ============================================================
# Testing and Validation
# ============================================================

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
	rm -rf build/debug-asan
	rm -rf build/debug-tsan
	@echo "Clean complete"

# ============================================================
# Resource Management Tools
# ============================================================

# Generated .inf file path
INF_FILE = $(DARKEDEN_DIR)/Data/Info/VS_UI_filepath.inf

# From .h generate .inf file
extract-resources:
	@echo "Extracting resource macros from VS_UI_filepath.h..."
	@mkdir -p $(DARKEDEN_DIR)/Data/Info
	@python3 tools/resource_management/extract_macros.py \
		VS_UI/src/header/VS_UI_filepath.h \
		$(INF_FILE) --platform=unix

# Run resource validator
check-resources: extract-resources
	@echo "Validating resource files..."
	@if [ ! -f build/debug/bin/resource_validator ]; then \
		echo "Resource validator not built, building..."; \
		cmake --build build/debug --target resource_validator -j$(NPROCS); \
	fi
	@build/debug/bin/resource_validator $(INF_FILE)

# Clean generated resource files
clean-resources:
	@echo "Cleaning generated resource files..."
	@rm -f $(INF_FILE)
	@echo "Clean complete"

# ============================================================
# Resource Viewing Tools
# ============================================================

# Sprite Viewer
sprite-viewer: build/debug/bin/sprite_viewer
	@echo "Sprite viewer built: build/debug/bin/sprite_viewer"

# Creature Viewer
creature-viewer: build/debug/bin/creature_viewer
	@echo "Creature viewer built: build/debug/bin/creature_viewer"

# Map Viewer
map-viewer: build/debug/bin/map_viewer
	@echo "Map viewer built: build/debug/bin/map_viewer"

# Item Viewer
item-viewer: build/debug/bin/item_viewer
	@echo "Item viewer built: build/debug/bin/item_viewer"

# Zone Parser
zone-parser: build/debug/bin/zone_parser
	@echo "Zone parser built: build/debug/bin/zone_parser"

# Show help
help:
	@echo "OpenDarkEden Client - Available targets:"
	@echo ""
	@echo "  make               - Build debug version (default)"
	@echo "  make debug         - Build debug version"
	@echo "  make release       - Build release version (optimized)"
	@echo "  make debug-asan     - Build with AddressSanitizer (memory errors)"
	@echo "  make debug-tsan     - Build with ThreadSanitizer (race conditions)"
	@echo "  make test          - Run tests (TODO)"
	@echo "  make fmt           - Format code (TODO)"
	@echo "  make fmt-check     - Check code formatting (TODO)"
	@echo "  make clean         - Clean build directories"
	@echo "  make help          - Show this help message"
	@echo ""
	@echo "Resource Management:"
	@echo "  make check-resources        - Validate all resource files"
	@echo "  make extract-resources      - Extract .inf from .h file"
	@echo "  make clean-resources        - Clean generated .inf file"
	@echo ""
	@echo "Resource Viewing Tools:"
	@echo "  make sprite-viewer         - Build sprite viewer"
	@echo "  make creature-viewer       - Build creature viewer"
	@echo "  make item-viewer           - Build item viewer"
	@echo "  make map-viewer            - Build map viewer"
	@echo "  make zone-parser           - Build zone parser"
	@echo ""
	@echo "Resource Management Options:"
	@echo "  DARKEDEN_DIR=<path>  - Specify DarkEden data directory (default: DarkEden)"
	@echo ""
	@echo "Build directories:"
	@echo "  Debug:   $(BUILD_DIR_DEBUG)"
	@echo "  Release: $(BUILD_DIR_RELEASE)"
	@echo "  ASAN:    $(BUILD_DIR_DEBUG_ASAN)"
	@echo "  TSAN:    $(BUILD_DIR_DEBUG_TSAN)"
	@echo ""
	@echo "Examples:"
	@echo "  make                      # Build debug"
	@echo "  make release              # Build release"
	@echo "  make debug-asan           # Build with AddressSanitizer"
	@echo "  make clean                # Clean all builds"
	@echo "  make NPROCS=8             # Build with 8 parallel jobs"
	@echo "  make check-resources      # Validate resources in DarkEden/"
	@echo "  make sprite-viewer        # Build and use sprite viewer"
	@echo "  make check-resources DARKEDEN_DIR=/path/to/game  # Custom data dir"
	@echo ""
	@echo "Running with sanitizers:"
	@echo "  build/debug-asan/bin/DarkEden 0000000001    # Run ASAN build"
	@echo "  build/debug-tsan/bin/DarkEden 0000000001    # Run TSAN build"

