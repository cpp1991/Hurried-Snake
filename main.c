#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <memory.h>

#define FIELD_DIM 30
#define MAX_SEGMENTS 30

void getKeyPress();
void renderScreen();
void setup_game();
void refill_game_field();
void check_collisions();
void move_snake();
void end_game();
void add_segment();

typedef enum Direction
{
NORTH,
SOUTH,
EAST,
WEST
} Direction;

typedef struct Segment
{
    int x;
    int y;
    Direction direction;
} Segment;

typedef struct Snake
{
    Segment snake_head;
    Segment segments[MAX_SEGMENTS];
    int segment_count;
    Direction direction;
} Snake;

// 0 = empty, 1 = fruit, 2 = snake body part, 3 = snake head
int game_field[FIELD_DIM][FIELD_DIM];
Segment fruit;
Snake snake;
int is_game_over = 0;
time_t seconds_since_start;
int score = 0;

int main()
{
// initialize game context
setup_game();

renderScreen();

// game loop
while(is_game_over == 0) {
    // clear screen
    system("cls");

    // print heading
    printf("\n\n\n   Collect the fruits!     time left / s: %d      score: %d",
     100 - (time(NULL) - seconds_since_start),score);

    // clear game_field and fill new
    refill_game_field();

    // render game field
    renderScreen();

    // check collisions
    check_collisions();

    // check for key presses
    getKeyPress();

    // update positions
    move_snake();
}

// show end dialogue until up is pressed
end_game();

return 0;
}

void end_game()
{
    system("cls");
    printf("\n\n\n     You collected %d fruit(s). Press up to quit!", score);
    while(1) {
        char key = 127;

        key = _getch();

        // get special key
        if (key == 0 || key == -32) {
            key = _getch();

		// up, down, left or right
		if (key == 72) {
                break;
        }

	}
    }
}

void setup_game()
{
    // set default values for snake and fruit
    snake.segment_count = 0;
    snake.snake_head.x = 20;
    snake.snake_head.y = 20;
    fruit.x = 2;
    fruit.y = 2;

    // initial moving direction
    snake.direction = EAST;

    // empty game field
    int i, j;
    for(i = 0; i < FIELD_DIM; ++i){
        for(j = 0; j < FIELD_DIM; ++j) {
            game_field[i][j] = 0;
        }
    }

    game_field[snake.snake_head.x][snake.snake_head.y] = 3;
    game_field[fruit.x][fruit.y] = 1;

    // set initial time
    seconds_since_start = time(NULL);

    // setup random number generator
    srand(time(NULL));
}

void check_collisions()
{
    // no time left
    if(time(NULL) - seconds_since_start >= 100) {
        is_game_over = 1;
        return;
    }

    int pos_x = snake.snake_head.x;
    int pos_y = snake.snake_head.y;

    // stepped over game field borders
    if(pos_x < 0 || pos_y < 0 || pos_x >= FIELD_DIM || pos_y >= FIELD_DIM) {
        is_game_over = 1;
        return;
    }
    // head collided with other body part
    int i;
    for(i = 0; i < snake.segment_count; ++i) {
        if(pos_x == snake.segments[i].x && pos_y == snake.segments[i].y) {
                is_game_over = 1;
                return;
        }
    }

    // collided with fruit
    if(pos_x == fruit.x && pos_y == fruit.y) {
        // increase score and replace fruit
        score++;
        fruit.x = (rand() % 20) + 5;
        fruit.y = (rand() % 20) + 5;

        // add segment to snake
        snake.segment_count++;
        add_segment();
    }
}

void add_segment()
{
    // append segment to head if first segment
    if(snake.segment_count == 1) {
        snake.segments[0].direction = snake.direction;
        switch(snake.direction) {
        case NORTH : snake.segments[snake.segment_count - 1].x = snake.snake_head.x; snake.segments[snake.segment_count - 1].y = snake.snake_head.y + 1; break;
        case SOUTH : snake.segments[snake.segment_count - 1].x = snake.snake_head.x; snake.segments[snake.segment_count - 1].y = snake.snake_head.y - 1; break;
        case EAST : snake.segments[snake.segment_count - 1].x = snake.snake_head.x - 1; snake.segments[snake.segment_count - 1].y = snake.snake_head.y; break;
        case WEST : snake.segments[snake.segment_count - 1].x = snake.snake_head.x + 1; snake.segments[snake.segment_count - 1].y = snake.snake_head.y; break;
        }
        }
        else {
        snake.segments[snake.segment_count - 1].direction = snake.segments[snake.segment_count - 2].direction;
        switch(snake.segments[snake.segment_count - 2].direction) {
        case NORTH : snake.segments[snake.segment_count - 1].x = snake.segments[snake.segment_count - 2].x; snake.segments[snake.segment_count - 1].y = snake.segments[snake.segment_count - 2].y + 1; break;
        case SOUTH : snake.segments[snake.segment_count - 1].x = snake.segments[snake.segment_count - 2].x; snake.segments[snake.segment_count - 1].y = snake.segments[snake.segment_count - 2].y - 1; break;
        case EAST : snake.segments[snake.segment_count - 1].x = snake.segments[snake.segment_count - 2].x - 1; snake.segments[snake.segment_count - 1].y = snake.segments[snake.segment_count - 2].y; break;
        case WEST : snake.segments[snake.segment_count - 1].x = snake.segments[snake.segment_count - 2].x + 1 ; snake.segments[snake.segment_count - 1].y = snake.segments[snake.segment_count - 2].y; break;
        }
}
}

void move_snake()
{
    // move head
    switch(snake.direction)
    {
        case NORTH : snake.snake_head.y -= 1; break;
        case SOUTH : snake.snake_head.y += 1; break;
        case EAST : snake.snake_head.x += 1; break;
        case WEST : snake.snake_head.x -= 1; break;
    }

    // move body parts
    int i;
    for(i = 0; i < snake.segment_count; ++i) {
        switch(snake.segments[i].direction)
        {
        case NORTH : snake.segments[i].y -= 1; break;
        case SOUTH : snake.segments[i].y += 1; break;
        case EAST : snake.segments[i].x += 1; break;
        case WEST : snake.segments[i].x -= 1; break;
        }
    }
// change directions for next iteration
        for(i = snake.segment_count - 1; i >= 0; --i) {
        if(i == 0) {
            snake.segments[i].direction = snake.direction;
        } else {
            snake.segments[i].direction = snake.segments[i - 1].direction;
        }
    }
}

void refill_game_field()
{
    // make each field empty
    int i, j;
    for(i = 0; i < FIELD_DIM; ++i) {
        for(j = 0; j < FIELD_DIM; ++j) {
            game_field[j][i] = 0;
        }
    }

    // fill in fruit
    game_field[fruit.y][fruit.x] = 1;

    // fill in snake head
    game_field[snake.snake_head.y][snake.snake_head.x] = 3;

    // fill in snake body parts
    for(i = 0; i < snake.segment_count; ++i) {
        game_field[snake.segments[i].y][snake.segments[i].x] = 2;
    }

}

void renderScreen()
{
    int i, j;

    printf("\n\n");
    printf("   O O O O O O O O O O O O O O O O O O O O O O O O O O O O O O O \n");
    for(i = 0; i < FIELD_DIM; ++i) {
        printf("   O");
        for(j = 0; j < FIELD_DIM; ++j) {
            switch(game_field[i][j]) {
                case 0 : printf(" "); break;
                case 1 : printf("9"); break;
                case 2 : printf("X"); break;
                case 3 : printf("8"); break;
            }
        printf(" ");
        }
        printf("O\n");
    }
    printf("   O O O O O O O O O O O O O O O O O O O O O O O O O O O O O O O \n");
}

// check which key is pressed in a non-blocking manner
void getKeyPress()
{
	char key = 127;

	key = _getch();

	// is something pressed?
	int something_pressed = 0;

	// get special key
	if (key == 0 || key == -32) {
		key = _getch();

		// up, down, left or right
		if (key == 72) {
			key = 'u';
			something_pressed = 1;
		} else if (key == 75) {
			key = 'l';
			something_pressed = 1;
		} else if (key == 77) {
			key = 'r';
			something_pressed = 1;
		} else if (key == 80) {
			key = 'd';
			something_pressed = 1;
		}
	}

	// set snake's direction
	if(something_pressed == 1) {
	switch(key)
	{
	    case 'u' : snake.direction = NORTH; break;
	    case 'd' : snake.direction = SOUTH; break;
	    case 'l' : snake.direction = WEST; break;
	    case 'r' : snake.direction = EAST; break;
	}
}
}
