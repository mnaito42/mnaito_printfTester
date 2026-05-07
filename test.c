/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnaito <mnaito@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 12:57:36 by mnaito            #+#    #+#             */
/*   Updated: 2026/05/07 18:29:54 by mnaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>

#include "ft_printf.h"

static void read_all(int fd, char *buf, size_t size)
{
	ssize_t n;
	size_t total = 0;

	while ((n = read(fd, buf + total, size - 1 - total)) > 0)
		total += n;
	buf[total] = '\0';
}

#define RUN_TEST(label, fmt, ...) \
	do { \
		int ret_std, ret_ft; \
		int pipe_std[2]; \
		int pipe_ft[2]; \
		char buf_std[4096] = {0}; \
		char buf_ft[4096] = {0}; \
\
		pipe(pipe_std); \
		pipe(pipe_ft); \
\
		int stdout_copy = dup(1); \
\
		printf("\n==============================\n"); \
		printf("[CASE] %s\n", label); \
		printf("format: \"%s\"\n", fmt); \
\
		/* printf */ \
		dup2(pipe_std[1], 1); \
		close(pipe_std[1]); \
		ret_std = printf(fmt, ##__VA_ARGS__); \
		fflush(stdout); \
		close(1); \
\
		/* ft_printf */ \
		dup2(pipe_ft[1], 1); \
		close(pipe_ft[1]); \
		ret_ft = ft_printf(fmt, ##__VA_ARGS__); \
		fflush(stdout); \
		close(1); \
\
		/* restore */ \
		dup2(stdout_copy, 1); \
		close(stdout_copy); \
\
		read_all(pipe_std[0], buf_std, sizeof(buf_std)); \
		read_all(pipe_ft[0], buf_ft, sizeof(buf_ft)); \
		close(pipe_std[0]); \
		close(pipe_ft[0]); \
\
		printf("printf   : [%s]\n", buf_std); \
		printf("ft_printf: [%s]\n", buf_ft); \
\
		printf("return -> printf=%d / ft_printf=%d\n", ret_std, ret_ft); \
\
		if (strcmp(buf_std, buf_ft) == 0 && ret_std == ret_ft) \
			printf("✅ OK\n"); \
		else \
			printf("❌ NG\n"); \
\
	} while (0)

void	char_test_sub(char ch)
{
	RUN_TEST("char/basic", "%c", ch);
	RUN_TEST("char/width", "%5c", ch);
	RUN_TEST("char/left", "%-5c", ch);
	RUN_TEST("char/zero-ignored", "%05c", ch);
	RUN_TEST("char/precision-ignored", "%.3c", ch);
	RUN_TEST("char/plus-ignored", "%+c", ch);
	RUN_TEST("char/space-ignored", "% c", ch);
	RUN_TEST("char/flag-conflict", "%-05c", ch);
	RUN_TEST("char/flag-plus-space", "%+ c", ch);
	RUN_TEST("char/all-flags", "%0-+# 5c", ch);
	RUN_TEST("char/precision+zero", "%05.3c", ch);
	RUN_TEST("char/width-zero", "%0c", ch);
}

void	char_test()
{
	char_test_sub('A');
	char_test_sub('~');
	char_test_sub('\0');
}

void	string_test_sub(char *s)
{
	RUN_TEST("str/basic", "%s", s);
	RUN_TEST("str/width", "%10s", s);
	RUN_TEST("str/width", "%3s", s);
	RUN_TEST("str/left", "%-10s", s);
	RUN_TEST("str/left", "%-3s", s);
	RUN_TEST("str/width-zero", "%0s", s);
	RUN_TEST("str/left, width-zero", "%-0s", s);
	RUN_TEST("str/precision", "%.5s", s);
	RUN_TEST("str/precision", "%.3s", s);
	RUN_TEST("str/zero-precision", "%.0s", s);
	RUN_TEST("str/width+precision", "%10.5s", s);
	RUN_TEST("str/left+precision", "%-10.5s", s);
	RUN_TEST("str/width+precision", "%10.3s", s);
	RUN_TEST("str/left+precision", "%-10.3s", s);
	RUN_TEST("str/width+precision", "%10.0s", s);
	RUN_TEST("str/left+precision", "%-10.0s", s);

	RUN_TEST("str/zero-ignored", "%010s", s);
	RUN_TEST("str/plus-ignored", "%+s", s);
	RUN_TEST("str/space-ignored", "% s", s);
	RUN_TEST("str/hash-ignored", "%#s", s);

	RUN_TEST("str/all-flags", "%0-+# 10.5s", s);
	RUN_TEST("str/all-flags", "%0-+# 2.10s", s);
	RUN_TEST("str/all-flags", "%0-+# 7.3s", s);
}

void	string_test()
{
	string_test_sub("Hello, world!");
	string_test_sub("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	string_test_sub("Hi");
	string_test_sub("");
	string_test_sub(NULL);
}

void	int_test_sub(int n)
{
	RUN_TEST("int/basic-d", "%d", n);
	RUN_TEST("int/basic-i", "%i", n);

	RUN_TEST("int/width", "%10d", n);
	RUN_TEST("int/left", "%-10d", n);

	RUN_TEST("int/precision", "%.5d", n);
	RUN_TEST("int/width+precision", "%10.5d", n);
	RUN_TEST("int/left+precision", "%-10.5d", n);

	RUN_TEST("int/zero", "%010d", n);
	RUN_TEST("int/zero+precision", "%010.5d", n);

	RUN_TEST("int/plus", "%+d", n);
	RUN_TEST("int/space", "% d", n);

	RUN_TEST("int/plus+space", "%+ d", n);

	RUN_TEST("int/all-flags", "%0-+10.5d", n);
	RUN_TEST("int/all-flags", "%0-+3.5d", n);
	RUN_TEST("int/all-flags", "%0-+1.10d", n);

	RUN_TEST("int/zero-value", "%.0d", n);
	RUN_TEST("int/zero-value-width", "%5.0d", n);

	RUN_TEST("int/negative-width", "%10d", n);
	RUN_TEST("int/negative-zero", "%010d", n);
	RUN_TEST("int/negative-precision", "%.5d", n);
}

void	int_test()
{
	int_test_sub(42);
	int_test_sub(1);
	int_test_sub(-42);
	int_test_sub(-12345678);
	int_test_sub(0);
	int_test_sub(INT_MAX);
	int_test_sub(INT_MIN);
}

void hex_test_sub(unsigned int n)
{
	RUN_TEST("hex/basic", "%x", n);
	RUN_TEST("hex/upper", "%X", n);

	RUN_TEST("hex/width", "%10x", n);
	RUN_TEST("hex/left", "%-10x", n);

	RUN_TEST("hex/precision", "%.5x", n);
	RUN_TEST("hex/width+precision", "%10.5x", n);
	RUN_TEST("hex/left+precision", "%-10.5x", n);

	RUN_TEST("hex/precision", "%.5X", n);
	RUN_TEST("hex/width+precision", "%10.5X", n);
	RUN_TEST("hex/left+precision", "%-10.5X", n);
	
	RUN_TEST("hex/hash", "%#x", n);
	RUN_TEST("hex/hash-upper", "%#X", n);

	RUN_TEST("hex/zero", "%010x", n);
	RUN_TEST("hex/hash+zero", "%#010x", n);

	RUN_TEST("hex/zero", "%010X", n);
	RUN_TEST("hex/hash+zero", "%#010X", n);

	RUN_TEST("hex/plus-ignored", "%+x", n);
	RUN_TEST("hex/space-ignored", "% x", n);

	RUN_TEST("hex/plus-ignored", "%+X", n);
	RUN_TEST("hex/space-ignored", "% X", n);

	RUN_TEST("hex/all-flags", "%0-+# 10.5x", n);
	RUN_TEST("hex/all-flags", "%0-+# 3.5x", n);
	RUN_TEST("hex/all-flags", "%0-+# 1.10x", n);

	RUN_TEST("hex/all-flags", "%0-+# 10.5X", n);
	RUN_TEST("hex/all-flags", "%0-+# 3.5X", n);
	RUN_TEST("hex/all-flags", "%0-+# 1.10X", n);
}

void	hex_test()
{
	hex_test_sub(42);
	hex_test_sub(0);
	hex_test_sub(0xabcdef);
	hex_test_sub(UINT_MAX);
}

void	uint_test_sub(unsigned int n)
{
	RUN_TEST("uint/basic", "%u", n);

	RUN_TEST("uint/width", "%10u", n);
	RUN_TEST("uint/left", "%-10u", n);

	RUN_TEST("uint/precision", "%.5u", n);
	RUN_TEST("uint/width+precision", "%10.5u", n);
	RUN_TEST("uint/left+precision", "%-10.5u", n);

	RUN_TEST("uint/zero", "%010u", n);
	RUN_TEST("uint/zero+precision", "%010.5u", n);

	RUN_TEST("uint/plus-ignored", "%+u", n);
	RUN_TEST("uint/space-ignored", "% u", n);
	RUN_TEST("uint/hash-ignored", "%#u", n);

	RUN_TEST("uint/all-flags", "%0-+# 10.5u", n);
	RUN_TEST("uint/all-flags", "%0-+# 3.5u", n);
	RUN_TEST("uint/all-flags", "%0-+# 1.10u", n);

	RUN_TEST("uint/zero-value", "%.0u", n);
	RUN_TEST("uint/zero-value-width", "%5.0u", n);
}

void	uint_test()
{
	uint_test_sub(42);
	uint_test_sub(0);
	uint_test_sub(5);
	uint_test_sub(123456789);
	uint_test_sub(UINT_MAX);

}

void	ptr_test_sub(void *p)
{
	RUN_TEST("ptr/basic", "%p", p);

	/* width */
	RUN_TEST("ptr/width", "%20p", p);
	RUN_TEST("ptr/width-small", "%2p", p);

	/* left */
	RUN_TEST("ptr/left", "%-20p", p);
	RUN_TEST("ptr/left-small", "%-2p", p);

	/* zero padding */
	RUN_TEST("ptr/zero", "%020p", p);
	RUN_TEST("ptr/zero-small", "%02p", p);

	/* precision */
	RUN_TEST("ptr/precision", "%.5p", p);
	RUN_TEST("ptr/precision-small", "%.1p", p);
	RUN_TEST("ptr/precision-zero", "%.0p", p);

	/* width + precision */
	RUN_TEST("ptr/width+precision", "%20.5p", p);
	RUN_TEST("ptr/left+precision", "%-20.5p", p);
	RUN_TEST("ptr/zero+precision", "%020.5p", p);

	/* conflicting flags */
	RUN_TEST("ptr/left+zero", "%-020p", p);
	RUN_TEST("ptr/plus-ignored", "%+p", p);
	RUN_TEST("ptr/space-ignored", "% p", p);
	RUN_TEST("ptr/hash-ignored", "%#p", p);

	/* all flags */
	RUN_TEST("ptr/all-flags", "%0-+# 20.5p", p);
	RUN_TEST("ptr/all-flags", "%0-+# 3.5p", p);
	RUN_TEST("ptr/all-flags", "%0-+# 1.10p", p);

	/* extreme width */
	RUN_TEST("ptr/big-width", "%100p", p);
	RUN_TEST("ptr/big-left", "%-100p", p);

	/* extreme precision */
	RUN_TEST("ptr/big-precision", "%.30p", p);
	RUN_TEST("ptr/big-width+precision", "%40.30p", p);
}

void	ptr_test()
{
	int x = 42;
	int y = -42;
	char *s = "Hello";

	ptr_test_sub(&x);
	ptr_test_sub(&y);
	ptr_test_sub(s);

	ptr_test_sub(NULL);

	ptr_test_sub((void *)0);
	ptr_test_sub((void *)1);
	ptr_test_sub((void *)42);

	ptr_test_sub((void *)LONG_MAX);
	ptr_test_sub((void *)ULONG_MAX);

	ptr_test_sub((void *)-1);
}

void	percent_test(void)
{
	RUN_TEST("percent/basic", "%%");

	RUN_TEST("percent/width", "%5%");
	RUN_TEST("percent/left", "%-5%");

	RUN_TEST("percent/zero", "%05%");
	RUN_TEST("percent/left+zero", "%-05%");

	RUN_TEST("percent/precision", "%.5%");
	RUN_TEST("percent/width+precision", "%10.5%");
	RUN_TEST("percent/left+precision", "%-10.5%");

	RUN_TEST("percent/plus-ignored", "%+%");
	RUN_TEST("percent/space-ignored", "% %");
	RUN_TEST("percent/hash-ignored", "%#%");

	RUN_TEST("percent/all-flags", "%0-+# 10.5%");

	RUN_TEST("percent/big-width", "%100%");
	RUN_TEST("percent/big-left", "%-100%");
}

int main(void)
{
	setvbuf(stdout, NULL, _IONBF, 0);

	printf("==== ft_printf char test ====\n");
	char_test();

	printf("==== ft_printf string test ====\n");
	string_test();

	printf("==== ft_printf int test ====\n");
	int_test();

	printf("==== ft_printf hex test ====\n");
	hex_test();

	printf("==== ft_printf uint test ====\n");
	uint_test();

	printf("==== ft_printf pointer test ====\n");
	ptr_test();

	printf("==== ft_printf percent test ====\n");
	percent_test();

	return (0);
}
