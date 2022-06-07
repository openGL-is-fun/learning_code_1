#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include <numbers>
#include <cmath>
#include <stdlib.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "glad/glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "scene.h"

struct WorldState;

using Clock = std::chrono::high_resolution_clock;

void update(GLFWwindow* wnd, WorldState& state);
void draw(GLFWwindow* wnd, const WorldState& state);
void init_gl(GLFWwindow* wnd);

template <typename V, typename I>
Geometry load_geometry(const void* vert_data, const I* index_data);

GLuint g_program_handle = 0;

constexpr float fov = 45.0f;
constexpr float TRANS_ROTATION_SPEED = 1.0f;
constexpr float EYE_ROTATION_SPEED = 1.5f;
constexpr float CAMERA_SPEED = 2.0f;

struct CameraState {
    glm::vec3 eye = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 target = glm::vec3(0.0, 0.0, 0.0);
    glm::mat4 proj = glm::identity<glm::mat4>();
};

struct WorldState {
    CameraState camera;
    ModelState model_state;
    Clock::time_point start_time = Clock::now();
    Scene scene;
};

void opengl_debug_callback(GLenum source,
                           GLenum type,
                           GLuint id,
                           GLenum severity,
                           GLsizei length,
                           const GLchar* message,
                           const void* userParam);

std::string get_shader_source(const char* file) {
    auto stream = std::ifstream(file, std::ios::in);
    if(!stream.is_open()){
        std::cout << "Shader file couldn't not be opened or does not exist";
    }

    stream.seekg (0, stream.end);
    int length = stream.tellg();
    stream.seekg(0, stream.beg);

    std::string data(length, '\0');
    stream.read(data.data(), length);
    return data;
}

std::string get_shader_log(GLuint handle) {
    GLint length = 0;
    glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &length);

    std::string log;
    log.resize(length, '\0');
    glGetShaderInfoLog(handle, length, &length, log.data());

    return log;
}

GLFWwindow* create_window(int width, int height) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    auto wnd = glfwCreateWindow(width, height, "OpenGL Example", nullptr, nullptr);
    glfwMakeContextCurrent(wnd);
    return wnd;
}

static void key_callback(GLFWwindow* wnd, int key, int scancode, int action, int mods) {
    switch (key) {
        case GLFW_KEY_ESCAPE: {
            glfwSetWindowShouldClose(wnd, GLFW_TRUE);
            break;
        }
        default: {}
    }
}

int main() {
    if(!glfwInit()) {
        std::cerr << "Failed to init glfw.\n";
        return -1;
    }
    GLFWwindow* wnd = create_window(1920, 1080);
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD\n";
        return -1;
    }

    glfwSetKeyCallback(wnd, key_callback);
    glfwSwapInterval(1); // sync your rendering loop to the refresh rate of the monitor
    init_gl(wnd);

    WorldState state {
        .camera = {
            .eye = glm::vec3(0.0, 0.0, 5.0),
            .target = glm::vec3(0.0, 0.0, 0.0),
            .proj = glm::identity<glm::mat3>(),
        },
        .model_state = {
            .transform = glm::identity<glm::mat4>(),
        },
        .scene = build_scene(),
    };

    while(!glfwWindowShouldClose(wnd)) {
        glfwPollEvents();
        update(wnd, state);
        draw(wnd, state);
        glfwSwapBuffers(wnd);
        //std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    glfwDestroyWindow(wnd);
    return 0;
}

void init_gl(GLFWwindow* wnd) {
    auto vert_src = get_shader_source("vert.glsl"); //get shader file
    auto frag_src = get_shader_source("frag.glsl"); //get shader file

    glDebugMessageCallback(opengl_debug_callback, nullptr); //specify a callback to receive debugging messages from the GL

    int vert_len = vert_src.length(); //length of vertex shader string
    auto vert_str = vert_src.c_str(); //makes a c string from the string object
    int frag_len = frag_src.length();
    auto frag_str = frag_src.c_str();

    GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vert_str, &vert_len);
    glCompileShader(vert_shader);
    GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_str, &frag_len);
    glCompileShader(frag_shader);


    int success = 0;
    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE) {
        std::cerr << get_shader_log(vert_shader) << std::endl;
        throw std::runtime_error("Compilation of vertex shader failed");
    }
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE) {
        std::cerr << get_shader_log(frag_shader) << std::endl;
        throw std::runtime_error("Compilation of frag shader failed");
    }

    g_program_handle = glCreateProgram();
    glAttachShader(g_program_handle, vert_shader);
    glAttachShader(g_program_handle, frag_shader);
    glLinkProgram(g_program_handle);

    glGetProgramiv(g_program_handle, GL_LINK_STATUS, &success);
    if(success == GL_FALSE) {
        throw std::runtime_error("Linking shader failed");
    }

    //glDisable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);

    glClearColor(0.2, 0.2, 0.2, 0.2);
}

void update(GLFWwindow* wnd, WorldState& state) {
    constexpr float FPS = 60.0f;
    int width = 0, height = 0;
    glfwGetFramebufferSize(wnd, &width, &height);// |---*---|

    state.camera.proj = glm::perspective(45.0f, static_cast<float>(width)/height, 0.05f, 100.0f);
    //fovy - Specifies the field of view angle, in degrees, in the y direction.
    //aspect - Specifies the aspect ratio that determines the field of view in the x direction. The aspect ratio is the ratio of x (width) to y (height).
    //state.camera.proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 100.0f);

    auto now = Clock::now();
    std::chrono::duration<float> elapsed = now - state.start_time;
    float transRads = std::fmod(elapsed.count() * TRANS_ROTATION_SPEED, (2.0f * std::numbers::pi_v<float>));

    // M2 * M1 * v
    // v * M1 * M2
    // [A A 0]
    // [A A 0]
    // [T T 1]
//    state.model_state.transform = glm::mat4(std::cos(transRads), -std::sin(transRads), 0.0, 0.0,
//                                            std::sin(transRads), std::cos(transRads), 0.0, 0.0,
//                                            0.0, 0.0, 1.0, 0.0,
//                                            0.0, 0.0, 0, 1.0);

    float eyeRads = std::fmod(elapsed.count() * EYE_ROTATION_SPEED, (2.0f * std::numbers::pi_v<float>));

    //state.camera.eye.x = 3 * std::cos(eyeRads); //rotates camera around (0,0,0)
    //state.camera.eye.z = 3 * std::sin(eyeRads);

    float y_rot = 0;

    glm::distance(state.camera.eye, state.camera.target);

    if (glfwGetKey(wnd, GLFW_KEY_W) == GLFW_PRESS) {
        state.camera.eye.y += CAMERA_SPEED / FPS;
    }
    if (glfwGetKey(wnd, GLFW_KEY_S) == GLFW_PRESS) {
        state.camera.eye.y -= CAMERA_SPEED / FPS;
    }
    if (glfwGetKey(wnd, GLFW_KEY_A) == GLFW_PRESS) {
        state.camera.eye.x -= CAMERA_SPEED / FPS;
    }
    if (glfwGetKey(wnd, GLFW_KEY_D) == GLFW_PRESS) {
        state.camera.eye.x += CAMERA_SPEED / FPS;
    }
    if (glfwGetKey(wnd, GLFW_KEY_H) == GLFW_PRESS) {
        state.camera.eye.z -= CAMERA_SPEED / FPS;
    }
    if (glfwGetKey(wnd, GLFW_KEY_N) == GLFW_PRESS) {
        state.camera.eye.z += CAMERA_SPEED / FPS;
    }
    if (glfwGetKey(wnd, GLFW_KEY_Q) == GLFW_PRESS) {
        y_rot -= EYE_ROTATION_SPEED / FPS;
    }
    if (glfwGetKey(wnd, GLFW_KEY_E) == GLFW_PRESS) {
        y_rot += EYE_ROTATION_SPEED / FPS;
    }

    //rotate camera around y axis
    auto camAroundY = glm::mat3(std::cos(y_rot), 0, -std::sin(y_rot),
                                  0, 1, 0,
                                  std::sin(y_rot), 0, std::cos(y_rot));

    state.camera.eye = camAroundY * state.camera.eye;

    // Make it so we always look straight down the Z
    //state.camera.target.x = state.camera.eye.x; //by commenting these lines out, we are always looking at (0, 0, 0)
    //state.camera.target.y = state.camera.eye.y;

//    std::cout << "eye.x: " << state.camera.eye.x << "\n";
//    std::cout << "eye.y: " << state.camera.eye.y << "\n";
//    std::cout << "eye.z: " << state.camera.eye.z << "\n\n";
}

void draw(GLFWwindow* wnd, const WorldState& state) {
    int width = 0, height = 0;
    glfwGetFramebufferSize(wnd, &width, &height);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(g_program_handle);

    auto view = glm::lookAt(state.camera.eye, state.camera.target, glm::vec3(0.0, 1.0, 0.0));
    glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(view)); //camera position
    glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(state.camera.proj)); //glm::perspective for fov and clipping planes

    glUniform4fv(3, 1, glm::value_ptr(glm::vec4(1,1,1,1))); //light color in fragment shader
    glUniform3fv(4, 1, glm::value_ptr(glm::vec3(3.0, 3.0, 1.0))); //light position

    //glm::cross for normal and glsl normalize to make it a unit vector

    for(const auto& geom : state.scene.geometry) {
        glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(geom.state.transform)); //triangle transformation
        glBindVertexArray(geom.vao);
        glDrawElements(geom.type, geom.elem_count, geom.index_type, 0);
    }
}

void opengl_debug_callback(GLenum source,
                           GLenum type,
                           GLuint id,
                           GLenum severity,
                           GLsizei length,
                           const GLchar* message,
                           const void* userParam) {
    std::cout << "OGL Debug: Source: " << source << " -- Type " << type << " -- ID " << id << " Severity: " << severity
              << " Msg: " << message << std::endl;
}

template <typename V, typename I>
Geometry load_geometry(const void* vert_data, std::size_t vert_len, const I* index_data) {
    GLuint vbo, ibo, vao;
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);
    glGenVertexArrays(1, &vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(V), nullptr);
    glBindVertexArray(0);

    return Geometry {
        .vbo = vbo,
        .ibo = ibo,
        .vao = vao,
    };
}
