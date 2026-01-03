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