#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ROOMS 1
#define MIN_ROOMS 1
#define MIN_SIZE 4
#define MAP_WIDTH 80
#define MAP_HEIGHT 21

typedef struct Room
{
	char x_pos, y_pos, x_size, y_size;
} Room;


int print_map(unsigned char map[MAP_HEIGHT][MAP_WIDTH]);

int generate_room(Room room, unsigned int random_num, unsigned int attempts){
	printf("x_pos: %d\n", random_num %  attempts);
	room.x_pos = random_num % (MAP_WIDTH - MIN_SIZE - attempts);
	room.y_pos = random_num % (MAP_HEIGHT - MIN_SIZE - attempts);
	room.x_size = (random_num + attempts * attempts) % (MAP_WIDTH - room.x_pos - MIN_SIZE) + MIN_SIZE;
	room.y_size = (random_num + attempts * attempts) % (MAP_HEIGHT - room.y_pos - MIN_SIZE) + MIN_SIZE;
	return 0;
}

int check_room(Room room, unsigned char map[MAP_HEIGHT][MAP_WIDTH]){
	
	return 0;
}


int add_rooms(unsigned int random_num, unsigned char map[MAP_HEIGHT][MAP_WIDTH], Room room_list[], unsigned char num_rooms){
	unsigned char generated_rooms;
	unsigned int individual_attempts, total_attempts;
	individual_attempts = 0;
	total_attempts = 0;
	Room room_holder;
	for(generated_rooms = 0; generated_rooms < num_rooms; generated_rooms++){
		individual_attempts = 0;
		generate_room(room_holder, random_num, total_attempts % individual_attempts);
		total_attempts++;
		while(check_room(room_holder, map)){
			generate_room(room_holder, random_num, total_attempts % individual_attempts);
			total_attempts++;
			individual_attempts++;	
			printf("Generating Room\n");
		}
		//Add in the room to the map
		room_list[generated_rooms] = room_holder;
		printf("%d %d %d %d\n", room_holder.x_pos, room_holder.y_pos, room_holder.x_size, room_holder.y_size);	
		unsigned int x_incr, y_incr;
		for(x_incr = 0; x_incr < room_holder.x_size; x_incr++){
			for(y_incr = 0; y_incr < room_holder.y_size; y_incr++){
				map[y_incr + room_holder.y_pos][x_incr + room_holder.x_pos] = '.';
				printf("Add . at: %d %d\n", x_incr + room_holder.x_pos, y_incr + room_holder.y_pos);
			}
		}	
	}
	print_map(map);
	return 0;
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
	return 0;
}

int print_map(unsigned char map[MAP_HEIGHT][MAP_WIDTH]){
	unsigned char column, row;
	for(row = 0; row < MAP_HEIGHT; row++){
		for(column = 0; column < MAP_WIDTH; column++){
			printf("%c", map[row][column]);
		}	
		printf("\n");
	}
	return 0;
}


int main(int argc, char *argv[]){
	srand(time(NULL));	
	unsigned int RAND_NUM = rand();
	if(argc > 1) sscanf(argv[1], "%d", &RAND_NUM);
	unsigned char number_rooms = MIN_ROOMS + RAND_NUM % (MAX_ROOMS - MIN_ROOMS + 1);
	unsigned char map[MAP_HEIGHT][MAP_WIDTH];
	Room room_list[number_rooms];
	printf("ROOMS: %d\n", number_rooms);

	create_map_base(map);
	add_rooms(RAND_NUM, map, room_list, number_rooms);
	printf("Random seed: %d\n", RAND_NUM);
	print_map(map);
	return 0;
}
