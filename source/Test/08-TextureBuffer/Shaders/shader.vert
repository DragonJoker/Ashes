layout( location = 0 ) in vec4 position;
layout( location = 1 ) in float texcoord;

out gl_PerVertex
{
  vec4 gl_Position;
};

layout( location = 0 ) out float vtx_texcoord;

void main()
{
	gl_Position = rendererScalePosition( position );
	vtx_texcoord = texcoord;
}
