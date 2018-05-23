layout( binding = 0 ) uniform sampler2D c3d_mapScene;
layout( binding = 1 ) uniform sampler2D c3d_mapPasses;

layout( location = 0 ) in vec2 vtx_texture;
layout( location = 0 ) out vec4 pxl_fragColor;

void  main()
{
#ifdef VULKAN
    vec2 texcoords = vec2( vtx_texture.x, 1.0 - vtx_texture.y );
#else
    vec2 texcoords = vtx_texture;
#endif
    pxl_fragColor = texture( c3d_mapScene, vtx_texture );
    pxl_fragColor += textureLod( c3d_mapPasses, texcoords, 0.0 );
    pxl_fragColor += textureLod( c3d_mapPasses, texcoords, 1.0 );
    pxl_fragColor += textureLod( c3d_mapPasses, texcoords, 2.0 );
    pxl_fragColor += textureLod( c3d_mapPasses, texcoords, 3.0 );
}
