layout( location = 0 ) in vec4 position;
layout( location = 0 ) out vec2 vtx_texture;

out gl_PerVertex
{
  vec4 gl_Position;
};

void  main()
{
    vtx_texture = ( position.xy + 1.0 ) / 2.0;
    gl_Position = rendererScalePosition( vec4( position.xy, 0.0, 1.0 ) );
}
