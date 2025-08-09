#ifndef __MY_PRINTF_H
#define __MY_PRINTF_H

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

typedef void (*putchar_fn)(char ch, void *arg); // 输出回调类型

void my_vprintf_core(putchar_fn out, void *arg, const char *fmt, va_list args);
void my_printf(const char *fmt, ...);             // 直接输出
void my_sprintf(char *buf, const char *fmt, ...); // 输出到字符串


#endif /* __MY_PRINTF_H */
