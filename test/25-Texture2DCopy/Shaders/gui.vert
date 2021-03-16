#version 450
#extension GL_KHR_vulkan_glsl : enable

layout( location = 0 ) in vec2 position;
layout( location = 1 ) in vec2 texcoord;
layout( location = 2 ) in vec4 colour;

layout( push_constant ) uniform PushConstants
{
	vec2 scale;
	vec2 translate;
} pushConstants;

layout( location = 0 ) out vec2 vtx_texcoord;
layout( location = 1 ) out vec4 vtx_colour;

out gl_PerVertex 
{
	vec4 gl_Position;
};

void main() 
{
	vtx_colour = colour;
	vtx_texcoord = texcoord;
	gl_Position = vec4( position * pushConstants.scale + pushConstants.translate, 0.0, 1.0 );
}
