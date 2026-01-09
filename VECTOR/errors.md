### Warning 1 :

Command :
```bash
gcc -Wall dynamic_array.c -o array.exe
```

Terminal :
```bash
dynamic_array.c: In function ‘vector_init’:
dynamic_array.c:28:20: warning: returning ‘int’ from a function with return type ‘int *’ makes pointer from integer without a cast [-Wint-conversion]
28 |             return 1;
   |                    ^
dynamic_array.c:36:20: warning: returning ‘int’ from a function with return type ‘int *’ makes pointer from integer without a cast [-Wint-conversion]
36 |             return 1;
   |                    ^
dynamic_array.c:45:20: warning: returning ‘int’ from a function with return type ‘int *’ makes pointer from integer without a cast [-Wint-conversion]
45 |             return 1;
   |                    ^
```

#### Explaining :
I've returned a 'int' in a function that returns a '*int'.
So I must define my function as int vector_init() instead of int *vector_init()

### Warning 2 :

Command :
```bash 
Command : gcc -Wall dynamic_array.c -o test.exe
```

Terminal :
```bash
dynamic_array.c: In function ‘vector_free’:
dynamic_array.c:88:9: warning: ‘free’ called on pointer ‘vector’ with nonzero offset 24 [-Wfree-nonheap-object]
   88 |         free(&vector->array);
      |         ^~~~~~~~~~~~~~~~~~~~
```

#### Explaining :
I've called a pointer of the array while we already have its pointer with *vector.

Changes : free(vector->array) instead of free(&vector->array)

### Error 1 :

Command :
```bash
./test.exe
```

Terminal :
```bash
item_size must be stricly positive.
Initialization failed.
munmap_chunk(): invalid pointer
Aborted (core dumped)
```

#### Explaining :
So, I did some criminal errors doesn't it.
Basically, in the init function, I didn't associated the value of item_size stocked in the vector with the parameter item_size.

This is bad, because item_size is NULL or 0 by default, so, if we don't do :
vector->item_size = item_size (the parameter, don't be confused xD)
Then item_size will be NULL forever. This is why I got the message "item_size must be strictly positive."

Also, as a reminder, I should also initialize the capacity.

### Error 2 :

Command :
```bash
./test.exe
```

Terminal :

```bash
dynamic_array.c:18:68: error: expected ‘;’, ‘,’ or ‘)’ before ‘=’ token
   18 | int sys_vector_resize_up(struct vector *vector, size_t coefficient = 2); //Adds 2x more items and processes a reallocation
      |                                                                    ^
dynamic_array.c:105:67: error: expected ‘;’, ‘,’ or ‘)’ before ‘=’ token
  105 | int sys_vector_resize_up(struct vector *vector, size_t coefficient=2) { //Adds 2x more items and processes a reallocation
      |                                                                   ^
```

#### Explaining : 
We cannot implement default parameters in C. So he is just tripping now, confusing if = is a ) xD. 

### Critical Error 1 :

Command :
```bash
./test.exe
```

Terminal :

```bash
0
munmap_chunk(): invalid pointer
Aborted (core dumped)
```

#### Explaining : 
In sys_vector_resize_up, to operate, I have to make sure the array has been initialized by the user.
To do so, I verified if the array == NULL. 
The thing is, we can't do that. So I must use another parameter such as item_size to verify that.

### Critical Error 2 : 

Command :
```bash
./test.exe
```

Terminal :

```bash
Critical : Array is NULL. Please initialize it.
Warning: You have set a vector with no elements at first.
Initialization successful.
Capacity : 2Reallocation not necessary.
Floating point exception (core dumped)
```

#### Explaining : 

In my script, v.count = 0, so, when we do a resize down, we basically do a division by 0, which is pretty illegal and
dangerous !!

### Error 3 :

Command :
```bash
./test.exe
```

Terminal :

```bash
./dynamic_array.c: In function ‘main’:
./dynamic_array.c:21:27: warning: passing argument 3 of ‘vector_set_index’ makes pointer from integer without a cast [-Wint-conversion]
   21 |     vector_set_index(&v,1,5);
      |                           ^
      |                           |
      |                           int
In file included from ./dynamic_array.c:2:
./dynamic_array.h:25:65: note: expected ‘void *’ but argument is of type ‘int’
   25 | int vector_set_index(struct vector *vector, size_t index, void *element); //Set a value at a specific index of the vector with a byte by byte copy.
      |                                                           ~~~~~~^~~~~~~
./dynamic_array.c:22:5: error: too many arguments to function ‘vector_get_index’
   22 |     vector_get_index(&v,1,'d');
      |     ^~~~~~~~~~~~~~~~
./dynamic_array.h:26:7: note: declared here
   26 | void *vector_get_index(struct vector *vector, size_t index); //Returns an element of the array at a specific index
      |       ^~~~~~~~~~~~~~~~
./dynamic_array.c: At top level:
./dynamic_array.c:212:7: error: conflicting types for ‘vector_get_index’; have ‘void *(struct vector *, size_t,  char)’ {aka ‘void *(struct vector *, long unsigned int,  char)’}
  212 | void *vector_get_index(struct vector *vector, size_t index, char type) { //Returns a pointer to an element of the array at a specific index
      |       ^~~~~~~~~~~~~~~~
./dynamic_array.h:26:7: note: previous declaration of ‘vector_get_index’ with type ‘void *(struct vector *, size_t)’ {aka ‘void *(struct vector *, long unsigned int)’}
   26 | void *vector_get_index(struct vector *vector, size_t index); //Returns an element of the array at a specific index
      |       ^~~~~~~~~~~~~~~~                                                              ^
```

#### Explaining : 
Basically, in my testing script, I put vector_set_index(&v,1,4); , which is illegal. We are asking for a pointer, not just a value.
For the error in get_index, I forget to update the definition in the header file.

### Error 4 :

Command :
```bash
make
```

Terminal :

```bash
dynamic_array.c: In function ‘vector_set_index’:
dynamic_array.c:202:15: error: comparison of unsigned expression in ‘< 0’ is always false [-Werror=type-limits]
  202 |     if (index < 0 || index >= vector->capacity) { //Checks if the index is correct
      |               ^
dynamic_array.c: In function ‘vector_get_index’:
dynamic_array.c:217:15: error: comparison of unsigned expression in ‘< 0’ is always false [-Werror=type-limits]
  217 |     if (index < 0 || index >= vector->capacity) { //Checks if the index is correct
      |               ^
dynamic_array.c:235:1: error: control reaches end of non-void function [-Werror=return-type]
  235 | }
      | ^
cc1: all warnings being treated as errors
make: *** [makefile:6: all] Error 1
```

#### Explaining : 
size_t is defined as an unsigned int, which means it doesn't have negative values. So we must avoid comparing a size_t with negative values, in fact : if (index < 0).

The right solution would be : if (index >= vector->capacity). It even makes my task easier.
I can't manage negative size_t, gcc makes it all for me! 

### Error 5 :

Command :
```bash
make
```

Terminal :

```bash
gcc -Wall -Wextra -O2 -Werror dynamic_array.c -o dynamic_array
dynamic_array.c: In function ‘main’:
dynamic_array.c:24:5: error: too few arguments to function ‘vector_get_index’
   24 |     vector_get_index(&v,1);
      |     ^~~~~~~~~~~~~~~~
In file included from dynamic_array.c:2:
dynamic_array.h:26:7: note: declared here
   26 | void *vector_get_index(struct vector *vector, size_t index, char type); //Returns an element of the array at a specific index
      |       ^~~~~~~~~~~~~~~~
dynamic_array.c:26:5: error: too few arguments to function ‘vector_get_index’
   26 |     vector_get_index(&v,1);
      |     ^~~~~~~~~~~~~~~~
dynamic_array.h:26:7: note: declared here
   26 | void *vector_get_index(struct vector *vector, size_t index, char type); //Returns an element of the array at a specific index
      |       ^~~~~~~~~~~~~~~~
dynamic_array.c: In function ‘vector_set_index’:
dynamic_array.c:202:15: error: comparison of unsigned expression in ‘< 0’ is always false [-Werror=type-limits]
  202 |     if (index < 0 || index >= vector->capacity) { //Checks if the index is correct
      |               ^
dynamic_array.c: At top level:
dynamic_array.c:216:7: error: conflicting types for ‘vector_get_index’; have ‘void *(struct vector *, size_t)’ {aka ‘void *(struct vector *, long unsigned int)’}
  216 | void *vector_get_index(struct vector *vector, size_t index) { //Returns a pointer to an element of the array at a specific index
      |       ^~~~~~~~~~~~~~~~
dynamic_array.h:26:7: note: previous declaration of ‘vector_get_index’ with type ‘void *(struct vector *, size_t,  char)’ {aka ‘void *(struct vector *, long unsigned int,  char)’}
   26 | void *vector_get_index(struct vector *vector, size_t index, char type); //Returns an element of the array at a specific index
      |       ^~~~~~~~~~~~~~~~
cc1: all warnings being treated as errors
make: *** [makefile:6: all] Error 1
```

#### Explaining : 
I've modified the parameters of my function vector_get_index but I forget to update it on the .h
I will do this error an infinite amount of times I think xD.