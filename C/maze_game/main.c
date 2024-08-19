//main.c
#include "labyrinth.h"


int e_w_s_n_2(Maze_IF *__map_IF, unsigned int **_map_c, element_location *ele_p, int dir, int num);
int runing();

//Started a version for multi-junction wayfinding ,e_w_s_n,find_helper,finding_way
/*
find_helper have problem !!!
*/
int e_w_s_n(Maze_IF *__map_IF, unsigned int **_map_c, element_location *ele_p);  
int find_helper(Maze_IF *__map_IF, unsigned int **_map_c, element_location *ele_p,elements *crossroads,elements *crossroad_been_dirs);
int finding_way(Maze_IF *__map_IF, unsigned int **_map_c, element_location *ele_p,elements *crossroads,elements *crossroad_been_dirs);



int main(){
    return runing();
}
int e_w_s_n_2(Maze_IF *__map_IF, unsigned int **_map_c, element_location *ele_p, int dir, int num){
    if(MAP[ele_p->Row][ele_p->Column] == 'E'){
        printf("succuessfuly reach the destination ! ! !.\n");
        return 1;
    }
    if(num >= 64){
        printf("Can't find 'E' the destination.\n");
        return 0;
    }
    switch(dir){
        case 0: // East
            if(east(__map_IF, _map_c, ele_p)){
                if(e_w_s_n_2(__map_IF, _map_c, ele_p, 0, num + 1)) {
                    return 1;
                }
            }
            break;
        case 1: // West
            if(west(__map_IF, _map_c, ele_p)){
                if(e_w_s_n_2(__map_IF, _map_c, ele_p, 1, num + 1)) {
                    return 1;
                }
            }
            break;
        case 2: // South
            if(south(__map_IF, _map_c, ele_p)){
                if(e_w_s_n_2(__map_IF, _map_c, ele_p, 2, num + 1)) {
                    return 1;
                }
            }
            break;
        case 3: // North
            if(north(__map_IF, _map_c, ele_p)){
                if(e_w_s_n_2(__map_IF, _map_c, ele_p, 3, num + 1)) {
                    return 1;
                }
            }
            break;
    }
    return e_w_s_n_2(__map_IF, _map_c, ele_p, (dir + 1) % 4, num + 1);
}

int runing(){
    //initialization
    Maze_IF *__map_IF = Read_Map_IF();
    MAP = save_map_data();
    unsigned int **_map_c = MAP_deeply_copy(__map_IF);
    element_location *ele_p = map_way_start(__map_IF); //Returns the starting position of 'S'

    unsigned int start_row = ele_p->Row;
    unsigned int start_column = ele_p->Column;
    show_MAP(__map_IF,MAP);
    printf("The 'S' in the Row: %u ,Column: %u .\n",ele_p->Row,ele_p->Column);
    show_player(__map_IF,MAP,start_column,start_row,"P"); //Shows where 'P' starts


    e_w_s_n_2(__map_IF,_map_c,ele_p,0,0);  //east,west,south,north
    
    
    
    elements *crossroads = (elements *)malloc(sizeof(elements *));  //Used to store multiple intersections
    crossroads->element_locations = NULL;
    crossroads->counts = 0;
    elements *crossroad_been_dirs = (elements *)malloc(sizeof(elements *));  //Used to store a certain direction in all multiple intersections
    crossroad_been_dirs->element_locations = NULL;
    crossroad_been_dirs->counts = 0;

    //finding_way(__map_IF,_map_c,ele_p,crossroads,crossroad_been_dirs);    

    free_find_elements_in_MAP(crossroad_been_dirs);
    free_find_elements_in_MAP(crossroads);

    free_map(__map_IF,MAP);
    free(ele_p);
    free(__map_IF); 
    return 0;
}



int e_w_s_n(Maze_IF *__map_IF, unsigned int **_map_c, element_location *ele_p){ 
    if(east(__map_IF,_map_c,ele_p)) return 1;
    if(west(__map_IF,_map_c,ele_p)) return 1;
    if(south(__map_IF,_map_c,ele_p)) return 1;
    if(north(__map_IF,_map_c,ele_p)) return 1;
    return 0;
}

int find_helper(Maze_IF *__map_IF, unsigned int **_map_c, element_location *ele_p,elements *crossroads,elements *crossroad_been_dirs){
    elements *crossroad = judge_crossroad(__map_IF,_map_c,ele_p);  ///返回的是当前多路口中是路的方向
    
    if(crossroad != NULL){  //当前位置是多路口
        int crossroad_again = 0;        
        //判断该多路口是不是走过
        if(crossroads->counts != 0) {//在存储的多路口中寻找
            for(int i=0;i<crossroads->counts;i++){
                if(ele_p == (element_location *)crossroads->element_locations[i]){//当前位置在存储的多路口中
                    crossroad_again = 1;
                }
            }            
        }
        else if(crossroads->counts == 0 || !crossroad_again){
            crossroads->counts++;
            // 假设 crossroads->counts 是 size_t 类型
            crossroads->element_locations = (element_location **)realloc(crossroads->element_locations, crossroads->counts * sizeof(element_location *));
            crossroads->element_locations[crossroads->counts-1] = ele_p;  //将当前位置存储走过多路口
        }
        //判断是不是多路口中走过的方向
        if(crossroad_again){   //是多路口中走过的方向
            for(int i=0;i< crossroad_been_dirs->counts;i++){
                element_location *crossroad_been_dir = (element_location *)crossroad_been_dirs->element_locations[i];
                unsigned int been_dir_column = crossroad_been_dir->Column;
                unsigned int been_dir_row = crossroad_been_dir->Row;
                int e_w = (been_dir_column == ele_p->Column);    
                int s_n = (been_dir_row == ele_p->Row);    
                if(e_w){
                    if(been_dir_row == ele_p->Row+1){  //走过的方向，在北方向
                        _map_c[ele_p->Row+1][ele_p->Column] = '*';
                        int have_way = e_w_s_n(__map_IF,_map_c,ele_p);
                        if(!have_way){
                            printf("have no way.");
                            free_find_elements_in_MAP(crossroad);
                            return 0;
                        }
                    }else{ //走过的方向，在南方向
                        _map_c[ele_p->Row-1][ele_p->Column] = '*';
                        int have_way = e_w_s_n(__map_IF,_map_c,ele_p);
                        if(!have_way){
                            printf("have no way.");
                            free_find_elements_in_MAP(crossroad);
                            return 0;
                        }
                    }
                }
                if(s_n){
                    if(been_dir_column == ele_p->Column+1){  //走过的方向，在东方向
                        _map_c[ele_p->Row][ele_p->Column+1] = '*';
                        int have_way = e_w_s_n(__map_IF,_map_c,ele_p);
                        if(!have_way){
                            printf("have no way.");
                            free_find_elements_in_MAP(crossroad);
                            return 0;
                        }
                    }else{ //走过的方向，在西方向
                        _map_c[ele_p->Row][ele_p->Column-1] = '*';
                        int have_way = e_w_s_n(__map_IF,_map_c,ele_p);
                        if(!have_way){
                            printf("have no way.");
                            free_find_elements_in_MAP(crossroad);
                            return 0;
                        }
                    }
                }
            }

        }else{  //是多路口，不是多路口中走过的方向，直接在可移动的方向中选一个
            if(east(__map_IF,_map_c,ele_p)){  //移动成功后S_ele会指向移动后的位置
                crossroad_been_dirs->counts++;
                crossroad_been_dirs->element_locations = (element_location **)realloc(crossroad_been_dirs->element_locations,crossroad_been_dirs->counts * sizeof(elements *));
                crossroad_been_dirs->element_locations[crossroad_been_dirs->counts-1] = ele_p;    //记录多路口中走过的方向    
            }else{
                if(west(__map_IF,_map_c,ele_p)){
                crossroad_been_dirs->counts++;
                crossroad_been_dirs->element_locations = (element_location **)realloc(crossroad_been_dirs->element_locations,crossroad_been_dirs->counts * sizeof(elements *));
                crossroad_been_dirs->element_locations[crossroad_been_dirs->counts-1] = ele_p;
                }else{
                    if(south(__map_IF,_map_c,ele_p)){
                    crossroad_been_dirs->counts++;
                    crossroad_been_dirs->element_locations = (element_location **)realloc(crossroad_been_dirs->element_locations,crossroad_been_dirs->counts * sizeof(elements *));
                    crossroad_been_dirs->element_locations[crossroad_been_dirs->counts-1] = ele_p;
                    }else{
                        if(north(__map_IF,_map_c,ele_p)){
                        crossroad_been_dirs->counts++;
                        crossroad_been_dirs->element_locations = (element_location **)realloc(crossroad_been_dirs->element_locations,crossroad_been_dirs->counts * sizeof(elements *));
                        crossroad_been_dirs->element_locations[crossroad_been_dirs->counts-1] = ele_p;
                        }
                    }
                }
                
            }
        }
    }
    for(int i=0;i<24;i++)printf("-");
    free_find_elements_in_MAP(crossroad);
    return 1;
}

int finding_way(Maze_IF *__map_IF, unsigned int **_map_c, element_location *ele_p,elements *crossroads,elements *crossroad_been_dirs){ 
    if(MAP[ele_p->Row][ele_p->Column] == 'E'){
        printf("Arrive at the destination 'E'. \n");
        return 1;
    } 
    else if(!find_helper(__map_IF,_map_c,ele_p,crossroads,crossroad_been_dirs)){
        printf("the map is not have destination. \n");
        return 0;
    }
    finding_way(__map_IF,_map_c,ele_p,crossroads,crossroad_been_dirs);
}
