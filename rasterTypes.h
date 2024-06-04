typedef unsigned char byte;

typedef struct{
    float x, y, z;
}Rvec3;

typedef struct{
    byte r, g, b;
}color;

//

typedef struct{
    byte* inputColors;
    byte* colors;
    //byte* indexBuffer;
    int length;
}colorBuffer;

typedef struct{
    float* inputVertices;
    float* vertices;
    byte* indexBuffer;
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
    //stride of 4
    int* scanlineSpec;
    int height;
    int width;
}renderContext;