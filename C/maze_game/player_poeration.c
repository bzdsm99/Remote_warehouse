//player_poeration.c
#include "labyrinth.h"

int show_player(Maze_IF *map_size, unsigned int **_map_, unsigned int column, unsigned int row,const char *dir) {
    unsigned int **temp_map = MAP_deeply_copy(map_size);
    if (temp_map == NULL) {
        return 0;
    }
    elements *wall_eles = find_element_in_MAP(map_size, _map_, '*'); // In the passed map, find the '*' element
    int wall_bool = 1; // When true, not a wall.
    for (int i = 0; i < (wall_eles->counts); i++) {
        element_location *p_wall = wall_eles->element_locations[i];
        if ((p_wall->Row == row) && (p_wall->Column == column)) {
            printf("'*' Location : Column=%u,Row=%u,\n", p_wall->Column, p_wall->Row);
            wall_bool = 0;
        }
    }
    if (wall_bool && column < map_size->Length && row < map_size->Width) {
        temp_map[row][column] = 'P';
        show_MAP(map_size, temp_map);
    } else {
        printf("%s There is wall, can't move.\n",dir);
    }
    free_find_elements_in_MAP(wall_eles);
    free_map(map_size, temp_map);
    return wall_bool;
}

int ewns_wall(Maze_IF *map_size, unsigned int **_map_c, element_location *orgin_start){
    int column = (int)orgin_start->Column;  //X
    int row = (int)orgin_start->Row;    //Y
    char e,w,n,s;
    
    // printf("column: %d, row: %d, Length: %d, Width: %d\n", column, row, map_size->Length, map_size->Width);
    // East
    if (column+1 >= 0 && column+1 < (int)map_size->Length && row >= 0 && row < (int)map_size->Width) {
        e = (char)_map_c[row][column+1];
    } else {
        e = '*';
    }
    // West
    if (column-1 >= 0 && column-1<= (int)map_size->Length && row >= 0 && row < (int)map_size->Width) {
        w = (char)_map_c[row][column-1];
    } else {
        w = '*';
    }
    // North
    if (row+1 >= 0 && row+1 < (int)map_size->Width && column >= 0 && column < (int)map_size->Length) {
        n = (char)_map_c[row+1][column];
    } else {
        n = '*';
    }
    // South
    if (row - 1 >= 0 && row - 1 < (int)map_size->Width && column >= 0 && column < (int)map_size->Length) {
        s = (char)_map_c[row-1][column];
    } else {
        s = '*';
    }
    if ((e == '*' && w == '*' && n == '*') ||
        (e == '*' && w == '*' && s == '*') ||
        (e == '*' && n == '*' && s == '*') ||
        (w == '*' && n == '*' && s == '*')){
        return 0;
    }
    return 1;
}


/*尝试向东走一步,如果该方向为路,则移动传入的指针位置,移动成功打印移动后的画面,
并且判断上一步的位置是不是有三个方向是墙,如果是则将_map_c中的该位置改为墙,下次不再走.
*/
//东
int east(Maze_IF *map_size, unsigned int **_map_c, element_location *p_start) {
    
    unsigned int p_row = p_start->Row;
    unsigned int p_column = p_start->Column;
    int wall_way = ewns_wall(map_size,_map_c,p_start);
    
    if (p_column + 1 >= map_size->Length) {
        printf("%d map_size->Length :%d.\n",p_column + 1,map_size->Length);
        printf("south boundary reached, can't move.\n");
        return 0;
    }
    p_column++;

    if(!wall_way){
        _map_c[p_row][p_column-1] = '*';
    }
    if (show_player(map_size, _map_c, p_column, p_row,"East")) {
        p_start->Column = p_column;
        return 1;
    }
    return 0;
}
//西
int west(Maze_IF *map_size, unsigned int **_map_c, element_location *p_start) {
    unsigned int p_row = p_start->Row;
    unsigned int p_column = p_start->Column;
    int wall_way = ewns_wall(map_size,_map_c,p_start);
    
    if ((int)p_column - 1 < 0) {
        printf("west boundary reached, can't move.\n");
        return 0;
    }
    p_column--;

    if(!wall_way){
        _map_c[p_row][p_column+1] = '*';
    }
    if (show_player(map_size, _map_c, p_column, p_row,"West")) {
        p_start->Column = p_column;
        return 1;
    }
    return 0;
}

//南
int south(Maze_IF *map_size, unsigned int **_map_c, element_location *p_start) {
    unsigned int p_row = p_start->Row;
    unsigned int p_column = p_start->Column;
    int wall_way = ewns_wall(map_size,_map_c,p_start);
    
    if (p_row + 1 >= map_size->Width) {
        printf("%d map_size->Width :%d.\n",p_row + 1,map_size->Width);
        printf("south boundary reached, can't move.\n");
        return 0;
    }
    p_row++;

    if(!wall_way){
        _map_c[p_row-1][p_column] = '*';
    }
    if (show_player(map_size, _map_c, p_column, p_row,"South")) {
        p_start->Row = p_row;
        return 1;
    }
    return 0;
}

//北
int north(Maze_IF *map_size, unsigned int **_map_c, element_location *p_start) {
    unsigned int p_row = p_start->Row;
    unsigned int p_column = p_start->Column;
    int wall_way = ewns_wall(map_size,_map_c,p_start);
    
    if ((int)p_row - 1 < 0) {
        printf("north boundary reached, can't move.\n");
        return 0;
    }
    p_row--;

    if(!wall_way){
        _map_c[p_row+1][p_column] = '*';
    }
    if (show_player(map_size, _map_c, p_column, p_row,"North")) {
        p_start->Row = p_row;
        return 1;
    }
    return 0;
}

//记得调用free_find_elements_in_MAP释放elements *类型的堆区
elements *judge_crossroad(Maze_IF *map_size, unsigned int **_map_c, element_location *_p_loc){
    //判断是不是十字路口(包括三字),如果是返回四个方向中是路的结构体 elements *
    unsigned int column = _p_loc->Column;
    unsigned int row = _p_loc->Row;
    char e = '*',w = '*';
    char n = '*',s = '*';
    // East
    if (column+1 >= 0 && column+1 < (int)map_size->Length && row >= 0 && row < (int)map_size->Width) {
        e = (char)_map_c[row][column+1];
    }
    // West
    if (column-1 >= 0 && column-1<= (int)map_size->Length && row >= 0 && row < (int)map_size->Width) {
        w = (char)_map_c[row][column-1];
    }
    // North
    if (row+1 >= 0 && row+1 < (int)map_size->Width && column >= 0 && column < (int)map_size->Length) {
        n = (char)_map_c[row+1][column];
    }
    // South
    if (row - 1 >= 0 && row - 1 < (int)map_size->Width && column >= 0 && column < (int)map_size->Length) {
        s = (char)_map_c[row-1][column];
    }
    if ((e == ' ' && w == ' ' && n == ' ') ||
        (e == ' ' && w == ' ' && s == ' ') ||
        (e == ' ' && n == ' ' && s == ' ') ||
        (w == ' ' && n == ' ' && s == ' ') ||
        (e == ' ' && w == ' ' && n == ' ' && s == ' ')){
    
        elements *crossroad_all_directons = (elements *)malloc(sizeof(elements *));
        crossroad_all_directons->element_locations = (element_location **)malloc(4 * sizeof(element_location *));
        
        crossroad_all_directons->counts = 0;
        unsigned int  dirs[4][2];
        dirs[0][0]  = column+1; dirs[0][1] = row;
        dirs[1][0]  = column-1; dirs[1][1] = row; 
        dirs[2][0]  = column; dirs[2][1] = row+1; 
        dirs[3][0]  = column; dirs[3][1] = row-1;

        for(int i=0;i<4;i++){
            element_location *dir = (element_location *)malloc(sizeof(element_location));
            dir->Column = dirs[i][0];
            dir->Row = dirs[i][1];
            if ((int)dir->Row >= 0 && dir->Row < map_size->Width &&
                (int)dir->Column >= 0 && dir->Column < map_size->Length)
            {
                if (_map_c[dir->Row][dir->Column] == ' ') {
                    crossroad_all_directons->element_locations[crossroad_all_directons->counts] = dir;
                    crossroad_all_directons->counts += 1;  // 这里等于 4 或 3
                } else {
                    free(dir);  // 释放未使用的内存
                }
            } else {
                free(dir);  // 释放无效位置的内存
            }
        }
        return crossroad_all_directons;
    } else {
        return NULL;
    }
}
