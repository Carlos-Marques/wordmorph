#include "dif.h"

int dif(char *a, char *b, int size){  /* calculates dif between 2 words of given size */
  int counter = 0, difvalue = 0, returnvalue = 0; /* counter to run through words, difvalue to count the number of char difference between words and returnvalue to return weight of given mutation */

  for(counter = 0; counter < size; counter++){  /* goes through the words char by char */
    if(a[counter] != b[counter]){ /* checks if char's are different */
      difvalue++;                 /* adds another difference */
    }
  }

  returnvalue = difvalue * difvalue; /* calculates weight of given mutation */

  return returnvalue; /* returns weight calculated */
}
