#include <stdio.h>
#include "linalg.h"

// matrix multiplication (4x4 times 4x4)
Mat4 multiply(Mat4 a, Mat4 b) {
    Mat4 result;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            result.values[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                result.values[i][j] += a.values[i][k] * b.values[k][j];
            }
        }
    }

    return result;
}

// multiply a matrix by a vector4
Vector4 multiply(Mat4 m, Vector4 v) {
    Vector4 result;

    result.x = m.values[0][0]*v.x + m.values[0][1]*v.y + m.values[0][2]*v.z + m.values[0][3]*v.w;
    result.y = m.values[1][0]*v.x + m.values[1][1]*v.y + m.values[1][2]*v.z + m.values[1][3]*v.w;
    result.z = m.values[2][0]*v.x + m.values[2][1]*v.y + m.values[2][2]*v.z + m.values[2][3]*v.w;
    result.w = m.values[3][0]*v.x + m.values[3][1]*v.y + m.values[3][2]*v.z + m.values[3][3]*v.w;

    return result;
}

float det(Mat4 matrix) {
    // Choosing to manually apply the determinant formula since only det4 is needed

    // Expecting all this stuff to be optimized away by the compiler,
    // so this is just for making the final expression at the bottom cleaner.
    float a = matrix.values[0][0];
    float b = matrix.values[0][1];
    float c = matrix.values[0][2];
    float d = matrix.values[0][3];

    float e = matrix.values[1][0];
    float f = matrix.values[1][1];
    float g = matrix.values[1][2];
    float h = matrix.values[1][3];

    float i = matrix.values[2][0];
    float j = matrix.values[2][1];
    float k = matrix.values[2][2];
    float l = matrix.values[2][3];

    float m = matrix.values[3][0];
    float n = matrix.values[3][1];
    float o = matrix.values[3][2];
    float p = matrix.values[3][3];

    // (this is the cleanest way I could think of to write this sorry)
    // [ a  b  c  d ]
    // [ e  f  g  h ]
    // [ i  j  k  l ]
    // [ m  n  o  p ]
    return a*(f*(k*p - l*o) - g*(j*p - l*n) + h*(j*o - k*n))
         - b*(e*(k*p - l*o) - g*(i*p - l*m) + h*(i*o - k*m))
         + c*(e*(j*p - l*n) - f*(i*p - l*m) + h*(i*n - j*m))
         - d*(e*(j*o - k*n) - f*(i*o - k*m) + g*(i*n - j*m));
}

Mat4 invert(Mat4 matrix) {
    // using these labels to simplify the later expression
    // [ a  b  c  d ]
    // [ e  f  g  h ]
    // [ i  j  k  l ]
    // [ m  n  o  p ]

    float a = matrix.values[0][0];
    float b = matrix.values[0][1];
    float c = matrix.values[0][2];
    float d = matrix.values[0][3];

    float e = matrix.values[1][0];
    float f = matrix.values[1][1];
    float g = matrix.values[1][2];
    float h = matrix.values[1][3];

    float i = matrix.values[2][0];
    float j = matrix.values[2][1];
    float k = matrix.values[2][2];
    float l = matrix.values[2][3];

    float m = matrix.values[3][0];
    float n = matrix.values[3][1];
    float o = matrix.values[3][2];
    float p = matrix.values[3][3];
    // [ a  b  c  d ]
    // [ e  f  g  h ]
    // [ i  j  k  l ]
    // [ m  n  o  p ]
    float det = DET4(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p);

    // cofactor matrix, transposed
    Mat4 result;

    result.values[0][0] = DET3(f,g,h, j,k,l, n,o,p) / det;
    result.values[1][0] = DET3(e,g,h, i,k,l, m,o,p) / -det;
    result.values[2][0] = DET3(e,f,h, i,j,l, m,n,p) / det;
    result.values[3][0] = DET3(e,f,g, i,j,k, m,n,o) / -det;

    result.values[0][1] = DET3(b,c,d, j,k,l, n,o,p) / -det;
    result.values[1][1] = DET3(a,c,d, i,k,l, m,o,p) / det;
    result.values[2][1] = DET3(a,b,d, i,j,l, m,n,p) / -det;
    result.values[3][1] = DET3(a,b,c, i,j,k, m,n,o) / det;

    result.values[0][2] = DET3(b,c,d, f,g,h, n,o,p) / det;
    result.values[1][2] = DET3(a,c,d, e,g,h, m,o,p) / -det;
    result.values[2][2] = DET3(a,b,d, e,f,h, m,n,p) / det;
    result.values[3][2] = DET3(a,b,c, e,f,g, m,n,o) / -det;

    result.values[0][3] = DET3(b,c,d, f,g,h, j,k,l) / -det;
    result.values[1][3] = DET3(a,c,d, e,g,h, i,k,l) / det;
    result.values[2][3] = DET3(a,b,d, e,f,h, i,j,l) / -det;
    result.values[3][3] = DET3(a,b,c, e,f,g, i,j,k) / det;

    return result;
}


// debug function that prints out the matrix
void printMatrix(Mat4 m) {
    for(int i = 0; i < 4; i++)
        printf("[ %f %f %f %f ]\n", m.values[i][0], m.values[i][1], m.values[i][2], m.values[i][3]);
    printf("(Mat4)\n");
}

// debug function that prints out a Vector4
void printVector(Vector4 v) {
    printf("[ %f %f %f %f ]\n(Vec4)\n", v.x, v.y, v.z, v.w);
}

// debug function that prints out a Vector3
void printVector(Vector3 v) {
    printf("[ %f %f %f ]\n", v.x, v.y, v.z);
}
