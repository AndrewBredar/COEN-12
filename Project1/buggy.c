/*
*runtime 1: O(n^2) [slowest]
*runtime 2: O(log(n)) [fastest]
*runtime 3: O(n)   [linear]
*runtime 4: O(nlog(n)) [slower than linear]
*/

#include <stdio.h>
#include <math.h>

struct point {
    float x, y;
};

typedef struct point POINT;


struct point add(POINT p1, POINT p2) //was not originally declared as a struct
{
    p1.x = p1.x + p2.x;
    p1.y = p1.y + p2.y; //changed from p1.x to p1.y because we want to add the y-coordinates not just the x-coordinates

    return p1;
}


float distance(POINT p1, POINT p2)
{
    float xdiff, ydiff; //ydiff was originally delcared as y_diff

    xdiff = p1.x - p2.x;
    ydiff = p1.y - p2.y;

    return sqrt(xdiff * xdiff + ydiff * ydiff);
}


int main(void)
{
    POINT a, b, c;

    a.x = 0;
    a.y = 0;

    b.x = 2;
    b.y = 2;

    //changed "->" to "." because there are no pointers involved here
    c.x = 2;
    c.y = 3;

    // I changed some of the values for the points to test that the program is running correctly

    printf("%f\n", distance(a, add(b, c)));
}

