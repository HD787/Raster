#include <SDL.h>
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


framebuffer* createFrameBuffer(int width, int height){
    framebuffer* temp = malloc(sizeof(framebuffer));
    //magic number 3 represents 3 rgb values;
    temp->pixels = calloc(height * width * 3, sizeof(byte));
    temp->height = height;
    temp->width = width;
    return temp;
}

void deleteFrameBuffer(framebuffer* fb){
    free(fb->pixels);
    free(fb);
}

void colorPixel(framebuffer* fb, int x, int y, byte r, byte g, byte b){
    int val = (y * fb->width + x) * 3;
    fb->pixels[val] = r;
    fb->pixels[val +1] = g;
    fb->pixels[val + 2] = b;
}

void scanline(framebuffer* fb, int** arr, byte r, byte g, byte b){
    for(int i = 0; i <  fb->height; i++){
        for(int j = arr[i][0]; j < arr[i][1]; j++){
            if(arr[i][0] != -1 && arr[i][0] != -1){
                colorPixel(fb, j, i, 255, 255, 255);
            }
        }
    }
}

void drawlines(framebuffer* fb, vertexBuffer* vb){
    for(int i = 0; i < vb->length; i += 9){
        int** scanlineSpec = malloc(sizeof(int*) * fb->height);
        for(int j = 0; j < fb->height; j++){
            scanlineSpec[j] = malloc(2 * sizeof(int));
            scanlineSpec[j][0] = -1;
            scanlineSpec[j][1] = -1; 
        }
        int dx1 = abs(vb->vertices[i + X1] - vb->vertices[i + X2]);
        int dy1 = abs(vb->vertices[i + Y1] - vb->vertices[i + Y2]);
        int sx1, sy1;
        if(vb->vertices[i + X1] < vb->vertices[i + X2]) sx1 = 1; else sx1 = -1;
        if(vb->vertices[i + Y1] < vb->vertices[i + Y2]) sy1 = 1; else sy1 = -1;
        int slopeError1 = dx1 - dy1; 
        for(int x = vb->vertices[i + X1], y = vb->vertices[i + Y1]; x != vb->vertices[i + X2];){
            colorPixel(fb, x, y, 255, 255, 255);
            if(x < scanlineSpec[y][0] || scanlineSpec[y][0] == -1) scanlineSpec[y][0] = x;
            if(x > scanlineSpec[y][1] || scanlineSpec[y][1] == -1) scanlineSpec[y][1] = x;
            int e2 = slopeError1 * 2;
            if(slopeError1 >= -dy1){
                slopeError1 -= dy1;
                x += sx1;
            }
            if(e2 < dx1){
                slopeError1 += dx1;
                y += sy1;
            }
        }
        //two to three
        int dx2 = abs(vb->vertices[i + X2] - vb->vertices[i + X3]);
        int dy2 = abs(vb->vertices[i + Y2] - vb->vertices[i + Y3]);
        int sx2, sy2;
        if(vb->vertices[i + X2] < vb->vertices[i + X3]) sx2 = 1; else sx2 = -1;
        if(vb->vertices[i + Y2] < vb->vertices[i + Y3]) sy2 = 1; else sy2 = -1;
        int slopeError2 = dx2 - dy2; 
        for(int x = vb->vertices[i + X2], y = vb->vertices[i + Y2]; x != vb->vertices[i + X3];){
            colorPixel(fb, x, y, 255, 255, 255);
            if(x < scanlineSpec[y][0] || scanlineSpec[y][0] == -1) scanlineSpec[y][0] = x;
            if(x > scanlineSpec[y][1] || scanlineSpec[y][1] == -1) scanlineSpec[y][1] = x;
            int e2 = slopeError2 * 2;
            if(slopeError2 >= -dy2){
                slopeError2 -= dy2;
                x += sx2;
            }
            if(e2 < dx2){
                slopeError2 += dx2;
                y += sy2;
            }
        }
        //three to one
        int dx3 = abs(vb->vertices[i + X1] - vb->vertices[i + X3]);
        int dy3 = abs(vb->vertices[i + Y1] - vb->vertices[i + Y3]);
        int sx3, sy3;
        if(vb->vertices[i + X1] < vb->vertices[i + X3]) sx3 = 1; else sx3 = -1;
        if(vb->vertices[i + Y1] < vb->vertices[i + Y3]) sy3 = 1; else sy3 = -1;
        int slopeError3 = dx3 - dy3; 
        for(int x = vb->vertices[i + X1], y = vb->vertices[i + Y1]; x != vb->vertices[i + X3];){
            colorPixel(fb, x, y, 255, 255, 255);
            if(x < scanlineSpec[y][0] || scanlineSpec[y][0] == -1) scanlineSpec[y][0] = x;
            if(x > scanlineSpec[y][1] || scanlineSpec[y][1] == -1) scanlineSpec[y][1] = x;
            int e2 = slopeError3 * 2;
            if(slopeError3 >= -dy3){
                slopeError3 -= dy3;
                x += sx3;
            }
            if(e2 < dx3){
                slopeError3 += dx3;
                y += sy3;
            }
        }  
        scanline(fb, scanlineSpec, 255, 255, 255);
        for(int i = 0; i < fb->height; i++){
            free(scanlineSpec[i]);
        }
        free(scanlineSpec);
    }
     
}