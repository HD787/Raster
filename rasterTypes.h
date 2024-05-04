typedef unsigned char byte;

typedef struct{
    float x, y, z;
}Rvec3;

typedef struct{
    byte r, g, b;
}color;


typedef struct{
    //color* colors;
    //i think this will be better as we can navigate it in the same way we would a vb
    byte* colors;
    int length;
}colorBuffer;

typedef struct{
    float* vertices;
    int length;
}vertexBuffer;

typedef struct{
    float* normals;
    int length;
}normalBuffer;

//this is now deprecated
// typedef struct{
//     byte* pixels;
//     int height;
//     int width;
// }frameBuffer;


typedef struct{
    byte* frameBuffer;
    int* zBuffer;
    int** scanlineSpec;
    int height;
    int width;
}renderContext;