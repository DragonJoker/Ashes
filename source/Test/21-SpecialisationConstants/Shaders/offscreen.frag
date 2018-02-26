layout( set=0, binding=0 ) uniform sampler2D mapColour;

#ifdef VULKAN
layout( constant_id = 0 ) const int COLOUR_INDEX = 0;
#else
layout( location = 0 ) uniform int COLOUR_INDEX;
#endif

layout( location = 0 ) in vec2 vtx_texcoord;

layout( location = 0 ) out vec4 pxl_colour;

void main()
{
	vec4 colour;

	if ( COLOUR_INDEX == 0 )
	{
		colour = vec4( 1.0, 0.0, 0.0, 1.0 );
	}
	else if ( COLOUR_INDEX == 1 )
	{
		colour = vec4( 0.0, 1.0, 0.0, 1.0 );
	}
	else
	{
		colour = vec4( 0.0, 0.0, 1.0, 1.0 );
	}

	pxl_colour = colour * texture( mapColour, vtx_texcoord );
}
