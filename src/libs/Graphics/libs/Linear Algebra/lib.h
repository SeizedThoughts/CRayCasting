#include "consts.h"
#include "classes.h"

Vector3D sumVector3Ds(Vector3D vector1, Vector3D vector2){
    Vector3D sum;

    sum.x = vector1.x + vector2.x;
    sum.y = vector1.y + vector2.y;
    sum.z = vector1.z + vector2.z;

    return sum;
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

    //-PI/2 < x < PI/2
    while(sum.x > PI / 2) sum.x = PI - sum.x;
    while(sum.x < -PI / 2) sum.x = -PI - sum.x;
    
    //-PI < y < PI
    while(sum.y > PI) sum.y = 2 * PI - sum.y;
    while(sum.y < -PI) sum.y = -2 * PI - sum.y;

    return sum;
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

double angleBetween(Vector3D vector1, Vector3D vector2){
    return acos(dotProductVector3Ds(vector1, vector2) / (magnitude(vector1) * magnitude(vector2)));
}