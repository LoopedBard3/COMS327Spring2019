#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ROOMS 1
#define MIN_SIZE 4
#define MAP_WIDTH 80
#define MAP_HEIGHT 21

typedef struct Room
{
	char x_pos, y_pos, x_size, y_size;
} Room;


int add_new_room(int random_num, unsigned char map[MAP_HEIGHT][MAP_WIDTH]){


	return 1;
}

int create_map_base(unsigned char map[MAP_HEIGHT][MAP_WIDTH]){
	unsigned char iteratorX, iteratorY;
	for(iteratorY = 0; iteratorY < MAP_HEIGHT; iteratorY++){
		for(iteratorX = 0; iteratorX < MAP_WIDTH; iteratorX++){
			map[iteratorY][iteratorX] = ' ';
			if(iteratorX == 0 || iteratorX == MAP_WIDTH - 1) map[iteratorY][iteratorX] = '|';
			if(iteratorY == 0 || iteratorY == MAP_HEIGHT - 1) map[iteratorY][iteratorX] = '-';
		}
	}
	return 1;
}


int main(int argc, char *argv[]){
	unsigned char map[MAP_HEIGHT][MAP_WIDTH];
	create_map_base(map);
	srand(time(NULL));
	int RAND_NUM = rand();
	printf("Random seed: %d\n", RAND_NUM); 


	unsigned char column, row;
	for(row = 0; row < MAP_HEIGHT; row++){
		for(column = 0; column < MAP_WIDTH; column++){
			printf("%c", map[row][column]);
		}	
		printf("\n");
	}
}
