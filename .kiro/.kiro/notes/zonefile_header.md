# ZoneFileHeader notes & map_loader design

This document captures my extraction/analysis notes for the original client's zone (.map) file format (ZoneFileHeader / MZone / MSector) and a concrete design for a small `map_loader` module that implements precise sector-block reading for the MapViewer MVP.

The goal:
- Replace the current heuristic sector detection in `tools/map_viewer/main.c` with a tidy `map_loader` that uses the zone header to find the sector table and exposes a small, testable API to fetch sector grids (sub-rectangles).
- Keep implementation small and local to tools while we validate behaviour; later we can promote to `src/graphics/tilemap`.

Status
- Using `adam_c.map` as the first test case (you confirmed).
- Known facts from original client scanning:
  - Each sector entry (record) in many zone files is 4 bytes:
    - `uint16_t spriteID` (2 bytes, little-endian)
    - `uint8_t property`  (1 byte)
    - `uint8_t light`     (1 byte)
  - Sector block order is typically row-major (y-major outer loop then x), but the original code must be consulted (we'll follow MZone/MSector logic). If row/col ordering differs the loader will expose it explicitly.
  - Byte ordering: little-endian (Intel x86 original client).
  - Zone header contains fields that point to tile/sector table offsets (names seen in original client source: `fpTile`, `m_fpTile`, `fpSector`, etc). Important: offsets may be absolute file offsets or relative offsets — `map_loader` will detect this and handle both (see heuristics below).
  - Many maps include a short header with ASCII tag "MAP" or other identifying strings — useful as a fallback but not authoritative.

Mapping of responsibilities
- `map_loader` responsibilities:
  - Parse the zone header (fixed fields) from the beginning of the `.map` file.
  - Extract width/height (tile grid dimensions) and the sector table file offset.
  - Expose an API to read a rectangular subset of sectors (row-major) as an array of `Sector` structs or plain sprite IDs.
  - Validate offsets and detect whether the recorded offset is absolute or relative (by checking plausibility during read).
  - If header-provided offset is invalid, fall back to heuristic detection (existing heuristic logic) but report diagnostics.
  - Provide debug facilities: dump first N sectors, return raw offsets used, and error codes for precise diagnostics.

Core data structures (conceptual)
- Sector record:
  - C view:
    - `typedef struct { uint16_t sprite_id; uint8_t property; uint8_t light; } MSectorRecord;`

Design API (header sketch)
- The main API is intentionally small and C-friendly. Example header (see code block below):

```client/resurrected/tools/map_viewer/map_loader.h#L1-200
/*
 * map_loader.h -- small loader for reading sector blocks from a .map (zone) file
 *
 * Exposes:
 * - map_loader_open: parse header and return MapHandle
 * - map_loader_read_region: read a rectangle of sectors into caller buffer
 * - map_loader_dump_info: print header/offset info for debugging
 * - map_loader_close: close file and free resources
 *
 * Notes:
 * - All offsets & sizes are interpreted in little-endian.
 * - The loader validates header-provided offsets; if invalid it sets 'offset_mode' to
 *   MAP_LOADER_OFFSET_FALLBACK and keeps going.
 */
#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include <stdint.h>

typedef enum {
    MAP_LOADER_OK = 0,
    MAP_LOADER_ERR_IO = -1,
    MAP_LOADER_ERR_FORMAT = -2,
    MAP_LOADER_ERR_OOB = -3,
    MAP_LOADER_ERR_NOMEM = -4,
} MapLoaderError;

typedef enum {
    MAP_LOADER_OFFSET_UNDEFINED = 0,
    MAP_LOADER_OFFSET_HEADER_ABSOLUTE,
    MAP_LOADER_OFFSET_HEADER_RELATIVE,
    MAP_LOADER_OFFSET_DETECTED_HEURISTIC,
    MAP_LOADER_OFFSET_FALLBACK /* default: 0 or header failure */
} MapLoaderOffsetMode;

typedef struct {
    uint16_t sprite_id; /* 0..65535, 0xFFFF usually means empty */
    uint8_t property;
    uint8_t light;
} MSectorRecord;

typedef struct MapHandle MapHandle;

/* Open and parse header. `map_path` must be a readable file path.
 * On success returns MAP_LOADER_OK and sets *out to a non-NULL handle.
 */
MapLoaderError map_loader_open(const char* map_path, MapHandle** out);

/* Read a rectangular region of sectors.
 * Coordinates are sector coordinates (x: 0..width-1, y: 0..height-1).
 * out_buf must point to an array of MSectorRecord of size (w*h).
 * Returns MAP_LOADER_OK on success.
 */
MapLoaderError map_loader_read_region(MapHandle* h,
                                     uint32_t x, uint32_t y,
                                     uint32_t w, uint32_t hgt,
                                     MSectorRecord* out_buf);

/* Convenience: read entire grid into allocated buffer. Caller frees buffer. */
MapLoaderError map_loader_read_all(MapHandle* h, MSectorRecord** out_buf /* alloced */);

/* Dump header/offset/diagnostic info to stderr (for debugging) */
void map_loader_dump_info(MapHandle* h);

/* Close and free handle */
void map_loader_close(MapHandle* h);

#endif /* MAP_LOADER_H */
```

Implementation notes / pseudocode
- High level `map_loader_open`:
  1. open file read-only binary.
  2. read a conservative prefix (e.g. first 1KB) to parse header fields.
  3. locate the header fields: width/height (tile count), sector table offset pointer(s).
     - Many zone headers include: width (uint16), height (uint16), and file pointers for various tables.
     - If pointer present, check if pointer value + expected table size <= file_size => treat as absolute.
     - Otherwise, check if pointer + header_start <= file_size => treat as relative.
  4. Seek to the candidate offset and perform a lightweight validation: read the first N sector records and verify sprite_id values are plausible (e.g. not all 0xFFFF, not absurd, or within provided sprite pack index range if pack information provided).
  5. If header offset validation fails, call heuristic detector (like current tool) to find plausible offset; set `offset_mode` accordingly.
  6. Fill-in MapHandle fields: file descriptor/FILE*, width, height, sector_offset, offset_mode, file_size, header_dump.

- `map_loader_read_region`:
  - Compute `row_major_index = y * width + x`.
  - Compute `byte_offset = sector_offset + (row_major_index * SECTOR_RECORD_SIZE)`.
  - Seek and read sequentially `w * hgt` records into out_buf.
  - Validate read size and return proper error on failure.

- Validation rules:
  - If `sprite_id == 0xFFFF` treat as empty.
  - If sprite_id > SOME_REASONABLE_LIMIT and pack info not available, still allow but flag in diagnostics.
  - If `byte_offset + requested_bytes > file_size` return `MAP_LOADER_ERR_OOB`.

Example pseudocode (implementation sketch)
- Implementation notes are provided as a code block below for quick copy/paste into a `.c` file:

```client/resurrected/tools/map_viewer/map_loader.c#L1-240
/* Pseudocode sketch (not full production code) */

/* open file, read header bytes, attempt to parse width/height and sector pointer */
MapLoaderError map_loader_open(const char* map_path, MapHandle** out) {
    FILE* f = fopen(map_path, "rb");
    if (!f) return MAP_LOADER_ERR_IO;
    long file_size = file_length(f);

    /* read first 1024 bytes to parse header heuristically */
    uint8_t header_buf[1024] = {0};
    size_t n = fread(header_buf, 1, sizeof(header_buf), f);
    /* parse header_buf for known fields (width/height offsets based on original client) */
    /* Example: suppose width at offset A, height at offset A+2, tile pointer at offset B */
    uint16_t width = read_u16_le_from(header_buf + A);
    uint16_t height = read_u16_le_from(header_buf + A+2);
    uint32_t candidate_ptr = read_u32_le_from(header_buf + B);

    /* verify candidate_ptr plausibility (absolute vs relative) */
    if (candidate_ptr + (width*height*4) <= file_size) {
        sector_offset = candidate_ptr; offset_mode = MAP_LOADER_OFFSET_HEADER_ABSOLUTE;
    } else if ((header_start_offset + candidate_ptr) + (width*height*4) <= file_size) {
        sector_offset = header_start_offset + candidate_ptr; offset_mode = MAP_LOADER_OFFSET_HEADER_RELATIVE;
    } else {
        /* try heuristic detection (scan) */
        sector_offset = detect_sector_offset(f, width, height);
        if (sector_offset >= 0) offset_mode = MAP_LOADER_OFFSET_DETECTED_HEURISTIC;
        else sector_offset = 0, offset_mode = MAP_LOADER_OFFSET_FALLBACK;
    }

    /* store into MapHandle and return */
}
```

Important implementation details and pitfalls
- Absolute vs relative offsets:
  - The header pointer may be absolute (file offset) or relative to a base pointer within the file. Implement both checks.
- Endianness:
  - Always read multi-byte fields as little-endian. Use helper functions `read_u16_le` and `read_u32_le` to make code robust.
- Alignment / padding:
  - Headers sometimes include alignment/padding. Do not assume perfect packing; read offsets from header fields instead of relying on fixed positions if possible.
- Sector ordering:
  - The loader exposes row-major access; if original code uses column-major, provide a flag or adjust read algorithm. Tests must verify with known sample or visual check.
- Partial maps / compressed formats:
  - Some zone files may use compression or extra encodings; the loader only supports the uncompressed/legacy format (4-byte sector records). If a map uses a different variant, a dedicated parser is required.

Debugging & test plan (MVP)
1. Implement `map_loader_open` to parse header and pick an offset (or detect fallback).
2. In MapViewer `main.c` replace heuristic detection with:
   - open handle with `map_loader_open`
   - call `map_loader_read_region(handle, rect_x, rect_y, rect_w, rect_h, out_buf)`
   - use out_buf contents instead of the old `read_sector_block`
   - print `map_loader_dump_info(handle)` to stderr on startup (header fields, computed sector_offset, offset_mode).
3. Run:
   - `MapViewer client/DarkEden/Data/Map/adam_c.map client/DarkEden/Data/Image/tile.spk --rect 0,0,40,30 --cell 32`
   - If wrong rendering, capture `map_loader_dump_info` output and dump first 128 spriteIDs to compare with expected bytes.
4. If header offset fails, validate by opening `adam_c.map` in hex editor and find known patterns (e.g. `0xFFFF` runs, readable "MAP" tag, or compare with known offsets from original code).
5. Use `--offset` as an override for manual verification; keep the command-line override for quick experiments.

Diagnostic prints that the loader should produce
- header parse results:
  - header_size, header_start_offset (if applicable)
  - width, height
  - raw pointer value from header
  - interpreted sector_offset (final value used)
  - offset_mode (absolute/relative/heuristic/fallback)
  - file_size
- sample sector dump (first 64 sectors: index, sprite_id, property, light)

Acceptance criteria for MVP
- `map_loader_open` must return `MAP_LOADER_OK` for `adam_c.map`.
- `map_loader_read_region` must fill `MSectorRecord[]` for the requested rectangle.
- MapViewer must render the decoded sprites and visually match (to a reasonable degree) the original map appearance for the selected region (no gross garbage).
- The loader must produce a deterministic sector_offset (no more guessing) when header is valid.

Next steps (after creating the notes & map_loader)
- Implement `client/resurrected/tools/map_viewer/map_loader.h` & `map_loader.c` using the API above.
- Update `client/resurrected/tools/map_viewer/main.c` to use `map_loader_*` APIs and remove heuristic-only code paths (keep `--offset` override).
- Run MapViewer and iterate on diagnostics. If header offsets differ across map variants, we can implement a small "map version" detection stage and support multiple header layouts.

References in the tree (source files to consult in original client)
- `client/Client/ZoneFileHeader.cpp` / `ZoneFileHeader.h`
- `client/Client/MZone.cpp` / `MZone.h`
- `client/Client/MSector.cpp` / `MSector.h`
- Constants: `client/Client/GAME1024.cpp` (tile/sector related constants)
- Current MVP implementation to update: `client/resurrected/tools/map_viewer/main.c`
- Reusable sprite pack + decode: `client/resurrected/lib/spritepack.c/.h` and `client/resurrected/lib/sprite.c/.h`

Appendix: small helper prototypes (copy into map_loader.c)
- Use these helpers to avoid subtle endian/IO bugs:

```/dev/null/map_loader_helpers.h#L1-120
/* Helper I/O functions (suggested) */

#include <stdint.h>
#include <stdio.h>

/* Read little-endian 16-bit from file at current pos. Returns -1 on EOF/IO error. */
int read_u16_le_file(FILE* f, uint16_t* out);

/* Read little-endian 32-bit from file at current pos. Returns -1 on EOF/IO error. */
int read_u32_le_file(FILE* f, uint32_t* out);

/* Read block (count of MSectorRecord) at offset into out buffer. */
int read_sector_block_at(FILE* f, long offset, uint32_t count, MSectorRecord* out);
```

Notes about implementing robust IO
- Always check return values of `fseek`, `ftell`, and `fread`.
- For portability prefer explicit `uint32_t`/`uint16_t` and fixed-width casts.
- Avoid reading large sections at once without bounds checks (`file_size` must be used).

If you want, I can now:
- produce the concrete `map_loader.h` and `map_loader.c` files in `client/resurrected/tools/map_viewer/` using the exact API above and implement the core functions (open, read_region, dump_info, close) — keeping the implementation small and focused.
- or, implement only `map_loader_open` + `map_loader_read_region` and a corresponding unit test harness that prints header diagnostics for `adam_c.map`.

Tell me which you'd prefer next:
- "Implement files" (I will create `map_loader.c/.h` and patch `main.c` to use them), or
- "Just header + tests" (I will implement only header and small driver for diagnostics), or
- "Stop here" (you want to review notes before any code changes).