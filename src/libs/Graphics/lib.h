#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "libs/Linear Algebra/lib.h"

#include "classes.h"

typedef struct {
    char *textures;
    unsigned int faceCount;
} Textures;

typedef struct {
    Model *model;
    Transform transform;
    Vector3D scalar;
    Textures textures;
} Renderer;

Textures newTextures(char *textureValues, unsigned int textureCount){
    Textures textures;

    textures.textures = textureValues;
    textures.faceCount = textureCount;

    return textures;
}

Vector3D scale(Vector3D vector, Vector3D scalar){
    Vector3D scaled;
    
    scaled.x = vector.x * scalar.x;
    scaled.y = vector.y * scalar.y;
    scaled.z = vector.z * scalar.z;

    return scaled;
}

Vector3D translate(Vector3D vector, Transform transform){
    EulerAngle angle = *(transform.rotation);

    EulerAngle helperAngle;
    helperAngle.x = PI / 2.0;
    helperAngle.y = 0;
    helperAngle.z = 0;

    Vector3D forward = normalFromEularAngles(angle);
    Vector3D noTiltUp = normalFromEularAngles(sumEulerAngles(angle, helperAngle));
    Vector3D noTiltRight = crossProductVector3Ds(noTiltUp, forward);

    Vector3D up = sumVector3Ds(crossProductScalarVector3D(cos(angle.z), noTiltUp), crossProductScalarVector3D(-sin(angle.z), noTiltRight));
    Vector3D right = sumVector3Ds(crossProductScalarVector3D(cos(angle.z), noTiltRight), crossProductScalarVector3D(sin(angle.z), noTiltUp));

    Vector3D appliedRotation = sumVector3Ds(sumVector3Ds(crossProductScalarVector3D(vector.x, right), crossProductScalarVector3D(vector.y, up)), crossProductScalarVector3D(vector.z, forward));
    
    return sumVector3Ds(appliedRotation, *(transform.position));
}

double dist(Vector3D p1, Vector3D p2, Vector3D p3, Vector3D ray){
    //plane surface is in
    Vector3D diffP1P2 = differenceVector3Ds(p1, p2);
    Vector3D diffP2P3 = differenceVector3Ds(p2, p3);
    Vector3D xProd1223 = crossProductVector3Ds(diffP1P2, diffP2P3); //plane

    //check intersection with vector
    double vInPlane = dotProductVector3Ds(xProd1223, ray);
    if(vInPlane == 0) return -1; //parallel vector to surface
    //scalar to plane
    double scalar = dotProductVector3Ds(xProd1223, p1) / vInPlane;
    if(scalar < 0) return -1; //surface behind vector

    Vector3D intersect = crossProductScalarVector3D(scalar, ray);

    Vector3D boundaryPlaneP1P2 = crossProductVector3Ds(xProd1223, diffP1P2);
    double offsetP1P2 = dotProductVector3Ds(boundaryPlaneP1P2, p1);
    
    if((dotProductVector3Ds(boundaryPlaneP1P2, p3) - offsetP1P2 > 0) == (dotProductVector3Ds(boundaryPlaneP1P2, intersect) - offsetP1P2 <= 0)) return -1; //unequal signs

    Vector3D boundaryPlaneP2P3 = crossProductVector3Ds(xProd1223, diffP2P3);
    double offsetP2P3 = dotProductVector3Ds(boundaryPlaneP2P3, p2);

    if((dotProductVector3Ds(boundaryPlaneP2P3, p1) - offsetP2P3 > 0) == (dotProductVector3Ds(boundaryPlaneP2P3, intersect) - offsetP2P3 <= 0)) return -1; //unequal signs

    Vector3D diffP3P1 = differenceVector3Ds(p3, p1);

    Vector3D boundaryPlaneP3P1 = crossProductVector3Ds(xProd1223, diffP3P1);
    double offsetP3P1 = dotProductVector3Ds(boundaryPlaneP3P1, p3);

    if((dotProductVector3Ds(boundaryPlaneP3P1, p2) - offsetP3P1 > 0) == (dotProductVector3Ds(boundaryPlaneP3P1, intersect) - offsetP3P1 <= 0)) return -1; //unequal signs

    return scalar;
}

char castRay(Vector3D position, Vector3D ray, Renderer *renderers, unsigned int rendererCount){
    double pickDist = -1;
    char color = '.';
    Renderer renderer = renderers[0];
    Face face = renderer.model->faces[0];
    for(int i = 0; i < rendererCount; renderer = renderers[++i]){
        for(int j = 0; j < renderer.model->faceCount; face = renderer.model->faces[++j]){
            //normalize to camera at position 0, 0, 0
            Vector3D p1 = differenceVector3Ds(translate(scale(*face.vertex1, renderer.scalar), renderer.transform), position);
            Vector3D p2 = differenceVector3Ds(translate(scale(*face.vertex2, renderer.scalar), renderer.transform), position);
            Vector3D p3 = differenceVector3Ds(translate(scale(*face.vertex3, renderer.scalar), renderer.transform), position);

            double scalar = dist(p1, p2, p3, ray);
            if(scalar > 0 && (pickDist == -1 || (pickDist > 0 && scalar < pickDist))){
                pickDist = scalar;
                color = renderer.textures.textures[j];
            }
        }
    }

    return color;
}

void render(Camera *camera, Renderer *renderers, unsigned int rendererCount){
    EulerAngle helperAngle;
    helperAngle.x = PI / 2.0;
    helperAngle.y = 0;
    helperAngle.z = 0;

    Vector3D forward = normalFromEularAngles(*(camera->transform->rotation));
    Vector3D noTiltUp = normalFromEularAngles(sumEulerAngles(*(camera->transform->rotation), helperAngle));
    Vector3D noTiltRight = normalize(crossProductVector3Ds(noTiltUp, forward));

    Vector3D up = sumVector3Ds(crossProductScalarVector3D(cos(camera->transform->rotation->z), noTiltUp), crossProductScalarVector3D(-sin(camera->transform->rotation->z), noTiltRight));
    Vector3D right = sumVector3Ds(crossProductScalarVector3D(cos(camera->transform->rotation->z), noTiltRight), crossProductScalarVector3D(sin(camera->transform->rotation->z), noTiltUp));

    Vector3D screenTop = sumVector3Ds(crossProductScalarVector3D(cos(camera->yFov / 2.0), forward), crossProductScalarVector3D(sin(camera->yFov / 2.0), up));
    Vector3D screenBottom = sumVector3Ds(crossProductScalarVector3D(cos(camera->yFov / 2.0), forward), crossProductScalarVector3D(-sin(camera->yFov / 2.0), up));
    Vector3D screenRight = sumVector3Ds(crossProductScalarVector3D(cos(camera->xFov / 2.0), forward), crossProductScalarVector3D(sin(camera->xFov / 2.0), right));

    Vector3D viewplane = sumVector3Ds(screenBottom, crossProductScalarVector3D(1 / 2.0, differenceVector3Ds(screenTop, screenBottom))); //normal to view plane, also the point
    screenRight = crossProductScalarVector3D(dotProductVector3Ds(viewplane, screenTop) / dotProductVector3Ds(viewplane, screenRight), screenRight);

    Vector3D ray = sumVector3Ds(viewplane, differenceVector3Ds(screenTop, screenRight)); //tl start

    Vector3D tr = sumVector3Ds(viewplane, differenceVector3Ds(screenRight, screenBottom));
    Vector3D bl = sumVector3Ds(viewplane, differenceVector3Ds(screenBottom, screenRight));

    Vector3D downInc = crossProductScalarVector3D(1 / (double) camera->height, differenceVector3Ds(bl, ray));
    Vector3D rightInc = crossProductScalarVector3D(1 / (double) camera->width, differenceVector3Ds(tr, ray));
    
    int x, y;
    for(y = 0; y < camera->height; y++){
        ray = sumVector3Ds(ray, downInc);
        for(x = 0; x < camera->width; x++){
            ray = sumVector3Ds(ray, rightInc);

            char point = castRay(*(camera->transform->position), ray, renderers, rendererCount);

            printf("%c", point);
            printf("%c", point);
        }
        printf("\n");
        ray = differenceVector3Ds(ray, crossProductScalarVector3D(camera->width, rightInc));
    }
}