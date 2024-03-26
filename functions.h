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

void drawlines(framebuffer* fb, vertexBuffer* vb){
    //one to two
    for(int i = 0; i < vb->length; i += 9){
        int** scanlineSpec = malloc(sizeof(int*) * fb->height);


        int dx1 = abs(vb->vertices[i + X1] - vb->vertices[i + X2]);
        int dy1 = abs(vb->vertices[i + Y1] - vb->vertices[i + Y2]);
        int sx1, sy1;
        if(vb->vertices[i + X1] < vb->vertices[i + X2]) sx1 = 1; else sx1 = -1;
        if(vb->vertices[i + Y1] < vb->vertices[i + Y2]) sy1 = 1; else sy1 = -1;
        int slopeError1 = dx1 - dy1; 
        for(int x = vb->vertices[i + X1], y = vb->vertices[i + Y1]; x != vb->vertices[i + X2];){
            colorPixel(fb, x, y, 255, 255, 255);
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
    }    
}

//this will likely be called from inside the line drawing function which draws the triangles
void scanline(int* arr, byte r, byte g, byte b){}

void drawLine(SDL_Renderer* renderer,vec4* a, vec4* b){
    float dx = b->x - a->x;
    float dy = b->y - a->y;
    float len = sqrt(dx * dx + dy * dy);
    float angle = atan2(dy, dx);
    for(float i = 0; i < len; i++){
        SDL_RenderDrawPoint(renderer,
        a->x + cos(angle) * i,
        a->y + sin(angle) * i);
    }
}