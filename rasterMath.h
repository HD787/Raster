//this will largely consist of parametric equations used for interpolating arbitrary points on a line
//which is useful for some optimizations, there may be some redundancy between this and 3Dmath but its
//important they exist independent of one another
Rvec3 interpolatePointForX(Rvec3 outer, Rvec3 inner, renderContext* rc){
    Rvec3 temp;
    if(outer.x > rc->width) temp.x = rc->width + 1;
    else temp.x = -1;
    int t = (temp.x - inner.x) / (outer.x - inner.x);
    temp.y = inner.y + t * (inner.y - outer.y);
    temp.z = inner.z+ t * (inner.z - outer.z);

    return temp;
}

Rvec3 interpolatePointForY(Rvec3 outer, Rvec3 inner, renderContext* rc){
    Rvec3 temp;
    if(outer.y > rc->height) temp.y = rc->height + 1;
    else temp.y = -1;
    int t = (temp.y - inner.y) / (outer.y - inner.y);
    temp.x = inner.x + t * (inner.x - outer.x);
    temp.z = inner.z+ t * (inner.z - outer.z);

    return temp;
}


//maybe only do this for negative values
Rvec3 interpolatePointForZ(Rvec3 outer, Rvec3 inner){
    Rvec3 temp;
    temp.z = -1;
    int t = (temp.z - inner.z) / (outer.z - inner.z);
    temp.x = inner.x + t * (inner.x - outer.x);
    temp.y = inner.y + t * (inner.y - outer.y);

    return temp;
}

//bounds checking funcntion
int vertexInFrustum(Rvec3 vector, renderContext* rc){
    if(vector.z < 0) return 0;
    if(vector.x < 0 || vector.x > rc->width) return 0;
    if(vector.y < 0 || vector.y > rc->height) return 0;
    return 1;
}

int triangleInFrustum(Rvec3 one, Rvec3 two, Rvec3 three, renderContext* rc){
    if(!vertexInFrustum(one, rc) && !vertexInFrustum(two, rc) && !vertexInFrustum(three, rc)) return 0;
    return 1;
}

