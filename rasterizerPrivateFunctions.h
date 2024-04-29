
// this is deprecated, dont use
// or do idc
void colorPixel(frameBuffer* fb, int x, int y, byte r, byte g, byte b)
{
    int val = (y * fb->width + x) * 3;
    fb->pixels[val] = r;
    fb->pixels[val + 1] = g;
    fb->pixels[val + 2] = b;
}

void cleanFrameBuffer(frameBuffer* fb)
{
    for (int i = 0; i < fb->height * fb->width * 3; i++)
    {
        fb->pixels[i] = 0;
    }
}
// thinking of moving this to public functions or even to the graphics engine
void NDC_ToScreenSpace(frameBuffer* fb, Rvec3* vec)
{
    float scaleX = fb->width / 2;
    float scaleY = fb->height / 2;
    vec->x = (vec->x + 1) * scaleX;
    vec->y = (1 - vec->y) * scaleY;
    // the z values need to be scaled for the sake of bresenhams
    // im thinkking this will work but tbh idk
    vec->z = vec->z * scaleX;
}

void scanline(frameBuffer* fb, int **scanlineSpec, int* zBuffer, color clr)
{
    for (int y = 0; y < fb->height; y++)
    {
        if (scanlineSpec[y][0] != -1000000 && scanlineSpec[y][2] != -1000000)
        {

            int x1 = scanlineSpec[y][0];
            int x2 = scanlineSpec[y][2];
            int z1 = scanlineSpec[y][1];
            int z2 = scanlineSpec[y][3];
            int dx = abs(x2 - x1);
            int dz = abs(z2 - z1);
            int zs;
            int xs = 1;
            if (z1 < z2)
                zs = 1;
            else
                zs = -1;
            int error = 0;
            if (dx > dz)
            {
                // driving x axis
                for (int z = z1, x = x1; x != x2; x += xs)
                {
                    if (x < fb->width && x >= 0 && y < (fb->height) && y >= 0 && z < zBuffer[(y * fb->width + x)] && z > 0)
                    {
                        zBuffer[(y * fb->width + x)] = z;
                        fb->pixels[((y * fb->width + x) * 3)] = clr.r;
                        fb->pixels[((y * fb->width + x) * 3) + 1] = clr.g;
                        fb->pixels[((y * fb->width + x) * 3) + 2] = clr.b;
                    }
                    error += dz;
                    if (error >= dx)
                    {
                        z += zs;
                        error -= dx;
                    }
                }
            }
            else
            {
                // driving z axis
                for (int z = z1, x = x1; z != z2; z += zs)
                {
                    if (x < fb->width && x >= 0 && y < (fb->height) && y >= 0 && z < zBuffer[(y * fb->width + x)] && z > 0)
                    {
                        zBuffer[(y * fb->width + x)] = z;
                        fb->pixels[((y * fb->width + x) * 3)] = clr.r;
                        fb->pixels[((y * fb->width + x) * 3) + 1] = clr.g;
                        fb->pixels[((y * fb->width + x) * 3) + 2] = clr.b;
                    }
                    error += dx;
                    if (error >= dz)
                    {
                        x += xs;
                        error -= dz;
                    }
                }
            }
        }
    }
}

void drawLines(frameBuffer* fb, int** scanlineSpec, int* zBuffer, int x1, int y1, int z1, int x2, int y2, int z2)
{
    // consider more structs to organize things
    if (x1 < fb->width && x1 >= 0 && y1 < (fb->height) && y1 >= 0 && z1 < zBuffer[(y1 * fb->width + x1)] && z1 > 0)
    {
        zBuffer[(y1 * fb->width + x1)] = z1;
        fb->pixels[((y1 * fb->width + x1) * 3)] = 255;
        fb->pixels[((y1 * fb->width + x1) * 3) + 1] = 255;
        fb->pixels[((y1 * fb->width + x1) * 3) + 2] = 255;
    }
    if (y1 >= 0 && y1 < fb->height && (x1 <= scanlineSpec[y1][0] || scanlineSpec[y1][0] == -1000000))
    {
        scanlineSpec[y1][0] = x1;
        scanlineSpec[y1][1] = z1;
    }
    if (y1 >= 0 && y1 < fb->height && (x1 >= scanlineSpec[y1][2] || scanlineSpec[y1][2] == -1000000))
    { 
        scanlineSpec[y1][2] = x1;
        scanlineSpec[y1][3] = z1;
    }
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

    // Driving axis is X-axis
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
            if (x1 < fb->width && x1 >= 0 && y1 < (fb->height) && y1 >= 0 && z1 < zBuffer[(y1 * fb->width + x1)] && z1 > 0)
            {
                zBuffer[(y1 * fb->width + x1)] = z1;
                fb->pixels[((y1 * fb->width + x1) * 3)] = 255;
                fb->pixels[((y1 * fb->width + x1) * 3) + 1] = 255;
                fb->pixels[((y1 * fb->width + x1) * 3) + 2] = 255;
            }
            if (y1 >= 0 && y1 < fb->height && (x1 <= scanlineSpec[y1][0] || scanlineSpec[y1][0] == -1000000))
            {
                scanlineSpec[y1][0] = x1;
                scanlineSpec[y1][1] = z1;
            }
            if (y1 >= 0 && y1 < fb->height && (x1 >= scanlineSpec[y1][2] || scanlineSpec[y1][2] == -1000000))
            {
                scanlineSpec[y1][2] = x1;
                scanlineSpec[y1][3] = z1;
            }
        }

        // Driving axis is Y-axis
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
            if (x1 < fb->width && x1 >= 0 && y1 < (fb->height) && y1 >= 0 && z1 < zBuffer[(y1 * fb->width + x1)] && z1 > 0)
            {   
                zBuffer[(y1 * fb->width + x1)] = z1;
                fb->pixels[((y1 * fb->width + x1) * 3)] = 255;
                fb->pixels[((y1 * fb->width + x1) * 3) + 1] = 255;
                fb->pixels[((y1 * fb->width + x1) * 3) + 2] = 255;
            }
            if (y1 >= 0 && y1 < fb->height && (x1 <= scanlineSpec[y1][0] || scanlineSpec[y1][0] == -1000000))
            {
                scanlineSpec[y1][0] = x1;
                scanlineSpec[y1][1] = z1;
            }
            if (y1 >= 0 && y1 < fb->height && (x1 >= scanlineSpec[y1][2] || scanlineSpec[y1][2] == -1000000))
            {
                scanlineSpec[y1][2] = x1;
                scanlineSpec[y1][3] = z1;
            }
        }
    }
    // Driving axis is Z-axis
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
            if (x1 < fb->width && x1 >= 0 && y1 < (fb->height) && y1 >= 0 && z1 < zBuffer[(y1 * fb->width + x1)] && z1 > 0)
            {
                zBuffer[(y1 * fb->width + x1)] = z1;
                fb->pixels[((y1 * fb->width + x1) * 3)] = 255;
                fb->pixels[((y1 * fb->width + x1) * 3) + 1] = 255;
                fb->pixels[((y1 * fb->width + x1) * 3) + 2] = 255;
            }
            if (y1 >= 0 && y1 < fb->height && (x1 <= scanlineSpec[y1][0] || scanlineSpec[y1][0] == -1000000))
            {
                scanlineSpec[y1][0] = x1;
                scanlineSpec[y1][1] = z1;
            }
            if (y1 >= 0 && y1 < fb->height && (x1 >= scanlineSpec[y1][2] || scanlineSpec[y1][2] == -1000000))
            {
                scanlineSpec[y1][2] = x1;
                scanlineSpec[y1][3] = z1;
            }
        }
    }
}



// this is deprecated
void renderFrameBuffer(frameBuffer* fb, int* zbuffer)
{
    for (int f = 0, z = 0; f < (fb->height * fb->width * 3); f += 3, z += 4)
    {
        fb->pixels[f] = zbuffer[z + 1];
        fb->pixels[f + 1] = zbuffer[z + 2];
        fb->pixels[f + 2] = zbuffer[z + 3];
    }
}