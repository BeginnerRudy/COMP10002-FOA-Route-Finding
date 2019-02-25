#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define DEBUG 1
#if DEBUG
#define DUMP_DBL(x) printf("line %d: %s = %.5f\n", __LINE__, #x, x)
#else
#define DUMP_DBL(x)
#endif
//DUMP_DBL(score);

#define ONEWAY 999 /*One way streets have cost of 999*/
typedef struct {
    int street_num_x;
    int street_num_y;
} dimension_t; /*The size of the block*/
typedef struct {
    int street_x;
    char street_y;
}loc_t;
typedef struct intersection intersection_t;
struct intersection{
    loc_t street_loc;
    int east;
    int north;
    int west;
    int south;
    intersection_t *d_east;
    intersection_t *d_north;
    intersection_t *d_west;
    intersection_t *d_south;
};/*The data about one intersection*/
typedef struct {
    loc_t loc_supplied;
} loc_supplied_t;/*Location supplied*/
typedef struct {
    dimension_t size;
    int nintersections;
    int possibilities;
    int useless;
    int total_cost;
    int nloc_supplied;
    loc_t first;
    loc_t last;
} data_stage1_t;/*data for stage 1*/
typedef struct {
    loc_t loc;
    int cost;
} loc_cost_t;
typedef struct {
    loc_cost_t *locs;
    int total_cost;
    int num_routes;
}route_t;
typedef struct {
    loc_t location;
    loc_t from;
    int total_cost;
}map_t


int
is_repeat(loc_t* A, loc_t key, int k){
    int i;
    for (i=0;i<=k,i++) {
        if (A[i].street_x == key.street_x 
            && A[i].street_y == key.street_y){
            return 1;
        }
    }
    return 0;
}

int
is_arrived(loc_t now, loc_t end){
    if (now.street_x == end.street_x &&
        now.street_x == end.street_x){
            return 1;
    }
    return 0;
}

void
assign_loc(loc_t *now, loc_t *reference){
    now->street_x = reference->street_x;
    now->street_y = reference->street_y;
}

int
check(intersection_t* A, loc_t* B, int c, loc_t now, loc_t end){
    if (A!=NULL && !is_repeat(B, now, c) && !is_arrived(now,end)){
        return 1;
    }
    return 0;
}
int
char_to_int(char c) {
    int n;
    n = c - 'a';
    return n;
}


map_t *
create_a_new_map(dimension_t size){
    map_t *map;
    int num_locs,i,k=0,x,y,current_size=1;
    char j;
    x = size.street_num_x;
    y = size.street_num_y;
    num_locs = x * y;
    map = (map_t *) malloc(sizeof(map));
    for (j='a';(j-'a')<y) {
        for (i=0;i<x;i++){
            if (k == current_size){
                current_size *= 2;
                map = (map_t *) realloc(map, 
                    current_size * sizeof(*map));
            }
            map[k].location.street_x = i;
            map[k].location.street_y = j;
            map[k].total_cost = ONEWAY;
            k++; 
        }
    }
    return map;
}






route_t*
search_best_route(loc_t start, loc_t end,
                     intersection_t*A, int limit){
    intersection_t *start1, *end1;
    static route_t* array_of_routes;
    static int current1=1, nroutes=0, i=0, k=0;
    array_of_routes = (route_t*) malloc (sizeof(route_t));
    start1 = search_intersection(start, A, limit);

    if (i == current1) {
        current1*=2;
        array_of_routes = (route_t*) realloc(array_of_routes, 
            current1*sizeof(    array_of_routes));
    }

    if ('check') {
       array_of_routes[i].allcost+=start1->d_east->east;
       assign_loc(&(start1->d_east->street_loc), start1);
       nroutes++;
       current1++;


    //west

    //north


    //south
       return search_best_route(start1->d_east->street_loc, end, A, limit);
    }



    
}




int mygetchar(); /*a function provided by the professor*/
void read_size(dimension_t*); /*return the size of the block*/
intersection_t*  read_intersection(dimension_t size);
loc_supplied_t* read_loc_supplied(int *);
void stage1_data(data_stage1_t*, int, dimension_t,
    intersection_t*, loc_supplied_t*, int D);
void print_stage1(data_stage1_t);
intersection_t* search_intersection(loc_t key,
    intersection_t* A, int limit);
loc_t f_key_east(loc_t current);
loc_t f_key_west(loc_t current);
loc_t f_key_north(loc_t current);
loc_t f_key_south(loc_t current);
void link_two_intersections(int cost, intersection_t** direction, 
    loc_t current,loc_t key, int limit, intersection_t* A);
void link_all_intersections(intersection_t* A, int limit);

int
main(int argc, char* argv[]) {
    dimension_t size;
    intersection_t *cross;
    loc_supplied_t *loc_supplied;
    data_stage1_t data_stage1;
    int num_loc_supplied, num_intersections;

    /*stage 1*/
    read_size(&size); 
    cross = read_intersection(size);
    num_intersections = size.street_num_x * size.street_num_y;
    loc_supplied = read_loc_supplied(&num_loc_supplied);
    stage1_data(&data_stage1, num_intersections, size, cross, loc_supplied, num_loc_supplied);
    print_stage1(data_stage1);

    /*stage 2*/
    link_all_intersections(cross, num_intersections);



    free(cross);
    cross = NULL;
    free(loc_supplied);
    loc_supplied = NULL;
    return 0;
}

int
mygetchar() {
    int c;
    while ((c=getchar())=='\r') {
    }
    return c;
}

void 
read_size(dimension_t* size) {
    int x, y;
    scanf("%d%d", &x, &y);
    size->street_num_x = x;
    size->street_num_y = y;
    return;
}

intersection_t* 
read_intersection(dimension_t size){
    int x, east, north, west, south, limit, i=0, current_size=1, numelements=0;
    char y;
    intersection_t* A;
    A = (intersection_t *) malloc(sizeof(*A));
    limit = size.street_num_x * size.street_num_y;
    for (i=0;i<limit;i++){
        if (numelements == current_size) {
                current_size *= 2;
            A = realloc(A, current_size * sizeof(*A));
            //exit_if_null(A, "reallocation");
        }
        scanf("%d%c%d%d%d%d", &x, &y, &east, &north, &west, &south);
        A[i].street_loc.street_x = x;
        A[i].street_loc.street_y = y;
        A[i].east  = east;
        A[i].north = north;
        A[i].west  = west;
        A[i].south = south;
        numelements++;
    }
    return A;
}

loc_supplied_t*
read_loc_supplied(int *num_supplied) {
    int x, current_size=1, i=0;
    char y;
    loc_supplied_t* A;
    A = (loc_supplied_t *) malloc(sizeof(*A));
    while (scanf("%d %c", &x, &y) == 2) {
        if (i == current_size) {
            current_size *= 2;
            A = realloc(A, current_size * sizeof(*A));
            //exit_if_null(A, "reallocation");
        }
        A[i].loc_supplied.street_x = x;
        A[i].loc_supplied.street_y = y;
        i++;
    }
    *num_supplied = i;
    return A;
}

void 
stage1_data(data_stage1_t* A, int limit, dimension_t size,
    intersection_t* B, loc_supplied_t* C, int num_loc_supplied){
    int numoneway=0, allcost=0,  i,
        E, N, W, S;
    for (i=0;i<limit;i++){
        E = B[i].east;
        N = B[i].north;
        W = B[i].west;
        S = B[i].south;
        if (E == ONEWAY) {
            numoneway++;
        } else {
            allcost += E;
        }
        if (N == ONEWAY) {
            numoneway++;
        } else {
            allcost += N;
        }
        if (W == ONEWAY) {
            numoneway++;
        } else {
            allcost += W;
        }
        if (S == ONEWAY) {
            numoneway++;
        } else {
            allcost += S;
        }
    }


    A->size.street_num_x = size.street_num_x;
    A->size.street_num_y = size.street_num_y;
    A->nintersections = A->size.street_num_x 
                        * A->size.street_num_y;
    A->possibilities = A->nintersections * 4;
    A->useless = numoneway;
    A->total_cost = allcost;
    A->nloc_supplied = num_loc_supplied;
    if (C != NULL) {
        A->first = C[0].loc_supplied;
        A->last = C[num_loc_supplied-1].loc_supplied;
    } else {
        exit(EXIT_FAILURE);
    }
    return;
}

void print_stage1(data_stage1_t A) {
    printf("S1: grid is %d x %d, and has %d intersections\n",
    A.size.street_num_x, A.size.street_num_y, A.nintersections);
    printf("S1: of %d possibilities, %d of them cannot be used\n",
    A.possibilities, A.useless);
    printf("S1: total cost of remaining possibilities is %d seconds\n",
    A.total_cost);
    printf("S1: %d grid locations supplied, first one is %d%c, last one is %d%c\n",
    A.nloc_supplied, A.first.street_x, A.first.street_y,
    A.last.street_x, A.last.street_y);
}

intersection_t*
search_intersection(loc_t key,intersection_t* A, int limit){
    int i;

    for (i=0;i<limit;i++) {
        if ( A[i].street_loc.street_x == key.street_x
            &&A[i].street_loc.street_y == key.street_y ){
            return (A+i);
        }
    }
    return NULL;
}

loc_t
f_key_east(loc_t current) {
    int n;
    loc_t east=current;
    n=current.street_x;
    east.street_x = n+1;
    return east;
}

loc_t
f_key_west(loc_t current) {
    int n;
    loc_t west=current;
    n=current.street_x;
    west.street_x = n-1;
    return west;
}

loc_t
f_key_north(loc_t current) {
    char c;
    loc_t north=current;
    c = current.street_y;
    north.street_y = c - 1;
    return north;
}

loc_t
f_key_south(loc_t current) {
    char c;
    loc_t south=current;
    c = current.street_y;
    south.street_y = c + 1;
    return south;
}

void
link_two_intersections(int cost, intersection_t** direction, 
    loc_t current,loc_t key, int limit, intersection_t* A){
    if (cost != ONEWAY) {
        *direction = search_intersection(key,A,limit);
    } else {
        *direction = NULL;
    }
    return;
}


void
link_all_intersections(intersection_t* A, int limit) {
    int i;
    loc_t key_east, key_west, key_north, key_south;
    for (i=0;i<limit;i++) {
        key_east = f_key_east(A[i].street_loc);
        key_west = f_key_west(A[i].street_loc);
        key_north = f_key_north(A[i].street_loc);
        key_south = f_key_south(A[i].street_loc);
        link_two_intersections(A[i].east, &(A[i].d_east), 
            A[i].street_loc, key_east, limit, A);
        link_two_intersections(A[i].west, &(A[i].d_west), 
            A[i].street_loc, key_west, limit, A);
        link_two_intersections(A[i].north, &(A[i].d_north),
            A[i].street_loc, key_north, limit, A);
        link_two_intersections(A[i].south, &(A[i].d_south), 
            A[i].street_loc, key_south, limit, A);
    }
    return;
}
