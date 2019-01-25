#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ROOMS 10
#define MIN_ROOMS 1
#define MIN_SIZE 4
#define MAX_SIZE 10
#define MAP_WIDTH 80
#define MAP_HEIGHT 21

typedef struct Room
{
	int x_pos, y_pos, x_size, y_size;
} Room;


int print_map(unsigned char map[MAP_HEIGHT][MAP_WIDTH]);

int generate_room(Room * room, unsigned int random_num, unsigned int attempts){
	room->x_pos = random_num % ((MAP_WIDTH - MAX_SIZE - attempts) ? (MAP_WIDTH - MAX_SIZE - attempts) : 1);
	room->y_pos = random_num % ((MAP_HEIGHT - MAX_SIZE - attempts) ? (MAP_HEIGHT - MAX_SIZE - attempts) : 1);
	if(room->x_pos == 0) room->x_pos = 1;
	if(room->y_pos == 0) room->y_pos = 1;
	if(room->x_pos < 0) room->x_pos = room->x_pos * -1;
	if(room->y_pos < 0) room->y_pos = room->y_pos * -1;
	room->x_pos = room->x_pos % (MAP_WIDTH - MAX_SIZE);
	room->y_pos = room->y_pos % (MAP_HEIGHT - MAX_SIZE);
	room->x_size = (random_num - room->x_pos - attempts) % (MAX_SIZE - MIN_SIZE) + MIN_SIZE;
	room->y_size = (random_num - room->y_pos -  attempts) % (MAX_SIZE - MIN_SIZE) + MIN_SIZE;
	return 0;
}

int check_room(Room room, unsigned char map[MAP_HEIGHT][MAP_WIDTH]){	
	int x_counter, y_counter;
	for(y_counter = -1; y_counter <= room.y_size; y_counter++){
		for(x_counter = -1; x_counter <= room.x_size; x_counter++){
			//If the room spot is not open
			if(map[y_counter + room.y_pos][x_counter + room.x_pos] != ' '){ 
				return 1;
			}
		}
	}
	return 0;
}


int add_rooms(int random_num, unsigned char map[MAP_HEIGHT][MAP_WIDTH], Room room_list[], unsigned int num_rooms){
	unsigned int generated_rooms;
	unsigned int individual_attempts, total_attempts;
	individual_attempts = 1;
	total_attempts = 1;
	Room room_holder;
	for(generated_rooms = 0; generated_rooms < num_rooms; generated_rooms++){
		individual_attempts = 1;
		generate_room(&room_holder, random_num, total_attempts % individual_attempts);
		total_attempts++;
		while(check_room(room_holder, map)){
			generate_room(&room_holder, random_num, total_attempts % individual_attempts);
			total_attempts++;
			individual_attempts++;
			if(individual_attempts > 10000){
				printf("Can't place room\n");
				random_num = random_num*2654435761 % 4294967295;
				individual_attempts = 1;
			}	
		}
		//Add in the room to the map
		room_list[generated_rooms] = room_holder;
		printf("%d %d %d %d %d\n", room_holder.x_pos, room_holder.y_pos, room_holder.x_size, room_holder.y_size, individual_attempts);	
		unsigned int x_incr, y_incr;
		for(x_incr = 0; x_incr < room_holder.x_size; x_incr++){
			for(y_incr = 0; y_incr < room_holder.y_size; y_incr++){
				map[y_incr + room_holder.y_pos][x_incr + room_holder.x_pos] = '.';
			}
		}	
	}
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
	unsigned int RAND_SEED =  time(NULL);
	if(argc > 1) sscanf(argv[1], "%d", &RAND_SEED);
	srand(RAND_SEED);
	unsigned int number_rooms = MIN_ROOMS + rand() % (MAX_ROOMS - MIN_ROOMS + 1);
	unsigned char map[MAP_HEIGHT][MAP_WIDTH];
	Room room_list[number_rooms];

	printf("ROOMS: %d\n", number_rooms);
	printf("Random seed: %d\n", RAND_SEED);
	create_map_base(map);
	add_rooms(rand(), map, room_list, number_rooms);
	print_map(map);
	return 0;
}