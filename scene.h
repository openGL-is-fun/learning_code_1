#ifndef OPENGL_LEARNING_SCENE_H
#define OPENGL_LEARNING_SCENE_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "glad/glad.h"

#include "geometry.h"

#include <vector>

struct ModelState {
    glm::mat4 transform = glm::identity<glm::mat3>();
};

struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
};


struct Geometry {
    GLuint vbo = 0;
    GLuint ibo = 0;
    GLuint vao = 0;
    GLenum type = GL_TRIANGLES;
    GLsizei elem_count = 0;
    GLenum index_type = GL_UNSIGNED_INT;
    ModelState state;
};

struct Scene {
    std::vector<Geometry> geometry;
};

Scene build_scene();

#endif //OPENGL_LEARNING_SCENE_H
