layout( binding=1 ) uniform Matrix
{
	mat4 mtxProjection;
};

layout( binding=2 ) uniform Object
{
	mat4 mtxModel;
};

layout( location=0 ) in vec4 position;

out gl_PerVertex
{
  vec4 gl_Position;
};

layout( location = 0 ) out vec3 vtx_texcoord;

void main()
{
	gl_Position = ( mtxProjection * mtxModel * ashesScalePosition( position ) ).xyww;
	vtx_texcoord = position.xyz;
}
