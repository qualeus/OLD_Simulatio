#include "../../include/Drawing/Shader.hpp"

namespace drw {
const bgfx::Memory *Shader::get_base_vs_shader() {
    switch (bgfx::getRendererType()) {
        case bgfx::RendererType::OpenGL: std::runtime_error("Can't find the shader type OpenGL");
        case bgfx::RendererType::Noop: std::runtime_error("Can't find the shader type Noop");
        case bgfx::RendererType::Direct3D9: std::runtime_error("Can't find the shader type Direct3D9");
        case bgfx::RendererType::Direct3D11: return bgfx::copy(vs_base_dx11, sizeof(vs_base_dx11));
        case bgfx::RendererType::Direct3D12: std::runtime_error("Can't find the shader type Direct3D12");
        case bgfx::RendererType::OpenGLES: std::runtime_error("Can't find the shader type OpenGLES");
        case bgfx::RendererType::Metal: std::runtime_error("Can't find the shader type Metal");
        case bgfx::RendererType::Vulkan: std::runtime_error("Can't find the shader type Vulkan");
        default: throw std::runtime_error("Can't find the shader type");
    }
}

const bgfx::Memory *Shader::get_base_fs_shader() {
    switch (bgfx::getRendererType()) {
        case bgfx::RendererType::OpenGL: std::runtime_error("Can't find the shader type OpenGL");
        case bgfx::RendererType::Noop: std::runtime_error("Can't find the shader type Noop");
        case bgfx::RendererType::Direct3D9: std::runtime_error("Can't find the shader type Direct3D9");
        case bgfx::RendererType::Direct3D11: return bgfx::copy(fs_base_dx11, sizeof(fs_base_dx11));
        case bgfx::RendererType::Direct3D12: std::runtime_error("Can't find the shader type Direct3D12");
        case bgfx::RendererType::OpenGLES: std::runtime_error("Can't find the shader type OpenGLES");
        case bgfx::RendererType::Metal: std::runtime_error("Can't find the shader type Metal");
        case bgfx::RendererType::Vulkan: std::runtime_error("Can't find the shader type Vulkan");
        default: throw std::runtime_error("Can't find the shader type");
    }
}

bgfx::ProgramHandle Shader::create_program(const char *name, const bgfx::Memory *memory_vs, const bgfx::Memory *memory_fs) {
    bgfx::ShaderHandle vs = bgfx::createShader(memory_vs);
    bgfx::ShaderHandle fs = bgfx::createShader(memory_fs);

    return bgfx::createProgram(vs, fs, true);
}
}  // namespace drw