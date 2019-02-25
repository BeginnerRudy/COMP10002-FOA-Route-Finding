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
typedef struct {
    loc_t street_loc;
    int east;
    int north;
    int west;
    int south;
} intersection_t;/*The data about one intersection*/
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

int mygetchar(); /*a function provided by the professor*/
void read_size(dimension_t*); /*return the size of the block*/
intersection_t*  read_intersection(dimension_t size);
loc_supplied_t* read_loc_supplied(int *);
void stage1_data(data_stage1_t*, int, dimension_t,
    intersection_t*, loc_supplied_t*, int D);
void print_stage1(data_stage1_t);
void free_intersections(intersection_t*, dimension_t);

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