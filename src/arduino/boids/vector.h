#ifndef VECTOR_H
#define VECTOR_H

struct vector
{
  double x, y;
};

typedef struct vector Vector;

Vector add(Vector u, Vector v);
Vector sub(Vector u, Vector v);
Vector multiply(Vector u, double s);
double dot(Vector u, Vector v);
double length(Vector u);
void zero(Vector* u);

#endif
