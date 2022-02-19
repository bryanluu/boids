#define N_DIMS 3

struct vector
{
  double x, y, z;
};

typedef struct vector Vector3D;

Vector3D add(Vector3D u, Vector3D v);
Vector3D sub(Vector3D u, Vector3D v);
Vector3D multiply(Vector3D u, double s);
double dot(Vector3D u, Vector3D v);
double length(Vector3D u);
void zero(Vector3D* u);