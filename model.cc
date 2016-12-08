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
// Author: Victor Fragoso (victor.fragoso@mail.wvu.edu)
#define _USE_MATH_DEFINES  // For using M_PI.
#include <cmath>
#include "model.h"
#include <iostream>
#include <string>
#include <CImg.h>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <gflags/gflags.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader_program.h"
#include "transformations.h"

namespace wvu {
  Model::Model(const Eigen::Vector3f& orientation,
               const Eigen::Vector3f& position,
               const Eigen::MatrixXf& vertices,
               const std::vector<GLuint>& indices) {
    orientation_ = orientation;
    position_ = position;
    vertices_ = vertices;
    indices_ = indices;
    texture_id_ = -1;
    radius_ = 0;
    vertex_buffer_object_id_ = 0;
    vertex_array_object_id_ = 0;
    element_buffer_object_id_ = 0;
  }

Model::Model(const Eigen::Vector3f& orientation,
             const Eigen::Vector3f& circle_center,
             const Eigen::MatrixXf& vertices,
             const std::vector<GLuint>& indices,
             const GLuint& texture_id,
             const Eigen::Vector3f& movement,
             const GLuint& radius) {
  orientation_ = orientation;
  vertices_ = vertices;
  indices_ = indices;
  texture_id_ = texture_id;
  movement_ = movement;
  radius_ = radius;
  circle_center_ = circle_center;
  vertex_buffer_object_id_ = 0;
  vertex_array_object_id_ = 0;
  element_buffer_object_id_ = 0;
}

Model::~Model() {
  glDeleteVertexArrays(1, &vertex_array_object_id_);
  glDeleteBuffers(1, &vertex_buffer_object_id_);
  glDeleteBuffers(1, &element_buffer_object_id_);
}

// Builds the model matrix from the orientation and position members.
Eigen::Matrix4f Model::ComputeModelMatrix() {
  Eigen::Matrix4f translation = ComputeTranslationMatrix(position_);
  Eigen::Matrix4f rotation = ComputeRotationMatrix(orientation_.normalized(), orientation_.norm());
  return translation * rotation;
}

// Setters set members by *copying* input parameters.
void Model::set_orientation(const Eigen::Vector3f& orientation) {
  orientation_ = orientation;
}

// Setters set members by *copying* input parameters.
void Model::set_position(const Eigen::Vector3f& position) {
  position_ = position;
}

void Model::set_circle_center(const Eigen::Vector3f& center) {
  circle_center_ = center;
}

Eigen::Vector3f* Model::mutable_orientation() {
  return &orientation_;
}

Eigen::Vector3f* Model::mutable_position() {
  return &position_;
}

const Eigen::Vector3f& Model::orientation() {
  return orientation_;
}

const Eigen::Vector3f& Model::position() {
  return position_;
}

const Eigen::MatrixXf& Model::vertices() const {
  return vertices_;
}

const std::vector<GLuint>& Model::indices() const {
  return indices_;
}

const GLuint& Model::texture_id() const {
  return texture_id_;
}

const Eigen::Vector3f& Model::movement() const {
  return movement_;
}

const GLuint& Model::radius() const {
  return radius_;
}

const Eigen::Vector3f& Model::circle_center() {
  return circle_center_;
}

const GLuint Model::vertex_buffer_object_id() const {
  return vertex_buffer_object_id_;
}

const GLuint Model::vertex_buffer_object_id() {
  return vertex_buffer_object_id_;
}

const GLuint Model::vertex_array_object_id() const {
  return vertex_array_object_id_;
}

const GLuint Model::vertex_array_object_id() {
  return vertex_array_object_id_;
}

const GLuint Model::element_buffer_object_id() const {
  return element_buffer_object_id_;
}

const GLuint Model::element_buffer_object_id() {
  return element_buffer_object_id_;
}

void Model::SetVerticesIntoGpu() {
  //vao
  constexpr int kNumVertexArrays = 1;
  glGenVertexArrays(kNumVertexArrays, &vertex_array_object_id_);
  glBindVertexArray(vertex_array_object_id_);

  //vbo
  glGenBuffers(1, &vertex_buffer_object_id_);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_id_);
  const int vertices_size = vertices_.rows() * vertices_.cols() * sizeof(vertices_(0,0));
  glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices_.data(), GL_STATIC_DRAW);
  constexpr GLuint kIndex = 0;

  if(texture_id() == -1) {
    constexpr GLuint kNumElementsPerVertex = 3;
    constexpr GLuint kStride = kNumElementsPerVertex * sizeof(vertices_(0, 0));
    const GLvoid* offset_ptr = nullptr;
    glVertexAttribPointer(kIndex, kNumElementsPerVertex, GL_FLOAT, GL_FALSE, kStride, offset_ptr);
    glEnableVertexAttribArray(kIndex);
  } else {
    constexpr GLuint kNumElementsPerVertex = 5;
    constexpr GLuint kStride = kNumElementsPerVertex * sizeof(vertices_(0, 0));
    const GLvoid* offset_ptr = nullptr;
    glVertexAttribPointer(kIndex, 3, GL_FLOAT, GL_FALSE, kStride, offset_ptr);
    glEnableVertexAttribArray(kIndex);

    const GLvoid* offset_texel = reinterpret_cast<GLvoid*>(3 * sizeof(vertices_(0, 0)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          kStride, offset_texel);
    glEnableVertexAttribArray(1);
  }


  glBindBuffer(GL_ARRAY_BUFFER, 0);

  //ebo
  glGenBuffers(1, &element_buffer_object_id_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object_id_);
  const int indices_size = indices_.size() * sizeof(indices_[0]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices_.data(), GL_STATIC_DRAW);


}

void Model::Draw(const ShaderProgram& shader_program,
                 const Eigen::Matrix4f& projection,
                 const Eigen::Matrix4f& view) {
  // The model transformation must be computed using ComputeModelMatrix().
  const Eigen::Matrix4f model_matrix = ComputeModelMatrix();

  const GLint model_location = glGetUniformLocation(shader_program.shader_program_id(), "model");
  const GLint view_location = glGetUniformLocation(shader_program.shader_program_id(), "view");
  const GLint projection_location = glGetUniformLocation(shader_program.shader_program_id(), "projection");


  if(texture_id() != -1) {
    glBindTexture(GL_TEXTURE_2D, texture_id_);
  } else {
    // glColor3f(1.0f, 1.0f, 0.0f);
  }

  glUniformMatrix4fv(model_location, 1, GL_FALSE, model_matrix.data());
  glUniformMatrix4fv(view_location, 1, GL_FALSE, view.data());
  glUniformMatrix4fv(projection_location, 1, GL_FALSE, projection.data());


  glBindVertexArray(vertex_array_object_id_);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);



}

}  // namespace wvu
