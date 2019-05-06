#include "quicksort.h"

void swap_str_ptrs(char **arg1, char **arg2)
{
    char *tmp = *arg1;
    *arg1 = *arg2;
    *arg2 = tmp;
}

void quicksort_strs(char *args[], unsigned int len)
{
    unsigned int i, pvt=0;

    if (len <= 1)
        return;

    /* swap a randomly selected value to the last node */
    swap_str_ptrs(args+((unsigned int)rand() % len), args+len-1);

    /* reset the pivot index to zero, then scan */
    for (i=0;i<len-1;++i)
    {
        if (strcmp(args[i], args[len-1]) < 0)
            swap_str_ptrs(args+i, args+pvt++);
    }

    /* move the pivot value into its place */
    swap_str_ptrs(args+pvt, args+len-1);

    /* and invoke on the subsequences. does NOT include the pivot-slot */
    quicksort_strs(args, pvt++);
    quicksort_strs(args+pvt, len - pvt);
}
