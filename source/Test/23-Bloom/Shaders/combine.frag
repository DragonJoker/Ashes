#version 450
#extension GL_KHR_vulkan_glsl : enable

layout( binding = 0 ) uniform sampler2D c3d_mapScene;
layout( binding = 1 ) uniform sampler2D c3d_mapPasses;

layout( location = 0 ) in vec2 vtx_texture;
layout( location = 0 ) out vec4 pxl_fragColor;

void main()
{
	vec2 texcoords = ashesInvertY( vtx_texture );
	pxl_fragColor = texture( c3d_mapScene, vtx_texture );
	pxl_fragColor += textureLod( c3d_mapPasses, texcoords, 0.0 );
	pxl_fragColor += textureLod( c3d_mapPasses, texcoords, 1.0 );
	pxl_fragColor += textureLod( c3d_mapPasses, texcoords, 2.0 );
	pxl_fragColor += textureLod( c3d_mapPasses, texcoords, 3.0 );
}
