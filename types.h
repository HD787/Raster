typedef unsigned char byte;

typedef struct{
    float x, y, z;
}Rvec3;

// typedef struct{
//     vec4 one, two, three;
// }triangle;

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
}framebuffer;