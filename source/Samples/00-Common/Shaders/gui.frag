#version 450

layout( set = 0, binding = 0 ) uniform sampler2D mapFont;

layout( location = 0 ) in vec2 vtx_texcoord;
layout( location = 1 ) in vec4 vtx_colour;

layout (location = 0) out vec4 pxl_colour;

void main() 
{
	pxl_colour = vtx_colour * texture( mapFont, vtx_texcoord );
}
