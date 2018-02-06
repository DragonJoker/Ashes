#version 450
#extension GL_KHR_vulkan_glsl : enable

layout( set=0, binding=0 ) uniform sampler2D depthMap;
layout( set=0, binding=1 ) uniform sampler2D diffuseMap;
layout( set=0, binding=2 ) uniform sampler2D specularMap;
layout( set=0, binding=3 ) uniform sampler2D emissiveMap;
layout( set=0, binding=4 ) uniform sampler2D normalMap;

layout( location = 0 ) in vec2 vtx_texcoord;

layout( location = 0 ) out vec4 pxl_colour;

void main()
{
#ifdef VULKAN
	vec2 texcoord = vtx_texcoord;
#else
	vec2 texcoord = vec2( vtx_texcoord.x, 1.0 - vtx_texcoord.y );
#endif

	pxl_colour = texture( diffuseMap, texcoord );
}
