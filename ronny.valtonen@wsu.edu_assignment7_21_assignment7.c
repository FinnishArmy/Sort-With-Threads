/***********************************************************************
name: Ronny Z Valtonen
    Q Sort With Threading
description:
    See CS 360 Lecture for details.

README:
    gcc -Wall -o [NAME] assignment7.c -lm -lpthread
    ./[NAME]
***********************************************************************/


#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <errno.h>

// Defines
#define SORT_THRESHOLD      40
#define SORT_WORDS          10

// Globals
pthread_mutex_t sortThread_mtx[SORT_THRESHOLD]; // Set up a mutex array of threads to use.
static int maximumThreads;              /* maximum # of threads to be used */


/* Definitions */
/*********************************************************************
Parameters:
char** array = Pass in a double array pointer.

int left = Give the left side.

int right = Give the right side.

General Run:
This is an implementation of insert sort, which although it is
n-squared, is faster at sorting short lists than quick sort,
due to its lack of recursive procedure call overhead.
// Writen by Ben Mccamish at Washington State University Washington.
********************************************************************/
static void insertSort(char** array, int left, int right);

/*********************************************************************
Parameters:
void* p;

General Run:
Recursive quick sort, but with a provision to use
insert sort when the range gets small.
// Writen by Ben Mccamish at Washington State University Washington.
********************************************************************/
static void quickSort(void* p);

/*********************************************************************
Parameters:
int count = Number of threads to set.

General Run:
User interface routine to set the number of threads sortT is permitted to use
// Writen by Ben Mccamish at Washington State University Washington.
********************************************************************/
void setSortThreads(int count);

/*********************************************************************
Parameters:
char** array = Double array pointer.
unisgned int count = Number of threads to make.

General Run:
User callable sort procedure, sorts array of count strings, beginning at address array.
// Writen by Ben Mccamish at Washington State University Washington.
********************************************************************/
void sortThreaded(char** array, unsigned int count) ;


// Set up the struct array.
typedef struct _sortParams {
    char** array;
    int left;
    int right;
} SortParams;


static void insertSort(char** array, int left, int right) {
    int i, j;
    for (i = left + 1; i <= right; i++) {
        char* pivot = array[i];
        j = i - 1;
        while (j >= left && (strcmp(array[j],pivot) > 0)) {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = pivot;
    }
}

static void quickSort(void* p) {
    SortParams* params = (SortParams*) p;
    char** array = params->array;
    int left = params->left;
    int right = params->right;
    int i = left, j = right;

    if (j - i > SORT_THRESHOLD) {           /* if the sort range is substantial, use quick sort */

        int m = (i + j) >> 1;               /* pick pivot as median of         */
        char* temp, *pivot;                 /* first, last and middle elements */
        if (strcmp(array[i],array[m]) > 0) {
            temp = array[i]; array[i] = array[m]; array[m] = temp;
        }
        if (strcmp(array[m],array[j]) > 0) {
            temp = array[m]; array[m] = array[j]; array[j] = temp;
            if (strcmp(array[i],array[m]) > 0) {
                temp = array[i]; array[i] = array[m]; array[m] = temp;
            }
        }
        pivot = array[m];

        for (;;) {
            while (strcmp(array[i],pivot) < 0) i++; /* move i down to first element greater than or equal to pivot */
            while (strcmp(array[j],pivot) > 0) j--; /* move j up to first element less than or equal to pivot      */
            if (i < j) {
                char* temp = array[i];      /* if i and j have not passed each other */
                array[i++] = array[j];      /* swap their respective elements and    */
                array[j--] = temp;          /* advance both i and j                  */
            } else if (i == j) {
                i++; j--;
            } else break;                   /* if i > j, this partitioning is done  */
        }

        SortParams first;
        first.array = array;
        first.left = left;
        first.right = j;

        quickSort(&first);                  /* sort the left partition  */

        SortParams second;
        second.array = array;
        second.left = i;
        second.right = right;

        quickSort(&second);                 /* sort the right partition */

    } else insertSort(array,i,j);           /* for a small range use insert sort */
}

void setSortThreads(int count) {
    maximumThreads = count;
}


void sortThreaded(char** array, unsigned int count) {
    SortParams parameters;
    parameters.array = array;
    parameters.left = 0;
    parameters.right = count - 1;
    quickSort(&parameters);
}

// # End of Program # //


// Ignore this.
// Main function testing. Takes in a file to sort words with.

// int main() {
//   char in_name[80];
//   FILE *in_file;
//   char word[50];
//   char * lines[50];
//   // char *wordarr[SORT_WORDS];
//   int loop = 0;
//   printf("Enter file name: ");
//   scanf("%s", in_name);
//
//   in_file = fopen(in_name, "r");
//
//   if (in_file == NULL)
//       printf("Can't open %s for reading.\n", in_name);
//
//   else {
//       for (i = 0; i < SORT_WORDS; ++i) {
//         lines[loop] = malloc(128);
//         fscanf (in_file, "%127s", lines[loop]);
//         printf("%s\n", lines);
//       }
//       while (fscanf(in_file, "%s", word) && loop < SORT_WORDS) {
//           printf("%s\n", word);
//           // sortThreaded(lines, SORT_WORDS);
//           loop++;
//       }
//       fclose(in_file);
//   }

//   return 0;
//
// }
