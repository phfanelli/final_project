#include "model_creator.h"
#define cimg_display 0
#include <CImg.h>

namespace wvu {
void CreateVerticesModel(std::vector<Model*>* models_to_draw) {
  std::vector<GLuint> cube_indices = {
      0, 1, 3,
      0, 3, 2,
      2, 3, 5,
      2, 5, 4,
      4, 5, 7,
      4, 7, 6,
      0, 1, 7,
      0, 7, 6,
      6, 0, 2,
      6, 2, 4,
      7, 1, 3,
      7, 3, 5
  };

  Eigen::MatrixXf cube_vertices(5, 8);
  // 0
  cube_vertices.block(0, 0, 3, 1) = Eigen::Vector3f(0.0f, 1.0f, 0.0f);
  cube_vertices.block(3, 0, 2, 1) = Eigen::Vector2f(0, 1);
  // 1
  cube_vertices.block(0, 1, 3, 1) = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
  cube_vertices.block(3, 1, 2, 1) = Eigen::Vector2f(0, 0);
  // 2
  cube_vertices.block(0, 2, 3, 1) = Eigen::Vector3f(1.0f, 1.0f, 0.0f);
  cube_vertices.block(3, 2, 2, 1) = Eigen::Vector2f(1, 1);
  // 3
  cube_vertices.block(0, 3, 3, 1) = Eigen::Vector3f(1.0f, 0.0f, 0.0f);
  cube_vertices.block(3, 3, 2, 1) = Eigen::Vector2f(1, 0);
  // 4
  cube_vertices.block(0, 4, 3, 1) = Eigen::Vector3f(1.0f, 1.0f, -1.0f);
  cube_vertices.block(3, 4, 2, 1) = Eigen::Vector2f(0, 1);
  // 5
  cube_vertices.block(0, 5, 3, 1) = Eigen::Vector3f(1.0f, 0.0f, -1.0f);
  cube_vertices.block(3, 5, 2, 1) = Eigen::Vector2f(0, 0);
  // 6
  cube_vertices.block(0, 6, 3, 1) = Eigen::Vector3f(0.0f, 1.0f, -1.0f);
  cube_vertices.block(3, 6, 2, 1) = Eigen::Vector2f(1, 1);
  // 7
  cube_vertices.block(0, 7, 3, 1) = Eigen::Vector3f(0.0f, 0.0f, -1.0f);
  cube_vertices.block(3, 7, 2, 1) = Eigen::Vector2f(1, 0);

  Eigen::Vector3f orientation(1, 0, 0);

  models_to_draw->push_back(new Model(orientation, Eigen::Vector3f(1,0.5f,-8), cube_vertices, cube_indices, 0));
}

GLuint LoadTexture(const std::string& texture_filepath) {
  cimg_library::CImg<unsigned char> image;
  image.load(texture_filepath.c_str());
  const int width = image.width();
  const int height = image.height();

  image.permute_axes("cxyz");
  GLuint texture_id;
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);


  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
               0, GL_RGB, GL_UNSIGNED_BYTE, image.data());

  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
  return texture_id;
}
}
