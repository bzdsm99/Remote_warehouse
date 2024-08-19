//find_element.c
#include "labyrinth.h"
//Don't forget free the memory allocated for result and its element_locations
elements *find_element_in_MAP(Maze_IF *map_size,unsigned int **map, char ele){
    int coulmn_row_counts = 0;
    element_location **P_locations = NULL;
    for (unsigned int i = 0; i < map_size->Width; i++) {
        for (unsigned int j = 0; j < map_size->Length; j++) {
            if (map[i][j] == ele) {   //j:X
                if (P_locations == NULL) {
                    P_locations = (element_location **)malloc(sizeof(element_location *));
                    if (P_locations == NULL) {
                        printf("Memory allocation failed for P_locations.\n");
                        return NULL;
                    }
                } else {
                    P_locations = (element_location **)realloc(P_locations, (coulmn_row_counts + 1) * sizeof(element_location *));
                    if (P_locations == NULL) {
                        printf("Memory reallocation failed for P_locations.\n");
                        return NULL;
                    }
                }
                P_locations[coulmn_row_counts] = (element_location *)malloc(sizeof(element_location));
                if (P_locations[coulmn_row_counts] == NULL) {
                    printf("Memory allocation failed for P_locations[%d].\n", coulmn_row_counts);
                    return NULL;
                }
                P_locations[coulmn_row_counts]->Column = j;  //X
                P_locations[coulmn_row_counts]->Row = i;     //Y
                coulmn_row_counts++;
            }
        }
    }
    if (coulmn_row_counts < 1) {
        printf("The element not in map\n");
        return NULL;
    }
    elements *eles = (elements *)malloc(sizeof(elements));
    if (eles == NULL) {
        printf("Memory allocation failed for elements.\n");
        return NULL;
    }
    eles->counts = coulmn_row_counts;
    eles->element_locations = P_locations;
    return eles;
}

void free_find_elements_in_MAP(elements *eles){
    for (int i = 0; i < eles->counts; i++) {
        //printf("i:%d counts:%u .\n",i,eles->counts);
        if (eles->element_locations != NULL && i < eles->counts && i>=0) {
            free(eles->element_locations[i]);
        }
    }
    free(eles->element_locations);
    free(eles);
}

void show_MAP(Maze_IF *map_size, unsigned int **map) {
    system("cls");
    int W = 6;
    for(int i=0;i<W*2;i++)    printf("\n");

    for (unsigned int i = 0; i < map_size->Width; i++) {
        for(int i=0;i<W;i++)    printf("\t");
        for (unsigned int j = 0; j < map_size->Length; j++) {
            printf("%c", (char)map[i][j]); // Convert unsigned int to char
        }
        printf("\t\t i: %d \n", i);
    }
    printf("\n");
    Sleep(TIME_SPEED); //800ms
    system("cls");
}

unsigned int **MAP_deeply_copy(Maze_IF *map_size) {
    unsigned int **_map_cope = (unsigned int **)malloc(map_size->Width * sizeof(unsigned int *));
    if (_map_cope == NULL) {
        printf("Memory allocation failed for _map_cope.\n");
        return NULL;
    }
    for (unsigned int i = 0; i < map_size->Width; i++) {
        _map_cope[i] = (unsigned int *)malloc(map_size->Length * sizeof(unsigned int));
        if (_map_cope[i] == NULL) {
            printf("Memory allocation failed for _map_cope[%d].\n", i);
            for (int j = 0; j < i; j++) {
                free(_map_cope[j]);
            }
            free(_map_cope);
            return NULL;
        }
    }
    for (unsigned int i = 0; i < map_size->Width; i++) {
        for (unsigned int j = 0; j < map_size->Length; j++) {
            _map_cope[i][j] = MAP[i][j];
        }
    }
    return _map_cope;
}


//Don't forget free.
element_location *map_way_start(Maze_IF *map_size){
    elements *start_eles = find_element_in_MAP(map_size,MAP,'S');
    if (start_eles->counts != 1){
        printf("The map is error,there can be one \"start\".");
        return NULL;
    }
    element_location *p_start = (start_eles->element_locations)[0];
    return p_start;
}