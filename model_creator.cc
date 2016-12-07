
#define cimg_display 0
#include <CImg.h>
#include "model_creator.h"
#include "model_loader.h"

namespace wvu {
void CreateVerticesModel(std::vector<Model*>* models_to_draw) {


  //middle train car
  std::vector<GLuint> indices_for_train_middle = {
      3, 0, 1,  // First triangle.
      3, 1, 2,  // Second triangle.
      7, 4, 5,  // Third triangle.
      7, 5, 6,  // Fourth triangle.
      7, 4, 0,  // Fifth triangle.
      7, 0, 3,  // Sixth triangle.
      2, 1, 5,  // Seventh triangle.
      2, 5, 6,  // Eigth triangle.
      7, 3, 2,  // Ninth Triangle
      7, 2, 6,  // Tenth Triangle
      0, 4, 5,  // Eleventh Triangle
      0, 5, 1   // Twelfth Triangle
    };
    Eigen::MatrixXf vertices_for_train_middle(5, 8);
    vertices_for_train_middle.block(0,0,3,1) = Eigen::Vector3f(-1.0f,0.0f,0.0f);
    vertices_for_train_middle.block(3,0,2,1) = Eigen::Vector2f(0,0);

    vertices_for_train_middle.block(0,1,3,1) = Eigen::Vector3f(1.0f,0.0f,0.0f);
    vertices_for_train_middle.block(3,1,2,1) = Eigen::Vector2f(1,0);

    vertices_for_train_middle.block(0,2,3,1) = Eigen::Vector3f(1.0f,1.0f,0.0f);
    vertices_for_train_middle.block(3,2,2,1) = Eigen::Vector2f(1,1);

    vertices_for_train_middle.block(0,3,3,1) = Eigen::Vector3f(-1.0f,1.0f,0.0f);
    vertices_for_train_middle.block(3,3,2,1) = Eigen::Vector2f(0,1);

    vertices_for_train_middle.block(0,4,3,1) = Eigen::Vector3f(-1.0f,0.0f,-1.0f);
    vertices_for_train_middle.block(3,4,2,1) = Eigen::Vector2f(0,0);

    vertices_for_train_middle.block(0,5,3,1) = Eigen::Vector3f(1.0f,0.0f,-1.0f);
    vertices_for_train_middle.block(3,5,2,1) = Eigen::Vector2f(1,0);

    vertices_for_train_middle.block(0,6,3,1) = Eigen::Vector3f(1.0f,1.0f,-1.0f);
    vertices_for_train_middle.block(3,6,2,1) = Eigen::Vector2f(1,1);

    vertices_for_train_middle.block(0,7,3,1) = Eigen::Vector3f(-1.0f,1.0f,-1.0f);
    vertices_for_train_middle.block(3,7,2,1) = Eigen::Vector2f(0,1);

    const std::string texture_filepath = "../texture2.png";
    const GLuint texture_id = LoadTexture(texture_filepath);
    Eigen::Vector3f orientation(0, 1, 0);
    Eigen::Vector3f movement(0,0.005,0);

    models_to_draw->push_back(new Model(orientation, Eigen::Vector3f(0,-2,-9), vertices_for_train_middle, indices_for_train_middle, texture_id, movement, 4));

    std::vector<GLuint> gift2_indices = {
        3, 0, 1,  // First triangle.
        3, 1, 2,  // Second triangle.
        7, 4, 5,  // Third triangle.
        7, 5, 6,  // Fourth triangle.
        7, 4, 0,  // Fifth triangle.
        7, 0, 3,  // Sixth triangle.
        2, 1, 5,  // Seventh triangle.
        2, 5, 6,  // Eigth triangle.
        7, 3, 2,  // Ninth Triangle
        7, 2, 6,  // Tenth Triangle
        0, 4, 5,  // Eleventh Triangle
        0, 5, 1   // Twelfth Triangle
      };
      Eigen::MatrixXf gift2_vertices(5, 8);
      gift2_vertices.block(0,0,3,1) = Eigen::Vector3f(0.0f,0.0f,0.0f);
      gift2_vertices.block(3,0,2,1) = Eigen::Vector2f(0,0);

      gift2_vertices.block(0,1,3,1) = Eigen::Vector3f(1.0f,0.0f,0.0f);
      gift2_vertices.block(3,1,2,1) = Eigen::Vector2f(1,0);

      gift2_vertices.block(0,2,3,1) = Eigen::Vector3f(1.0f,1.0f,0.0f);
      gift2_vertices.block(3,2,2,1) = Eigen::Vector2f(1,1);

      gift2_vertices.block(0,3,3,1) = Eigen::Vector3f(0.0f,1.0f,0.0f);
      gift2_vertices.block(3,3,2,1) = Eigen::Vector2f(0,1);

      gift2_vertices.block(0,4,3,1) = Eigen::Vector3f(0.0f,0.0f,-1.0f);
      gift2_vertices.block(3,4,2,1) = Eigen::Vector2f(0,0);

      gift2_vertices.block(0,5,3,1) = Eigen::Vector3f(1.0f,0.0f,-1.0f);
      gift2_vertices.block(3,5,2,1) = Eigen::Vector2f(1,0);

      gift2_vertices.block(0,6,3,1) = Eigen::Vector3f(1.0f,1.0f,-1.0f);
      gift2_vertices.block(3,6,2,1) = Eigen::Vector2f(1,1);

      gift2_vertices.block(0,7,3,1) = Eigen::Vector3f(0.0f,1.0f,-1.0f);
      gift2_vertices.block(3,7,2,1) = Eigen::Vector2f(0,1);

      const std::string texture_filepath2 = "../texture2.png";
      const GLuint texture_id2 = LoadTexture(texture_filepath2);
      Eigen::Vector3f orientation2(0, 1, 0);
      Eigen::Vector3f movement2(0,0.005,0);

      models_to_draw->push_back(new Model(orientation2, Eigen::Vector3f(0,-2,-9), gift2_vertices, gift2_indices, texture_id2, movement2, 2));

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
  const std::string texture_filepath ="../texture2.png";
  const GLuint texture_id = LoadTexture(texture_filepath);

  models_to_draw->push_back(new Model(Eigen::Vector3f(0, 1, 0), Eigen::Vector3f(0,-2,-9), vertices, indices));

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
