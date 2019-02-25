/****************************************************************************
 * Copyright (C) 2017 by Calvin Zhuoqun Huang                               *
 *                                                                          *
 * This file is all of assignment 2 file                                    *
 *                                                                          *
 *   The following part is SERIOUS SERIOUS SERIOUS Brief of programme:      *
 *                                                                          *
 *   ass2.c is part of the modern 2D city taxi planning software (or is it) *
 *   The program is created as part of PROGRAM assignment 2(/classified)    *
 *   Observation of any other country obtaining this secret program should  *
 *   be reported to command center immediatly, since this violating part of *
 *   HUMAN-ALIEN-COMMON-AGREEEEEEMENTS in alien word 'plagiarism aggrement' *
 *   signed by comrade Huang and alien leader StarAli `F` Mofft             *
 *   in 2017 peace conference held near pluto orbit                         *
 *                                                                          *
 *   You shouldn't have received any copy of the GNU Lesser General Public  *
 *   License along with ass2.c.   If you do, see                            *
 *   <hppt://mmm.nolicence.edu/absolutely_no_licenses/tricked>.             *
 ****************************************************************************/

/**
 * @file ass2.c
 * @author Calvin Zhuoqun Huang 908525
 * @date 01 Oct 2017
 * @brief: All malloc functions' parameter is listed in detail in the comment
 *         explaining struct.(size in bracket).
 */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>



// Common value
#define TRUE 1
#define FALSE 0

// Number dimension and number direction
#define NUM_DIM 2
#define NUM_DIR NUM_DIM * 2

// #define MAX_DIG_TIME 4 abandoned since concat strings is not breautiful

// macro convert between char and ints
#define INT_TO_ALPHA(x) (x + 'a')
#define ALPHA_TO_INT(c) (c - 'a')

// prompts for 3 stages and newline
#define STAGE1_PROMPT printf("S1: ")
#define STAGE2_PROMPT printf("S2: ")
// omitting one space for convenience of formatting for stage 3
#define STAGE3_PROMPT printf("S3:")
#define NEWLINE printf("\n")

// Initial car number, will be realloced if needed
#define DEFAULT_CAR_NUM 5

// Sentinel value
#define NOPASS 999

// Formatting value
#define DIR_LEFT " <<<<"
#define DIR_RIGHT " >>>>"
#define DIR_UP "   ^"
#define DIR_DOWN "   v"
#define DIR_NONE "     "


/*
'|'      indicates element of a struct
'-'      indicates sub(base)-type of the super type
'*'      separates elements of the same level
'\t'     indicates the next level struct/type

Overview of Struct
-town_t                                     #all info of that town or isle
 |int ncol                                  #Store column number
 *
 |int nrow                                  #Store row number
 *
 |int time_total                            #Store all time of passable path
 *
 |int ninvalid                              #Store non-passable path
 *
 |int ncar                                  #Store car or input number
 *
 |all_cross_t all_cross[nrow]               #2-D array of 1-D array 
                                                #of all grid pass time
    -row_cross_t[ncol]                      #1-D array of 1-D array
                                                #of single row pass time
        -cross_t                            #1-D array of all time of a cross
            -int[NUM_DIR]                   #same as above
 *
 |all_car_t all_car[ncar]                   #1-D array of car_t
    -car_t                                  #store needed info of one car
        |car_loc_t car_loc[NUM_DIM]         #store location of one car
        *
        |all_time_cross_t all_time_cross[nrow]  #store info of time and
                                                    #position to all cross
            -row_time_cross_t[ncol]             #store info of time and
                                                    #position to row of cross
                -time_cross_t                   #store info of time and
                                                    #position to one cross
                    |int ntime                  #time taken to one cross
                    *
                    |loc_t prev_loc             #the previous location to cross
                        -int[NUM_DIM]
 *
 |all_time_cross_t map_time_cross[nrow]     #store the overall mapping of all
                                                #cars, same as all_time_cross
                                                    #in car_t above
    -row_time_cross_t[ncol]
        -time_cross_t
            |int ntime
            *
            |loc_t prev_loc
                -int[NUM_DIM]
*/

/*detailed struct break up above*/
typedef int cross_t[NUM_DIR];
typedef cross_t *row_cross_t;
typedef row_cross_t *all_cross_t;
typedef int loc_t[NUM_DIM];
typedef loc_t car_loc_t;
typedef struct time_cross_s {
    int ntime;
    loc_t prev_loc;
} time_cross_t;
typedef time_cross_t *row_time_cross_t;
typedef row_time_cross_t *all_time_cross_t;
typedef struct car_s {
    car_loc_t car_loc;
    all_time_cross_t all_time_cross;
} car_t;
typedef car_t *all_car_t;
typedef struct town_s {
    int ncol, nrow, time_total, ninvalid, ncar;
    all_cross_t all_cross;
    all_car_t all_car;
    all_time_cross_t map_time_cross;
} town_t;

/**
 * @brief This function takes initial car number and returns a pointer to an
 *        empty town struct
 *
 * @param default car_t array size
 *
 * @return pointer to the created town
 * 
 */
town_t *create_empty_town(int ini_car);

/**
 * @brief Reads town info from input file and fill it into struct town
 * 
 * @param town that need to be filled
 * @param default car_t array size
 * 
 */
void town_info_reader(town_t *town, int ini_car);


/**
 * @brief Reads dimension from input file to town->nrow, ncol
 * 
 * @param town that need to be filled
 *
 * static because this doesn't interact with user
 * 
 */
static void read_dimension(town_t *town);

/**
 * @brief malloc correct size of space for reading time travelling
 * 
 * @param town that need to be filled
 *
 * static because this doesn't interact with user
 * 
 */
static void alloc_cross_map(town_t *town);

/**
 * @brief read dimensions from file to town
 * 
 * @param town that need to be filled
 * 
 * static because this doesn't interact with user
 * 
 */
static void read_intersection(town_t *town);

/**
 * @brief read from the input file all the car locations
 * 
 * @param town that need to be filled
 * @param number of max cars that can be read by default
 *
 * static because this doesn't interact with user
 * 
 */
static void read_car(town_t *town, int max);

/**
 * @brief prints the info of input data after collected by town_info_reader
 * 
 * @param town that need to be filled
 * 
 */
void town_info_printer(town_t *town);

/**
 * @brief malloc correct space for car of ind position the 
 *        correct storage space
 * 
 * @param the town that the info is needed
 * @param index of the car position needed
 *
 * static because this doesn't interact with user
 * 
 */
static void alloc_init_time_car(town_t *town, int ind);

/**
 * @brief calculate min time of the car to every point in the town
 * 
 * @param the town that the info is needed
 * @param index of the car position needed
 * 
 */
void calculate_time(town_t *town, int ind);

/**
 * @brief descending comparision function that takes two const void argument
 *        and used to determine the lexicographic ordering of two locations
 *        legit tie condition ordering
 * 
 * @param location 1 of loc_t
 * @param location 2 of loc_t
 * 
 * @return positive int if l1 > l2, zero if l1 = l2, else negative int
 *
 * static because this doesn't interact with user
 * 
 */
static int desc_cmp_loc(const void *l1, const void *l2);

/**
 * @brief recursive call part of the previous calculate_time function, that
 *        passes current location x, y and previous loacation prev_x, prev_y,
 *        and cost of time to current location to function. And recursively 
 *        calls 4 different directions.
 * 
 * @param the town that the info is needed
 * @param index of the car position needed
 * @param current point's x-axis position
 * @param current point's y-axis position
 * @param previous point's x-axis position
 * @param previous point's y-axis position
 * @param cost to current location of current moving path
 * 
 * static because this doesn't interact with user
 * 
 */
static void recursive_calculate_time(town_t *town, int ind, int x, int y,
                                            int prev_x, int prev_y, int cost);

/**
 * @brief after car's time map has been calculated, call this function to
 *        return a array of loc_t of reversed order of getting from car of ind
 *        to dest destination
 * 
 * @param array of loc_t, the path is stored in this 
 *       array after functioncall
 * @param the town that the info is needed
 * @param index of the car position needed
 * @param pointer to loc_t of destination location
 * 
 * @return int of the length of array path(number of intermediate points)
 *
 * non-static because this might interact with user(but in this case hasn't)
 * 
 */
int car_to_point(loc_t *path, town_t *town, int ind, loc_t *dest);

/**
 * @brief prints stage 2 output, from car of ind to destination of loc_t dest
 * 
 * @param the town that the info is needed
 * @param index of the car position needed
 * @param pointer to loc_t of destination location
 * 
 */
void print_path(town_t *town, int ind, loc_t *dest);

/**
 * @brief comparing all cars' map and unify them to create a whole map
 *        of shortest path
 * 
 * @param the town that the info is needed
 * 
 */
void integrate_all_car(town_t *town);

/**
 * @brief checking if the integer x, y is same as the given loc_t location
 * 
 * @param the loc that needed to be compared
 * @param x axis of the other location
 * @param y axis of the other location
 * 
 * @return 1 if same location, 0 otherwise
 *
 * static because this doesn't interact with user
 * a bit redundant(less generalised than desc_cmp_loc function), but used to
 * avoid the need to create new loc_t to store location for print_map function
 */
static int check_loc(loc_t *loc, int x, int y);

/**
 * @brief print the optimum car travel map
 * 
 * @param the town that the info is needed
 * 
 */
void print_map(town_t *town);

/**
 * @brief free the malloced space for a single car object, I had this function
 *        due to the fact that stage 2 only calculates one car but not all cars
 *        by using this to avoid freeing unmalloced space.
 * 
 * @param the town that the info is needed
 * @param index of the car position needed
 *
 * static because this doesn't interact with user
 * 
 */
static void free_car(town_t *town, int ind);

/**
 * @brief free memory allocated to the town obaject
 * 
 * @param the town that the info is needed
 * 
 */
void free_town(town_t *town);


/*
 *  Note on main: 
 *  
 *  I've considered further parsing loops and stages into
 *  more abstractive functions. 
 *  However under serious consideration, I decided to leave it as this form. 
 *  For I find no meaning parsing it further.
 *  If this is an industural implementation, this file will only be of one of
 *  the HEAD files. Further parsed function won't be as useful as the current
 *  level abstraction. 
 *  Since they are too specific and cost too much time to
 *  compute in larger scale.
 *  
 */
int
main(int argc, char *argv[]) {
    // initialising town and dummy variable
    int i = 0;
    town_t *town = create_empty_town(DEFAULT_CAR_NUM);

    // Stage 1
    town_info_reader(town, DEFAULT_CAR_NUM);
    town_info_printer(town);
    NEWLINE;

    // Calculate time for all car
    for (i = 0; i < town->ncar; i++) {
        calculate_time(town, i);
    }

    // Stage 2
    for (i = 1; i < town->ncar; i++) {
        print_path(town, 0, &town->all_car[i].car_loc);
    }
    NEWLINE;

    // Stage 3
    integrate_all_car(town);
    print_map(town);
    NEWLINE;

    // free malloced space
    free_town(town);
    return 0;
}



town_t *create_empty_town(int ini_car) {
    town_t *town = (town_t*)malloc(sizeof(*town));
    assert(town != NULL);
    town->all_car= (car_t*)malloc(ini_car * sizeof(car_t));
    assert(town->all_car != NULL);
    // initiating all values
    town->ncol       = 0;
    town->nrow       = 0;
    town->ninvalid   = 0;
    town->time_total = 0;
    town->ncar       = 0;
    return town;
}


void town_info_reader(town_t *town, int ini_car) {
    read_dimension(town);
    alloc_cross_map(town);
    read_intersection(town);
    read_car(town, ini_car);
}


void read_dimension(town_t *town) {
    scanf("%d%d", &town->ncol, &town->nrow);
}



void alloc_cross_map(town_t *town) {
    int y = 0,
        i = 0;
    // mallocing for all cross' time recording array
    town->all_cross = (row_cross_t*)malloc(town->nrow * sizeof(row_cross_t));
    assert(town->all_cross != NULL);
    for (y = 0; y < town->nrow; y++) {
        (town->all_cross)[y] = (cross_t*)malloc(town->ncol * sizeof(cross_t));
        assert((town->all_cross)[y] != NULL);
    }

    // mallocing for final optimised mapping of car-cross
    town->map_time_cross = 
             (row_time_cross_t*)malloc(town->nrow * sizeof(row_time_cross_t));
    assert(town->map_time_cross != NULL);
    for (y = 0; y < town->nrow; y++) {
        town->map_time_cross[y] = 
                    (time_cross_t*)malloc(town->ncol * sizeof(time_cross_t));
        assert(town->map_time_cross[y] != NULL);
        for (i = 0; i < town->ncol; i++) {
            // Setting the initial value of one cross
            town->map_time_cross[y][i].ntime = NOPASS;
            town->map_time_cross[y][i].prev_loc[0] = i;
            town->map_time_cross[y][i].prev_loc[1] = y;
        }
    }
}


void read_intersection(town_t *town) {
    char c_y = '\0';
    int y = -1,
        x = -1,
        i =  0,
        dirs[NUM_DIR];
    while (y + 1 < town->nrow || x + 1 < town->ncol) {
        scanf("%d%c", &x, &c_y);
        y = ALPHA_TO_INT(c_y);
        for (i = 0; i < NUM_DIR; i++) {
            scanf("%d", dirs + i);
            town->all_cross[y][x][i] = dirs[i];
            // is passable added to total time, else increment invalid num
            if (dirs[i] != NOPASS) {
                town->time_total += dirs[i];
            } else {
                town->ninvalid++;
            }
        }
    }
}


void read_car(town_t *town, int max) {
    char c_y = '\0';
    int x   = 0,
        y   = 0;
    while (scanf("%d%c", &x, &c_y) == 2) {
        // reach cap size of the car array
        if (town->ncar + 1 == max) {
            max = max * 2;
            town->all_car = (car_t*)realloc(town->all_car, max*sizeof(car_t));
            assert(town->all_car != NULL);
        }
        y = ALPHA_TO_INT(c_y);
        (town->all_car[town->ncar]).car_loc[0] = x;
        (town->all_car[town->ncar]).car_loc[1] = y;
        // successfully reading one car, increaments number car
        town->ncar++;
    }
}



void town_info_printer(town_t *town) {
    STAGE1_PROMPT;
    printf("grid is %d x %d, and has %d intersections\n",
                            town->ncol, town->nrow, (town->ncol * town->nrow));
    STAGE1_PROMPT;
    printf("of %d possibilities, %d of them cannot be used\n",
                        NUM_DIR * (town->ncol * town->nrow), town->ninvalid);
    STAGE1_PROMPT;
    printf("total cost of remaining possibilities is %d seconds\n",
                                                             town->time_total);
    STAGE1_PROMPT;
    printf("%d grid locations supplied, \
first one is %d%c, last one is %d%c\n",
                                    town->ncar, town->all_car[0].car_loc[0], 
                                    INT_TO_ALPHA(town->all_car[0].car_loc[1]),
                                    town->all_car[town->ncar - 1].car_loc[0], 
                    INT_TO_ALPHA(town->all_car[town->ncar - 1].car_loc[1]));
}


void alloc_init_time_car(town_t *town, int ind) {
    int y = 0,
        i = 0;
    (town->all_car[ind]).all_time_cross = 
        (row_time_cross_t*)malloc(town->nrow * sizeof(row_time_cross_t));
    assert((town->all_car[ind]).all_time_cross != NULL);
    for (y = 0; y < town->nrow; y++) {
        ((town->all_car[ind]).all_time_cross)[y] = 
                    (time_cross_t*)malloc(town->ncol * sizeof(time_cross_t));
        assert(((town->all_car[ind]).all_time_cross)[y] != NULL);
        for (i = 0; i < town->ncol; i++) {
            // initialling individual value to NOPASS sentinel. i.e.999
            ((town->all_car[ind]).all_time_cross)[y][i].ntime = NOPASS;
            ((town->all_car[ind]).all_time_cross)[y][i].prev_loc[0] = i;
            ((town->all_car[ind]).all_time_cross)[y][i].prev_loc[1] = y;
        }
    }
}



void calculate_time(town_t *town, int ind) {
    int loc_x = town->all_car[ind].car_loc[0],
        loc_y = town->all_car[ind].car_loc[1];
    alloc_init_time_car(town, ind);
    // calling the recursive function with cost being zero
    recursive_calculate_time(town, ind, loc_x, loc_y, loc_x, loc_y, 0);
}


int desc_cmp_loc(const void *l1, const void *l2) {
    loc_t *loc1 = (loc_t*)l1,
          *loc2 = (loc_t*)l2;
    // first location is prioritised over the second position
    if (*loc1[0] > *loc2[0]) {
        return 1;
    } else if (*loc1[0] < *loc2[0]) {
        return -1;
    } else {
        return *loc1[1] - *loc2[1];
    }
}


static void recursive_calculate_time(town_t *town, int ind, int x, int y,
                                        int prev_x, int prev_y, int cost) {
    loc_t prev_loc = {prev_x, prev_y};
    int swap = FALSE;
    // if hasn't reaches edges, navigate this option
    if (x >= 0 && y >= 0 && x < town->ncol && y < town->nrow &&
                    town->all_car[ind].all_time_cross[y][x].ntime > cost) {
        swap = TRUE;
    } else if (x >= 0 && y >= 0 && x < town->ncol && y < town->nrow &&
              town->all_car[ind].all_time_cross[y][x].ntime==cost&&cost!= 999
              && desc_cmp_loc(town->all_car[ind].all_time_cross[y][x].prev_loc,
                              prev_loc) > 0){
        // in the tie case, check two different previous location,
        // if the later one was smaller, insert the later one.
        swap = TRUE;
    }
    // if the argument path is valid(both strictly faster or tie and smaller)
    if (swap) {
                town->all_car[ind].all_time_cross[y][x].ntime = cost;
        town->all_car[ind].all_time_cross[y][x].prev_loc[0] = prev_x;
        town->all_car[ind].all_time_cross[y][x].prev_loc[1] = prev_y;
        recursive_calculate_time(town, ind, x - 1, y, x, y,
                                        cost + town->all_cross[y][x][2]);           
        recursive_calculate_time(town, ind, x + 1, y, x, y,
                                        cost + town->all_cross[y][x][0]);
        recursive_calculate_time(town, ind, x, y - 1, x, y,
                                        cost + town->all_cross[y][x][1]);           
        recursive_calculate_time(town, ind, x, y + 1, x, y,
                                        cost + town->all_cross[y][x][3]);
    }
}


int car_to_point(loc_t *path ,town_t *town, int ind, loc_t *dest) {
    assert(town->all_car[ind].all_time_cross != NULL);
        // current cross x-coordinate, y-coordingate
    int cur_x  = town->all_car[ind].all_time_cross[(*dest)[1]][(*dest)[0]]
                                                                .prev_loc[0],
        cur_y  = town->all_car[ind].all_time_cross[(*dest)[1]][(*dest)[0]]
                                                                .prev_loc[1],
        // required car starting location car_x, car_y
        car_x  = town->all_car[ind].car_loc[0],
        car_y  = town->all_car[ind].car_loc[1],
        temp_x = 0,
        i      = 0;
    path[i  ][0] = (*dest)[0];
    path[i++][1] = (*dest)[1];
    // while hasn't reached the car's location store location of current cross
    // and read next one
    while (cur_x != car_x || cur_y != car_y) {
        path[i  ][0] = cur_x;
        path[i++][1] = cur_y;
        temp_x = cur_x;
        cur_x = town->all_car[ind].all_time_cross[cur_y][cur_x].prev_loc[0];
        cur_y = town->all_car[ind].all_time_cross[cur_y][temp_x].prev_loc[1];
    }
    return i;
}



void print_path(town_t *town, int ind, loc_t *dest) {
    int cost = 0,
        num  = 0;
    loc_t path[town->ncol * town->nrow];
    assert(path != NULL);
    num = car_to_point(path, town, ind, dest);
    STAGE2_PROMPT;
    printf("start at grid %d%c, cost of %d\n", town->all_car[ind].car_loc[0],
                            INT_TO_ALPHA(town->all_car[ind].car_loc[1]), cost);
    // reversely printing the array for correct order locations
    while (num > 0) {
        cost = town->all_car[ind]
                    .all_time_cross[path[num - 1][1]][path[num - 1][0]].ntime;
        STAGE2_PROMPT;
        printf("      then to %d%c, cost of %d\n", path[num - 1][0],
                                    INT_TO_ALPHA(path[num - 1][1]), cost);
        num--;
    }
}


void integrate_all_car(town_t *town) {
    int i = 0,
        x = 0,
        y = 0;
    for (y = 0; y < town->nrow; y++) {
        for (x = 0; x < town->ncol; x++) {
            for (i = 0; i < town->ncar; i++) {
                // if the current car is faster, turn the cross record to
                // current car's record
                if (town->all_car[i].all_time_cross[y][x].ntime < 
                                            town->map_time_cross[y][x].ntime) {
                    town->map_time_cross[y][x] = 
                            town->all_car[i].all_time_cross[y][x];
                } else if (town->all_car[i].all_time_cross[y][x].ntime == 
                            town->map_time_cross[y][x].ntime && 
                            desc_cmp_loc(town->map_time_cross[y][x].prev_loc,
                                         town->all_car[i].all_time_cross[y][x]
                                         .prev_loc) > 0) {
                    // in the tie case, check two different previous location,
                    // if the later one was smaller, insert the later one.
                    town->map_time_cross[y][x] = 
                            town->all_car[i].all_time_cross[y][x];
                }
            }
        }
    }
}


int check_loc(loc_t *loc, int x, int y) {
    return ((*loc)[0] == x && (*loc)[1] == y);
}


void print_map(town_t *town) {
    int y = 0,
        x = 0,
        i = 0;
    // first print all integers in the first line
    STAGE3_PROMPT;
    for (x = 0; x < town->ncol; x++) {
        printf("     %4d", x);
    }
    NEWLINE;

    // then print the second line separator
    STAGE3_PROMPT;
    for (x = 0; x < town->ncol; x++) {
        x == 0 ? printf("   +----+") : printf("--------+");
    }
    NEWLINE;

    // Here iteration type are splited to two different types, of even type 
    // and odd type:
    //      even type: print line with marking of abcd then check one by one
    //                 of two left right adjecent cross whether they are the 
    //                 previous location of the other, if so print accordingly
    // 
    //      odd type : repeatitively print two lines while checking if two up
    //                 and down crosses are previous location of each other,
    //                 then print accordingly;
    // Since y is doubled, the value for accessing y in struct is y / 2
    for (y = 0; y < (town->nrow) * 2; y++) {
        if (y % 2 == 0) {
            STAGE3_PROMPT;
            printf(" %c | ", INT_TO_ALPHA(y / 2));
            for (x = 0; x < town->ncol; x++) {
                printf("%4d", town->map_time_cross[y / 2][x].ntime);
                if (x + 1 < town->ncol) {
                    if (check_loc(&town->map_time_cross[y / 2][x]
                                                    .prev_loc, x + 1, y / 2)) {
                        printf(DIR_LEFT);
                    } else if (check_loc(&town->map_time_cross[y / 2][x + 1]
                                                        .prev_loc, x, y / 2)) {
                        printf(DIR_RIGHT);
                    } else {
                        printf(DIR_NONE);
                    }
                }
            }
            NEWLINE;
        } else if (y % 2 == 1 && y / 2 + 1 < town->nrow) {
            for (i = 0; i < 2; i++) {
                STAGE3_PROMPT;
                printf("   | ");
                for (x = 0; x < town->ncol; x++) {
                    if (check_loc(&town->map_time_cross[y / 2][x]
                                                    .prev_loc, x, y / 2 + 1)) {
                        printf(DIR_UP);
                    } else if (check_loc(&town->map_time_cross[y / 2 + 1][x]
                                                        .prev_loc, x, y / 2)) {
                        printf(DIR_DOWN);
                    } else {
                        printf("    ");
                    }
                    // if it's not the end of the line, print 5 spaces to
                    // separate two locations
                    if (x + 1 != town->ncol) printf(DIR_NONE);
                }
                NEWLINE;
            }
        }
    }
}


static void free_car(town_t *town, int ind) {
    int y = 0;
    for (y = 0; y < town->nrow; y++) {
        // free all allocated row of time recording memory
        free(town->all_car[ind].all_time_cross[y]);
        town->all_car[ind].all_time_cross[y] = NULL;
    }
    // free the array of row of time recording memory
    free(town->all_car[ind].all_time_cross);
    town->all_car[ind].all_time_cross = NULL;
}


void free_town(town_t *town) {
    // free and set all allocated memory to NULL
    int i = 0;
    for (i = 0; i < town->nrow; i++) {
        free(town->all_cross[i]);
        town->all_cross[i] = NULL;
    }
    for (i = 0; i < town->ncar; i++) {
        free_car(town, i);
    }
    for (i = 0; i < town->nrow; i++) {
        free(town->map_time_cross[i]);
        town->map_time_cross[i] = NULL;
    }
    free(town->map_time_cross);
    town->map_time_cross = NULL;
    free(town->all_cross);
    town->all_cross = NULL;
    free(town->all_car);
    town->all_car = NULL;
    free(town);
    town = NULL;
}

// algorithms are fun ? 0.5 : 0;