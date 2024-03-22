#include <SDL.h>
#include <math.h>
#include "types.h"

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
void drawlines(framebuffer* fb, triangle tri){
    //one to two
    float dx1 = tri.one.x - tri.two.x;
    float dy1 = tri.one.y - tri.two.y;
    float len1 = sqrt(dx1 * dx1+ dy1 * dy1);
    float angle1 = atan2(dy1, dx1);
    for(float i = 0; i< len1; i++){
        colorPixel(fb,
        round(tri.two.x + cos(angle1) * i),
        round(tri.two.y + sin(angle1) * i), 
        255, 255, 255);
    }
    //two to three
    float dx2 = tri.two.x - tri.three.x;
    float dy2 = tri.two.y - tri.three.y;
    float len2 = sqrt(dx2 * dx2 + dy2 * dy2);
    float angle2 = atan2(dy2, dx2);
    for(float i = 0; i < len2; i++){
        colorPixel(fb,
        round(tri.three.x + cos(angle2) * i),
        round(tri.three.y + sin(angle2) * i), 
        255, 255, 255);
    }
    //thee to one
    float dx3 = tri.one.x - tri.three.x;
    float dy3 = tri.one.y - tri.three.y;
    float len3 = sqrt(dx3 * dx3 + dy3 * dy3);
    float angle3 = atan2(dy3, dx3);
    for(float i = 0; i < len3; i++){
        colorPixel(fb,
        round(tri.three.x + cos(angle3) * i),
        round(tri.three.y + sin(angle3) * i), 
        255, 255, 255);
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