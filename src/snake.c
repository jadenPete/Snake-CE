#include "snake.h"

size_t cell_size = sizeof(uint8_t) * 2;

void snake_init(Snake *s, uint8_t *element){
	s->vector = (uint8_t**) malloc(2 * cell_size);
	s->vector[0] = element;
	s->capacity = 2;
	s->size = 1;
}

void snake_push(Snake *s, uint8_t *element, bool pop){
	uint8_t i;

	s->vector[s->size] = element;

	if(!pop){
		s->capacity++;
		s->size++;

		s->vector = (uint8_t**) realloc(s->vector, s->capacity * cell_size);
	}

	for(i = s->capacity - 1; i > 0; i--){
		uint8_t *tmp = s->vector[i];

		s->vector[i] = s->vector[i - 1];
		s->vector[i - 1] = tmp;
	}
}

void snake_free(Snake *s){
	free(s->vector);
}
