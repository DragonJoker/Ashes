// shadertype=glsl

#version 450
#extension GL_KHR_vulkan_glsl : enable

#define MAX_TEXTURES 6
#define MAX_LIGHTS 10

struct TextureOperator
{
	int diffuse; // 0 or 1
	int specular; // 0 or 1
	int emissive; // 0 or 1
	int normal; // 0 for none, 1 for normals, 2 for inverted normals
	uint shininess; // 0 for none, 1 for R, 2 for G, 4 for B, 8 for A
	uint opacity; // 0 for none, 1 for R, 2 for G, 4 for B, 8 for A
	uint height; // 0 for none, 1 for R, 2 for G, 4 for B, 8 for A
	float fill; // align to 16 bytes.
};

struct Material
{
	vec4 diffuse;
	vec4 specular;
	vec4 emissive;
	float shininess;
	float opacity;
	int texturesCount;
	int backFace; // 0 or 1
	TextureOperator textureOperators[MAX_TEXTURES];
};

struct Light
{
	vec4 colour;
	vec4 intensities;
};

struct DirectionalLight
{
	Light base;
	vec4 direction;
};

struct PointLight
{
	Light base;
	vec4 position;
	vec4 attenation;
};

struct SpotLight
{
	PointLight base;
	vec4 direction;
	vec4 coeffs;// .x = cutoff, .y = exponent
};

layout( set=0, binding=0 ) uniform ObjectMaterial
{
	Material material;
};

layout( set=0, binding=3 ) uniform Lights
{
	ivec4 lightsCount;
	DirectionalLight directionalLights[MAX_LIGHTS];
	PointLight pointLights[MAX_LIGHTS];
	SpotLight spotLights[MAX_LIGHTS];
};

layout( set=1, binding=0 ) uniform sampler2D textures[MAX_TEXTURES];

layout( location = 0 ) in vec3 vtx_normal;
layout( location = 1 ) in vec3 vtx_tangent;
layout( location = 2 ) in vec3 vtx_bitangent;
layout( location = 3 ) in vec2 vtx_texcoord;
layout( location = 4 ) in vec3 vtx_worldPosition;

layout( location = 0 ) out vec4 pxl_colour;

vec3 getDiffuse( TextureOperator texOp, vec4 sampled, vec3 diffuse )
{
	return mix( diffuse, /*diffuse * */sampled.rgb, float( texOp.diffuse ) );
}

vec3 getSpecular( TextureOperator texOp, vec4 sampled, vec3 specular )
{
	return mix( specular, /*specular * */sampled.rgb, float( texOp.specular ) );
}

vec3 getEmissive( TextureOperator texOp, vec4 sampled, vec3 emissive )
{
	return mix( emissive, /*emissive * */sampled.rgb, float( texOp.emissive ) );
}

float getShininess( TextureOperator texOp, vec4 sampled, float shininess )
{
	vec4 channel = vec4( float( ( texOp.shininess & 0x01 ) >> 0 )
		, float( ( texOp.shininess & 0x02 ) >> 1 )
		, float( ( texOp.shininess & 0x04 ) >> 2 )
		, float( ( texOp.shininess & 0x08 ) >> 3 ) );
	return mix( shininess, /*shininess * */length( channel * sampled ), float( texOp.shininess ) );
}

float getOpacity( TextureOperator texOp, vec4 sampled, float opacity )
{
	vec4 channel = vec4( float( ( texOp.opacity & 0x01 ) >> 0 )
		, float( ( texOp.opacity & 0x02 ) >> 1 )
		, float( ( texOp.opacity & 0x04 ) >> 2 )
		, float( ( texOp.opacity & 0x08 ) >> 3 ) );
	return mix( opacity, /*opacity * */max( length( channel * sampled ), min( 1.0, 1.0 - float( opacity ) ) ), float( texOp.opacity ) );
}

vec3 getNormal( TextureOperator texOp, vec4 sampled, vec3 tangent, vec3 bitangent, vec3 normal )
{
	vec3 mapNormal = 2.0 * sampled.rgb - vec3( 1.0, 1.0, 1.0 );
	mat3 tbn = mat3( tangent, bitangent, normal );
	return mix( normal, normalize( tbn * mapNormal ), float( texOp.normal ) );
}

void computeLight( Light light, vec3 direction, vec3 normal, float shininess, inout vec3 diffuse, inout vec3 specular )
{
	float diffuseFactor = max( dot( normal, -direction ), 0.0 );
	diffuse += light.colour.xyz * light.intensities.x * diffuseFactor;
	vec3 vertexToEye = normalize( -vtx_worldPosition );
	vec3 lightReflect = normalize( reflect( direction, normal ) );
	float specularFactor = max( dot( vertexToEye, lightReflect ), 0.0 );
	specularFactor = pow( specularFactor, light.intensities.y );
	specular += vec3( light.colour * shininess * specularFactor );
}

void computeDirectionalLight( int index, vec3 normal, float shininess, inout vec3 diffuse, inout vec3 specular )
{
	DirectionalLight light = directionalLights[index];
	computeLight( light.base, light.direction.xyz, normal, shininess, diffuse, specular );
}

void main()
{
	pxl_colour = vec4( 0.0, 0.0, 0.0, 1.0 );
	vec3 diffuse = material.diffuse.rgb;
	vec3 specular = material.specular.rgb;
	vec3 emissive = material.emissive.rgb;
	float shininess = material.shininess;
	float opacity = material.opacity;
	vec3 normal = normalize( vtx_normal );
	vec3 tangent = normalize( vtx_tangent );
	tangent = normalize( tangent - dot( tangent, normal ) * normal );
	vec3 bitangent = cross( tangent, normal );

	if ( 0 < material.texturesCount )
	{
		vec4 sampled = texture( textures[0], vtx_texcoord );
		TextureOperator texOp = material.textureOperators[0];
		opacity = getOpacity( texOp, sampled, opacity );
		diffuse = getDiffuse( texOp, sampled, diffuse );
		specular = getSpecular( texOp, sampled, specular );
		emissive = getEmissive( texOp, sampled, emissive );
		shininess = getShininess( texOp, sampled, shininess );
		normal = getNormal( texOp, sampled, tangent, bitangent, normal );
	}

	if ( 1 < material.texturesCount )
	{
		vec4 sampled = texture( textures[1], vtx_texcoord );
		TextureOperator texOp = material.textureOperators[1];
		opacity = getOpacity( texOp, sampled, opacity );
		diffuse = getDiffuse( texOp, sampled, diffuse );
		specular = getSpecular( texOp, sampled, specular );
		emissive = getEmissive( texOp, sampled, emissive );
		shininess = getShininess( texOp, sampled, shininess );
		normal = getNormal( texOp, sampled, tangent, bitangent, normal );
	}

	if ( 2 < material.texturesCount )
	{
		vec4 sampled = texture( textures[2], vtx_texcoord );
		TextureOperator texOp = material.textureOperators[2];
		opacity = getOpacity( texOp, sampled, opacity );
		diffuse = getDiffuse( texOp, sampled, diffuse );
		specular = getSpecular( texOp, sampled, specular );
		emissive = getEmissive( texOp, sampled, emissive );
		shininess = getShininess( texOp, sampled, shininess );
		normal = getNormal( texOp, sampled, tangent, bitangent, normal );
	}

	if ( 3 < material.texturesCount )
	{
		vec4 sampled = texture( textures[3], vtx_texcoord );
		TextureOperator texOp = material.textureOperators[3];
		opacity = getOpacity( texOp, sampled, opacity );
		diffuse = getDiffuse( texOp, sampled, diffuse );
		specular = getSpecular( texOp, sampled, specular );
		emissive = getEmissive( texOp, sampled, emissive );
		shininess = getShininess( texOp, sampled, shininess );
		normal = getNormal( texOp, sampled, tangent, bitangent, normal );
	}

	if ( 4 < material.texturesCount )
	{
		vec4 sampled = texture( textures[4], vtx_texcoord );
		TextureOperator texOp = material.textureOperators[4];
		opacity = getOpacity( texOp, sampled, opacity );
		diffuse = getDiffuse( texOp, sampled, diffuse );
		specular = getSpecular( texOp, sampled, specular );
		emissive = getEmissive( texOp, sampled, emissive );
		shininess = getShininess( texOp, sampled, shininess );
		normal = getNormal( texOp, sampled, tangent, bitangent, normal );
	}

	if ( 5 < material.texturesCount )
	{
		vec4 sampled = texture( textures[5], vtx_texcoord );
		TextureOperator texOp = material.textureOperators[5];
		opacity = getOpacity( texOp, sampled, opacity );
		diffuse = getDiffuse( texOp, sampled, diffuse );
		specular = getSpecular( texOp, sampled, specular );
		emissive = getEmissive( texOp, sampled, emissive );
		shininess = getShininess( texOp, sampled, shininess );
		normal = getNormal( texOp, sampled, tangent, bitangent, normal );
	}

	if ( opacity < 0.5 )
	{
		discard;
	}
	
	float normalCoeff = ( 3.0 - 2 * float( material.backFace ) ) - 2.0; // put from {0, 1} to {1, -1}
	normal = /*normalCoeff * */normalize( normal );
	vec3 lightDiffuse = vec3( 0.0, 0.0, 0.0 );
	vec3 lightSpecular = vec3( 0.0, 0.0, 0.0 );

	for ( int i = 0; i < lightsCount.x; ++i )
	{
		computeDirectionalLight( i, normal, shininess, lightDiffuse, lightSpecular );
	}

	pxl_colour = vec4( diffuse * ( lightDiffuse + lightSpecular ), opacity );
}
