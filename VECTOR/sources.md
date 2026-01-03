1 : [10/12/2025.10h38] **TheLinuxCode** :
    **Mastering Malloc Handling in C**
    https://thelinuxcode.com/check-malloc-error-c/ 
    Tell me how to check for errors for malloc.

2 : [10/12/2025.13h00] **Stack Overflow** :
    **Whats the point of malloc(0)?
    https://stackoverflow.com/questions/2022335/whats-the-point-of-malloc0
    Wanted to know if malloc(0) was illegal and returned an error.
    -->It is legal and thats why you must always check if a value = NULL after a malloc!

3 : [17/12/2025.20h12] **Stack Overflow** :
    **Is it good practice to free a null pointer in C?
    https://stackoverflow.com/questions/6084218/is-it-good-practice-to-free-a-null-pointer-in-c
    -->It's useless to free a NULL pointer because free() will do nothing.

4 : [19/12/2025.11h23] **Stack Overflow** :
    **Is it mandatory to check if realloc worked?**
    https://stackoverflow.com/questions/72332551/is-it-mandatory-to-check-if-realloc-worked
    -->YES, we have to check if a realloc was done successfully. Its as important as malloc.

5 : [19/12/2025.12h10] **Stack Overflow :
    **How to know whether an array is initialized in C or not?**
    https://stackoverflow.com/questions/8282518/how-to-know-whether-an-array-is-initialized-in-c
    -->Short : WE CAN'T. This is up to C / gcc to know it. 