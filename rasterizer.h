#include <math.h>
#include "types.h"

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

void deleteFrameBuffer(framebuffer *fb)
{
    free(fb->pixels);
    free(fb);
}

void colorPixel(framebuffer *fb, int x, int y, byte r, byte g, byte b)
{
    int val = (y * fb->width + x) * 3;
    fb->pixels[val] = r;
    fb->pixels[val + 1] = g;
    fb->pixels[val + 2] = b;
}

//this will need to essentially be redone to determint the depth of the drawn pixels
void scanline(framebuffer *fb, int **arr, int* zBuffer, byte r, byte g, byte b)
{
    for (int i = 0; i < fb->height; i++)
    {
        for (int j = arr[i][0]; j < arr[i][1]; j++)
        {
            if (arr[i][0] != -1 && arr[i][0] != -1)
            {
                colorPixel(fb, j, i, 255, 255, 255);
            }
        }
    }
}

void drawLines(framebuffer *fb, int **scanlineSpec, int *zBuffer, int x1, int y1, int z1, int x2, int y2, int z2)
{
    // this code is shamefully stolen from Ishan Khandelwals
    // as sometimes progress is the valuable thing in a project

    // this will essentially be the draw pixel call
    if (z1 > zBuffer[(y1 * fb->height + x1) * 4])
        zBuffer[(y1 * fb->height + x1) * 4] = z1;
    // this will be for scanline, obviously
    if (x1 < scanlineSpec[y1][0] || scanlineSpec[y1][0] == -1)
        scanlineSpec[y1][0] = x1;
    if (x1 > scanlineSpec[y1][1] || scanlineSpec[y1][1] == -1)
        scanlineSpec[y1][1] = x1;
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int dz = abs(z2 - z1);
    int xs, ys, zs;
    if (x2 > x1)
        xs = 1;
    else
        xs = -1;

    if (y2 > y1)
        ys = 1;
    else
        ys = -1;

    if (z2 > z1)
        zs = 1;
    else
        zs = -1;

    // Driving axis is X-axis"
    if (dx >= dy && dx >= dz)
    {
        int p1 = 2 * dy - dx;
        int p2 = 2 * dz - dx;
        while (x1 != x2)
        {
            x1 += xs;
            if (p1 >= 0)
            {
                y1 += ys;
                p1 -= 2 * dx;
            }
            if (p2 >= 0)
            {
                z1 += zs;
                p2 -= 2 * dx;
            }
            p1 += 2 * dy;
            p2 += 2 * dz;
            if (z1 > zBuffer[(y1 * fb->height + x1) * 4])
                zBuffer[(y1 * fb->height + x1) * 4] = z1;
            if (x1 < scanlineSpec[y1][0] || scanlineSpec[y1][0] == -1)
                scanlineSpec[y1][0] = x1;
            if (x1 > scanlineSpec[y1][1] || scanlineSpec[y1][1] == -1)
                scanlineSpec[y1][1] = x1;
        }

        // Driving axis is Y-axis"
    }
    else if (dy >= dx && dy >= dz)
    {
        int p1 = 2 * dx - dy;
        int p2 = 2 * dz - dy;
        while (y1 != y2)
        {
            y1 += ys;
            if (p1 >= 0)
            {
                x1 += xs;
                p1 -= 2 * dy;
            }
            if (p2 >= 0)
            {
                z1 += zs;
                p2 -= 2 * dy;
            }
            p1 += 2 * dx;
            p2 += 2 * dz;
            if (z1 > zBuffer[(y1 * fb->height + x1) * 4])
                zBuffer[(y1 * fb->height + x1) * 4] = z1;
            if (x1 < scanlineSpec[y1][0] || scanlineSpec[y1][0] == -1)
                scanlineSpec[y1][0] = x1;
            if (x1 > scanlineSpec[y1][1] || scanlineSpec[y1][1] == -1)
                scanlineSpec[y1][1] = x1;
        }

        // Driving axis is Z-axis"
    }
    else
    {
        int p1 = 2 * dy - dz;
        int p2 = 2 * dx - dz;
        while (z1 != z2)
        {
            z1 += zs;
            if (p1 >= 0)
            {
                y1 += ys;
                p1 -= 2 * dz;
            }
            if (p2 >= 0)
            {
                x1 += xs;
                p2 -= 2 * dz;
            }
            p1 += 2 * dy;
            p2 += 2 * dx;
            if (z1 > zBuffer[(y1 * fb->height + x1) * 4])
                zBuffer[(y1 * fb->height + x1) * 4] = z1;
            if (x1 < scanlineSpec[y1][0] || scanlineSpec[y1][0] == -1)
                scanlineSpec[y1][0] = x1;
            if (x1 > scanlineSpec[y1][1] || scanlineSpec[y1][1] == -1)
                scanlineSpec[y1][1] = x1;
        }
    }
}

void rasterize(framebuffer *fb, vertexBuffer *vb)
{
    int *zBuffer = malloc(sizeof(int) * fb->width * fb->height * 4);
    for (int i = 0; i < fb->width * fb->height * 4; i += 4)
    {
        zBuffer[i] = 1000;
    }
    for (int i = 0; i < vb->length; i += 9)
    {
        int **scanlineSpec = malloc(sizeof(int *) * fb->height);
        for (int j = 0; j < fb->height; j++)
        {
            scanlineSpec[j] = malloc(2 * sizeof(int));
            scanlineSpec[j][0] = -1;
            scanlineSpec[j][1] = -1;
        }
        drawLines(fb, scanlineSpec, zBuffer, 
        vb->vertices[i + X1], vb->vertices[i + Y1], vb->vertices[i + Z1],
        vb->vertices[i + X2], vb->vertices[i + Y2], vb->vertices[i + Z2]);

        drawLines(fb, scanlineSpec, zBuffer, 
        vb->vertices[i + X2], vb->vertices[i + Y2], vb->vertices[i + Z3],
        vb->vertices[i + X3], vb->vertices[i + Y3], vb->vertices[i + Z3]);

        drawLines(fb, scanlineSpec, zBuffer, 
        vb->vertices[i + X1], vb->vertices[i + Y1], vb->vertices[i + Z1],
        vb->vertices[i + X3], vb->vertices[i + Y3], vb->vertices[i + Z3]);


        scanline(fb, scanlineSpec, zBuffer, 255, 255, 255);
        for (int i = 0; i < fb->height; i++)
        {
            free(scanlineSpec[i]);
        }
        free(scanlineSpec);
    }
    free(zBuffer);
}