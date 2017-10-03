#include <string.h>

#include <keypadc.h>
#include <tice.h>
#include <graphx.h>

#include "snake.h"

#define DOWN  0
#define LEFT  1
#define RIGHT 2
#define UP    3

#define INTERVAL 32768/10

uint8_t pix_size = 8;
uint8_t grid_size[2];

Snake snake;
bool alive = true;

uint8_t dir = DOWN;
bool moved = false;

void fillScreen(uint8_t index){
	memset(lcd_Ram, index, LCD_SIZE);
}

void draw(uint8_t index, uint8_t color){
	uint8_t w;
	uint8_t h;

	gfx_SetColor(color);

	for(w = 0; w < pix_size; w++){
		for(h = 0; h < pix_size; h++){
			gfx_SetPixel(snake.vector[index][0] * pix_size + w,
						 snake.vector[index][1] * pix_size + h);
		}
	}
}

void move(){
	uint8_t new_loc[2];

	new_loc[0] = snake.vector[snake.size - 1][0];
	new_loc[1] = snake.vector[snake.size - 1][1];

	draw(snake.size - 1, 0);

	if(dir == DOWN){
		new_loc[1]++;
	} else if(dir == LEFT){
		new_loc[0]--;
	} else if(dir == RIGHT){
		new_loc[0]++;
	} else {
		new_loc[1]--;
	}

	if(new_loc[0] >= 0 && new_loc[0] < grid_size[0] &&
	   new_loc[1] >= 0 && new_loc[1] < grid_size[1]){
		snake_push(&snake, new_loc, true);
	} else {
		alive = false;
	}

	draw(0, 1);
}

void moveAuto(){
	if(!moved){
		move();
	} else {
		moved = false;
	}
}

void changeDir(uint8_t new_dir){
	if(dir != new_dir && dir + new_dir != 3){
		dir = new_dir;

		move();
		moved = true;
	}
}

void main(void){
	uint8_t cell[2] = {0, 0};

    os_ClrHome();
	gfx_Begin();

	gfx_palette[0] = gfx_RGBTo1555(0, 0, 0);
	gfx_palette[1] = gfx_RGBTo1555(0, 255, 0);

	grid_size[0] = LCD_WIDTH / pix_size;
	grid_size[1] = LCD_HEIGHT / pix_size;

	snake_init(&snake, cell);

	fillScreen(0);
	draw(0, 1);

	timer_Control = TIMER1_DISABLE;

	timer_1_ReloadValue = timer_1_Counter = INTERVAL;
	timer_Control = TIMER1_ENABLE | TIMER1_32K | TIMER1_0INT | TIMER1_DOWN;

	while(kb_Data[6] != kb_Clear && alive){
		kb_key_t key = kb_Data[7];

		kb_Scan();

		if(key & kb_Down){
			changeDir(DOWN);
		} else if(key & kb_Left){
			changeDir(LEFT);
		} else if(key & kb_Right){
			changeDir(RIGHT);
		} else if(key & kb_Up){
			changeDir(UP);
		}

		if(timer_IntStatus & TIMER1_RELOADED){
			moveAuto();

			timer_IntAcknowledge = TIMER1_RELOADED;
		}
	}

	snake_free(&snake);

	gfx_End();
}
