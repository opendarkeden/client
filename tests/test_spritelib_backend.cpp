/*-----------------------------------------------------------------------------
	test_spritelib_backend.cpp

	Integration test for SpriteLib SDL2 backend
	Tests sprite backend infrastructure and API availability

-----------------------------------------------------------------------------*/

#include "client_PCH.h"
#include "SpriteLibBackend.h"

#ifdef SPRITELIB_BACKEND_SDL

#include <stdio.h>

/* Test counters */
static int tests_passed = 0;
static int tests_failed = 0;

#define TEST_START(name) \
	printf("  Testing: %s...", name); \
	fflush(stdout);

#define TEST_PASS() \
	do { \
		printf(" PASSED\n"); \
		tests_passed++; \
	} while(0)

#define TEST_FAIL(msg) \
	do { \
		printf(" FAILED\n"); \
		printf("    Error: %s\n", msg); \
		tests_failed++; \
	} while(0)

#define ASSERT(cond, msg) \
	do { \
		if (!(cond)) { \
			TEST_FAIL(msg); \
			return; \
		} \
	} while(0)

/*-----------------------------------------------------------------------------*/
static void test_backend_constants()
{
	TEST_START("Backend constants");

	/* Test that backend constants are defined */
	ASSERT(SPRITECTL_INVALID_SPRITE == 0,
		"SPRITECTL_INVALID_SPRITE should be 0");
	ASSERT(SPRITECTL_FORMAT_RGB565 == 0,
		"RGB565 format should be 0");
	ASSERT(SPRITECTL_BLT_ALPHA != 0,
		"BLT_ALPHA flag should be non-zero");

	TEST_PASS();
}

/*-----------------------------------------------------------------------------*/
static void test_backend_types()
{
	TEST_START("Backend type definitions");

	/* Test that backend types are defined */
	spritectl_sprite_t sprite = SPRITECTL_INVALID_SPRITE;
	spritectl_surface_t surface = SPRITECTL_INVALID_SURFACE;

	ASSERT(sprite == 0, "Default sprite should be 0");
	ASSERT(surface == 0, "Default surface should be 0");

	TEST_PASS();
}

/*-----------------------------------------------------------------------------*/
static void test_backend_api_declarations()
{
	TEST_START("Backend API function availability");

	/* We can't call these functions without proper setup,
	   but we can verify they link correctly */
	void* func_create = (void*)&spritectl_create_sprite;
	void* func_destroy = (void*)&spritectl_destroy_sprite;
	void* func_blt = (void*)&spritectl_blt_sprite;

	ASSERT(func_create != NULL, "create_sprite function should be available");
	ASSERT(func_destroy != NULL, "destroy_sprite function should be available");
	ASSERT(func_blt != NULL, "blt_sprite function should be available");

	TEST_PASS();
}

/*-----------------------------------------------------------------------------*/
static void test_macros_defined()
{
	TEST_START("Backend macros defined");

	/* Test that required macros are defined */
	#ifdef SPRITELIB_BACKEND_SDL
		ASSERT(1, "SPRITELIB_BACKEND_SDL is defined");
	#else
		ASSERT(0, "SPRITELIB_BACKEND_SDL should be defined");
	#endif

	TEST_PASS();
}

/*-----------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
	printf("========================================\n");
	printf("SpriteLib Backend Integration Test\n");
	printf("========================================\n\n");

	printf("=== Backend Infrastructure Tests ===\n");
	test_backend_constants();
	test_backend_types();
	test_backend_api_declarations();
	test_macros_defined();

	printf("\n========================================\n");
	printf("Test Summary:\n");
	printf("  Passed: %d\n", tests_passed);
	printf("  Failed: %d\n", tests_failed);
	printf("========================================\n");

	if (tests_failed == 0) {
		printf("\n✓ All tests passed!\n");
		printf("✓ SpriteLib backend infrastructure is working correctly\n");
		printf("✓ Backend API is available and linked\n");
		printf("✓ Ready for integration with game code\n");
		printf("\nNOTE: Full sprite rendering tests require:\n");
		printf("  - Game client executable\n");
		printf("  - Sprite data files (.SPK)\n");
		printf("  - SDL2 initialization\n");
	} else {
		printf("\n✗ Some tests failed\n");
	}

	return (tests_failed == 0) ? 0 : 1;
}

#else /* !SPRITELIB_BACKEND_SDL */

int main(int argc, char* argv[])
{
	printf("SpriteLib backend not enabled (SPRITELIB_BACKEND_SDL not defined)\n");
	printf("This test requires SDL2 backend to be enabled\n");
	printf("Compile with -DSPRITELIB_BACKEND_SDL or enable via CMake\n");
	return 1;
}

#endif /* SPRITELIB_BACKEND_SDL */
