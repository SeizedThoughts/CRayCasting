//Euler Angle

typedef struct {
    //angles in radians
    double x, y, z;
} EulerAngle;

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

void printEulerAngle(EulerAngle angle){
    printf("x: %f, y: %f, z: %f\n", angle.x, angle.y, angle.z);
}

//Vector3D

typedef struct {
    double x, y, z;
} Vector3D;

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

void printVector3D(Vector3D vector3D){
    printf("x: %f, y: %f, z: %f\n", vector3D.x, vector3D.y, vector3D.z);
}