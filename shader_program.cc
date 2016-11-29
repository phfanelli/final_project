#include "shader_program.h"

#include <GL/glew.h>

namespace project {
namespace {

constexpr int kNumCharsInfoLog = 512;

enum ShaderType {
  VERTEX = 0,
  FRAGMENT = 1
};

GLuint CompileShader(const std::string& shader_src, const ShaderType shader_type, std::string* info_log) {
  GLuint shader_id = 0;
  switch (shader_type) {
    case VERTEX:
      shader_id = glCreateShader(GL_VERTEX_SHADER);
      break;
    case FRAGMENT:
      shader_id = glCreateShader(GL_FRAGMENT_SHADER);
      break;
  }

  const char* shader_src_ptr = shader_src.c_str();
  glShaderSource(shader_id, 1, &shader_src_ptr, nullptr);
  glCompileShader(shader_id);
  GLint success = 0;
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

  if (!success) {
    if (info_log) {
        // Allocate the number of chars in the string.
      info_log->resize(kNumCharsInfoLog);
        // Retrieve the error ingo log.
      glGetShaderInfoLog(shader_id, kNumCharsInfoLog, nullptr, &info_log->front());
    }
    return 0;
  }
  return shader_id;
}

GLuint CreateShaderProgram(const GLuint vertex_shader, const GLuint fragment_shader, std::string* info_log) {
  const GLuint shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);
  GLint success = 0;
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

  if (!success) {
    if (info_log) {
      info_log->resize(kNumCharsInfoLog);
      glGetProgramInfoLog(shader_program, kNumCharsInfoLog, nullptr, &info_log->front());
    }
    return 0;
  }
  return shader_program;
}

void ReleaseShaderResources(const GLuint vertex_shader, const GLuint fragment_shader) {
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}

}
bool ShaderProgram::LoadVertexShaderFromString(const std::string& vertex_shader_source) {
  vertex_shader_src_ = vertex_shader_source;
  return true;
}

bool ShaderProgram::LoadFragmentShaderFromString(const std::string& fragment_shader_source) {
  fragment_shader_src_ = fragment_shader_source;
  return true;
}

bool ShaderProgram::Create(std::string* error_info_log) {
  if (created_) return true;
  std::string info_log;
  if (!BuildVertexShader(&info_log)) {
    if (error_info_log) {
      *error_info_log = info_log;
    }
    return false;
  }
  if (!BuildFragmentShader(&info_log)) {
    if (error_info_log) {
      *error_info_log = info_log;
    }
    return false;
  }
  if (!LinkProgram(&info_log)) {
    if (error_info_log) {
      *error_info_log = info_log;
    }
    return false;
  }
  created_ = true;
  return true;
}

bool ShaderProgram::BuildVertexShader(std::string* info_log) {
  vertex_shader_ = CompileShader(vertex_shader_src_, VERTEX, info_log);
  return vertex_shader_ != 0;
}

bool ShaderProgram::BuildFragmentShader(std::string* info_log) {
  fragment_shader_ = CompileShader(fragment_shader_src_, FRAGMENT, info_log);
  return fragment_shader_ != 0;
}

bool ShaderProgram::LinkProgram(std::string* info_log) {
  shader_program_id_ = CreateShaderProgram(vertex_shader_, fragment_shader_, info_log);
  ReleaseShaderResources(vertex_shader_, fragment_shader_);
  return shader_program_id_ != 0;
}

}
