typedef unsigned char byte;

typedef struct{
    float x, y, z, w;
}vec4;

typedef struct{
    vec4 one, two, three;
}triangle;

typedef struct{
    int* vertices;
    int length;
}vertexBuffer;

typedef struct{
    byte* pixels;
    int height;
    int width;
}framebuffer;