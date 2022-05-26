#include "scene.h"

#include <array>

Scene build_scene() {
    Scene scene;
    std::array<Vertex, 8> cube_verts;
    std::array<GLuint, 38> indices;

    create_cube(cube_verts.data(), indices.data(), 0, sizeof(Vertex), 0);
    for(auto& vert : cube_verts) {
        vert.color = glm::vec4(glm::vec3(vert.position) + glm::vec3(0.5f), 1.0f);
    }

    Geometry cube {
        .type = GL_TRIANGLES,
        .elem_count = indices.size(),
        .index_type = GL_UNSIGNED_INT,
    };
    glGenBuffers(1, &cube.vbo);
    glGenBuffers(1, &cube.ibo);
    glGenVertexArrays(1, &cube.vao);

    glBindBuffer(GL_ARRAY_BUFFER, cube.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube.ibo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*cube_verts.size(), cube_verts.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), indices.data(), GL_STATIC_DRAW);
    glBindVertexArray(cube.vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube.ibo);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));
    glBindVertexArray(0);

    scene.geometry.push_back(cube);

    return scene;
}
