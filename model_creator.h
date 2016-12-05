#ifndef MODEL_CREATOR_H_
#define MODEL_CREATOR_H_

#include <vector>
#include "model.h"
namespace wvu {
void CreateVerticesModel(std::vector<Model*>* models_to_draw);
void CreateLoadedModel(std::vector<Model*>* models_to_draw);
GLuint LoadTexture(const std::string& texture_filepath);
}

#endif
