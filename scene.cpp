#include "scene.h"

#include <array>

Scene build_scene() {
    Scene scene;
    std::array<Vertex, 8> cube_verts;
    std::array<Vertex, 8> light_verts;
    std::array<GLuint, 38> indices;

    create_cube(cube_verts.data(), indices.data(), 0, sizeof(Vertex), 0);
    for(auto& vert : cube_verts) {
        vert.color = glm::vec4(glm::vec3(vert.position) + glm::vec3(0.5f), 1.0f);
    }

    cube_verts[0].normal = glm::cross(cube_verts[1].position, cube_verts[2].position);
    cube_verts[1].normal = glm::cross(cube_verts[0].position, cube_verts[3].position);
    cube_verts[2].normal = glm::cross(cube_verts[0].position, cube_verts[3].position);
    cube_verts[3].normal = glm::cross(cube_verts[2].position, cube_verts[1].position);
    cube_verts[4].normal = glm::cross(cube_verts[0].position, cube_verts[1].position);
    cube_verts[5].normal = glm::cross(cube_verts[4].position, cube_verts[1].position);
    cube_verts[6].normal = glm::cross(cube_verts[4].position, cube_verts[7].position);
    cube_verts[7].normal = glm::cross(cube_verts[3].position, cube_verts[5].position);

    //replace colors with normal value or use gllines and draw a line for each of the normals
    //x red
    //y green
    //z blue

    create_cube(light_verts.data(), indices.data(), 0, sizeof(Vertex), 0);
    for(auto& vert : light_verts) {
        vert.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }

    Geometry cube {
        .type = GL_TRIANGLES,
        .elem_count = indices.size(),
        .index_type = GL_UNSIGNED_INT,
    };

    Geometry lightSource{
        .type = GL_TRIANGLES,
        .elem_count = indices.size(),
        .index_type = GL_UNSIGNED_INT,
    };

    lightSource.state.transform = glm::translate(lightSource.state.transform, glm::vec3(3.0, 3.0, 1.0));
    lightSource.state.transform = glm::scale(lightSource.state.transform, glm::vec3(0.5, 0.5,0.5));

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
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
    glBindVertexArray(0);



    glGenBuffers(1, &lightSource.vbo);
    glGenBuffers(1, &lightSource.ibo);
    glGenVertexArrays(1, &lightSource.vao);

    glBindBuffer(GL_ARRAY_BUFFER, lightSource.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightSource.ibo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*light_verts.size(), light_verts.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), indices.data(), GL_STATIC_DRAW);
    glBindVertexArray(lightSource.vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightSource.ibo);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));
    glBindVertexArray(0);

    scene.geometry.push_back(cube);
    scene.geometry.push_back(lightSource);

    return scene;
}
