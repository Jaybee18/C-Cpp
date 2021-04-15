#include<stdio.h>

int main(){    
    int number;
    char meinString='d'; 
    printf("enter a number:");
    scanf("%d", &number); // keine weiteren character in den input string packen!!!
    // %d  -> int
    // %ld -> long int
    // %c  -> char
    // %lc -> long char
    printf("%d my dick\n", number);
    printf("wen habe ich genommen? %c \n", meinString);
    return 0;  
}  