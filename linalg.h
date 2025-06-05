// an extremely limited class for performing linear algebra operations.

// macros for determinant of a matrix
#define DET3(a,b,c, d,e,f, g,h,i) ((float) (a*(e*i - f*h) - b*(d*i - f*g) + c*(d*h - e*g)))
#define DET4(a,b,c,d, e,f,g,h, i,j,k,l, m,n,o,p) (a*DET3(f,g,h,j,k,l,n,o,p) - b*DET3(e,g,h,i,k,l,m,o,p) + \
                                                 c*DET3(e,f,h,i,j,l,m,n,p) - d*DET3(e,f,g,i,j,k,m,n,o))

// A point in 3D space
struct Vector3 {
    float x;
    float y;
    float z;
};

struct Vector4 {
    float x;
    float y;
    float z;
    float w;
};

// 4x4 matrix
// Indexed like so:
// [ [0][0]  [0][1]  ... ]
// [ [1][0]          ... ]
// [  ...            ... ]
struct Mat4 {
    float values[4][4];
};

Mat4 multiply(Mat4 a, Mat4 b);

Vector4 multiply(Mat4 m, Vector4 v);

Mat4 invert(Mat4 matrix);

void printMatrix(Mat4 m);

void printVector(Vector4 v);

void printVector(Vector3 v);
