#version 450
#extension GL_KHR_vulkan_glsl : enable

#define MAX_TEXTURES 6

struct TextureOperator
{
	int diffuse; // 0 or 1
	int specular; // 0 or 1
	int emissive; // 0 or 1
	uint shininess; // 0 for none, 1 for R, 2 for G, 4 for B, 8 for A
	uint opacity; // 0 for none, 1 for R, 2 for G, 4 for B, 8 for A
	float normalModifier;
	vec2 fill; // align to 16 bytes.
};

struct Material
{
	vec4 diffuse;
	vec4 specular;
	vec4 emissive;
	float shininess;
	float opacity;
	int texturesCount;
	float fill; // align to 16 bytes.
	TextureOperator textureOperators[MAX_TEXTURES];
};

layout( set=0, binding=2 ) uniform ObjectMaterial
{
	Material material;
};

layout( set=1, binding=0 ) uniform sampler2D textures[MAX_TEXTURES];

layout( location = 0 ) in vec3 vtx_normal;
layout( location = 1 ) in vec3 vtx_tangent;
layout( location = 2 ) in vec3 vtx_bitangent;
layout( location = 3 ) in vec2 vtx_texcoord;

layout( location = 0 ) out vec4 pxl_colour;

vec3 getDiffuse( int index, vec4 sampled )
{
	return material.textureOperators[index].diffuse * sampled.rgb;
}

vec3 getSpecular( int index, vec4 sampled )
{
	return material.textureOperators[index].specular * sampled.rgb;
}

vec3 getEmissive( int index, vec4 sampled )
{
	return material.textureOperators[index].emissive * sampled.rgb;
}

float getShininess( int index, vec4 sampled )
{
	uint shininess = material.textureOperators[index].shininess;
	vec4 channel = vec4( float( ( shininess & 0x01 ) >> 0 )
		, float( ( shininess & 0x02 ) >> 1 )
		, float( ( shininess & 0x04 ) >> 2 )
		, float( ( shininess & 0x08 ) >> 3 ) );
	return length( channel * sampled );
}

float getOpacity( int index, vec4 sampled )
{
	uint opacity = material.textureOperators[index].opacity;
	vec4 channel = vec4( float( ( opacity & 0x01 ) >> 0 )
		, float( ( opacity & 0x02 ) >> 1 )
		, float( ( opacity & 0x04 ) >> 2 )
		, float( ( opacity & 0x08 ) >> 3 ) );
	return max( length( channel * sampled ), max( 0.0, 1.0 - float( opacity ) ) );
}

void main()
{
	pxl_colour = vec4( 0.0, 0.0, 0.0, 1.0 );
	vec3 diffuse = material.diffuse.rgb;
	vec3 specular = material.specular.rgb;
	vec3 emissive = material.emissive.rgb;
	float shininess = material.shininess;
	float opacity = material.opacity;

	for ( int i = 0; i < material.texturesCount; ++i )
	{
		vec4 sampled = texture( textures[i], vtx_texcoord );
		diffuse *= getDiffuse( i, sampled );
		specular *= getSpecular( i, sampled );
		emissive *= getEmissive( i, sampled );
		shininess *= getShininess( i, sampled );
		opacity *= getOpacity( i, sampled );
	}

	pxl_colour = vec4( diffuse, opacity );
}
