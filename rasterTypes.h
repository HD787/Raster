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
    byte* inputColors;
    byte* colors;
    int length;
}colorBuffer;

typedef struct{
    float* inputVertices;
    float* vertices;
    int length;
}vertexBuffer;

//idk if this should be here as its not used in the rasterizer
//but maybe it will be in the future
typedef struct{
    float* normals;
    int length;
}normalBuffer;


typedef struct{
    byte* frameBuffer;
    int* zBuffer;
    int** scanlineSpec;
    int height;
    int width;
}renderContext;