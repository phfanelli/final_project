// Copyright (C) 2016 West Virginia University.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//
//     * Neither the name of West Virginia University nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Please contact the author of this library if you have any questions.
// TODO: Add your names and emails using the following format:
// Author: Shelby Shuff (sshuff@mix.wvu.edu)
// Author: Philip Fanelli (phfanelli@mix.wvu.edu)

// Use the right namespace for google flags.
#ifdef GFLAGS_NAMESPACE_GOOGLE
#define GLUTILS_GFLAGS_NAMESPACE google
#else
#define GLUTILS_GFLAGS_NAMESPACE gflags
#endif

#include <iostream>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader_program.h"
#include "model.h"
#include "transformations.h"
#include "camera_utils.h"
#include "model_creator.h"

#include <gflags/gflags.h>
#include <glog/logging.h>
// TODO: Include the headers you need for your project.
//Google flags for our GLSL shaders
DEFINE_string(vertex_shader_filepath, "",
              "Filepath of the vertex shader.");
DEFINE_string(fragment_shader_filepath, "",
              "Filepath of the fragment shader.");


namespace {
using wvu::Model;

constexpr int kWindowWidth = 640;
constexpr int kWindowHeight = 480;



static void ErrorCallback(int error, const char* description) {
  std::cerr << "ERROR: " << description << std::endl;
}

static void KeyCallback(GLFWwindow* window,
                        int key,
                        int scancode,
                        int action,
                        int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}

// sets opengl version
// open the window
// sets resizable to false
void SetWindowHints() {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

void ConfigureViewPort(GLFWwindow* window) {
  int width;
  int height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);
}

void ClearTheFrameBuffer() {
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool CreateShaderProgram(wvu::ShaderProgram* shader_program) {
  if (shader_program == nullptr) return false;
  //make local accessors to the google flag GLSL's
  const std::string fragment_shader_filepath = FLAGS_fragment_shader_filepath;

  const std::string vertex_shader_filepath = FLAGS_vertex_shader_filepath;

  shader_program->LoadFragmentShaderFromFile(fragment_shader_filepath);
  shader_program->LoadVertexShaderFromFile(vertex_shader_filepath);
  std::cout << vertex_shader_filepath << std::endl;
  std::cout << fragment_shader_filepath << std::endl;
  std::string error_info_log;
  if (!shader_program->Create(&error_info_log)) {
    std::cout << "ERROR: " << error_info_log << "\n";
  }
  if (!shader_program->shader_program_id()) {
    std::cerr << "ERROR: Could not create a shader program.\n";
    return false;
  }
  return true;
}

void RenderScene(const wvu::ShaderProgram& shader_program,
                 const Eigen::Matrix4f& projection,
                 const Eigen::Matrix4f& view,
                 std::vector<Model*>* models_to_draw,
                 GLFWwindow* window,
                 GLfloat angle) {
  ClearTheFrameBuffer();
  shader_program.Use();
  // Draw the models.
  int i=0;
  for (Model* model : *models_to_draw) {
    Eigen::Vector3f orientation = model->orientation();
/*
    //train rotation code
    Eigen::Vector3f pos = model->position();
    model->set_position(Eigen::Vector3f(pos[0]*cos(angle) - pos[2]*sin(angle), pos[1], pos[2]*sin(angle) + pos[0]*sin(angle) ));
*/
    Eigen::Vector3f circle_center = model->circle_center();
    Eigen::Vector3f position = model->position();
    GLuint radius = model->radius();
    Eigen::Vector3f movement = model->movement();
    model->set_orientation(Eigen::Vector3f(orientation(0)+movement(0), orientation(1)+movement(1), orientation(2)+movement(2)));

    if (radius != 0) {
      if(i==0) {
      // model->set_position(Eigen::Vector3f(radius+circle_center(0), circle_center(1), radius+circle_center(2)));
        model->set_position(Eigen::Vector3f(radius*cos(glfwGetTime()) + circle_center(0), circle_center(1), radius*sin(glfwGetTime()) + circle_center(2)));
        i++;
      } else {
        model->set_position(Eigen::Vector3f(-(radius*cos(glfwGetTime()) + circle_center(0)), circle_center(1), radius*sin(glfwGetTime()) + circle_center(2)));

      }
    }
    // i++;
    model->Draw(shader_program, projection, view);

  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void ConstructModels(std::vector<Model*>* models_to_draw) {
  wvu::CreateVerticesModel(models_to_draw);
  wvu::CreateLoadedModel(models_to_draw);
  for (Model* model : *models_to_draw) {
    model->SetVerticesIntoGpu();
  }
}

void DeleteModels(std::vector<Model*>* models_to_draw) {
  for (Model* model : *models_to_draw) {
      delete(model);
  }
}


}

int main(int argc, char** argv) {
  GLUTILS_GFLAGS_NAMESPACE::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  if (!glfwInit()) {
    return -1;
  }

  glfwSetErrorCallback(ErrorCallback);

  SetWindowHints();

  const std::string window_name = "A christmas scene";
  GLFWwindow* window = glfwCreateWindow(kWindowWidth,
                                        kWindowHeight,
                                        window_name.c_str(),
                                        nullptr,
                                        nullptr);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  glfwSetKeyCallback(window, KeyCallback);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::cerr << "Glew did not initialize properly!" << std::endl;
    glfwTerminate();
    return -1;
  }

  ConfigureViewPort(window);

  wvu::ShaderProgram shader_program;
  if (!CreateShaderProgram(&shader_program)) {
    return -1;
  }

  std::vector<Model*> models_to_draw;
  ConstructModels(&models_to_draw);

  const float field_of_view = wvu::ConvertDegreesToRadians(45.0f);
  const float aspect_ratio = static_cast<float>(kWindowWidth / kWindowHeight);
  const float near_plane = 0.1f;
  const float far_plane = 20.0f;
  const Eigen::Matrix4f& projection =
      wvu::ComputePerspectiveProjectionMatrix(field_of_view, aspect_ratio,
                                              near_plane, far_plane);
  const Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

//train rotation angle and speed
  GLfloat angle = 0.0f;
  const GLfloat rotation_speed = 2.0f;

  // Loop until the user closes the window.
  while (!glfwWindowShouldClose(window)) {
    // Render the scene!

    //angle changing over time
    angle = rotation_speed * static_cast<GLfloat>(glfwGetTime()) * M_PI / 180.f;

    RenderScene(shader_program, projection, view, &models_to_draw, window, angle);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  DeleteModels(&models_to_draw);
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
