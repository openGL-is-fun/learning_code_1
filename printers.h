//
// Created by tyler on 5/12/22.
//

#ifndef OPENGL_LEARNING_PRINTERS_H
#define OPENGL_LEARNING_PRINTERS_H

#include <ostream>

#include <glm/glm.hpp>

template <typename T>
std::ostream& operator<<(std::ostream& stream, glm::vec<3, T, glm::defaultp> val) {
    stream << "(" << val.x << ", " << val.y << ", " << val.z;
    return stream
}


#endif //OPENGL_LEARNING_PRINTERS_H
