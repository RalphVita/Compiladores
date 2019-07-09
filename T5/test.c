#include <stdio.h>
int main(){
    int x = 12;
    int *y = &x;//malloc (sizeof (int));
    //*y = 14;
    int end = y;
    *end = *y;

    int **z = end;//end;

    int *h = &end;
    *h = 17;
    //int *h = x;
    //**x = 17;
    //int z = &end;
    //end = &x;
    printf("\n==%d == %d == %d\n",end,&y,*(&(*y)));
   
    //printf("\n==%d == %d == %d\n",h, &y ,&(*y));
    return 0;
}