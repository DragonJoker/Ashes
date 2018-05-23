layout( binding = 0 ) uniform sampler2D samplerColor;

layout( std140, binding = 1 ) uniform Config
{    
    vec2 inTextureSize;
    uint inCoefficientsCount;
    uint inDump;
    vec4 inCoefficients[15];
};

#ifdef VULKAN
layout (constant_id = 0) const int blurDirection = 0;
#else
layout (location = 0) uniform int blurDirection = 0;
#endif

layout (location = 0) in vec2 inUV;

layout (location = 0) out vec4 outFragColor;

void main() 
{
	vec2 base = vec2( 1.0, 0 ) / inTextureSize;

	if ( blurDirection == 1 )
	{
		base = vec2( 0.0, 1.0 ) / inTextureSize;
	}

	vec2 offset = vec2( 0.0, 0.0 );
	outFragColor = texture( samplerColor, inUV ) * inCoefficients[0][0];

	for ( uint i = 1u; i < inCoefficientsCount; ++i )
	{
		offset += base;
		outFragColor += inCoefficients[i / 4u][i % 4u] * texture( samplerColor, inUV - offset );
		outFragColor += inCoefficients[i / 4u][i % 4u] * texture( samplerColor, inUV + offset );
	}
}
