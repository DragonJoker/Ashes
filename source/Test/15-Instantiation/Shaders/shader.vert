layout( set=0, binding=1 ) uniform Matrix
{
	mat4 mtxViewProjection;
};

layout( location=0 ) in vec4 position;
layout( location=1 ) in vec2 texcoord;
layout( location=2 ) in mat4 transform;

out gl_PerVertex
{
  vec4 gl_Position;
};

layout( location = 0 ) out vec2 vtx_texcoord;

void main()
{
	gl_Position = mtxViewProjection * transform * ashesScalePosition( position );
	vtx_texcoord = texcoord;
}
