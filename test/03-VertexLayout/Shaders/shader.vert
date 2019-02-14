#version 450
#extension GL_KHR_vulkan_glsl : enable

layout( location = 0 ) in vec4 position;
layout( location = 1 ) in vec4 colour;

out gl_PerVertex
{
  vec4 gl_Position;
};

layout( location = 0 ) out vec4 vtx_colour;

void main()
{
    gl_Position = position;
    vtx_colour = colour;
}
