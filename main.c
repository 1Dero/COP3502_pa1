/*
Darel Gomez
COP3502
January 28, 2023
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define INITSIZE 10
#define MAXLEN 50
#define MAXROWS 100000

typedef struct order {
    int s_seat;
    int e_seat;
    char* name;
} order;

typedef struct theaterrow {
    order** list_orders;
    int max_size;
    int cur_size;
} theaterrow;

order* make_order(int start, int end, char* this_name) {
    order* output = (order*) malloc(sizeof(order));
    output->s_seat = start;
    output->e_seat = end;

    // Allocating memory for order name
    output->name = (char*) calloc(strlen(this_name)+1, sizeof(char));
    strcpy(output->name, this_name);

    return output;
}
theaterrow* make_empty_row() {
    // Allocating memory for theatherrow
    theaterrow* pointer = (theaterrow*) malloc(sizeof(theaterrow));

    // Initalizing max_size and cur_size
    pointer->max_size = INITSIZE;
    pointer->cur_size = 0;

    // Allocating memory for list of orders
    pointer->list_orders = (order**) calloc(pointer->max_size, sizeof(order*));

    return pointer;
}
int conflict(order* order1, order* order2) {
    if(contains(order2, order1->s_seat)) {
        // If order1 start seat is within order2
        return 1;
    }
    else if(contains(order2, order1->e_seat)) {
        // If order1 end seat is within order2
        return 1;
    }
    else if(contains(order1, order2->s_seat)) {
        // If order2 start seat is within order1
        return 1;
    }
    else if(contains(order1, order2->e_seat)) {
        // If order2 end seat is within order1
        return 1;
    }
    return 0;
}
int can_add_order(theaterrow* this_row, order* this_order) {
    for(int i = 0; i < this_row->cur_size; i++) {
        if(conflict(this_row->list_orders[i], this_order)) {
            // Order can't be added to row
            return 0;
        }
    }
    // Order can be added to row
    return 1;
}
int add_order(theaterrow* this_row, order* this_order) {
    if(can_add_order(this_row, this_order)) {
        if(this_row->cur_size == this_row->max_size) {
            // Reallocates memory for list of orders if full
            this_row->max_size *= 2;
            this_row->list_orders = realloc(this_row->list_orders, this_row->max_size);
        }
        this_row->list_orders[this_row->cur_size] = this_order;
        this_row->cur_size += 1;
      
        // Order was successfully added
        return 1;
    }
    // Order was unable to be added
    return 0;
}
char* get_owner(theaterrow** theater, int row, int seat_num) {
    if(theater[row] == NULL) {
        return NULL;
    }
    return get_row_owner(theater[row], seat_num);
}
char* get_row_owner(theaterrow* this_row, int seat_num) {
    for(int i = 0; i < this_row->cur_size; i++) {
        if(contains(this_row->list_orders[i], seat_num)) {
            // Owner found
            return this_row->list_orders[i]->name;
        }
    }
    // Owner not found
    return NULL;
}
int contains(order* myorder, int seat_no) {
    if(myorder->s_seat <= seat_no && myorder->e_seat >= seat_no) {
        // Seat is within order
        return 1;
    }
    // Seat is not within order
    return 0;
}
void free_order(order* this_order) {
    free(this_order->name);
    free(this_order);
}
void free_row(theaterrow* this_row) {
    for(int i = 0; i < this_row -> cur_size; i++) {
        free_order(this_row->list_orders[i]);
    }
    free(this_row->list_orders);
    free(this_row);
}

int main() {
    theaterrow** rows= calloc(MAXROWS+1, sizeof(theaterrow*));

    char command[MAXLEN], name[MAXLEN];
    int row, start, end, seat;
    order* tempOrder;
    theaterrow* tempRow;
    char* tempName;

    // Menu
    scanf("%s", command);
    while(strcmp(command, "EXIT")) {
        if(!strcmp(command, "BUY")) { // BUY command
            scanf("%d %d %d %s", &row, &start, &end, name);

            // Creating order
            tempOrder = make_order(start-1, end-1, name);

            if(rows[row-1] == NULL) { // Creates a row if it doesn't exist
                rows[row-1] = make_empty_row();
            }

            if(add_order(rows[row-1], tempOrder)) {
                printf("SUCCESS\n");
            }
            else {
                printf("FAILURE\n");
                free_order(tempOrder);
            }
        }
        else if(!strcmp(command, "LOOKUP")) { // LOOKUP command
            scanf("%d %d", &row, &seat);
            tempName = get_owner(rows, row-1, seat-1);
            if(tempName == NULL) {
                printf("None\n");
            }
            else {
                printf("%s\n", tempName);
            }
        }
        scanf("%s", command);
    }
    // Freeing memory
    for(int i = 0; i < MAXROWS; i++) {
      if(rows[i] != NULL) {
        free_row(rows[i]);
      }
    }
    free(rows);

    return 0;
}
