#include <math.h>
#include <stdlib.h>
#include <stdio.h>
//delete SDL from here later only for debugging purposes
//#include <SDL.h>
#include "rasterTypes.h"
#include "rasterMath.c"
#include "rasterizerPrivateFunctions.c"

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

    temp->scanlineSpec = malloc(sizeof(int) * height * 4);
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

renderContext* createRenderContextFloat(int width, int height){
    renderContext* temp = malloc(sizeof(renderContext));

    temp->frameBuffer = calloc(height * width * 3, sizeof(byte));

    temp->zBufferFloat = malloc(sizeof(int) * width * height);
    for (int i = 0; i < width * height; i ++)
        temp->zBufferFloat[i] = 1000;

    temp->scanlineSpecFloat = malloc(sizeof(int) * height * 4);
    for (int j = 0; j < height * 4; j += 4){
        temp->scanlineSpecFloat[j] = -1000000;
        temp->scanlineSpecFloat[j + 1] = -1000000;
        temp->scanlineSpecFloat[j + 2] = -1000000;
        temp->scanlineSpecFloat[j + 3] = -1000000;
    }

    temp->height = height;
    temp->width = width;
    return temp;
}

void deleteRenderContext(renderContext* rc){
    free(rc->frameBuffer);
    free(rc->zBuffer);
    free(rc->scanlineSpec);
    free(rc);
}

void deleteRenderContextFloat(renderContext* rc){
    free(rc->frameBuffer);
    free(rc->zBufferFloat);
    free(rc->scanlineSpecFloat);
    free(rc);
}

//size is the number of vertices not triangles, this may have been a poor decision
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

void cleanRenderContextFloat(renderContext*rc){
    cleanFrameBuffer(rc);
    cleanzBufferFloat(rc);
}

void rasterize(renderContext* rc, vertexBuffer *vb, colorBuffer* cb){   
    for (int i = 0; i < vb->length; i += 9){
        //commenting out this line disables backface culling
        //if(vb->indexBuffer[i/3] == 0) { continue;}
        cleanScanlineSpec(rc);
        color clr;
        clr.r = cb->colors[i]; clr.g = cb->colors[i + 1]; clr.b = cb->colors[i + 2];

        Rvec3 first, second, third;
        first.x = vb->vertices[i + X1];  first.y  = vb->vertices[i + Y1]; first.z = vb->vertices[i + Z1];
        second.x = vb->vertices[i + X2]; second.y = vb->vertices[i + Y2]; second.z = vb->vertices[i + Z2];
        third.x = vb->vertices[i + X3];  third.y = vb->vertices[i + Y3];  third.z =  vb->vertices[i + Z3];

        drawLines(rc, clr, first, second);
        drawLines(rc, clr, second, third);
        drawLines(rc, clr, first, third);
        //scanline(rc, clr);
    } 
}

void rasterizeFloat(renderContext* rc, vertexBuffer *vb, colorBuffer* cb){   
    for (int i = 0; i < vb->length; i += 9){
        //commenting out this line disables backface culling
        //if(vb->indexBuffer[i/3] == 0) { continue;}
        cleanScanlineSpecFloat(rc);
        color clr;
        clr.r = cb->colors[i]; clr.g = cb->colors[i + 1]; clr.b = cb->colors[i + 2];

        Rvec3 first, second, third;
        first.x = vb->vertices[i + X1];  first.y  = vb->vertices[i + Y1]; first.z = vb->vertices[i + Z1];
        second.x = vb->vertices[i + X2]; second.y = vb->vertices[i + Y2]; second.z = vb->vertices[i + Z2];
        third.x = vb->vertices[i + X3];  third.y = vb->vertices[i + Y3];  third.z =  vb->vertices[i + Z3];

        drawLinesFloat(rc, clr, first, second);
        drawLinesFloat(rc, clr, second, third);
        drawLinesFloat(rc, clr, first, third);
        scanlineFloat(rc, clr);
    } 
}

void rasterizeChunk(renderContext* rc, vertexBuffer *vb, colorBuffer* cb, unsigned char* indexBuffer){ 
    int c = 0;
    for (int j = 0; j < vb->length; j += 108){
        if(indexBuffer[j/108] == 0) {continue;}
        c++;
        for(int i = 0; i < 108; i += 9){
            cleanScanlineSpec(rc);
            color clr;
            clr.r = cb->colors[j + i]; clr.g = cb->colors[j + i + 1]; clr.b = cb->colors[j + i + 2];

            Rvec3 first, second, third;
            first.x = vb->vertices[j + i + X1];  first.y  = vb->vertices[j + i + Y1]; first.z = vb->vertices[j + i + Z1];
            second.x = vb->vertices[j + i + X2]; second.y = vb->vertices[j + i + Y2]; second.z = vb->vertices[j + i + Z2];
            third.x = vb->vertices[j + i + X3];  third.y = vb->vertices[j + i + Y3];  third.z =  vb->vertices[j + i + Z3];
            drawLines(rc, clr, first, second);
            drawLines(rc, clr, second, third);
            drawLines(rc, clr, first, third);
            scanline(rc, clr);
        }
    }
}

void rasterizeChunkFloat(renderContext* rc, vertexBuffer *vb, colorBuffer* cb, unsigned char* indexBuffer){ 
    int c = 0;
    for (int j = 0; j < vb->length; j += 108){
        if(indexBuffer[j/108] == 0) {continue;}
        c++;
        for(int i = 0; i < 108; i += 9){
            cleanScanlineSpecFloat(rc);
            color clr;
            clr.r = cb->colors[j + i]; clr.g = cb->colors[j + i + 1]; clr.b = cb->colors[j + i + 2];

            Rvec3 first, second, third;
            first.x = vb->vertices[j + i + X1];  first.y  = vb->vertices[j + i + Y1]; first.z = vb->vertices[j + i + Z1];
            second.x = vb->vertices[j + i + X2]; second.y = vb->vertices[j + i + Y2]; second.z = vb->vertices[j + i + Z2];
            third.x = vb->vertices[j + i + X3];  third.y = vb->vertices[j + i + Y3];  third.z =  vb->vertices[j + i + Z3];
            drawLinesFloat(rc, clr, first, second);
            drawLinesFloat(rc, clr, second, third);
            drawLinesFloat(rc, clr, first, third);
            scanlineFloat(rc, clr);
        }
    }
}

/* ALTERNATE RGBA FUNCTIONS */

void cleanRenderContext_RGBA(renderContext* rc){
    cleanFrameBuffer_RGBA(rc);
    cleanzBuffer(rc);
}

renderContext* createRenderContext_RGBA(int width, int height){
   renderContext* temp = malloc(sizeof(renderContext));
    //the RGBA version has a stride of 4 in the framebuffer, no other differences.
    temp->frameBuffer = malloc(sizeof(byte) * height * width * 4);
    for(int i = 0; i < height * width * 4; i++){
        if(i % 4 == 0)
        temp->frameBuffer[i] = 255;
        temp->frameBuffer[i] = 0;
    }

    temp->zBuffer = malloc(sizeof(int) * width * height);
    for (int i = 0; i < width * height; i ++)
        temp->zBuffer[i] = 1000;

    temp->scanlineSpec = malloc(sizeof(int) * height * 4);
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

void rasterize_RGBA(renderContext* rc, vertexBuffer *vb, colorBuffer* cb){   
    for (int i = 0; i < vb->length; i += 9){
        //commenting out this line disables backface culling
        //if(vb->indexBuffer[i/3] == 0) { continue;}
        cleanScanlineSpec(rc);
        color clr;
        clr.r = cb->colors[i]; clr.g = cb->colors[i + 1]; clr.b = cb->colors[i + 2];

        Rvec3 first, second, third;
        first.x = vb->vertices[i + X1];  first.y  = vb->vertices[i + Y1]; first.z = vb->vertices[i + Z1];
        second.x = vb->vertices[i + X2]; second.y = vb->vertices[i + Y2]; second.z = vb->vertices[i + Z2];
        third.x = vb->vertices[i + X3];  third.y = vb->vertices[i + Y3];  third.z =  vb->vertices[i + Z3];

        drawLines_RGBA(rc, clr, first, second);
        drawLines_RGBA(rc, clr, second, third);
        drawLines_RGBA(rc, clr, first, third);

        scanline_RGBA(rc, clr);
    }
    
}

void rasterizeNoScanline_RGBA(renderContext* rc, vertexBuffer *vb, colorBuffer* cb){   
    for (int i = 0; i < vb->length; i += 9){
        //commenting out this line disables backface culling
        //if(vb->indexBuffer[i/3] == 0) { continue;}
        cleanScanlineSpec(rc);
        color clr;
        clr.r = cb->colors[i]; clr.g = cb->colors[i + 1]; clr.b = cb->colors[i + 2];

        Rvec3 first, second, third;
        first.x = vb->vertices[i + X1];  first.y  = vb->vertices[i + Y1]; first.z = vb->vertices[i + Z1];
        second.x = vb->vertices[i + X2]; second.y = vb->vertices[i + Y2]; second.z = vb->vertices[i + Z2];
        third.x = vb->vertices[i + X3];  third.y = vb->vertices[i + Y3];  third.z =  vb->vertices[i + Z3];

        drawLines_RGBA(rc, clr, first, second);
        drawLines_RGBA(rc, clr, second, third);
        drawLines_RGBA(rc, clr, first, third);

        scanline_RGBA(rc, clr);
    }
    
}