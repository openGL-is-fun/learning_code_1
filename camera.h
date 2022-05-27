#ifndef OPENGL_LEARNING_CAMERA_H
#define OPENGL_LEARNING_CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
    Camera() = default;
    virtual ~Camera() = default;

    virtual glm::vec3 eye() const = 0;
    virtual glm::vec3 target() const = 0;
    virtual glm::vec3 up() const = 0;
    virtual glm::mat4 projection() const = 0;
private:
};

class CameraController {
public:
    CameraController() = default;
    virtual ~CameraController() = default;
};

class BasicCamera: public Camera {
public:
    BasicCamera(glm::vec3 eye, glm::vec3 target, glm::vec3 up, glm::mat4 proj):
        m_eye(eye), m_target(target), m_up(up), m_proj(proj) {}

    virtual glm::vec3 eye() const override { return m_eye; }
    virtual glm::vec3 target() const override { return m_target; }
    virtual glm::vec3 up() const override { return m_up; }
    virtual glm::mat4 projection() const override { return m_proj; }

    glm::vec3 m_eye = glm::vec3(0.0f);
    glm::vec3 m_target = glm::vec3(0.0f);
    glm::vec3 m_up = glm::vec3(0.0f);
    glm::mat4 m_proj = glm::mat4(1.0f);
};

class SphericalCamera: public Camera {
public:
    SphericalCamera(float theta, float phi, float radius, glm::vec3 center, glm::mat4 proj):
        m_theta(theta), m_phi(phi), m_radius(radius), m_center(center), m_proj(proj) {}

    virtual glm::vec3 eye() const override {
        return {
            m_radius*std::cos(m_theta)*std::sin(m_phi),
            m_radius*std::sin(m_theta)*std::sin(m_phi),
            m_radius*std::cos(m_theta)
        };
    }
    virtual glm::vec3 target() const override { return m_center; }
    virtual glm::vec3 up() const override { return {0.0f, 1.0f, 0.0f}; }
    virtual glm::mat4 projection() const override { return m_proj; }

    float m_theta = 0.0f;
    float m_phi = 0.0f;
    float m_radius = 0.0f;
    glm::vec3 m_center = glm::vec3(0.0f);
    glm::mat4 m_proj = glm::mat4(1.0f);
};

#endif //OPENGL_LEARNING_CAMERA_H
