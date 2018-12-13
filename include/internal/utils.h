#ifndef TYM_INTERNAL_UTILS_H
#define TYM_INTERNAL_UTILS_H

#include <stddef.h>

void* tym_i_copy(size_t s, void* init);

#define TYM_UNPACK(...) __VA_ARGS__
#define TYM_ZALLOC(T,I) ((T*)tym_i_copy(sizeof(T),0))
#define TYM_COPY(I) tym_i_copy(sizeof(I),&I)

#endif
