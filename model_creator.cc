#include "model_creator.h"

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
  Eigen::MatrixXf cube_vertices(3, 8);
  cube_vertices.col(0) = Eigen::Vector3f(0.0f, 1.0f, 0.0f);
  cube_vertices.col(1) = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
  cube_vertices.col(2) = Eigen::Vector3f(1.0f, 1.0f, 0.0f);
  cube_vertices.col(3) = Eigen::Vector3f(1.0f, 0.0f, 0.0f);
  cube_vertices.col(4) = Eigen::Vector3f(1.0f, 1.0f, -1.0f);
  cube_vertices.col(5) = Eigen::Vector3f(1.0f, 0.0f, -1.0f);
  cube_vertices.col(6) = Eigen::Vector3f(0.0f, 1.0f, -1.0f);
  cube_vertices.col(7) = Eigen::Vector3f(0.0f, 0.0f, -1.0f);

  Eigen::Vector3f orientation(1, 0, 0);
  models_to_draw->push_back(new Model(orientation, Eigen::Vector3f(1,0.5f,-8), cube_vertices, cube_indices));
}

}
