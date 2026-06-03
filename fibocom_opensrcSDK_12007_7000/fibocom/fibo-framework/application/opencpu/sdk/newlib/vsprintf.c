#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "oc_libc.h"
#include "fibo_libc.h"

#include "fibo_opencpu_api.h"

int __wrap_vsnprintf(char *buf, size_t size, const char *fmt, va_list ap)
{
    return fibo_vsnprintf(buf, size, fmt, ap);
}

int __wrap_vsprintf(char *buf, const char *fmt, va_list ap)
{
    return fibo_vsprintf(buf, fmt, ap);
}

int __wrap_sprintf(char *buf, const char *fmt, ...)
{
    va_list ap;
    int len;

    va_start(ap, fmt);
    len = vsprintf(buf, fmt, ap);
    va_end(ap);
    return (len);
}

int __wrap_snprintf(char *buf, size_t size, const char *fmt, ...)
{
    va_list ap;
    int len;

    va_start(ap, fmt);
    len = fibo_vsnprintf(buf, size, fmt, ap);
    va_end(ap);
    return (len);
}
int __wrap_sscanf(const char *str, const char *fmt, ...)
{
    va_list ap;
    int len;

    va_start(ap, fmt);
    len = fibo_vsscanf(str, fmt, ap);
    va_end(ap);
    return (len);
}

int __wrap_vsscanf(const char *str, const char *format, va_list ap)
{
    return fibo_vsscanf(str, format, ap);
}

char *__wrap_strtok(char *str, const char *delim)
{
    return fibo_strtok(str, delim);
}

void *__wrap_malloc(size_t size)
{
    return fibo_malloc(size);
}

void __wrap_free(void *p)
{
    fibo_free(p);
}

void __wrap_srand(unsigned int seed)
{
    return;
}

int __wrap_rand(void)
{
    int t;
    fibo_rng_generate(&t, sizeof(t));
    if (t < 0)
    {
        t = 0 - t;
    }
    return t;
}

struct tm *__wrap_gmtime(const time_t *timep)
{
    return fibo_gmtime(timep);
}

struct tm *__wrap_gmtime_r(const time_t *timep, struct tm *result)
{
    return fibo_gmtime_r(timep, result);
}

double __wrap_atof(const char *nptr)
{
    return fibo_atof(nptr);
}

double __wrap_strtod(const char *nptr, char **endptr)
{
    return fibo_strtod(nptr, endptr);
}

float __wrap_strtof(const char *nptr, char **endptr)
{
    return fibo_strtof(nptr, endptr);
}

int __wrap_gettimeofday(struct timeval *tv, void *tz)
{
    return fibo_gettimeofday(tv, tz);
}

int fibo_parse_version_size(const char *content, char *version, int version_size, uint32_t *download_size)
{
    int pos = 0;
    const char *start = strstr(content, "version:");
    if (start == NULL)
    {
        return -1;
    }
    start += (sizeof("version:") - 1);

    while (start[0] == ' ')
    {
        start++;
    }

    while (start[pos] != ' ' && pos < version_size)
    {
        version[pos] = start[pos];
        pos++;
    }
    start += pos;
    pos = 0;

    start = strstr(start, "downloaded_size:");
    if (start == NULL)
    {
        return -1;
    }
    start += (sizeof("downloaded_size:") - 1);

    while (start[0] == ' ')
    {
        start++;
    }

    *download_size = 0;
    while (start[pos] != ' ' && start[pos] != 0)
    {
        if (start[pos] < '0' && start[pos] > '9')
        {
            *download_size = 0;
            return -1;
        }

        *download_size = (*download_size) * 10 + (start[pos] - '0');
        pos++;
    }

    return 0;
}

double __wrap_pow(double x, double y)
{
    return fibo_pow(x,y);
}

double __wrap_floor (double x)
{
    return fibo_floor(x);
}

double __wrap_sqrt(double x)
{
    return fibo_sqrt(x);
}

double __wrap_sin(double x)
{
    return fibo_sin(x);
}

double __wrap_asin(double x)
{
    return fibo_asin(x);
}

double __wrap_cos(double x)
{
    return fibo_cos(x);
}

double __wrap_acos(double x)
{
    return fibo_acos(x);
}

double __wrap_atan(double x)
{
    return fibo_atan(x);
}

double __wrap_atan2(double y, double x)
{
    return fibo_atan2(y, x);
}

double __wrap_tan(double x)
{
    return fibo_tan(x);
}

float __wrap_sinf(float x)
{
    return fibo_sinf(x);
}

float __wrap_cosf(float x)
{
    return fibo_cosf(x);
}

float __wrap_tanf(float x)
{
    return fibo_tanf(x);
}

float __wrap_asinf(float x)
{
    return fibo_asinf(x);
}

float __wrap_acosf(float x)
{
    return fibo_acosf(x);
}

float __wrap_atanf(float x)
{
    return fibo_atanf(x);
}

float __wrap_sqrtf(float x)
{
    return fibo_sqrtf(x);
}
float __wrap_atan2f(float x, float y)
{
    return fibo_atan2f(x, y);
}

time_t __wrap_time(time_t *utc_time)
{
    return fibo_time(utc_time);
}
int __wrap_putchar(int character)
{
    return fibo_putchar(character);
}
int __wrap_puts(const char *str)
{
    return fibo_puts(str);
}

char *__wrap_strptime(const char *__restrict s, const char *__restrict format, struct tm *__restrict t)
{
    return fibo_strptime(s, format, t);
}
void *__wrap_calloc(size_t num_elements, size_t size_of_element)
{
    return fibo_calloc(num_elements, size_of_element);
}
char *__wrap_strdup(const char *s)
{
    return fibo_strdup(s);
}
