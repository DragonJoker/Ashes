#version 450
#extension GL_KHR_vulkan_glsl : enable

layout( set=0, binding=0 ) uniform sampler2D mapColour;

layout( location = 0 ) in vec2 vtx_texcoord;

layout( location = 0 ) out vec4 pxl_colour;

layout( set=0, binding=1 ) uniform LodSelect
{
	float src;
	float dst;
	float percent;
};

void main()
{
	pxl_colour = mix( texture( mapColour, vtx_texcoord, src ), texture( mapColour, vtx_texcoord, dst ), percent );
}
