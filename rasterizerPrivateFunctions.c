
// this is deprecated, dont use
// or do idc
void colorPixel(renderContext* rc, int x, int y, byte r, byte g, byte b)
{
    int val = (y * rc->width + x) * 3;
    rc->frameBuffer[val] = r;
    rc->frameBuffer[val + 1] = g;
    rc->frameBuffer[val + 2] = b;
}


void cleanScanlineSpec(renderContext* rc){
    for(int i = 0; i < rc->height * 4 ; i+=4){
        rc->scanlineSpec[i] = -1000000;
        rc->scanlineSpec[i + 1] = -1000000;
        rc->scanlineSpec[i + 2] = -1000000;
        rc->scanlineSpec[i + 3] = -1000000;
    }
}

void cleanzBuffer(renderContext* rc){
    for (int i = 0; i < (rc->width * rc->height); i++){
        rc->zBuffer[i] = 1000;
    }
}

void cleanFrameBuffer(renderContext* rc){
    for (int i = 0; i < rc->height * rc->width * 3; i++)
    {
        rc->frameBuffer[i] = 0;
    } 
}


void scanline(renderContext* rc, color clr)
{
    for (int y = 0; y < rc->height; y += 1)
    {
        if (rc->scanlineSpec[y * 4] != -1000000 && rc->scanlineSpec[y * 4 + 2] != -1000000)
        {
            int x1 = rc->scanlineSpec[y * 4];
            int x2 = rc->scanlineSpec[y * 4 + 2];
            int z1 = rc->scanlineSpec[y * 4 + 1];
            int z2 = rc->scanlineSpec[y * 4 + 3];
            if(x1 < 0 && x2 < 0) continue;
            if(x1 > rc->width && x2 > rc->width) continue;
            if(z1 < 0 && z2 < 0) continue;
            if(x1 < 0){
                Rvec3 first, second;
                first.x = x1; first.y = y; first.z = z1;
                second.x = x2; second.y = y; second.z = z2; 
                Rvec3 result = interpolatePointForX(first, second, rc); 
                x1 = result.x; z1 = result.z;
            }
            if(x2 > rc->width){
                Rvec3 first, second;
                first.x = x1; first.y = y; first.z = z1;
                second.x = x2; second.y = y; second.z = z2; 
                Rvec3 result = interpolatePointForX(second, first, rc);
                x2 = result.x; z2 = result.z;
            }
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
                    if (x < rc->width && x >= 0 && y < (rc->height) && y >= 0 && z < rc->zBuffer[(y * rc->width + x)] && z > 0)
                    {
                        rc->zBuffer[(y * rc->width + x)] = z;
                        rc->frameBuffer[((y * rc->width + x) * 3)] = clr.r;
                        rc->frameBuffer[((y * rc->width + x) * 3) + 1] = clr.g;
                        rc->frameBuffer[((y * rc->width + x) * 3) + 2] = clr.b;
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
                    if (x < rc->width && x >= 0 && y < (rc->height) && y >= 0 && z < rc->zBuffer[(y * rc->width + x)] && z > 0)
                    {
                        rc->zBuffer[(y * rc->width + x)] = z;
                        rc->frameBuffer[((y * rc->width + x) * 3)] = clr.r;
                        rc->frameBuffer[((y * rc->width + x) * 3) + 1] = clr.g;
                        rc->frameBuffer[((y * rc->width + x) * 3) + 2] = clr.b;
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

void drawLines(renderContext* rc, color clr, Rvec3 first, Rvec3 second)
{
    int x1 = first.x;
    int y1 = first.y;
    int z1 = first.z;
    int x2 = second.x;
    int y2 = second.y;
    int z2 = second.z;
    if (x1 < rc->width && x1 >= 0 && y1 < (rc->height) && y1 >= 0 && z1 < rc->zBuffer[(y1 * rc->width + x1)] && z1 > 0)
    {
        rc->zBuffer[(y1 * rc->width + x1)] = z1;
        rc->frameBuffer[((y1 * rc->width + x1) * 3)] = clr.r;
        rc->frameBuffer[((y1 * rc->width + x1) * 3) + 1] = clr.g;
        rc->frameBuffer[((y1 * rc->width + x1) * 3) + 2] = clr.b;
    }
    if (y1 >= 0 && y1 < rc->height && (x1 <= rc->scanlineSpec[y1 * 4] || rc->scanlineSpec[y1 * 4] == -1000000))
    {
        rc->scanlineSpec[y1 * 4] = x1;
        rc->scanlineSpec[(y1 * 4) + 1] = z1;
    }
    if (y1 >= 0 && y1 < rc->height && (x1 >= rc->scanlineSpec[(y1 * 4) + 2] || rc->scanlineSpec[(y1 * 4) + 2] == -1000000))
    { 
        rc->scanlineSpec[(y1 * 4) + 2] = x1;
        rc->scanlineSpec[(y1 * 4) + 3] = z1;
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
            if (x1 < rc->width && x1 >= 0 && y1 < (rc->height) && y1 >= 0 && z1 < rc->zBuffer[(y1 * rc->width + x1)] && z1 > 0)
            {
                rc->zBuffer[(y1 * rc->width + x1)] = z1;
                rc->frameBuffer[((y1 * rc->width + x1) * 3)] = clr.r;
                rc->frameBuffer[((y1 * rc->width + x1) * 3) + 1] = clr.g;
                rc->frameBuffer[((y1 * rc->width + x1) * 3) + 2] = clr.b;
            }
            if (y1 >= 0 && y1 < rc->height && (x1 <= rc->scanlineSpec[y1 * 4] || rc->scanlineSpec[y1 * 4] == -1000000))
            {
                rc->scanlineSpec[y1 * 4] = x1;
                rc->scanlineSpec[(y1 * 4) + 1] = z1;
            }
            if (y1 >= 0 && y1 < rc->height && (x1 >= rc->scanlineSpec[(y1 * 4) + 2] || rc->scanlineSpec[(y1 * 4) + 2] == -1000000))
            {
                rc->scanlineSpec[(y1 * 4) + 2] = x1;
                rc->scanlineSpec[(y1 * 4) + 3] = z1;
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
            if (x1 < rc->width && x1 >= 0 && y1 < (rc->height) && y1 >= 0 && z1 < rc->zBuffer[(y1 * rc->width + x1)] && z1 > 0)
            {   
                rc->zBuffer[(y1 * rc->width + x1)] = z1;
                rc->frameBuffer[((y1 * rc->width + x1) * 3)] = clr.r;
                rc->frameBuffer[((y1 * rc->width + x1) * 3) + 1] = clr.g;
                rc->frameBuffer[((y1 * rc->width + x1) * 3) + 2] = clr.b;
            }
            if (y1 >= 0 && y1 < rc->height && (x1 <= rc->scanlineSpec[y1 * 4] || rc->scanlineSpec[y1 * 4] == -1000000))
            {
                rc->scanlineSpec[y1 * 4] = x1;
                rc->scanlineSpec[y1 * 4 + 1] = z1;
            }
            if (y1 >= 0 && y1 < rc->height && (x1 >= rc->scanlineSpec[(y1 * 4) + 2] || rc->scanlineSpec[(y1 * 4) + 2] == -1000000))
            {
                rc->scanlineSpec[(y1 * 4) + 2] = x1;
                rc->scanlineSpec[(y1 * 4) + 3] = z1;
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
            if (x1 < rc->width && x1 >= 0 && y1 < (rc->height) && y1 >= 0 && z1 < rc->zBuffer[(y1 * rc->width + x1)] && z1 > 0)
            {
                rc->zBuffer[(y1 * rc->width + x1)] = z1;
                rc->frameBuffer[((y1 * rc->width + x1) * 3)] = clr.r;
                rc->frameBuffer[((y1 * rc->width + x1) * 3) + 1] = clr.g;
                rc->frameBuffer[((y1 * rc->width + x1) * 3) + 2] = clr.b;
            }
            if (y1 >= 0 && y1 < rc->height && (x1 <= rc->scanlineSpec[y1 * 4] || rc->scanlineSpec[y1 * 4] == -1000000))
            {
                rc->scanlineSpec[y1 * 4] = x1;
                rc->scanlineSpec[(y1 * 4) + 1] = z1;
            }
            if (y1 >= 0 && y1 < rc->height && (x1 >= rc->scanlineSpec[(y1 * 4) + 2] || rc->scanlineSpec[(y1 * 4) + 2] == -1000000))
            {
                rc->scanlineSpec[(y1 * 4) + 2] = x1;
                rc->scanlineSpec[(y1 * 4) + 3] = z1;
            }
        }
    }
}



/* ALTERNATE FUNCTIONS FOR RGBA OUTPUT */

void cleanFrameBuffer_RGBA(renderContext* rc){
    for (int i = 0; i < rc->height * rc->width * 4; i += 4)
    {
        rc->frameBuffer[i] = 0;
        rc->frameBuffer[i + 1] = 0;
        rc->frameBuffer[i + 2] = 0;
        rc->frameBuffer[i + 3] = 255;
    } 
}


void scanline_RGBA(renderContext* rc, color clr)
{
    for (int y = 0; y < rc->height; y += 1)
    {
        if (rc->scanlineSpec[y * 4] != -1000000 && rc->scanlineSpec[y * 4 + 2] != -1000000)
        {
            int x1 = rc->scanlineSpec[y * 4];
            int x2 = rc->scanlineSpec[y * 4 + 2];
            int z1 = rc->scanlineSpec[y * 4 + 1];
            int z2 = rc->scanlineSpec[y * 4 + 3];
            if(x1 < 0 && x2 < 0) continue;
            if(x1 > rc->width && x2 > rc->width) continue;
            if(z1 < 0 && z2 < 0) continue;
            // if(x1 < 0){
            //     Rvec3 first, second;
            //     first.x = x1; first.y = y; first.z = z1;
            //     second.x = x2; second.y = y; second.z = z2; 
            //     Rvec3 result = interpolatePointForX(first, second, rc); 
            //     x1 = result.x; z1 = result.z;
            // }
            // if(x2 > rc->width){
            //     Rvec3 first, second;
            //     first.x = x1; first.y = y; first.z = z1;
            //     second.x = x2; second.y = y; second.z = z2; 
            //     Rvec3 result = interpolatePointForX(second, first, rc);
            //     x2 = result.x; z2 = result.z;
            // }
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
                    if (x < rc->width && x >= 0 && y < (rc->height) && y >= 0 && z < rc->zBuffer[(y * rc->width + x)] && z > 0)
                    {
                        rc->zBuffer[(y * rc->width + x)] = z;
                        rc->frameBuffer[((y * rc->width + x) * 4)] = clr.r;
                        rc->frameBuffer[((y * rc->width + x) * 4) + 1] = clr.g;
                        rc->frameBuffer[((y * rc->width + x) * 4) + 2] = clr.b;
                        rc->frameBuffer[((y * rc->width + x) * 4) + 3] = 255;
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
                    if (x < rc->width && x >= 0 && y < (rc->height) && y >= 0 && z < rc->zBuffer[(y * rc->width + x)] && z > 0)
                    {
                        rc->zBuffer[(y * rc->width + x)] = z;
                        rc->frameBuffer[((y * rc->width + x) * 4)] = clr.r;
                        rc->frameBuffer[((y * rc->width + x) * 4) + 1] = clr.g;
                        rc->frameBuffer[((y * rc->width + x) * 4) + 2] = clr.b;
                        rc->frameBuffer[((y * rc->width + x) * 4) + 3] = 255;
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

void drawLines_RGBA(renderContext* rc, color clr, Rvec3 first, Rvec3 second)
{
    int x1 = first.x;
    int y1 = first.y;
    int z1 = first.z;
    int x2 = second.x;
    int y2 = second.y;
    int z2 = second.z;
    if (x1 < rc->width && x1 >= 0 && y1 < (rc->height) && y1 >= 0 && z1 < rc->zBuffer[(y1 * rc->width + x1)] && z1 > 0)
    {
        rc->zBuffer[(y1 * rc->width + x1)] = z1;
        rc->frameBuffer[((y1 * rc->width + x1) * 4)] = clr.r;
        rc->frameBuffer[((y1 * rc->width + x1) * 4) + 1] = clr.g;
        rc->frameBuffer[((y1 * rc->width + x1) * 4) + 2] = clr.b;
        rc->frameBuffer[((y1 * rc->width + x1) * 4) + 3] = 255;
    }
    if (y1 >= 0 && y1 < rc->height && (x1 <= rc->scanlineSpec[y1 * 4] || rc->scanlineSpec[y1 * 4] == -1000000))
    {
        rc->scanlineSpec[y1 * 4] = x1;
        rc->scanlineSpec[(y1 * 4) + 1] = z1;
    }
    if (y1 >= 0 && y1 < rc->height && (x1 >= rc->scanlineSpec[(y1 * 4) + 2] || rc->scanlineSpec[(y1 * 4) + 2] == -1000000))
    { 
        rc->scanlineSpec[(y1 * 4) + 2] = x1;
        rc->scanlineSpec[(y1 * 4) + 3] = z1;
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
            if (x1 < rc->width && x1 >= 0 && y1 < (rc->height) && y1 >= 0 && z1 < rc->zBuffer[(y1 * rc->width + x1)] && z1 > 0)
            {
                rc->zBuffer[(y1 * rc->width + x1)] = z1;
                rc->frameBuffer[((y1 * rc->width + x1) * 4)] = clr.r;
                rc->frameBuffer[((y1 * rc->width + x1) * 4) + 1] = clr.g;
                rc->frameBuffer[((y1 * rc->width + x1) * 4) + 2] = clr.b;
                rc->frameBuffer[((y1 * rc->width + x1) * 4) + 3] = 255;
            }
            if (y1 >= 0 && y1 < rc->height && (x1 <= rc->scanlineSpec[y1 * 4] || rc->scanlineSpec[y1 * 4] == -1000000))
            {
                rc->scanlineSpec[y1 * 4] = x1;
                rc->scanlineSpec[(y1 * 4) + 1] = z1;
            }
            if (y1 >= 0 && y1 < rc->height && (x1 >= rc->scanlineSpec[(y1 * 4) + 2] || rc->scanlineSpec[(y1 * 4) + 2] == -1000000))
            {
                rc->scanlineSpec[(y1 * 4) + 2] = x1;
                rc->scanlineSpec[(y1 * 4) + 3] = z1;
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
            if (x1 < rc->width && x1 >= 0 && y1 < (rc->height) && y1 >= 0 && z1 < rc->zBuffer[(y1 * rc->width + x1)] && z1 > 0)
            {   
                rc->zBuffer[(y1 * rc->width + x1)] = z1;
                rc->frameBuffer[((y1 * rc->width + x1) * 4)] = clr.r;
                rc->frameBuffer[((y1 * rc->width + x1) * 4) + 1] = clr.g;
                rc->frameBuffer[((y1 * rc->width + x1) * 4) + 2] = clr.b;
                rc->frameBuffer[((y1 * rc->width + x1) * 4) + 3] = 255;
            }
            if (y1 >= 0 && y1 < rc->height && (x1 <= rc->scanlineSpec[y1 * 4] || rc->scanlineSpec[y1 * 4] == -1000000))
            {
                rc->scanlineSpec[y1 * 4] = x1;
                rc->scanlineSpec[y1 * 4 + 1] = z1;
            }
            if (y1 >= 0 && y1 < rc->height && (x1 >= rc->scanlineSpec[(y1 * 4) + 2] || rc->scanlineSpec[(y1 * 4) + 2] == -1000000))
            {
                rc->scanlineSpec[(y1 * 4) + 2] = x1;
                rc->scanlineSpec[(y1 * 4) + 3] = z1;
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
            if (x1 < rc->width && x1 >= 0 && y1 < (rc->height) && y1 >= 0 && z1 < rc->zBuffer[(y1 * rc->width + x1)] && z1 > 0)
            {
                rc->zBuffer[(y1 * rc->width + x1)] = z1;
                rc->frameBuffer[((y1 * rc->width + x1) * 4)] = clr.r;
                rc->frameBuffer[((y1 * rc->width + x1) * 4) + 1] = clr.g;
                rc->frameBuffer[((y1 * rc->width + x1) * 4) + 2] = clr.b;
                rc->frameBuffer[((y1 * rc->width + x1) * 4) + 3] = 255;
            }
            if (y1 >= 0 && y1 < rc->height && (x1 <= rc->scanlineSpec[y1 * 4] || rc->scanlineSpec[y1 * 4] == -1000000))
            {
                rc->scanlineSpec[y1 * 4] = x1;
                rc->scanlineSpec[(y1 * 4) + 1] = z1;
            }
            if (y1 >= 0 && y1 < rc->height && (x1 >= rc->scanlineSpec[(y1 * 4) + 2] || rc->scanlineSpec[(y1 * 4) + 2] == -1000000))
            {
                rc->scanlineSpec[(y1 * 4) + 2] = x1;
                rc->scanlineSpec[(y1 * 4) + 3] = z1;
            }
        }
    }
}