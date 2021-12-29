#include "libs/Graphics/lib.h"
#define PI 3.14159265

int main(void){
    char *textures = "##@@&&8CXUOP";
    //                ^^^^^^

    Camera *camera = newCamera(2, 2, -2, -0.61547970867, PI / 4, 0, 64, 36, PI * 0.5, PI * 10 / 32);//newCamera(2, 2, -2, -0.61547970867, PI / 4, 0, 64, 36, PI * 0.5, PI * 10 / 32);

    Renderer *renderers = malloc(1 * sizeof(Renderer));
    Renderer renderer;

    renderer.model = modelFromFile("assets/models/cube/");
    renderer.transform = *newTransform(0, 0, 0, 0, PI/3, PI / 4);
    renderer.scalar = *newVector3D(1, 1, 1);
    renderer.textures = newTextures(textures, 12);

    renderers[0] = renderer;

    render(camera, renderers, 1);

    freeCamera(camera);
    // freeModel(model);

    // Vector3D vert = renderer.model->verticies[0];
    // for(int i = 0; i < renderer.model->vertexCount; vert = renderer.model->verticies[++i]){
    //     printf("pre %d: ", i);
    //     printVector3D(vert);
    //     printf("post %d: ", i);
    //     printVector3D(translate(vert, renderer.transform));
    // }

    return 0;
}