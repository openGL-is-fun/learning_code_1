//
// Created by tyler on 5/12/22.
//

#include "geometry.h"

#include <cstdint>
#include <array>

void create_cube(void* out, unsigned int* index_out, std::size_t offset, std::size_t stride, int index_start) {
    static constexpr std::array<int, 38> indices = {
        //Front face
        0, 1, 2,
        1, 3, 2,
        // Top face
        2, 3, 6,
        3, 7, 6,
        // Right face
        3, 1, 7,
        1, 5, 7,
        // Bottom face
        0, 1, 4,
        1, 5, 4,
        // Left face
        0, 2, 4,
        2, 6, 4,
        // Back face
        4, 5, 6,
        5, 7, 6,
    };
    constexpr int NUM_VERT = 8;
    auto target = ptr_to_obj<float>(out, offset, stride, 0);
    if (stride == 0) {
        stride = sizeof(float)*3;
    }

    for(int i = 0; i < NUM_VERT; ++i) {
        // x
        *target = (i & 0x1) > 0 ? 0.5f : -0.5f;
        target += 1;
        // y
        *target = (i & 0x2) > 0 ? 0.5f : -0.5f;
        target += 1;
        // z
        *target = (i & 0x4) > 0 ? 0.5f : -0.5f;
        target += 1;

        //target = reinterpret_cast<float*>(static_cast<uint8_t*>(out) + offset + stride*i);
        target = ptr_to_obj<float>(out, offset, stride, i+1);
    }

    for(auto idx : indices) {
        *index_out = index_start+idx;
        index_out += 1;
    }

}