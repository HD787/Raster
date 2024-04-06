void colorPixel(framebuffer *fb, int x, int y, byte r, byte g, byte b)
{
    int val = (y * fb->width + x) * 3;
    fb->pixels[val] = r;
    fb->pixels[val + 1] = g;
    fb->pixels[val + 2] = b;
}


void scanline(framebuffer *fb, int **scanlineSpec, int* zBuffer, byte r, byte g, byte b)
{
    for (int y = 0; y < fb->height; y++)
    {
        for (int j = scanlineSpec[y][0]; j < scanlineSpec[y][2]; j++)
        {
            if (scanlineSpec[y][0] != -1 && scanlineSpec[y][2] != -1)
            {   
                int x1 = scanlineSpec[y][0];
                int x2 = scanlineSpec[y][2];
                int z1 = scanlineSpec[y][1];
                int z2 = scanlineSpec[y][3];
                int dz = abs(x2 - x1);
                int dx = abs(z2 - z1); 
                int sz;
                int sx = 1;
                if(z1 < z2) sz = 1;
                else sz = -1;
                int error = 0;
                if(dz > dx){
                    //driving z axis
                    for(int z = z1, x = x1; z != z2; z += sz){
                        if (z > zBuffer[(y * fb->height + x) * 4]){
                            zBuffer[(y * fb->height + x) * 4] = z;
                            zBuffer[(y * fb->height + x) * 4 + 1] = r;
                            zBuffer[(y * fb->height + x) * 4 + 2] = g;
                            zBuffer[(y * fb->height + x) * 4 + 3] = b;
                        }
                        error += dx;
                        if(error >= dz){
                            x++;
                            error -= dz;
                        }

                    }
                }
                else{
                    //driving x axis
                    for(int z = z1, x = x1; x != x2; x += sx){
                        if (z > zBuffer[(y * fb->height + x) * 4]){
                            zBuffer[(y * fb->height + x) * 4] = z;
                            zBuffer[(y * fb->height + x) * 4 + 1] = r;
                            zBuffer[(y * fb->height + x) * 4 + 2] = g;
                            zBuffer[(y * fb->height + x) * 4 + 3] = b;
                        }
                        error += dz;
                        if(error >= dx){
                            z++;
                            error -= dx;
                        }

                    }
                }
            }
        }
    }
}

void drawLines(framebuffer *fb, int **scanlineSpec, int *zBuffer, int x1, int y1, int z1, int x2, int y2, int z2)
{
    // this code is shamefully stolen from Ishan Khandelwals
    // as sometimes progress is the most valuable thing in a project

    //consider more structs to organize things
    if (z1 > zBuffer[(y1 * fb->height + x1) * 4]){
        zBuffer[(y1 * fb->height + x1) * 4] = z1;
        zBuffer[(y1 * fb->height + x1) * 4 + 1] = 255;
        zBuffer[(y1 * fb->height + x1) * 4 + 2] = 255;
        zBuffer[(y1 * fb->height + x1) * 4 + 3] = 255;
    }
    // this will be for scanline, obviously
    if (x1 < scanlineSpec[y1][0] || scanlineSpec[y1][0] == -1){
        scanlineSpec[y1][0] = x1;
        scanlineSpec[y1][1] = z1;
    }
    if (x1 > scanlineSpec[y1][1] || scanlineSpec[y1][1] == -1){
        scanlineSpec[y1][2] = x1;
        scanlineSpec[y1][2] = z1;
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
            if (z1 > zBuffer[(y1 * fb->height + x1) * 4]){
                zBuffer[(y1 * fb->height + x1) * 4] = z1;
                zBuffer[(y1 * fb->height + x1) * 4 + 1] = 255;
                zBuffer[(y1 * fb->height + x1) * 4 + 2] = 255;
                zBuffer[(y1 * fb->height + x1) * 4 + 3] = 255;
            }
            // this will be for scanline, obviously
            if (x1 < scanlineSpec[y1][0] || scanlineSpec[y1][0] == -1){
                scanlineSpec[y1][0] = x1;
                scanlineSpec[y1][1] = z1;
            }
            if (x1 > scanlineSpec[y1][1] || scanlineSpec[y1][1] == -1){
                scanlineSpec[y1][2] = x1;
                scanlineSpec[y1][2] = z1;
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
            if (z1 > zBuffer[(y1 * fb->height + x1) * 4]){
                zBuffer[(y1 * fb->height + x1) * 4] = z1;
                zBuffer[(y1 * fb->height + x1) * 4 + 1] = 255;
                zBuffer[(y1 * fb->height + x1) * 4 + 2] = 255;
                zBuffer[(y1 * fb->height + x1) * 4 + 3] = 255;
            }
            // this will be for scanline, obviously
            if (x1 < scanlineSpec[y1][0] || scanlineSpec[y1][0] == -1){
                scanlineSpec[y1][0] = x1;
                scanlineSpec[y1][1] = z1;
            }
            if (x1 > scanlineSpec[y1][1] || scanlineSpec[y1][1] == -1){
                scanlineSpec[y1][2] = x1;
                scanlineSpec[y1][2] = z1;
            }
        }

        // Driving axis is Z-axis
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
            if (z1 > zBuffer[(y1 * fb->height + x1) * 4]){
                zBuffer[(y1 * fb->height + x1) * 4] = z1;
                zBuffer[(y1 * fb->height + x1) * 4 + 1] = 255;
                zBuffer[(y1 * fb->height + x1) * 4 + 2] = 255;
                zBuffer[(y1 * fb->height + x1) * 4 + 3] = 255;
            }
            // this will be for scanline, obviously
            if (x1 < scanlineSpec[y1][0] || scanlineSpec[y1][0] == -1){
                scanlineSpec[y1][0] = x1;
                scanlineSpec[y1][1] = z1;
            }
            if (x1 > scanlineSpec[y1][1] || scanlineSpec[y1][1] == -1){
                scanlineSpec[y1][2] = x1;
                scanlineSpec[y1][2] = z1;
            }
        }
    }
}

void renderFrameBuffer(framebuffer* fb, int* zbuffer){
    for(int f = 0, z = 0; f < fb->height * fb->width * 3; f += 3, z += 4){
        fb->pixels[f] = zbuffer[z + 1];
        fb->pixels[f+ 1] = zbuffer[z + 2];
        fb->pixels[f + 2] = zbuffer[z + 3];
    }
}