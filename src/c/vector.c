#include <math.h>
#include "vector.h"

// add Vectors u & v together
Vector3D add(Vector3D u, Vector3D v)
{
  Vector3D result;
  result.x = u.x + v.x;
  result.y = u.y + v.y;
  result.z = u.z + v.z;
  return result;
}

// subtract v from u
Vector3D sub(Vector3D u, Vector3D v)
{
  Vector3D result;
  result.x = u.x - v.x;
  result.y = u.y - v.y;
  result.z = u.z - v.z;
  return result;
}

// calculate dot product of vectors
double dot(Vector3D u, Vector3D v)
{
  double result = (u.x * v.x + u.y * v.y + u.z * v.z);
  return result;
}

// calculate length of vector
double length(Vector3D u)
{
   return sqrt(dot(u, u));
}

// multiply by scalar
Vector3D multiply(Vector3D u, double s)
{
  Vector3D result;
  result.x = u.x * s;
  result.y = u.y * s;
  result.z = u.z * s;
  return result;
}

// zero the vector
void zero(Vector3D* u)
{
  u->x = 0;
  u->y = 0;
  u->z = 0;
}