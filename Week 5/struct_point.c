#include <stdio.h>
#include <stdlib.h>

#define SIZE 10

struct t_point {

    int x;
    int y;

};
typedef struct t_point Point1;

typedef struct {

    int x;
    int y;
} Point2;

int cmp_point(const void *a, const void *b){

    // convert void * to Point *
    Point1 *c = (Point1 *)a;
    Point1 *d = (Point1 *)b;

    // compute their length square
    int c_l = c->x*c->x + c->y*c->y;
    int d_l = d->x*d->x + d->y*d->y;

    // return values
    if(c_l == d_l) return 0;
    if(c_l > d_l) return 1;
    else return -1;

}

int main(void) {

    Point1 p1 = {0, 0};
    Point2 p2 = {0, 0};

    p1.x = 10;
    p1.y = 20;

    p2.x = 30;
    p2.y = 40;

    printf("%d %d\n", p1.x, p1.y);
    printf("%d %d\n", p2.x, p2.y);

    Point1 *pp;
    pp = &p1;

    (*pp).x = 50;
    pp->y = 60;
    printf("%d %d\n", p1.x, p1.y);

    Point1 p3 = {p2.x, p2.y};
    Point1 points[SIZE] = {p1, p3};
    qsort(points, SIZE, sizeof(Point1), cmp_point);
    printf("Sorted points by distance from origin:\n");
    for (int i = 0; i < SIZE; i++) {
        printf("Point %d: (%d, %d)\n", i, points[i].x, points[i].y);
    }
    return 0;
}