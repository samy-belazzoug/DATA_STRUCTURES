#ifndef VECTOR_H
#define VECTOR_H

//Libraries
#include <stdio.h> //printf
#include <stdlib.h> //malloc, free, realloc

//The structure
struct vector { //Array that can dynamically change its size
    size_t item_size; //Size of an item (critical)
    size_t capacity; //How many items can the array stock
    size_t count; //How many items are currently in use
    void *array; //Stock all the elements. It's the array.
};

//Critical functions.
int vector_init(struct vector *vector, size_t item_size, size_t capacity); //Allocate enough memory for the array belonging to the parameters
int vector_free(struct vector *vector); //Frees the vector (void *array)

//Functions not recommended for the user. (it facilitates myself)
int sys_vector_resize_up(struct vector *vector, size_t coefficient); //Adds x2 more items and processes a reallocation
int sys_vector_resize_down(struct vector *vector); //remove x2 less items and processes a reallocation

//Getters / Setters
int vector_set_index(struct vector *vector, size_t index, void *element); //Set a value at a specific index of the vector with a byte by byte copy.
void *vector_get_index(struct vector *vector, size_t index, char type); //Returns an element of the array at a specific index

//Operations
int vector_push_back(struct vector *vector, void *element); //Adds a new element at the end of the queue (NOTE : Processes a reallocation!)
int vector_pop_back(struct vector *vector); //Erase the last element of the index (NOTE : Processes a reallocation!)

#endif