#include <math.h>
#include <stdlib.h>
//delete SDL from here later only for debugging purposes
#include <SDL.h>
#include "rasterTypes.h"
#include "rasterMath.h"
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


renderContext* createRenderContext(int width, int height){
    renderContext* temp = malloc(sizeof(renderContext));

    temp->frameBuffer = calloc(height * width * 3, sizeof(byte));

    temp->zBuffer = malloc(sizeof(int) * width * height);
    for (int i = 0; i < width * height; i ++)
        temp->zBuffer[i] = 1000;

    temp->scanlineSpec = malloc(sizeof(int *) * height* 4);
    for (int j = 0; j < height * 4; j += 4){
        temp->scanlineSpec[j] = -1000000;
        temp->scanlineSpec[j + 1] = -1000000;
        temp->scanlineSpec[j + 2] = -1000000;
        temp->scanlineSpec[j + 3] = -1000000;
    }

    temp->height = height;
    temp->width = width;
    return temp;
}

void deleteRenderContext(renderContext* rc){
    free(rc->frameBuffer);
    free(rc->zBuffer);
    free(rc->scanlineSpec);
}

//size is not count/stride
//thats a division sign btw
//but maybe it should be
vertexBuffer* createVertexBuffer(int size){
    vertexBuffer* temp = malloc(sizeof(vertexBuffer));
    temp->inputVertices = malloc(sizeof(float) * size);
    temp->vertices = malloc(sizeof(float) * size);
    temp->indexBuffer = malloc(sizeof(byte) * size/3);
    temp->length = size;
    return temp;
}

void cleanIndexBuffer(vertexBuffer* vb){
    //this may not be necessary tbh
    for(int i = 0; i < vb->length/3; i++){
        vb->indexBuffer[i] = 0;
    }
}

void deleteVertexBuffer(vertexBuffer* vb){
    free(vb->indexBuffer);
    free(vb->vertices);
    free(vb->inputVertices);
    free(vb);
}

colorBuffer* createColorBuffer(int size){
    colorBuffer* temp = malloc(sizeof(colorBuffer));
    temp->inputColors = malloc(sizeof(byte) * size);
    temp->colors = malloc(sizeof(byte) * size);
    temp->length = size;
    return temp;
}

void deleteColorBuffer(colorBuffer* cb){
    free(cb->colors);
    free(cb->inputColors);
    free(cb);
}



void cleanRenderContext(renderContext* rc){
    cleanFrameBuffer(rc);
    cleanzBuffer(rc);
}

void rasterize(renderContext* rc, vertexBuffer *vb, colorBuffer* cb)
{   
    SDL_Init(SDL_INIT_TIMER);
    Uint32  linesStart, linesEnd,
            scanLineStart, scanLineEnd,
            cleanStart, cleanEnd,
            loadStart, loadEnd;
    Uint32 lineInterval = 0;
    Uint32 scanLineInterval = 0;
    Uint32 cleanInterval = 0;
    Uint32 loadInterval = 0;

    for (int i = 0; i < vb->length; i += 9)
    {
        //commenting out this line disables backface culling
        //if(vb->indexBuffer[i/3] == 0) { continue;}
        cleanStart = SDL_GetTicks();
        cleanScanlineSpec(rc);
        cleanEnd = SDL_GetTicks();
        loadStart = SDL_GetTicks();
        color clr;
        clr.r = cb->colors[i]; clr.g = cb->colors[i + 1]; clr.b = cb->colors[i + 2];

        Rvec3 first, second, third;
        first.x = vb->vertices[i + X1];  first.y  = vb->vertices[i + Y1]; first.z = vb->vertices[i + Z1];
        second.x = vb->vertices[i + X2]; second.y = vb->vertices[i + Y2]; second.z = vb->vertices[i + Z2];
        third.x = vb->vertices[i + X3];  third.y = vb->vertices[i + Y3];  third.z =  vb->vertices[i + Z3];
        loadEnd = SDL_GetTicks();
        // //two vertices behind viewport
        // //first and second
        // if(first.z < 0 && second.z < 0 && third.z > 0){

        // }
        // //first and third
        // if(first.z < 0 && second.z > 0 && third.z < 0){

        // }
        // //second and third
        // if(first.z > 0 && second.z < 0 && third.z < 0){

        // }

        // //one vertex behind the viewport
        // //first
        // if(first.z < 0 && second.z > 0 && third.z > 0){
        //     Rvec3 one, two;
        //     one = interpolatePoint(first, second);
        //     two = interpolatePoint(first, third);
        // }
        // //second
        // if(first.z > 0 && second.z < 0 && third.z > 0){
        //     Rvec3 one, two;
        //     one = interpolatePoint(first, second);
        //     two = interpolatePoint(first, third);
        // }
        // //third
        // if(first.z > 0 && second.z > 0 && third.z < 0){
        //     Rvec3 one, two;
        //     one = interpolatePoint(first, second);
        //     two = interpolatePoint(first, third);
            
        //     drawLines(rc, clr,)
        //     drawLines(rc, clr)
        //     drawLines(rc, clr)
        //     drawlines(rc, clr)
        //     drawlines(rc, clr)

        //     continue;
        // }
        linesStart = SDL_GetTicks();
        drawLines(rc, clr,
        first.x, first.y, first.z,
        second.x, second.y, second.z);

        drawLines(rc, clr,
        second.x, second.y, second.z,
        third.x, third.y, third.z);

        drawLines(rc, clr,
        first.x, first.y, first.z,
        third.x, third.y, third.z);
        linesEnd = SDL_GetTicks();
        // for(int i = 0; i <= rc->height * 4; i += 4)
        // printf("%d, %d, %d, %d\n", rc->scanlineSpec[i], rc->scanlineSpec[i + 1], rc->scanlineSpec[i + 2], rc->scanlineSpec[i + 3]);
        scanLineStart = SDL_GetTicks();
        scanline(rc, clr);
        scanLineEnd = SDL_GetTicks();
        
        loadInterval += loadEnd - loadStart;
        cleanInterval += cleanEnd - cleanStart;
        lineInterval += linesEnd - linesStart;
        scanLineInterval += scanLineEnd - scanLineStart;
    }
    
    
    //printf("lines: %d, scan: %d, clean: %d, load: %d\n", lineInterval, scanLineInterval, cleanInterval, loadInterval);
}