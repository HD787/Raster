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

        int m1 = 2 * vb->vertices[i + Y1] - vb->vertices[i + Y2];
        int slopeError1 = m1 - (vb->vertices[i + X1] - vb->vertices[i + X2]); 
        for(int x = vb->vertices[i + X1], y = vb->vertices[i + Y1]; x <= vb->vertices[i + X2]; x++){
            colorPixel(fb, x, y, 255, 255, 255);
            slopeError1 += m1;
            if(slopeError1 >= 0){
                y++;
                slopeError1 -= 2 * (vb->vertices[i + X1] - vb->vertices[i + X2]);
            }
        }
        //two to three
        int m2 = 2 * vb->vertices[i + Y2] - vb->vertices[i + Y3];
        int slopeError2 = m2 - (vb->vertices[i + X2] - vb->vertices[i + X3]);
        for(int x = vb->vertices[i + X2], y = vb->vertices[i + Y2]; x <= vb->vertices[i + X3]; x++){
            colorPixel(fb, x, y, 255, 255, 255);
            slopeError2 += m2;
            if(slopeError2 >= 0){
                y++;
                slopeError2 -= 2 * (vb->vertices[i + X2] - vb->vertices[i + X3]);
            }
        }
        //three to one
        int m3 = 2 * vb->vertices[i + Y1] - vb->vertices[i + Y3];
        int slopeError3 = m3 - (vb->vertices[i + X1] - vb->vertices[i + X3]);
        for(int x = vb->vertices[i + X1], y = vb->vertices[i + Y1]; x <= vb->vertices[i + X3]; x++){
            colorPixel(fb, x, y, 255, 255, 255);
            slopeError3 += m3;
            if(slopeError3 >= 0){
                y++;
                slopeError3 -= 2 * (vb->vertices[i + X1] - vb->vertices[i + X3]);
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