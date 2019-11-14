#version 450
#extension GL_KHR_vulkan_glsl : enable

layout( set=0, binding=1 ) uniform sampler2D mapColour;

layout( location = 0 ) in vec3 vtx_position;

layout( location = 0 ) out vec4 pxl_colour;

vec2 sampleSphericalMap( vec3 v )
{
	vec2 uv = vec2( atan( v.z, v.x ), asin( -v.y ) );
	uv *= vec2( 0.1591, 0.3183 );
	uv += 0.5;
	return uv;
}

void main()
{
	vec2 uv = sampleSphericalMap( normalize( vtx_position ) );
	pxl_colour = vec4( texture( mapColour, uv ).rgb, 1.0 );
}
