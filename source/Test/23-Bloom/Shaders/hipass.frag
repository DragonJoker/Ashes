#version 450
#extension GL_KHR_vulkan_glsl : enable

layout( binding = 0 ) uniform sampler2D c3d_mapDiffuse;
layout( location = 0 ) in vec2 vtx_texture;
layout( location = 0 ) out vec4 pxl_fragColor;

void  main()
{    
    pxl_fragColor = vec4( textureLod( c3d_mapDiffuse, vec2( vtx_texture.x, 1.0 - vtx_texture.y ), 0.0 ).xyz, 1.0 );
    float maxComponent = max( pxl_fragColor.r, pxl_fragColor.g );
    maxComponent = max( maxComponent, pxl_fragColor.b );
    
    if ( maxComponent > 0.7 )
    {
        pxl_fragColor.xyz /= maxComponent;
    }
    else
    {
        pxl_fragColor.xyz = vec3( 0.0, 0.0, 0.0 );
    }
}
