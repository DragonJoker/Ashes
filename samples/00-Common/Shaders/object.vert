#version 450
#extension GL_KHR_vulkan_glsl : enable

layout( set=0, binding=1 ) uniform Scene
{
	mat4 mtxProjection;
	mat4 mtxView;
	vec4 cameraPosition;
};

layout( set=0, binding=2 ) uniform Object
{
	mat4 mtxModel;
};

layout( location=0 ) in vec3 position;
layout( location=1 ) in vec3 normal;
layout( location=2 ) in vec3 tangent;
layout( location=3 ) in vec3 bitangent;
layout( location=4 ) in vec2 texcoord;

out gl_PerVertex
{
  vec4 gl_Position;
};

layout( location = 0 ) out vec3 vtx_normal;
layout( location = 1 ) out vec3 vtx_tangent;
layout( location = 2 ) out vec3 vtx_bitangent;
layout( location = 3 ) out vec2 vtx_texcoord;
layout( location = 4 ) out vec3 vtx_worldPosition;

void main()
{
	vec4 worldPosition = mtxModel * vec4( position, 1.0 );
	gl_Position = mtxProjection * mtxView * worldPosition;
	mat3 mtxNormal = mat3( transpose( inverse( mtxModel ) ) );

	vtx_worldPosition = worldPosition.xyz;
	vtx_normal = mtxNormal * normal;
	vtx_tangent = mtxNormal * tangent;
	vtx_bitangent = mtxNormal * bitangent;
	vtx_texcoord = texcoord;
}
