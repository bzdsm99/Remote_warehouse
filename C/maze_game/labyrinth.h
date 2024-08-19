//labyrinth.h
#ifndef LABYRINRH
#define LABYRINRH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define TIME_SPEED (500UL)

const char *MAP_PATH = "F:\\C\\test\\maze_game\\maze_map.txt";
unsigned int **MAP;
typedef struct {
    unsigned int Length;
    unsigned int Width;
} Maze_IF;

// store player information
typedef struct {
    unsigned int Column;  //Y
    unsigned int Row;   //X
} element_location;

typedef struct {
    element_location **element_locations;
    int counts;
} elements;
//obtain_maze_information.c
Maze_IF *Read_Map_IF(); //Get the infromation of the map
unsigned int ** save_map_data();
void free_map(Maze_IF *map_size,unsigned int **map); //Release the MAP with "Maze IF *"

//find_element.c
elements *find_element_in_MAP(Maze_IF *map_size,unsigned int **map, char ele);
void free_find_elements_in_MAP(elements *eles);
void show_MAP(Maze_IF *map_size,unsigned int **map);
unsigned int **MAP_deeply_copy(Maze_IF *map_size); //Deep copy MAP
element_location *map_way_start(Maze_IF *map_size);

//player_poeration.c
int show_player(Maze_IF *map_size, unsigned int **_map_, unsigned int column, unsigned int row,const char *dir);
int ewns_wall(Maze_IF *map_size, unsigned int **_map_c, element_location *orgin_start);
int east(Maze_IF *map_size, unsigned int **_map_c, element_location *p_start);
int west(Maze_IF *map_size, unsigned int **_map_c, element_location *p_start);
int south(Maze_IF *map_size, unsigned int **_map_c, element_location *p_start);
int north(Maze_IF *map_size, unsigned int **_map_c, element_location *p_start);
elements *judge_crossroad(Maze_IF *map_size, unsigned int **_map_c, element_location *_p_loc);

// int way_finding(Maze_IF *map_size);
// static int find_path(Maze_IF *map_size, unsigned int **_map_c, element_location *p_start);
// static int south(Maze_IF *map_size, unsigned int **_map_c, element_location *p_start);

#ifndef OBAIN_MAZE_INFORMATION
#define OBAIN_MAZE_INFORMATION
#include "obtain_maze_information.c"
#endif

#ifndef FIND_ELEMENT
#define FIND_ELEMENT
#include "find_element.c"
#endif

#ifndef PLAYER_POERATION
#define PLAYER_POERATION
#include "player_poeration.c"
#endif




#endif