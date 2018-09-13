layout( location = 0 ) in vec4 position;
layout( location = 1 ) in vec2 texcoord;
layout( location = 0 ) out vec2 vtx_texture;

out gl_PerVertex
{
  vec4 gl_Position;
};

void  main()
{
    vtx_texture = texcoord;
    gl_Position = ashesScalePosition( vec4( position.xy, 0.0, 1.0 ) );
}
