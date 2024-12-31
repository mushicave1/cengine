#ifndef CENGINE_TEST_H
#define CENGINE_TEST_H

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#define CENGINE_TEST_RUN_TEST(T)                                                                                                            \
	do {                                                                                                                         \
		int result = T();                                                                                                        \
		if (result == -1) {                                                                                                   \
			printf("test failed: " #T "\n");                                                                                   \
			return -1;                                                                                                            \
		} else {                                                                                                                        \
			printf("test passed: " #T "\n");                                                                                   \
		}                                                                                                                        \
	} while (false)

#define CENGINE_TEST_ASSERT_TRUE(condition)																									\
	do {																														\
		if ((condition) == false) 																								\
		{																														\
			printf("condition failed: " #condition "\r\n");																	\
			return -1;																											\
		}																														\
	} while (false)

#endif