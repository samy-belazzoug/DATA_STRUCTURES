//Libraries
#include "dynamic_array.h"

int main() {
    struct vector v;
    //Stress testing / Bad cases
    /*
    sys_vector_resize_up(&v,1);
    vector_init(&v,1,8);
    printf("Capacity : %ld\n",v.capacity);
    sys_vector_resize_up(&v,1);
    v.count = 2;
    sys_vector_resize_down(&v);
    printf("Capacity : %ld\n",v.capacity);
    vector_free(&v);
    sys_vector_resize_up(&v,1);
    */
    //Good use
    vector_init(&v,1,3);
    vector_get_index(&v,1);
    vector_set_index(&v,1,"c");
    vector_get_index(&v,1);
    vector_free(&v);
    return 0;
}

//Critical functions.
int vector_init(struct vector *vector, size_t item_size, size_t capacity) { //Allocate enough memory for the array belonging to the parameters
    vector->item_size = item_size;
    if (vector->item_size < 1) { //If item_size is not a stricly positive number
        printf("Critical : item_size must be stricly positive.\nInitialization failed.\n");
        return 1;
    }
    else { //We can properly initialize the vector
        if (capacity <= 1) { //The user wants an empty vector
            /* Important :
                Even if the user wants an empty vector, it will have 2 elements allocated for optimisation purpose.
                We do not want a null vector (malloc(0)). So the vector will have 1 element at least.
                However, with 2 elemenets instead of one, we avoid a reallocation for only one element.
            */
            printf("Warning: You have set a vector with no elements at first.\n");
            //Allocation
            vector->array = malloc(2 * vector->item_size); //Initialize the vector with 2 elements
            
            //Check if allocation was done.
            if (vector->array != NULL) { //Allocation successful
                printf("Initialization successful.\n");
                vector->capacity = 2; //We initialize the capacity as the allocation was successful.
                vector->count = 0;
                return 0;
            }
            else { //Allocation failed
                printf("Initialization failed.\n");
                return 1;
            }
        }
        else { //The user wants a vector with a specific size
            //Allocation
            vector->array = malloc(vector->capacity * vector->item_size); //Vector set with capacity elements
            
            //Check if allocation was done.
            if (vector->array != NULL) { //Allocation successful
                printf("Initialization successful.\n");
                vector->capacity = capacity; //We initialize the capacity as the allocation was successful.*
                vector->count = 0;
                return 0;
            }
            else { //Allocation failed
                printf("Initialization failed.\n");
                return 1;
            }
        }
    }
}

int vector_free(struct vector *vector) { //Frees the vector (void *array)
    /*Note : 
    We only free the void *array because it's the only one ressource we can actually free
    as its stocked in the HEAP.
    All the other variables are stocked in the STACK, so it will be freed automatically.*/
    //Checks if the array is NULL
    if (vector->array == NULL) { //The array is a NULL
        printf("Warning : The array is NULL.\nFree failed.\n");
        return 1;
    }
    else { //The array is not a NULL
        free(vector->array);
        vector->count = 0;
        vector->capacity = 0;
        printf("Array freed successfully.\n");
        return 0;
    }
}

//Functions not recommended for the user. (it facilitates myself)
int sys_vector_resize_up(struct vector *vector, size_t coefficient) { //Adds x2 more items and processes a reallocation
    /*NOTE :
    As the reallocation is a very heavy task, we want to minimise the number of calls of realloc().
    To do so, we will resize only when necessary, which means, when the number of elements occupied is as big as the capacity.
    Also, to really optimize the call, we have a coefficient, this is to do more than 2x resizing. This is for specific cases,
    e.g : Avoid doing a realloc in a for loop. 
    We want to do one reallocation max in any function.
    */
    if (vector->item_size < 1 || vector->capacity == 0) { //Critical error. We cannot operate on a NULL vector.
        /*NOTE :
        Because we can't direcly verify if an array is NULL, I used item_size as a check mark.
        With item_size, we can check whether the user initialized the array or not. If he didn't, 
        it means the array has not been initializd.
        In case the user freed the array but still wants to call a resizing operation, we must check if 
        the capacity is equal to 0 because in free, we set 0 to capacity after freeing the array. 
        */
        printf("Critical : Array is NULL. Please initialize it.\n");
        return 1;
    }
    else { //We can proceses a reallocation.
        if (vector->count >= vector->capacity) { //If count is as big as the capacity
            if (coefficient < 2) { //Default resizing (x2)
                vector->capacity *= 2;
                vector->array = realloc(vector->array, vector->capacity);
                
                //Check if reallocation was done.
                if (vector->array == NULL) { //Reallocation failed
                    printf("Reallocation failed.\n");
                    return 1;
                }
                else { //Reallocation successful
                    printf("Reallocation successful\n"); 
                    return 0;
                }
            }
            else { //Bigger resizing
                vector->capacity *= (2 * coefficient);
                vector->array = realloc(vector->array, vector->capacity);
                
                //Check if reallocation was done.
                if (vector->array == NULL) { //Reallocation failed
                    printf("Reallocation failed.\n");
                    return 1;
                }
                else { //Reallocation successful
                    printf("Reallocation successful\n"); 
                    return 0;
                }
            }
        }
        else { //THe reallocation is not neccessary.
            printf("Reallocation not necessary.\n");
            return 0;
        }
    }
}

int sys_vector_resize_down(struct vector *vector) { //remove x2 less items and processes a reallocation
    if (vector->item_size < 1 || vector->capacity == 0) { //Critical error. We cannot operate on a NULL vector.
        /*NOTE :
        Because we can't direcly verify if an array is NULL, I used item_size as a check mark.
        With item_size, we can check whether the user initialized the array or not. If he didn't, 
        it means the array has not been initializd.
        In case the user freed the array but still wants to call a resizing operation, we must check if 
        the capacity is equal to 0 because in free, we set 0 to capacity after freeing the array. 
        */
        printf("Critical : Array is NULL. Please initialize it.\n");
        return 1;
    }
    else if ((vector->count < 1)){ //Critical error, if count < 1, it means it's equal to 0. We cannot do a division by 0
        printf("Critical : count < 1, illegal operation (division by 0).\n");
        return 1;
    }
    else { //We can resize
        size_t gap = vector->capacity / vector->count;
        if (gap >= 4 && gap % 2 == 0) { //If the gap is big enough and the gap is an even number
            /*NOTE :
            Let's say the gap = 4, if we divide by 4, then capacity = count, so we will have to reallocate. Which is suboptimal.
            But if we divide by 4 - 2, basically 2, there will still be enough room to put elements.
            */
            vector->capacity /= (gap-2);
            vector->array = realloc(vector->array, vector->capacity);
            //Check if reallocation was done.
            if (vector->array == NULL) { //Reallocation failed
                printf("Reallocation failed.\n");
                return 1;
            }
            else { //Reallocation successful
                printf("Reallocation successful\n"); 
                return 0;
            }
        }
        else { //The gap was not big enough to resize
            printf("Reallocation not necessary.\n");
            return 0;
        }
    }     
}

//Getters / Setters
int vector_set_index(struct vector *vector, size_t index, void *element) { //Set a value at a specific index of the vector with a byte by byte copy.
    if (index < 0 || index >= vector->capacity) { //Checks if the index is correct
        printf("Warning: index out of bounds.\n");
        return 1;
    }
    else { //We can proceed.
        unsigned char *index_array = (unsigned char *)vector->array + index;
        unsigned char *index_element = (unsigned char *)element;
        for (size_t i; i < sizeof(vector->item_size);i++) {
            index_array[i] = index_element[i];
        }
        return 0;
    }
}

void *vector_get_index(struct vector *vector, size_t index) { //Returns a pointer to an element of the array at a specific index
    if (index < 0 || index >= vector->capacity) { //Checks if the index is correct
        printf("Warning: index out of bounds.\n");
    }
    else { //We can proceed.
        unsigned char *index_array = (unsigned char *)vector->array + index;
        printf("%c\n",index_array[0]);
        return vector->array + index;
    }
}

//Operations
int vector_push_back(struct vector *vector, void *element); //Adds a new element at the end of the queue (NOTE : Processes a reallocation!)

int vector_pop_back(struct vector *vector); //Erase the last element of the index (NOTE : Processes a reallocation!)
