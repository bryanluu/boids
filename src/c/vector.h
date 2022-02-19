#define N_DIMS 3

struct vector
{
  int x, y, z;
};

typedef struct vector Vector3D;

Vector3D add(Vector3D u, Vector3D v);
Vector3D sub(Vector3D u, Vector3D v);
unsigned int dot(Vector3D u, Vector3D v);
double length(Vector3D u);