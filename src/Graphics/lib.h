#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265

typedef struct {
    double x, y, z;
} Vector3D;

typedef struct {
    //angles in radians
    double x, y, z;
} EulerAngle;

typedef struct {
    Vector3D *position;
    EulerAngle *rotation;
} Transform;

typedef struct {
    Transform *transform;
    unsigned int width, height;
    double xFov, yFov;
} Camera;

typedef struct {
    Vector3D *vertex1, *vertex2, *vertex3;
} Face;

typedef struct {
    Face *faces;
    Vector3D *verticies;
    unsigned int faceCount, vertexCount;
} Model;

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

void printVector3D(Vector3D vector3D){
    printf("x: %f, ", vector3D.x);
    printf("y: %f, ", vector3D.y);
    printf("z: %f\n", vector3D.z);
}

void printEulerAngle(EulerAngle angle){
    printf("x: %f, ", angle.x);
    printf("y: %f, ", angle.y);
    printf("z: %f\n", angle.z);
}

Vector3D *newVector3D(double x, double y, double z){
    Vector3D *newVector = malloc(1 * sizeof(Vector3D));

    newVector->x = x;
    newVector->y = y;
    newVector->z = z;

    return newVector;
}

void freeVector3D(Vector3D *vector3D){
    free(vector3D);
}

EulerAngle *newEulerAngle(double x, double y, double z){
    EulerAngle *newAngle = malloc(1 * sizeof(EulerAngle));

    newAngle->x = x;
    newAngle->y = y;
    newAngle->z = z;

    return newAngle;
}

void freeEulerAngle(EulerAngle *eulerAngle){
    free(eulerAngle);
}

Transform *newTransform(double xPos, double yPos, double zPos, double xTurn, double yTurn, double zTurn){
    Transform *newTransform = malloc(1 * sizeof(Transform));

    newTransform->position = newVector3D(xPos, yPos, zPos);
    newTransform->rotation = newEulerAngle(xTurn, yTurn, zTurn);

    return newTransform;
}

void freeTransform(Transform *transform){
    freeVector3D(transform->position);
    freeEulerAngle(transform->rotation);
    free(transform);
}

Camera *newCamera(double xPos, double yPos, double zPos, double xTurn, double yTurn, double zTurn, unsigned int width, unsigned int height, double xFov, double yFov){
    Camera *newCamera = malloc(1 * sizeof(Camera));

    newCamera->transform = newTransform(xPos, yPos, zPos, xTurn, yTurn, zTurn);
    newCamera->width = width;
    newCamera->height = height;
    newCamera->xFov = xFov;
    newCamera->yFov = yFov;

    return newCamera;
}

void freeCamera(Camera *camera){
    freeTransform(camera->transform);
    free(camera);
}

Face *newFace(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3){
    Face *face = malloc(1 * sizeof(face));
    
    face->vertex1 = newVector3D(x1, y1, z1);
    face->vertex2 = newVector3D(x2, y2, z2);
    face->vertex3 = newVector3D(x3, y3, z3);

    return face;
}

void freeFace(Face *face){
    freeVector3D(face->vertex1);
    freeVector3D(face->vertex2);
    freeVector3D(face->vertex3);
    free(face);
}

Model *modelFromFile(void){
    FILE *f;
    char c;
    f = fopen("assets/models/cube/verts.csv", "r");

    int vertexCount = 0;

    while((c = fgetc(f)) != EOF){
        if(c == '\n'){
            vertexCount++;
        }
    }

    rewind(f);

    Vector3D *verticies = malloc(vertexCount * sizeof(Vector3D));

    unsigned int i;
    double x, y, z;
    while((c = fgetc(f)) != '\n');
    for(i = 0; i < vertexCount; i++){
        fscanf(f, "%lf,%lf,%lf\n", &x, &y, &z);
        verticies[i].x = x;
        verticies[i].y = y;
        verticies[i].z = z;
    }

    fclose(f);

    f = fopen("assets/models/cube/faces.csv", "r");

    int faceCount = 0;

    while((c = fgetc(f)) != EOF){
        if(c == '\n'){
            faceCount++;
        }
    }

    rewind(f);

    Face *faces = malloc(faceCount * sizeof(Face));

    unsigned int p1, p2, p3;
    while((c = fgetc(f)) != '\n');
    for(i = 0; i < faceCount; i++){
        fscanf(f, "%d,%d,%d", &p1, &p2, &p3);
        faces[i].vertex1 = &verticies[p1];
        faces[i].vertex2 = &verticies[p2];
        faces[i].vertex3 = &verticies[p3];
    }

    fclose(f);

    Model *cube = malloc(1 * sizeof(Model));
    
    cube->verticies = verticies;
    cube->vertexCount = vertexCount;
    cube->faces = faces;
    cube->faceCount = faceCount;

    return cube;
}

void freeModel(Model *model){
    for(int i = 0; i < model->faceCount; i++){
        freeFace(&(model->faces[i]));
    }

    free(model);
}

Vector3D sumVector3Ds(Vector3D vector1, Vector3D vector2){
    Vector3D sum;

    sum.x = vector1.x + vector2.x;
    sum.y = vector1.y + vector2.y;
    sum.z = vector1.z + vector2.z;

    return sum;
}

Textures newTextures(char *textureValues, unsigned int textureCount){
    Textures textures;

    textures.textures = textureValues;
    textures.faceCount = textureCount;

    return textures;
}

Vector3D differenceVector3Ds(Vector3D vector1, Vector3D vector2){
    Vector3D diff;

    diff.x = vector1.x - vector2.x;
    diff.y = vector1.y - vector2.y;
    diff.z = vector1.z - vector2.z;

    return diff;
}

double dotProductVector3Ds(Vector3D vector1, Vector3D vector2){
    double prod = 0;

    prod += vector1.x * vector2.x;
    prod += vector1.y * vector2.y;
    prod += vector1.z * vector2.z;

    return prod;
}

Vector3D crossProductVector3Ds(Vector3D vector1, Vector3D vector2){
    Vector3D prod;

    prod.x = vector1.y * vector2.z - vector1.z * vector2.y;
    prod.y = vector1.z * vector2.x - vector1.x * vector2.z;
    prod.z = vector1.x * vector2.y - vector1.y * vector2.x;

    return prod;
}

Vector3D crossProductScalarVector3D(double scalar, Vector3D vector){
    Vector3D prod;

    prod.x = scalar * vector.x;
    prod.y = scalar * vector.y;
    prod.z = scalar * vector.z;

    return prod;
}

Vector3D scale(Vector3D vector, Vector3D scalar){
    Vector3D scaled;
    
    scaled.x = vector.x * scalar.x;
    scaled.y = vector.y * scalar.y;
    scaled.z = vector.z * scalar.z;

    return scaled;
}

Vector3D normalFromEularAngles(EulerAngle angles){
    Vector3D vector;

    vector.x = -sin(angles.y) * cos(angles.x);
    vector.y = sin(angles.x);
    vector.z = cos(angles.y) * cos(angles.x);

    return vector;
}

double magnitude(Vector3D vector){
    return sqrt(dotProductVector3Ds(vector, vector));
}

EulerAngle angleFromVector3D(Vector3D vector){
    double mag = magnitude(vector);
    EulerAngle angle;

    angle.x = asin(vector.y / mag);
    angle.y = acos(vector.z / mag / cos(angle.x));
    angle.z = 0;

    if(vector.x > 0) angle.y *= -1; //flip sign

    //-PI/2 < x < PI/2
    while(angle.x > PI / 2) angle.x = PI - angle.x;
    while(angle.x < -PI / 2) angle.x = -PI - angle.x;
    
    //-PI < y < PI
    while(angle.y > PI) angle.y = 2 * PI - angle.y;
    while(angle.y < -PI) angle.y = -2 * PI - angle.y;

    return angle;
}

EulerAngle sumEulerAngles(EulerAngle angle1, EulerAngle angle2){
    EulerAngle sum;

    sum.x = angle1.x + angle2.x;
    sum.y = angle1.y + angle2.y;
    sum.z = angle1.z + angle2.z;

    // //-PI/2 < x < PI/2
    // while(sum.x > PI / 2) sum.x = PI - sum.x;
    // while(sum.x < -PI / 2) sum.x = -PI - sum.x;
    
    // //-PI < y < PI
    // while(sum.y > PI) sum.y = 2 * PI - sum.y;
    // while(sum.y < -PI) sum.y = -2 * PI - sum.y;

    return sum;
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

Vector3D normalize(Vector3D vector){
    //implement with fast inv sqrt
    double mag = magnitude(vector);

    Vector3D newVector;

    newVector.x = vector.x / mag;
    newVector.y = vector.y / mag;
    newVector.z = vector.z / mag;

    return newVector;
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

    // printf("Intersect: ");
    // printVector3D(intersect);
    // printf("Plane: ");
    // printVector3D(xProd1223);

    Vector3D boundaryPlaneP1P2 = crossProductVector3Ds(xProd1223, diffP1P2);
    double offsetP1P2 = dotProductVector3Ds(boundaryPlaneP1P2, p1);
    
    // printf("Boundary P1P2: ");
    // printVector3D(boundaryPlaneP1P2);
    // printf("1&2: %f, %f, %f\n", dotProductVector3Ds(boundaryPlaneP1P2, p3), dotProductVector3Ds(boundaryPlaneP1P2, intersect), offsetP1P2);
    
    if((dotProductVector3Ds(boundaryPlaneP1P2, p3) - offsetP1P2 > 0) == (dotProductVector3Ds(boundaryPlaneP1P2, intersect) - offsetP1P2 <= 0)) return -1; //unequal signs

    Vector3D boundaryPlaneP2P3 = crossProductVector3Ds(xProd1223, diffP2P3);
    double offsetP2P3 = dotProductVector3Ds(boundaryPlaneP2P3, p2);

    // printf("Boundary P2P3: ");
    // printVector3D(boundaryPlaneP2P3);
    // printf("2&3: %f, %f, %f\n", dotProductVector3Ds(boundaryPlaneP2P3, p1), dotProductVector3Ds(boundaryPlaneP2P3, intersect), offsetP2P3);

    if((dotProductVector3Ds(boundaryPlaneP2P3, p1) - offsetP2P3 > 0) == (dotProductVector3Ds(boundaryPlaneP2P3, intersect) - offsetP2P3 <= 0)) return -1; //unequal signs

    Vector3D diffP3P1 = differenceVector3Ds(p3, p1);

    Vector3D boundaryPlaneP3P1 = crossProductVector3Ds(xProd1223, diffP3P1);
    double offsetP3P1 = dotProductVector3Ds(boundaryPlaneP3P1, p3);

    // printf("Boundary P3P1: ");
    // printVector3D(boundaryPlaneP3P1);
    // printf("3&1: %f, %f, %f\n", dotProductVector3Ds(boundaryPlaneP3P1, p2), dotProductVector3Ds(boundaryPlaneP3P1, intersect), offsetP3P1);
    
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
            // printf("vert1: ");
            // printVector3D(*face.vertex1);
            //normalize to camera at position 0, 0, 0
            Vector3D p1 = differenceVector3Ds(/**face.vertex1, position);*/translate(scale(*face.vertex1, renderer.scalar), renderer.transform), position);
            Vector3D p2 = differenceVector3Ds(/**face.vertex2, position);*/translate(scale(*face.vertex2, renderer.scalar), renderer.transform), position);
            Vector3D p3 = differenceVector3Ds(/**face.vertex3, position);*/translate(scale(*face.vertex3, renderer.scalar), renderer.transform), position);

            // printf("V1: ");
            // printVector3D(*face.vertex1);
            // printf("V2: ");
            // printVector3D(*face.vertex2);
            // printf("V3: ");
            // printVector3D(*face.vertex3);
            // printf("P1: ");
            // printVector3D(p1);
            // printf("P2: ");
            // printVector3D(p2);
            // printf("P3: ");
            // printVector3D(p3);
            double scalar = dist(p1, p2, p3, ray);
            // printf("%d, %f\n", j, scalar);
            if(scalar > 0 && (pickDist == -1 || (pickDist > 0 && scalar < pickDist))){
                pickDist = scalar;
                // printf("%d, %f\n", j, scalar);
                color = renderer.textures.textures[j];
            }
        }
    }

    return color;
}

double angleBetween(Vector3D vector1, Vector3D vector2){
    return acos(dotProductVector3Ds(vector1, vector2) / (magnitude(vector1) * magnitude(vector2)));
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