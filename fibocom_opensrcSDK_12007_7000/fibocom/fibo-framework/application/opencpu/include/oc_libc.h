#ifndef DD1ACE63_7D2C_4F03_A3BD_854BF6531B55
#define DD1ACE63_7D2C_4F03_A3BD_854BF6531B55

#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <time.h>

int fibo_printf(const char *fmt, ...);
int fibo_printf_text(const char *str);

int fibo_vsnprintf(char *buff,int n, const char *fmt,va_list arg);
int fibo_snprintf(char* buffer, size_t count, const char* format, ...);
int fibo_sprintf(char* buffer, const char* format, ...);
int fibo_vprintf(const char* format, va_list va);
int fibo_vsprintf(char *buff, const char *fmt,va_list arg);
int fibo_sscanf(const char* str, const char* format, ...);
int fibo_vsscanf(const char *buf, const char *format, va_list ap);
double fibo_atof(const char *str);
double fibo_strtod(const char *nptr, char **endptr);
float fibo_strtof(const char *nptr, char **endptr);
char *fibo_strtok(char *str, const char *delim);
struct tm *fibo_gmtime(const time_t *timep);
int fibo_str2int32(const char *str, int32_t *result);
int fibo_str2uint32(const char *str, uint32_t *result);
struct tm *fibo_gmtime_r(const time_t *timep, struct tm *result);
int fibo_gettimeofday(struct timeval *tv, void *tz);
const char *fibo_int64_to_string(int64_t number, char *str);
double fibo_pow(double x, double y);
double fibo_floor (double x);
double fibo_sqrt(double x);
double fibo_sin(double x);
double fibo_asin(double x);
double fibo_cos(double x);
double fibo_acos(double x);
double fibo_atan(double x);
double fibo_atan2(double y, double x);
double fibo_tan(double x);

float fibo_sinf(float x);
float fibo_cosf(float x);
float fibo_tanf(float x);
float fibo_asinf(float x);
float fibo_acosf(float x);
float fibo_atanf(float x);
float fibo_atan2f(float x, float y);
float fibo_sqrtf(float x);
time_t fibo_time(time_t *utc_time);
int fibo_putchar(int character);
int fibo_puts(const char *str);
void *fibo_realloc(void *ptr, size_t size);
void *fibo_calloc(size_t num_elements, size_t size_of_element);
char *fibo_strdup(const char *s);
int fibo_atoi(const char *str);
char *fibo_strchr(const char *str, int c);
char *fibo_strstr(const char *haystack, const char *needle);
struct tm *fibo_localtime_libc_r(const time_t *clock, struct tm *result);
void *fibo_memmove(void *dest, const void *src, size_t n);
time_t fibo_mktime(struct tm *tm);
char *fibo_strptime(const char *__restrict s, const char *__restrict format, struct tm *__restrict t);
void fibo_tesecase_send_urc(const char *text, size_t length);
#endif /* DD1ACE63_7D2C_4F03_A3BD_854BF6531B55 */
