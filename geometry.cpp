//
// Created by tyler on 5/12/22.
//

#include "geometry.h"

#include <cstdint>
#include <array>

void create_cube(void* out, int* index_out, std::size_t stride, int index_start) {
    static constexpr std::array<int, 6> indices = {
        // Front face
        0, 1, 2,
        2, 1, 3,
        //Top face
    };
    constexpr int NUM_VERT = 8;
    auto target = static_cast<float*>(out);
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

        target = reinterpret_cast<float*>(static_cast<uint8_t*>(out) + stride*i);
    }

    for(auto idx : indices) {
        *index_out = index_start+idx;
        index_out += 1;
    }

}