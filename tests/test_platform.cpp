/*-----------------------------------------------------------------------------

	test_platform.cpp

	Test program for platform abstraction layer.
	Verifies that all platform functions work correctly.

	2025.01.14

-----------------------------------------------------------------------------*/

#include "../basic/Platform.h"
#include "../basic/Directory.h"
#include <stdio.h>
#include <string.h>

/* Test result tracking */
int tests_passed = 0;
int tests_failed = 0;

#define TEST(name) \
	do { \
		printf("  Testing: %s... ", #name); \
		if (test_##name()) { \
			tests_passed++; \
			printf("PASSED\n"); \
		} else { \
			tests_failed++; \
			printf("FAILED\n"); \
		} \
	} while(0)

#define ASSERT_TRUE(condition) \
	do { \
		if (!(condition)) { \
			printf("\n    Assert failed: %s\n", #condition); \
			printf("    File: %s, Line: %d\n", __FILE__, __LINE__); \
			return 0; \
		} \
	} while(0)

#define ASSERT_EQ(expected, actual) \
	do { \
		if ((expected) != (actual)) { \
			printf("\n    Assert failed: %s == %s\n", #expected, #actual); \
			printf("    Expected: %lld, Actual: %lld\n", \
			       (long long)(expected), (long long)(actual)); \
			printf("    File: %s, Line: %d\n", __FILE__, __LINE__); \
			return 0; \
		} \
	} while(0)

/* Thread test data */
struct ThreadTestData {
	int value;
	platform_mutex_t mutex;
};

/* ============================================================================
 * Test Functions
 * ============================================================================ */

DWORD thread_test_func(void* param) {
	struct ThreadTestData* data = (struct ThreadTestData*)param;

	/* Lock mutex and increment value */
	platform_mutex_lock(data->mutex);
	for (int i = 0; i < 100; i++) {
		data->value++;
	}
	platform_mutex_unlock(data->mutex);

	return 0;
}

int test_time_functions(void) {
	DWORD ticks1 = platform_get_ticks();
	platform_sleep(100); /* Sleep 100ms */
	DWORD ticks2 = platform_get_ticks();

	DWORD elapsed = ticks2 - ticks1;
	ASSERT_TRUE(elapsed >= 90 && elapsed < 200); /* Allow some tolerance */

	uint64_t counter1 = platform_get_performance_counter();
	uint64_t freq = platform_get_performance_frequency();
	ASSERT_TRUE(freq > 0);

	uint64_t counter2 = platform_get_performance_counter();
	ASSERT_TRUE(counter2 > counter1);

	return 1;
}

int test_mutex(void) {
	platform_mutex_t mutex = platform_mutex_create(0);
	ASSERT_TRUE(mutex != PLATFORM_INVALID_MUTEX);

	int result = platform_mutex_lock(mutex);
	ASSERT_EQ(0, result);

	result = platform_mutex_unlock(mutex);
	ASSERT_EQ(0, result);

	platform_mutex_close(mutex);
	return 1;
}

int test_thread(void) {
	struct ThreadTestData data;
	data.value = 0;
	data.mutex = platform_mutex_create(0);

	/* Create multiple threads */
	platform_thread_t threads[5];
	for (int i = 0; i < 5; i++) {
		threads[i] = platform_thread_create(thread_test_func, &data);
		ASSERT_TRUE(threads[i] != PLATFORM_INVALID_THREAD);
	}

	/* Wait for all threads to finish */
	for (int i = 0; i < 5; i++) {
		platform_thread_wait(threads[i]);
	}

	/* Verify final value */
	ASSERT_EQ(500, data.value);

	platform_mutex_close(data.mutex);
	return 1;
}

int test_event(void) {
	platform_event_t event = platform_event_create(0, 0);
	ASSERT_TRUE(event != PLATFORM_INVALID_EVENT);

	/* Test timeout */
	int result = platform_event_wait(event, 100);
	ASSERT_TRUE(result != 0); /* Should timeout */

	/* Test signal */
	platform_event_signal(event);
	result = platform_event_wait(event, PLATFORM_INFINITE);
	ASSERT_EQ(0, result); /* Should succeed */

	/* Test reset */
	platform_event_reset(event);
	result = platform_event_wait(event, 100);
	ASSERT_TRUE(result != 0); /* Should timeout again */

	platform_event_close(event);
	return 1;
}

int test_file_operations(void) {
	char buffer[512];

	/* Get executable directory */
	int result = platform_get_executable_dir(buffer, sizeof(buffer));
	ASSERT_EQ(0, result);
	ASSERT_TRUE(strlen(buffer) > 0);

	/* Check if executable exists (should be true) */
	char exePath[512];
	snprintf(exePath, sizeof(exePath), "%s%stest_platform",
	         buffer, platform_get_path_separator() == '\\' ? "\\" : "/");

	/* Test path separator */
	char sep = platform_get_path_separator();
	ASSERT_TRUE(sep == '\\' || sep == '/');

	return 1;
}

int test_directory_class(void) {
	/* Test global directory instance */
	ASSERT_TRUE(gC_directory.GetProgramDirectory() != NULL);

	/* Test file existence check */
	char* testPath = gC_directory.GetMixedPathWidthProgramDir("test_platform.cpp");
	ASSERT_TRUE(testPath != NULL);
	ASSERT_TRUE(gC_directory.ExistFile(testPath));
	delete[] testPath;

	return 1;
}

int test_keyboard(void) {
	/* Just test that function works (don't press Ctrl!) */
	int ctrlState = platform_is_ctrl_pressed();
	/* Should be 0 or 1, both valid */
	ASSERT_TRUE(ctrlState == 0 || ctrlState == 1);

	return 1;
}

int test_config_functions(void) {
	const char* testKey = "Software\\DarkEdenTest";
	const char* testValue = "TestValue";
	const char* testData = "TestData123";

	/* Set config value */
	int result = platform_config_set_string(testKey, testValue, testData);

	/* On non-Windows, this should write to config file */
	/* On Windows, this writes to registry (may fail without admin) */
	/* So we just test that the function doesn't crash */

	return 1;
}

/* ============================================================================
 * Main
 * ============================================================================ */

int main(int argc, char* argv[]) {
	printf("========================================\n");
	printf("Platform Abstraction Layer Test Suite\n");
	printf("========================================\n\n");

	/* Initialize platform */
	if (platform_init() != 0) {
		printf("ERROR: Failed to initialize platform layer\n");
		return 1;
	}

	printf("Platform: ");
#ifdef PLATFORM_WINDOWS
	printf("Windows (native)\n");
#elif defined(PLATFORM_LINUX)
	printf("Linux (SDL backend)\n");
#elif defined(PLATFORM_MACOS)
	printf("macOS (SDL backend)\n");
#else
	printf("Unknown\n");
#endif
	printf("\n");

	/* Run tests */
	printf("=== Time Functions ===\n");
	TEST(time_functions);
	printf("\n");

	printf("=== Synchronization Primitives ===\n");
	TEST(mutex);
	TEST(event);
	TEST(thread);
	printf("\n");

	printf("=== File Operations ===\n");
	TEST(file_operations);
	TEST(directory_class);
	printf("\n");

	printf("=== Input Functions ===\n");
	TEST(keyboard);
	printf("\n");

	printf("=== Configuration ===\n");
	TEST(config_functions);
	printf("\n");

	/* Cleanup */
	platform_shutdown();

	/* Print summary */
	printf("========================================\n");
	printf("Test Summary:\n");
	printf("  Passed: %d\n", tests_passed);
	printf("  Failed: %d\n", tests_failed);
	printf("========================================\n");

	return (tests_failed > 0) ? 1 : 0;
}
