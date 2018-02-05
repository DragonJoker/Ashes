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

layout( set=0, binding=2 ) uniform ObjectMaterial
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

vec3 getDiffuse( TextureOperator operator, vec4 sampled, vec3 diffuse )
{
	return mix( diffuse, /*diffuse * */sampled.rgb, float( operator.diffuse ) );
}

vec3 getSpecular( TextureOperator operator, vec4 sampled, vec3 specular )
{
	return mix( specular, /*specular * */sampled.rgb, float( operator.specular ) );
}

vec3 getEmissive( TextureOperator operator, vec4 sampled, vec3 emissive )
{
	return mix( emissive, /*emissive * */sampled.rgb, float( operator.emissive ) );
}

float getShininess( TextureOperator operator, vec4 sampled, float shininess )
{
	vec4 channel = vec4( float( ( operator.shininess & 0x01 ) >> 0 )
		, float( ( operator.shininess & 0x02 ) >> 1 )
		, float( ( operator.shininess & 0x04 ) >> 2 )
		, float( ( operator.shininess & 0x08 ) >> 3 ) );
	return mix( shininess, /*shininess * */length( channel * sampled ), float( operator.shininess ) );
}

float getOpacity( TextureOperator operator, vec4 sampled, float opacity )
{
	vec4 channel = vec4( float( ( operator.opacity & 0x01 ) >> 0 )
		, float( ( operator.opacity & 0x02 ) >> 1 )
		, float( ( operator.opacity & 0x04 ) >> 2 )
		, float( ( operator.opacity & 0x08 ) >> 3 ) );
	return mix( opacity, /*opacity * */max( length( channel * sampled ), min( 1.0, 1.0 - float( opacity ) ) ), float( operator.opacity ) );
}

vec3 getNormal( TextureOperator operator, vec4 sampled, vec3 tangent, vec3 bitangent, vec3 normal )
{
	vec3 mapNormal = 2.0 * sampled.rgb - vec3( 1.0, 1.0, 1.0 );
	mat3 tbn = mat3( tangent, bitangent, normal );
	return mix( normal, normalize( tbn * mapNormal ), float( operator.normal ) );
}

void computeLight( Light light, vec3 direction, vec3 normal, float shininess, out vec3 diffuse, out vec3 specular )
{
	float diffuseFactor = max( dot( normal, -direction ), 0.0 );
	diffuse += light.colour.xyz * light.intensities.x * diffuseFactor;
	vec3 vertexToEye = normalize( -vtx_worldPosition );
	vec3 lightReflect = normalize( reflect( direction, normal ) );
	float specularFactor = max( dot( vertexToEye, lightReflect ), 0.0 );
	specularFactor = pow( specularFactor, light.intensities.y );
	specular += vec3( light.colour * shininess * specularFactor );
}

void computeDirectionalLight( int index, vec3 normal, float shininess, out vec3 diffuse, out vec3 specular )
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

	for ( int i = 0; i < material.texturesCount; ++i )
	{
		vec4 sampled = texture( textures[i], vtx_texcoord );
		TextureOperator operator = material.textureOperators[i];
		opacity = getOpacity( operator, sampled, opacity );
		diffuse = getDiffuse( operator, sampled, diffuse );
		specular = getSpecular( operator, sampled, specular );
		emissive = getEmissive( operator, sampled, emissive );
		shininess = getShininess( operator, sampled, shininess );
		normal = getNormal( operator, sampled, tangent, bitangent, normal );
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
