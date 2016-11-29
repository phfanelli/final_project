#include <string>
#include <GL/glew.h>

namespace project {
class ShaderProgram {
  public:
    ShaderProgram() :
      vertex_shader_src_(""), fragment_shader_src_(""),
      vertex_shader_(0), fragment_shader_(0), shader_program_id_(0),
      created_(false) {}
    virtual ~ShaderProgram(){
      if (created_) {
        glDeleteProgram(shader_program_id_);
      }
    }

    GLuint shader_program_id() const {
      return shader_program_id_;
    }

    bool LoadVertexShaderFromString(const std::string& vertex_shader_source);
    bool LoadFragmentShaderFromString(const std::string& fragment_shader_source);
    bool Create(std::string* error_info_log);

  protected:
    bool BuildVertexShader(std::string* info_log);
    bool BuildFragmentShader(std::string* info_log);
    bool LinkProgram(std::string* info_log);

  private:
   std::string vertex_shader_src_;
   std::string fragment_shader_src_;
   GLuint vertex_shader_;
   GLuint fragment_shader_;
   GLuint shader_program_id_;
   bool created_;
};
}
