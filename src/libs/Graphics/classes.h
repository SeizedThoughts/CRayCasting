#include "util/strcpy.h"

//Transform

typedef struct {
    Vector3D *position;
    EulerAngle *rotation;
} Transform;

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

//Camera

typedef struct {
    Transform *transform;
    unsigned int width, height;
    double xFov, yFov;
} Camera;

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

//Face

typedef struct {
    Vector3D *vertex1, *vertex2, *vertex3;
} Face;

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

//Model

typedef struct {
    Face *faces;
    Vector3D *verticies;
    unsigned int faceCount, vertexCount;
} Model;

Model *modelFromFile(char *dir){
    FILE *f;
    unsigned int i;
    char c;

    unsigned int dirLength = 0;

    while((c = dir[++dirLength]) != '\0');

    char filePath[dirLength + 11];

    strcpy(filePath, dir);
    strcpy(&filePath[dirLength], "verts.csv");

    f = fopen(filePath, "r");

    int vertexCount = 0;

    while((c = fgetc(f)) != EOF){
        if(c == '\n'){
            vertexCount++;
        }
    }

    rewind(f);

    Vector3D *verticies = (Vector3D*)malloc(vertexCount * sizeof(Vector3D));

    double x, y, z;
    while((c = fgetc(f)) != '\n');
    for(i = 0; i < vertexCount; i++){
        fscanf(f, "%lf,%lf,%lf\n", &x, &y, &z);
        verticies[i].x = x;
        verticies[i].y = y;
        verticies[i].z = z;
    }

    fclose(f);

    strcpy(&filePath[dirLength], "faces.csv");

    f = fopen(filePath, "r");

    int faceCount = 0;

    while((c = fgetc(f)) != EOF){
        if(c == '\n'){
            faceCount++;
        }
    }

    rewind(f);

    Face *faces = (Face*)malloc(faceCount * sizeof(Face));

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