/**
 * @file test_zone.c
 * @brief Unit tests for zone.h
 */

#include "zone.h"
#include "zone.c"
#include <stdio.h>
#include <string.h>
#include <assert.h>

/* Test helper macros */
#define TEST_ASSERT(cond, msg) do { \
    if (!(cond)) { \
        printf("FAIL: %s\n", msg); \
        return 1; \
    } \
} while(0)

#define TEST_PASS(name) printf("PASS: %s\n", name)

/* ============================================================================
 * Tests
 * ============================================================================ */

static int test_zone_strerror(void) {
    TEST_ASSERT(strcmp(zone_strerror(ZONE_OK), "OK") == 0,
                "ZONE_OK should return 'OK'");
    TEST_ASSERT(strcmp(zone_strerror(ZONE_ERR_IO), "I/O error") == 0,
                "ZONE_ERR_IO should return 'I/O error'");
    TEST_ASSERT(strcmp(zone_strerror(ZONE_ERR_FORMAT), "Invalid format") == 0,
                "ZONE_ERR_FORMAT should return 'Invalid format'");
    TEST_ASSERT(strcmp(zone_strerror(ZONE_ERR_VERSION), "Unsupported version") == 0,
                "ZONE_ERR_VERSION should return 'Unsupported version'");
    TEST_ASSERT(strcmp(zone_strerror(ZONE_ERR_NOMEM), "Out of memory") == 0,
                "ZONE_ERR_NOMEM should return 'Out of memory'");
    TEST_ASSERT(strcmp(zone_strerror(ZONE_ERR_PARAM), "Invalid parameter") == 0,
                "ZONE_ERR_PARAM should return 'Invalid parameter'");

    TEST_PASS("test_zone_strerror");
    return 0;
}

static int test_zone_load_null_params(void) {
    Zone zone;
    ZoneHeader header;

    TEST_ASSERT(zone_load(NULL, &zone)->code == ZONE_ERR_PARAM,
                "zone_load with NULL path should return ZONE_ERR_PARAM");
    TEST_ASSERT(zone_load("test.map", NULL)->code == ZONE_ERR_PARAM,
                "zone_load with NULL zone should return ZONE_ERR_PARAM");
    TEST_ASSERT(zone_load_header(NULL, &header)->code == ZONE_ERR_PARAM,
                "zone_load_header with NULL path should return ZONE_ERR_PARAM");
    TEST_ASSERT(zone_load_header("test.map", NULL)->code == ZONE_ERR_PARAM,
                "zone_load_header with NULL header should return ZONE_ERR_PARAM");
    TEST_ASSERT(zone_load_from_file(NULL, &zone)->code == ZONE_ERR_PARAM,
                "zone_load_from_file with NULL file should return ZONE_ERR_PARAM");

    TEST_PASS("test_zone_load_null_params");
    return 0;
}

static int test_zone_load_nonexistent(void) {
    Zone zone;

    TEST_ASSERT(zone_load("nonexistent_file_12345.map", &zone)->code == ZONE_ERR_IO,
                "zone_load with nonexistent file should return ZONE_ERR_IO");

    TEST_PASS("test_zone_load_nonexistent");
    return 0;
}

static int test_zone_get_sector_null(void) {
    TEST_ASSERT(zone_get_sector(NULL, 0, 0) == NULL,
                "zone_get_sector with NULL zone should return NULL");

    Zone zone;
    memset(&zone, 0, sizeof(zone));
    TEST_ASSERT(zone_get_sector(&zone, 0, 0) == NULL,
                "zone_get_sector with NULL sectors should return NULL");

    TEST_PASS("test_zone_get_sector_null");
    return 0;
}

static int test_zone_can_walk_null(void) {
    TEST_ASSERT(zone_can_walk_ground(NULL, 0, 0) == false,
                "zone_can_walk_ground with NULL should return false");
    TEST_ASSERT(zone_can_walk_flying(NULL, 0, 0) == false,
                "zone_can_walk_flying with NULL should return false");
    TEST_ASSERT(zone_can_walk_underground(NULL, 0, 0) == false,
                "zone_can_walk_underground with NULL should return false");

    TEST_PASS("test_zone_can_walk_null");
    return 0;
}

static int test_zone_free_null(void) {
    /* Should not crash */
    zone_free(NULL);

    Zone zone;
    memset(&zone, 0, sizeof(zone));
    zone_free(&zone);

    TEST_PASS("test_zone_free_null");
    return 0;
}

static int test_sector_flags(void) {
    /* Test flag constants match expected values */
    TEST_ASSERT(SECTOR_BLOCK_UNDERGROUND == 0x01, "SECTOR_BLOCK_UNDERGROUND should be 0x01");
    TEST_ASSERT(SECTOR_BLOCK_GROUND == 0x02, "SECTOR_BLOCK_GROUND should be 0x02");
    TEST_ASSERT(SECTOR_BLOCK_FLYING == 0x04, "SECTOR_BLOCK_FLYING should be 0x04");
    TEST_ASSERT(SECTOR_BLOCK_ALL == 0x07, "SECTOR_BLOCK_ALL should be 0x07");
    TEST_ASSERT(SECTOR_PORTAL == 0x80, "SECTOR_PORTAL should be 0x80");

    TEST_PASS("test_sector_flags");
    return 0;
}

/* ============================================================================
 * Test Runner
 * ============================================================================ */

int test_zone_run_all(void) {
    int failures = 0;

    printf("\n=== Zone Tests ===\n");

    failures += test_zone_strerror();
    failures += test_zone_load_null_params();
    failures += test_zone_load_nonexistent();
    failures += test_zone_get_sector_null();
    failures += test_zone_can_walk_null();
    failures += test_zone_free_null();
    failures += test_sector_flags();

    printf("\nZone tests: %d failures\n", failures);
    return failures;
}

int main(void) {
    test_zone_run_all();
}
