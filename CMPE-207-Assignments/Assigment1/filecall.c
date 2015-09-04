/* seq.c - A conventional C program that sums integers from 1 to 5 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
int sum;              /* global variable */

int main() {
    int i;            /* local variable */

    sum = 0;
    printf("from call \n %d \n", getpid());
    for (i=1; i<=5; i++) {   /* iterate from 1 to 5 */
        printf("The value of i is %d\n", i);
        fflush (stdout);     /* flush buffer */
        sum += i;
    }
    printf ("The sum is %d\n", sum);
    exit (0);
    return(0);
}
