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
unsigned int dot(Vector3D u, Vector3D v)
{
  unsigned int result = (unsigned int) (u.x * v.x + u.y * v.y + u.z * v.z);
  return result;
}

// calculate length of vector
double length(Vector3D u)
{
   return sqrt(dot(u, u));
}