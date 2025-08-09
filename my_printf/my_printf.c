#include "my_printf.h"

static void out_str(putchar_fn out, void *arg, const char *s)
{
    while (*s) {
        out(*s++, arg);
    }
}

static void utoa_base(unsigned long val, char *buf, int base, int uppercase)
{
    char tmp[32];
    int i              = 0;
    const char *digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";

    if (val == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }

    while (val) {
        tmp[i++] = digits[val % base];
        val /= base;
    }

    for (int j = 0; j < i; j++) {
        buf[j] = tmp[i - 1 - j];
    }
    buf[i] = '\0';
}

static void itoa_base(long val, char *buf, int base, int uppercase)
{
    if (val < 0) {
        *buf++ = '-';
        utoa_base((unsigned long)(-val), buf, base, uppercase);
    } else {
        utoa_base((unsigned long)val, buf, base, uppercase);
    }
}

// 极简 ftoa
static void ftoa_min(double num, char *buf, int decimals)
{
    if (num < 0) {
        *buf++ = '-';
        num    = -num;
    }

    long int_part = (long)num;
    double frac   = num - (double)int_part;

    itoa_base(int_part, buf, 10, 0);
    while (*buf) buf++; // 移到末尾

    if (decimals > 0) {
        *buf++ = '.';
        for (int i = 0; i < decimals; i++) {
            frac *= 10;
        }
        utoa_base((unsigned long)(frac + 0.5), buf, 10, 0);
    }
}

void my_vprintf_core(putchar_fn out, void *arg, const char *fmt, va_list args)
{
    char buf[64];
    while (*fmt) {
        if (*fmt != '%') {
            out(*fmt++, arg);
            continue;
        }
        fmt++; // 跳过 '%'

        int decimals = 6;
        if (*fmt == '.') {
            fmt++;
            decimals = *fmt - '0';
            fmt++;
        }

        switch (*fmt) {
            case 'd':
                itoa_base(va_arg(args, int), buf, 10, 0);
                out_str(out, arg, buf);
                break;
            case 'u':
                utoa_base(va_arg(args, unsigned int), buf, 10, 0);
                out_str(out, arg, buf);
                break;
            case 'ld':
                itoa_base(va_arg(args, long), buf, 10, 0);
                out_str(out, arg, buf);
                break;
            case 'lu':
                utoa_base(va_arg(args, unsigned long), buf, 10, 0);
                out_str(out, arg, buf);
                break;
            case 'x':
                utoa_base(va_arg(args, unsigned int), buf, 16, 0);
                out_str(out, arg, buf);
                break;
            case 'X':
                utoa_base(va_arg(args, unsigned int), buf, 16, 1);
                out_str(out, arg, buf);
                break;
            case 'p':
                out_str(out, arg, "0x");
                utoa_base((uintptr_t)va_arg(args, void *), buf, 16, 0);
                out_str(out, arg, buf);
                break;
            case 'c':
                out((char)va_arg(args, int), arg);
                break;
            case 's':
                out_str(out, arg, va_arg(args, char *));
                break;
            case 'f':
                ftoa_min(va_arg(args, double), buf, decimals);
                out_str(out, arg, buf);
                break;
            case '%':
                out('%', arg);
                break;
            default:
                out('%', arg);
                out(*fmt, arg);
                break;
        }
        fmt++;
    }
}

// ================= 接口自定义 =================
__attribute__((weak)) void uart_putchar(char ch)
{
    // 默认什么都不做
    (void)ch;
}
// ================= 接口自定义 =================

static void my_uart_putchar(char ch, void *arg)
{
    (void)arg;
    uart_putchar(ch);
}

void my_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    my_vprintf_core(my_uart_putchar, NULL, fmt, args);
    va_end(args);
}

static void buf_putchar(char ch, void *arg)
{
    char **p = (char **)arg;
    *(*p)++  = ch;
}

void my_sprintf(char *buf, const char *fmt, ...)
{
    char *p = buf;
    va_list args;
    va_start(args, fmt);
    my_vprintf_core(buf_putchar, &p, fmt, args);
    va_end(args);
    *p = '\0';
}
