layout( set=0, binding=0 ) uniform sampler2D mapColour;

#ifdef VULKAN
layout( push_constant ) uniform Colour
{
	vec4 colour;
} colour;
#else
layout( location=3 ) uniform vec4 colour;
#endif

layout( location = 0 ) in vec2 vtx_texcoord;

layout( location = 0 ) out vec4 pxl_colour;

void main()
{
#ifdef VULKAN
	pxl_colour = colour.colour * texture( mapColour, vtx_texcoord );
#else
	pxl_colour = colour * texture( mapColour, vtx_texcoord );
#endif
}
