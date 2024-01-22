/*
 * Copyright (C) 2020  Jimmy Aguilar Mena
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <nanos6.h>

#ifdef NDEBUG
#define assert(p) do_not_use_assert!!(p)
#endif

#define fail_if(cond, FORMAT, ...)										\
	do {																\
		if (cond) {														\
			fprintf(stderr, "%s:%d (%s) " FORMAT,						\
			        __FILE__, __LINE__, #cond, ##__VA_ARGS__);			\
			abort();													\
		}																\
	} while(0)


#define assert_that(cond) fail_if((cond) == false, "But true expected")

// Function prototype
typedef bool (*test_fn)(void);

// Test type pair (helper)
typedef struct _test{
    test_fn func;
    const char *name;
} test_t;

// Macro to add functions to the list
#define FUNC_DEF(func) { func, #func }

extern size_t ARRAY_SZ;
extern size_t TASK_SZ;
extern size_t EXPECTED_VALUE;

// Function to run tests
/* Unit test functions */
static inline int run_test(test_t *test)
{
	printf("TEST %s: ", test->name);
	const bool success = test->func();

	if (!success) {
		printf("\033[0;31m%s\033[0m\n", "FAILED");
		exit(EXIT_FAILURE);
	}

	printf("\033[0;32m%s\033[0m\n", "PASSED");

	return EXIT_SUCCESS;
}


static inline int run_tests(test_t tests[])
{
	test_t *it = tests;

	while (it->func != NULL) {
		const int ret = run_test(it);

		assert_that(ret == EXIT_SUCCESS);
		++it;
	}

	return EXIT_SUCCESS;
}


static inline void print_array(int *buff, size_t size)
{
	printf("%p: ", buff);
	for (size_t i = 0; i < size; ++i)
		printf("%c%d", (i == 0) ? '[' : ' ', buff[i]);
	printf("]\n");
}


#define define_init(type)												\
																		\
	static inline type *array_alloc_##type(size_t size, bool local)		\
	{																	\
		void *ret;														\
		if (local)														\
			ret = nanos6_lmalloc(size * sizeof(type));					\
		else															\
			ret = nanos6_dmalloc(size * sizeof(type), nanos6_equpart_distribution, 0, NULL); \
																		\
		fail_if(ret == NULL, "Nanos allocation failed\n");				\
																		\
		return ret;														\
	}																	\
																		\
	static inline void array_free_##type(type *ptr, size_t size, bool local) \
	{																	\
		if (local)														\
			nanos6_lfree(ptr, size * sizeof(type));						\
		else															\
			nanos6_dfree(ptr, size * sizeof(type));						\
	}																	\
																		\
	static inline void init_buff_range_##type(type *buff, size_t size, type start) \
	{																	\
		for (size_t i = 0; i < size; ++i) {								\
			const type value = i + start;								\
			buff[i] = value;											\
			fail_if(value + 1 < value, "Init overflow in initialization\n"); \
		}																\
	}																	\
																		\
	/* buffer initialization functions */								\
	static inline void init_buff_value_##type(type *buff, size_t size, type value) \
	{																	\
		for (size_t i = 0; i < size; ++i) {								\
			buff[i] = value;											\
		}																\
	}


define_init(int)
define_init(size_t)
define_init(float)
define_init(double)

#undef init_buff_range
#undef init_buff_value


static inline void sum_task(int *buff, size_t size, int *sum)
{
	int _sum = 0;
	for (size_t i = 0; i < size; ++i)
		_sum += buff[i];

	*sum += _sum;
	print_array(buff, size);
	print_array(sum, 1);
}

/* sum functions */
static inline void sum_decompose(int *buff, size_t size, int *sum, size_t TS)
{
	print_array(buff, size);
	print_array(sum, size / TS);
	for (size_t i = 0; i < size; i += TS) {
		#pragma oss task in(buff[i;TS]) inout(sum[i / TS]) label("sum_task")
		sum_task(&buff[i], TS, &sum[i / TS]);
	}
}


/* sanity checks */
static inline void test_result(
	int *sum,
	size_t tasks_nr,
	bool *success,
	int expected_value
) {
	int _sum = 0;
	print_array(sum, tasks_nr);
	for (size_t i = 0; i < tasks_nr; ++i)
		_sum += sum[i];

	*success = (_sum == expected_value);
	if (!*success)
		printf("%d/%d\n", _sum, expected_value);
}

#endif /* __UTIL_H__ */
