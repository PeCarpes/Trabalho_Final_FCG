#pragma once

#include "tiny_obj_loader.h"
#include <glm/glm.hpp>
#include <Matrices.h>
#include <glad/glad.h>
#include <stdexcept>

#include <string>
#include <vector>

class ObjModel{
public:
    ObjModel(const char* filename, const char* basepath = NULL, bool triangulate = true);
    void ComputeNormals();
    void BuildTriangles();
    void draw() const;

    std::string getName() const;

private:
    tinyobj::attrib_t                attrib;
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;

    std::string name;
    int first_index;
    int num_indices;
    int rendering_mode;    
    GLuint vertex_array_object_id;

};
