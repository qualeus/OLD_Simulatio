#ifndef Shader_HPP
#define Shader_HPP

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <Geometry/String.hpp>
#include <glm/vec3.hpp>
#include <stdexcept>
#include <string>

// base shader
#include "../../assets/shaders/default/fs_base.bin.hpp"
#include "../../assets/shaders/default/vs_base.bin.hpp"

// circle shader
#include "../../assets/shaders/circle/fs_circle.bin.hpp"
#include "../../assets/shaders/circle/vs_circle.bin.hpp"

namespace drw {

// Triangle mesh
class Shader {
   public:
    static const bgfx::Memory *get_base_vs_shader();
    static const bgfx::Memory *get_base_fs_shader();

    static const bgfx::Memory *get_circle_vs_shader();
    static const bgfx::Memory *get_circle_fs_shader();

    static bgfx::ProgramHandle create_program(const char *name, const bgfx::Memory *memory_vs, const bgfx::Memory *memory_fs);
};

}  // namespace drw

#endif