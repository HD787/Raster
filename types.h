typedef unsigned char byte;

typedef struct{
    float x, y, z;
}Rvec3;

typedef struct{
    byte r, g, b;
}color;


typedef struct{
    color* colors;
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

typedef struct{
    byte* pixels;
    int height;
    int width;
}frameBuffer;

typedef struct{
    frameBuffer* fb;
    vertexBuffer* vb;
    normalBuffer* nb;
    colorBuffer* cb;
}renderContext;