
#include <stdio.h>
int main(){
    long expected = 10000000;
    long value = 10000;
    long multiplier = 1000;
    long result = value * multiplier;
    printf("e:%ld\n", expected);
    printf("v:%ld\n", result);
}