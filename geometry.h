//
// Created by tyler on 5/12/22.
//

#ifndef OPENGL_LEARNING_GEOMETRY_H
#define OPENGL_LEARNING_GEOMETRY_H

#include <cstdint>

#include <glm/glm.hpp>

void create_cube(void* out, unsigned int* index_out, std::size_t offset, std::size_t stride, int index_start);

// Helper to get a T* to a location in a potentially multi-element object array.
// Uses the same idea as glVertexAttribPointer to specify where to find vertex elements.
template <typename T>
T* ptr_to_obj(void* root, std::size_t obj_offset, std::size_t stride, std::size_t obj_index) {
    return reinterpret_cast<T*>(static_cast<uint8_t*>(root) + obj_offset + stride*obj_index);
}

#endif //OPENGL_LEARNING_GEOMETRY_H
