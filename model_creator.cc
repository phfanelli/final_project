
#define cimg_display 0
#include <CImg.h>
#include "model_creator.h"
#include "model_loader.h"

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
  const std::string texture_filepath = "../texture2.png";
  const GLuint texture_id = LoadTexture(texture_filepath);
  Eigen::Vector3f orientation(1, 0, 0);
  Eigen::Vector3f movement(0.0006,0.0006,0);

  models_to_draw->push_back(new Model(orientation, Eigen::Vector3f(1,0.5f,-8), cube_vertices, cube_indices, texture_id, movement));
}

void CreateLoadedModel(std::vector<Model*>* models_to_draw) {
  std::vector<Eigen::Vector3f> model_vertices;
  std::vector<Eigen::Vector2f> model_texels;
  std::vector<Eigen::Vector3f> model_normals;
  std::vector<wvu::Face> model_faces;
  wvu::LoadObjModel("../tree.obj", &model_vertices, &model_texels, &model_normals, &model_faces);
  Eigen::MatrixXf vertices(3, model_vertices.size());
  for (int col = 0; col < model_vertices.size(); ++col) {
    vertices.col(col) = model_vertices[col];
  }
  std::vector<GLuint> indices;
  for (int face_id = 0; face_id < model_faces.size(); ++face_id) {
    const wvu::Face& face = model_faces[face_id];
    indices.push_back(face.vertex_indices[0]);
    indices.push_back(face.vertex_indices[1]);
    indices.push_back(face.vertex_indices[2]);
  }
  const std::string texture_filepath = "../test.jpg";
  const GLuint texture_id = LoadTexture(texture_filepath);

  models_to_draw->push_back(new Model(Eigen::Vector3f(0, 1, 0), Eigen::Vector3f(0,-2,-9), vertices, indices, Eigen::Vector3f(0, 0, 0)));

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
