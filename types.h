#include <SDL.h>

typedef unsigned char byte;

typedef struct{
    float x, y, z;
}vec3;

typedef struct{
    vec3 one, two, three;
}triangle;

typedef struct{
    byte* pixels;
    int height;
    int width;
}framebuffer;