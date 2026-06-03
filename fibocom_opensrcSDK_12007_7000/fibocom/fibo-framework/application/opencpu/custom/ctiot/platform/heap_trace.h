
#ifndef __HEAP_TRACE_H__
#define __HEAP_TRACE_H__


#ifdef HEAP_TRACE 

#define TRACE_VARIABLE_PARAMS

void print_trace(void);

#ifdef  TRACE_VARIABLE_PARAMS
void *malloc_trace(size_t size, ...);
void free_trace(void *p, ...);
char *strdup_trace(const char *str, ...);
#else
void *malloc_trace(size_t size, const char *file, const char *function, size_t lineno);
void free_trace(void *p, const char *file, const char *function, size_t lineno);
char *strdup_trace(const char *str, const char *file, const char *function, size_t lineno);
#endif

#endif  //HEAP_TRACE 

#endif

