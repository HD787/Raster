#include <math.h>
#include "types.h"
#include "rasterizerPrivateFunctions.h"

#define X1 0
#define Y1 1
#define Z1 2
#define X2 3
#define Y2 4
#define Z2 5
#define X3 6
#define Y3 7
#define Z3 8

framebuffer *createFrameBuffer(int width, int height)
{
    framebuffer *temp = malloc(sizeof(framebuffer));
    // magic number 3 represents 3 rgb values;
    temp->pixels = calloc(height * width * 3, sizeof(byte));
    temp->height = height;
    temp->width = width;
    return temp;
}


void deleteFrameBuffer(framebuffer* fb)
{
    free(fb->pixels);
    free(fb);
}


void rasterize(framebuffer* fb, vertexBuffer *vb)
{   
    cleanFrameBuffer(fb);
    int *zBuffer = malloc(sizeof(int) * fb->width * fb->height);
    for (int i = 0; i < fb->width * fb->height; i ++)
        zBuffer[i] = 1000;

    for (int i = 0; i < vb->length; i += 9)
    {
        int **scanlineSpec = malloc(sizeof(int *) * fb->height);
        for (int j = 0; j < fb->height; j++)
        {
            scanlineSpec[j] = malloc(4 * sizeof(int));
            scanlineSpec[j][0] = -1000000;
            scanlineSpec[j][1] = -1000000;
            scanlineSpec[j][2] = -1000000;
            scanlineSpec[j][3] = -1000000;
        }
        Rvec3 first, second, third;
        first.x = vb->vertices[i + X1];  first.y  = vb->vertices[i + Y1]; first.z = vb->vertices[i + Z1];
        second.x = vb->vertices[i + X2]; second.y = vb->vertices[i + Y2]; second.z = vb->vertices[i + Z2];
        third.x = vb->vertices[i + X3];  third.y = vb->vertices[i + Y3];  third.z =  vb->vertices[i + Z3]; 

        drawLines(fb, scanlineSpec, zBuffer, 
        first.x,  first.y,  first.z,
        second.x, second.y, second.z);

        drawLines(fb, scanlineSpec, zBuffer, 
        second.x, second.y, second.z,
        third.x,  third.y,  third.x);

        drawLines(fb, scanlineSpec, zBuffer, 
        first.x, first.y, first.z,
        third.x,  third.y, third.z);


        scanline(fb, scanlineSpec, zBuffer, 255, 255, 255);
        for (int i = 0; i < fb->height; i++)
        {
            free(scanlineSpec[i]);
        }
        free(scanlineSpec);
    }
    //renderFrameBuffer(fb, zBuffer);
    free(zBuffer);
}