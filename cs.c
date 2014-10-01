#include<stdio.h>
main()
{
int x;
long int y=0xabcde;
x=25;
x=x+=x*=x-=15;
printf("%d %d",x,y);
}
