#version 450
#extension GL_KHR_vulkan_glsl : enable

layout( set=0, binding=1 ) uniform Scene
{
	mat4 mtxProjection;
	mat4 mtxView;
	vec4 cameraPosition;
};

layout( location=0 ) in vec3 position;
layout( location=1 ) in vec3 normal;
layout( location=2 ) in vec3 tangent;
layout( location=3 ) in vec3 bitangent;
layout( location=4 ) in vec2 texcoord;
layout( location=5 ) in vec3 offset;
layout( location=6 ) in vec2 dimensions;

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
	vec3 right = vec3( mtxView[0][0], mtxView[1][0], mtxView[2][0] );
	vec3 up = vec3( mtxView[0][1], mtxView[1][1], mtxView[2][1] );
	float width = float( dimensions.x );
	float height = float( dimensions.y );
	vec4 mPosition = vec4( position + offset, 1.0 );
	mPosition.xyz += ( right * texcoord.x * width )
			+ ( up * texcoord.y * height );
	gl_Position = mtxProjection * mtxView * mPosition;

	vtx_worldPosition = mPosition.xyz;
	vtx_normal = normal;
	vtx_tangent = tangent;
	vtx_bitangent = bitangent;
	vtx_texcoord = ashesInvertY( texcoord + vec2( 0.5, 0.5 ) );
}
