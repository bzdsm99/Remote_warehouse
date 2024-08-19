//obtain_maze_information.c
#include "labyrinth.h"


//Make the file pointer point to the previous map character '\n'
static void seek_to_map(FILE *map){
    fseek(map,0,SEEK_SET);
    int ch = fgetc(map);
    while(ch != '-')ch = fgetc(map);
    while(ch == '-')ch = fgetc(map); 
}

Maze_IF *Read_Map_IF(){
    FILE *map_R = fopen(MAP_PATH, "r");
    if (map_R == NULL) {
        printf("The path is problematic.\n");
        return NULL;
    }
    seek_to_map(map_R);
    Maze_IF *map = (Maze_IF *)malloc(sizeof(Maze_IF));
    int ch = fgetc(map_R);
    unsigned int maxlen=0,len=0;
    unsigned int width = 0;
    int this_map = 0;
    while((ch = fgetc(map_R)) != EOF) {
        if(ch == '*' || ch=='E' || ch=='S' || ch==' '||ch=='\n'){
            if(ch != '\n'){
                len++;
                this_map = 1;
            } else if(ch == '\n' && this_map) {
                width++;
                if(maxlen < len){
                    maxlen = len;
                }
                len = 0;
                this_map = 0;
            }
        }else{
            ch == EOF;//While not is map element,we think map is end.
        } 
    }
    map->Length = maxlen;
    map->Width = width;
    fclose(map_R);
    return map;
}


unsigned int **save_map_data() {
    Maze_IF *size = Read_Map_IF();
    unsigned int **map = (unsigned int **)malloc(size->Width * sizeof(unsigned int *));
    for (int i = 0; i < size->Width; i++) {
        map[i] = (unsigned int *)malloc(size->Length * sizeof(unsigned int));
    }
    printf("Width: %d Length: %d \n", size->Width, size->Length);

    FILE *ptr_map = fopen(MAP_PATH, "r");
    if (ptr_map == NULL) {
        printf("The path is problematic.\n");
        return NULL;
    } 
    seek_to_map(ptr_map);
    int ch;
    int row = 0, column = 0;
    do {
        ch = fgetc(ptr_map);
        switch (ch) {
            case '*':
                map[row][column] = (unsigned int)'*';
                if ((column + 1) <= size->Length) column++; 
                break;
            case ' ':
                map[row][column] = (unsigned int)' ';
                if ((column + 1) <= size->Length) column++; 
                break;
            case 'E':
                map[row][column] = (unsigned int)'E';
                if ((column + 1) <= size->Length) column++; 
                break;
            case 'S':
                map[row][column] = (unsigned int)'S';
                if ((column + 1) <= size->Length) column++; // 列增加
                break;
            case '\n':
                if ((row + 1) <= size->Width) {
                    row++;
                    column = 0;
                }
                break;
        }
    } while (ch != EOF);
    fclose(ptr_map);
    return map;
}

void free_map(Maze_IF *map_size,unsigned int **map){
    for (int i = 0; i < map_size->Width; i++) {
        free(map[i]);
    }
    free(map);
}
