#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
	uint8_t **vector;
	uint16_t capacity;
	uint16_t size;
} Snake;

void snake_init(Snake *v, uint8_t *element);
void snake_push(Snake *v, uint8_t *element, bool pop);
void snake_free(Snake *v);

#endif
