#include "libs/Graphics/lib.h"
#define PI 3.14159265

void frameToFile(Frame *frame, const char *filePath){
    FILE* pgmimg;
    pgmimg = fopen(filePath, "wb");
  
    // Writing Magic Number to the File
    fprintf(pgmimg, "P3\n"); 
  
    // Writing Width and Height
    fprintf(pgmimg, "%d %d\n", frame->width, frame->height); 
  
    // Writing the maximum gray value
    fprintf(pgmimg, "255\n");
    Color temp;
    int i, j;
    int count = 0;
    for (i = 0; i < frame->height; i++) {
        for (j = 0; j < frame->width; j++) {
            temp = frame->pixels[frame->width * i + j];
  
            // Writing the gray values in the 2D array to the file
            fprintf(pgmimg, "%d %d %d ", temp.r, temp.g, temp.b);
        }
        fprintf(pgmimg, "\n");
    }
    fclose(pgmimg);
}

int main(void){
    Color *colors = malloc(12 * sizeof(Color));
    for(int i = 0; i < 12; i++){
        colors[i] = *newColor(255, 0, 0, i + 1);
    }

    Camera *camera = newCamera(2, 2, -2, -0.61547970867, PI / 4, 0, PI * 0.5, PI * 10 / 32);//newCamera(2, 2, -2, -0.61547970867, PI / 4, 0, 64, 36, PI * 0.5, PI * 10 / 32);
    Frame *frame = newFrame(1920, 1080);

    Renderer *renderers = malloc(1 * sizeof(Renderer));
    Renderer renderer;

    renderer.model = modelFromFile("../assets/models/cube/");
    renderer.transform = *newTransform(0, 0, 0, 0, PI/3, PI / 4);
    renderer.scalar = *newVector3D(1, 1, 1);
    renderer.textures = newTextures(colors, 12);

    renderers[0] = renderer;

    render(camera, renderers, 1, frame);

    frameToFile(frame, "pgmimg.ppm");

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