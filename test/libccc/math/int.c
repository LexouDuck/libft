
#include "libccc/format.h"
#include "libccc/int.h"
#include "libccc/math/int.h"

#include "test.h"



// Disable certain GCC warnings temporarily
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdivision-by-zero"




#define TEST(OPERAND1, OPERATOR, OPERAND2) \
	((OPERAND1) OPERATOR (OPERAND2)), (OPERAND1), (OPERAND2)

#define TEST_FLOW(OPERAND1, OPERATOR, OPERAND2, BOUND) \
	((OPERAND1) OPERATOR (OPERAND2)), (OPERAND1), (OPERAND2)



#define DEFINETEST_UINT_(BITS) \
void	print_test_u##BITS##(char const* test_name, int can_segfault,		\
		t_u##BITS	a,														\
		t_u##BITS	b)														\
{																			\
	TEST_INIT(u##BITS)														\
	TEST_PERFORM_LIBC(	u##BITS##, a, b)									\
	TEST_PRINT(u##BITS,	u##BITS##, "a="SF_U##BITS", b="SF_U##BITS, a, b)	\
}																			\
void	test_u##BITS##(void)												\
{																			\
//	| TEST FUNCTION        | TEST NAME        |CAN SEGV| EXPECTING           | TEST ARGS //	\
	print_test_u##BITS##("u"#BITS"     ",	FALSE,               (0 + 0), 0, 0      );	\
}

#ifndef c_u8
void test_u8(void)	{}
#else
DEFINETEST_UINT_(8)
#endif

#ifndef c_u16
void test_u16(void)	{}
#else
DEFINETEST_UINT_(16)
#endif

#ifndef c_u32
void test_u32(void)	{}
#else
DEFINETEST_UINT_(32)
#endif

#ifndef c_u64
void test_u64(void)	{}
#else
DEFINETEST_UINT_(64)
#endif

#ifdef __int128
#ifndef c_u128
void test_u128(void)	{}
#else
DEFINETEST_UINT_(128)
#endif
#endif



#define DEFINETEST_UINT_OPERATOR(OP, FUNC, BITS) \
void	print_test_u##BITS##FUNC(char const* test_name, int can_segfault,	\
		t_u##BITS	expecting,												\
		t_u##BITS	a,														\
		t_u##BITS	b)														\
{																			\
	TEST_INIT(u##BITS)														\
	TEST_PERFORM(		u##BITS##FUNC, a, b)								\
	TEST_PRINT(u##BITS,	u##BITS##FUNC, "a="SF_U##BITS", b="SF_U##BITS, a, b)\
}																			\
void	test_u##BITS##FUNC(void)											\
{																			\
/*	| TEST FUNCTION        | TEST NAME              |CAN SEGV| TEST		*/	\
	print_test_u##BITS##FUNC("u"#BITS#FUNC"        ",	FALSE, TEST(	0,  OP, 0) );	\
	print_test_u##BITS##FUNC("u"#BITS#FUNC"        ",	FALSE, TEST(	1,  OP, 0) );	\
	print_test_u##BITS##FUNC("u"#BITS#FUNC"        ",	FALSE, TEST(	0,  OP, 1) );	\
	print_test_u##BITS##FUNC("u"#BITS#FUNC"        ",	FALSE, TEST(	1,  OP, 1) );	\
	print_test_u##BITS##FUNC("u"#BITS#FUNC"        ",	FALSE, TEST(	1,  OP, 1) );	\
	print_test_u##BITS##FUNC("u"#BITS#FUNC"        ",	FALSE, TEST(	2,  OP, 1) );	\
	print_test_u##BITS##FUNC("u"#BITS#FUNC"        ",	FALSE, TEST(	1,  OP, 2) );	\
	print_test_u##BITS##FUNC("u"#BITS#FUNC"        ",	FALSE, TEST(	2,  OP, 2) );	\
	print_test_u##BITS##FUNC("u"#BITS#FUNC"        ",	FALSE, TEST(	3,  OP, 5) );	\
	print_test_u##BITS##FUNC("u"#BITS#FUNC"        ",	FALSE, TEST(	10, OP, 5) );	\
	print_test_u##BITS##FUNC("u"#BITS#FUNC" (n=max)",	FALSE, TEST(	 1, OP, U##BITS##_MAX - 1) );	\
	print_test_u##BITS##FUNC("u"#BITS#FUNC" (n=max)",	FALSE, TEST(	 0, OP, U##BITS##_MAX    ) );	\
	print_test_u##BITS##FUNC("u"#BITS#FUNC" (n>max)",	FALSE, TEST_FLOW(1, OP, U##BITS##_MAX    , U##BITS##_MAX) );	\
	print_test_u##BITS##FUNC("u"#BITS#FUNC" (n>max)",	FALSE, TEST_FLOW(2, OP, U##BITS##_MAX    , U##BITS##_MAX) );	\
}

#ifndef c_uadd
void test_u8add(void)	{}
void test_u8sub(void)	{}
void test_u8mul(void)	{}
void test_u8div(void)	{}
void test_u8mod(void)	{}

void test_u16add(void)	{}
void test_u16sub(void)	{}
void test_u16mul(void)	{}
void test_u16div(void)	{}
void test_u16mod(void)	{}

void test_u32add(void)	{}
void test_u32sub(void)	{}
void test_u32mul(void)	{}
void test_u32div(void)	{}
void test_u32mod(void)	{}

void test_u64add(void)	{}
void test_u64sub(void)	{}
void test_u64mul(void)	{}
void test_u64div(void)	{}
void test_u64mod(void)	{}

#ifdef __int128
void test_u128add(void)	{}
void test_u128sub(void)	{}
void test_u128mul(void)	{}
void test_u128div(void)	{}
void test_u128mod(void)	{}
#endif

#else
DEFINETEST_UINT_OPERATOR(+, add, 8)
DEFINETEST_UINT_OPERATOR(-, sub, 8)
DEFINETEST_UINT_OPERATOR(*, mul, 8)
DEFINETEST_UINT_OPERATOR(/, div, 8)
DEFINETEST_UINT_OPERATOR(%, mod, 8)

DEFINETEST_UINT_OPERATOR(+, add, 16)
DEFINETEST_UINT_OPERATOR(-, sub, 16)
DEFINETEST_UINT_OPERATOR(*, mul, 16)
DEFINETEST_UINT_OPERATOR(/, div, 16)
DEFINETEST_UINT_OPERATOR(%, mod, 16)

DEFINETEST_UINT_OPERATOR(+, add, 32)
DEFINETEST_UINT_OPERATOR(-, sub, 32)
DEFINETEST_UINT_OPERATOR(*, mul, 32)
DEFINETEST_UINT_OPERATOR(/, div, 32)
DEFINETEST_UINT_OPERATOR(%, mod, 32)

DEFINETEST_UINT_OPERATOR(+, add, 64)
DEFINETEST_UINT_OPERATOR(-, sub, 64)
DEFINETEST_UINT_OPERATOR(*, mul, 64)
DEFINETEST_UINT_OPERATOR(/, div, 64)
DEFINETEST_UINT_OPERATOR(%, mod, 64)

#ifdef __int128
DEFINETEST_UINT_OPERATOR(+, add, 128)
DEFINETEST_UINT_OPERATOR(-, sub, 128)
DEFINETEST_UINT_OPERATOR(*, mul, 128)
DEFINETEST_UINT_OPERATOR(/, div, 128)
DEFINETEST_UINT_OPERATOR(%, mod, 128)
#endif

#endif



#define DEFINETEST_SINT_OPERATOR(OP, FUNC, BITS) \
void	print_test_s##BITS##FUNC(char const* test_name, int can_segfault,	\
		t_s##BITS	expecting,												\
		t_s##BITS	a,														\
		t_s##BITS	b)														\
{																			\
	TEST_INIT(s##BITS)														\
	TEST_PERFORM(		s##BITS##FUNC, a, b)								\
	TEST_PRINT(s##BITS,	s##BITS##FUNC, "a="SF_S##BITS", b="SF_S##BITS, a, b)\
}																			\
void	test_s##BITS##FUNC(void)											\
{																			\
/*	| TEST FUNCTION         | TEST NAME             |CAN SEGV| TEST		*/	\
	print_test_s##BITS##FUNC("s"#BITS#FUNC"        ",	FALSE, TEST(		0,  OP, 0) );	\
	print_test_s##BITS##FUNC("s"#BITS#FUNC"        ",	FALSE, TEST(		1,  OP, 0) );	\
	print_test_s##BITS##FUNC("s"#BITS#FUNC"        ",	FALSE, TEST(		0,  OP, 1) );	\
	print_test_s##BITS##FUNC("s"#BITS#FUNC"        ",	FALSE, TEST(		1,  OP, 1) );	\
	print_test_s##BITS##FUNC("s"#BITS#FUNC"        ",	FALSE, TEST(		1,  OP, 1) );	\
	print_test_s##BITS##FUNC("s"#BITS#FUNC"        ",	FALSE, TEST(		2,  OP, 1) );	\
	print_test_s##BITS##FUNC("s"#BITS#FUNC"        ",	FALSE, TEST(		1,  OP, 2) );	\
	print_test_s##BITS##FUNC("s"#BITS#FUNC"        ",	FALSE, TEST(		2,  OP, 2) );	\
	print_test_s##BITS##FUNC("s"#BITS#FUNC"        ",	FALSE, TEST(		3,  OP, 5) );	\
	print_test_s##BITS##FUNC("s"#BITS#FUNC"        ",	FALSE, TEST(		10, OP, 5) );	\
	print_test_s##BITS##FUNC("s"#BITS#FUNC" (n=max)",	FALSE, TEST(		+1, OP, S##BITS##_MAX - 1) );	\
	print_test_s##BITS##FUNC("s"#BITS#FUNC" (n=max)",	FALSE, TEST(		+0, OP, S##BITS##_MAX    ) );	\
	print_test_s##BITS##FUNC("s"#BITS#FUNC" (n>max)",	FALSE, TEST_FLOW(	+1, OP, S##BITS##_MAX    , S##BITS##_MAX) );	\
	print_test_s##BITS##FUNC("s"#BITS#FUNC" (n>max)",	FALSE, TEST_FLOW(	+2, OP, S##BITS##_MAX    , S##BITS##_MAX) );	\
	print_test_s##BITS##FUNC("s"#BITS#FUNC" (n=min)",	FALSE, TEST(		-1, OP, S##BITS##_MIN + 1) );	\
	print_test_s##BITS##FUNC("s"#BITS#FUNC" (n=min)",	FALSE, TEST(		-0, OP, S##BITS##_MIN    ) );	\
	print_test_s##BITS##FUNC("s"#BITS#FUNC" (n<min)",	FALSE, TEST_FLOW(	-1, OP, S##BITS##_MIN    , S##BITS##_MIN) );	\
	print_test_s##BITS##FUNC("s"#BITS#FUNC" (n<min)",	FALSE, TEST_FLOW(	-2, OP, S##BITS##_MIN    , S##BITS##_MIN) );	\
}

#ifndef c_sadd
void test_s8add(void)	{}
void test_s8sub(void)	{}
void test_s8mul(void)	{}
void test_s8div(void)	{}
void test_s8mod(void)	{}

void test_s16add(void)	{}
void test_s16sub(void)	{}
void test_s16mul(void)	{}
void test_s16div(void)	{}
void test_s16mod(void)	{}

void test_s32add(void)	{}
void test_s32sub(void)	{}
void test_s32mul(void)	{}
void test_s32div(void)	{}
void test_s32mod(void)	{}

void test_s64add(void)	{}
void test_s64sub(void)	{}
void test_s64mul(void)	{}
void test_s64div(void)	{}
void test_s64mod(void)	{}

#ifdef __int128
void test_s128add(void)	{}
void test_s128sub(void)	{}
void test_s128mul(void)	{}
void test_s128div(void)	{}
void test_s128mod(void)	{}
#endif

#else
DEFINETEST_SINT_OPERATOR(+, add, 8)
DEFINETEST_SINT_OPERATOR(-, sub, 8)
DEFINETEST_SINT_OPERATOR(*, mul, 8)
DEFINETEST_SINT_OPERATOR(/, div, 8)
DEFINETEST_SINT_OPERATOR(%, mod, 8)

DEFINETEST_SINT_OPERATOR(+, add, 16)
DEFINETEST_SINT_OPERATOR(-, sub, 16)
DEFINETEST_SINT_OPERATOR(*, mul, 16)
DEFINETEST_SINT_OPERATOR(/, div, 16)
DEFINETEST_SINT_OPERATOR(%, mod, 16)

DEFINETEST_SINT_OPERATOR(+, add, 32)
DEFINETEST_SINT_OPERATOR(-, sub, 32)
DEFINETEST_SINT_OPERATOR(*, mul, 32)
DEFINETEST_SINT_OPERATOR(/, div, 32)
DEFINETEST_SINT_OPERATOR(%, mod, 32)

DEFINETEST_SINT_OPERATOR(+, add, 64)
DEFINETEST_SINT_OPERATOR(-, sub, 64)
DEFINETEST_SINT_OPERATOR(*, mul, 64)
DEFINETEST_SINT_OPERATOR(/, div, 64)
DEFINETEST_SINT_OPERATOR(%, mod, 64)

#ifdef __int128
DEFINETEST_SINT_OPERATOR(+, add, 128)
DEFINETEST_SINT_OPERATOR(-, sub, 128)
DEFINETEST_SINT_OPERATOR(*, mul, 128)
DEFINETEST_SINT_OPERATOR(/, div, 128)
DEFINETEST_SINT_OPERATOR(%, mod, 128)
#endif

#endif





/*
** ************************************************************************** *|
**                            Test Suite Function                             *|
** ************************************************************************** *|
*/

int		testsuite_math_int(void)
{
	print_suite_title("libccc/math/int");

	print_nonstd();

	test_u8add();
	test_u8sub();
	test_u8mul();
	test_u8div();
	test_u8mod();

	test_s8add();
	test_s8sub();
	test_s8mul();
	test_s8div();
	test_s8mod();

	test_u16add();
	test_u16sub();
	test_u16mul();
	test_u16div();
	test_u16mod();

	test_s16add();
	test_s16sub();
	test_s16mul();
	test_s16div();
	test_s16mod();

	test_u32add();
	test_u32sub();
	test_u32mul();
	test_u32div();
	test_u32mod();

	test_s32add();
	test_s32sub();
	test_s32mul();
	test_s32div();
	test_s32mod();

	test_u64add();
	test_u64sub();
	test_u64mul();
	test_u64div();
	test_u64mod();

	test_s64add();
	test_s64sub();
	test_s64mul();
	test_s64div();
	test_s64mod();

	test_u128add();
	test_u128sub();
	test_u128mul();
	test_u128div();
	test_u128mod();

	test_s128add();
	test_s128sub();
	test_s128mul();
	test_s128div();
	test_s128mod();

//	test_pow();
//	test_sqrt();
//	test_abs();

	return (OK);
}


// Re-enable certain GCC warnings
#pragma GCC diagnostic pop