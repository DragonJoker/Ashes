#version 450
#extension GL_KHR_vulkan_glsl : enable

layout( set=0, binding=1 ) uniform Scene
{
	mat4 mtxProjection;
	mat4 mtxView;
	vec4 cameraPosition;
};

layout( location=0 ) in vec4 position;
layout( location=1 ) in vec2 texcoord;
layout( location=2 ) in vec3 offset;
layout( location=3 ) in ivec2 dimensions;

out gl_PerVertex
{
  vec4 gl_Position;
};

layout( location = 0 ) out vec2 vtx_texcoord;

void main()
{
	vec3 right = vec3( mtxView[0][0], mtxView[1][0], mtxView[2][0] );
	vec3 up = -vec3( mtxView[0][1], mtxView[1][1], mtxView[2][1] );
	float width = dimensions.x;
	float height = dimensions.y;
	vec4 mPosition = vec4( offset, 1.0 );
	mPosition.xyz += ( right * texcoord.x * width )
			+ ( up * texcoord.y * height );
	gl_Position = mtxProjection * mtxView * mPosition;
	vtx_texcoord = texcoord + vec2( 0.5, 0.5 );
}
