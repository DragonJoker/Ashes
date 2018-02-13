#version 450
#extension GL_KHR_vulkan_glsl : enable

layout( set=0, binding=0 ) uniform sampler2D mapColour;
layout( set=0, binding=1 ) uniform sampler2D mapGui;

layout( location = 0 ) in vec2 vtx_texcoord;

layout( location = 0 ) out vec4 pxl_colour;

void main()
{
#ifdef VULKAN
	vec2 guicoord = vtx_texcoord;
	vec2 colcoord = vec2( vtx_texcoord.x, 1.0 - vtx_texcoord.y );
#else
	vec2 guicoord = vec2( vtx_texcoord.x, 1.0 - vtx_texcoord.y );
	vec2 colcoord = vtx_texcoord;
#endif

	vec4 gui = texture( mapGui, guicoord );
	vec4 colour = texture( mapColour, colcoord );
	pxl_colour.rgb = gui.rgb * gui.a + colour.rgb * (1.0 - gui.a);
}
