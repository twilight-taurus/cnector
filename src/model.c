#include <model.h>
#include <string.h>

CModel cmodel_load(const char * file_name) {
    CModel my_model;

    const char * path = file_name;
    my_model.model = LoadModel(file_name); // add model
    strcpy_s(my_model.path, 64 * sizeof(char), path);

/*
    int * index_arr =
        calloc( (my_model.model.meshes[0].triangleCount) * 3, sizeof(int) );

    unsigned short * index_base = (RENDER_STATE->terrain_model.model.meshes[0].indices);
    int vertexStride = (int)(3*sizeof(float));

    for (int i = 0 ; i < (RENDER_STATE->terrain_model.model.meshes[0].triangleCount) * 3 ; i++) {
        index_arr[i] = (int)index_base[i];
    }

    my_model.indices = index_arr;
*/

    return my_model;
}

void cmodel_unload(CModel * cmodel) {
    UnloadModel(cmodel->model);
    free(cmodel->indices);

    return;
}