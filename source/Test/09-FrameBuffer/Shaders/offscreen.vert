layout( set=0, binding=1 ) uniform Matrix
{
	mat4 mtxProjection;
};

layout( location=0 ) in vec4 position;
layout( location=1 ) in vec2 texcoord;

out gl_PerVertex
{
  vec4 gl_Position;
};

layout( location = 0 ) out vec2 vtx_texcoord;

void main()
{
#ifdef VULKAN
	gl_Position = mtxProjection * position;
#else
	mat4 scale;
	scale[0] = vec4( 1.0, 0.0, 0.0, 0.0 );
	scale[1] = vec4( 0.0, -1.0, 0.0, 0.0 );
	scale[2] = vec4( 0.0, 0.0, 1.0, 0.0 );
	scale[3] = vec4( 0.0, 0.0, 0.0, 1.0 );
	gl_Position = mtxProjection * scale * position;
#endif
	vtx_texcoord = texcoord;
}
