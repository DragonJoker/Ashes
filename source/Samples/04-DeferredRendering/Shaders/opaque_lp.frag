#version 450
#extension GL_KHR_vulkan_glsl : enable

#define MAX_LIGHTS 10

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

layout( set=1, binding=0 ) uniform Lights
{
	ivec4 lightsCount;
	DirectionalLight directionalLights[MAX_LIGHTS];
	PointLight pointLights[MAX_LIGHTS];
	SpotLight spotLights[MAX_LIGHTS];
};

layout( set=1, binding=1 ) uniform Matrix
{
	mat4 mtxInvViewProj;
};

layout( set=0, binding=0 ) uniform sampler2D depthMap;
layout( set=0, binding=1 ) uniform sampler2D diffuseMap;
layout( set=0, binding=2 ) uniform sampler2D specularMap;
layout( set=0, binding=3 ) uniform sampler2D emissiveMap;
layout( set=0, binding=4 ) uniform sampler2D normalMap;

layout( location = 0 ) in vec2 vtx_texcoord;

layout( location = 0 ) out vec4 pxl_colour;

void computeLight( Light light
	, vec3 direction
	, vec3 normal
	, float shininess
	, vec3 worldPosition
	, inout vec3 diffuse
	, inout vec3 specular )
{
	float diffuseFactor = max( dot( normal, -direction ), 0.0 );
	diffuse += light.colour.xyz * light.intensities.x * diffuseFactor;
	vec3 vertexToEye = normalize( -worldPosition );
	vec3 lightReflect = normalize( reflect( direction, normal ) );
	float specularFactor = max( dot( vertexToEye, lightReflect ), 0.0 );
	specularFactor = pow( specularFactor, light.intensities.y );
	specular += vec3( light.colour * shininess * specularFactor );
}

void computeDirectionalLight( int index
	, vec3 normal
	, float shininess
	, vec3 worldPosition
	, inout vec3 diffuse
	, inout vec3 specular )
{
	DirectionalLight light = directionalLights[index];
	computeLight( light.base
		, light.direction.xyz
		, normal
		, shininess
		, worldPosition
		, diffuse
		, specular );
}

vec3 computeWorldSpacePosition( float depth
	, vec2 uv
	, mat4 invViewProj )
{
	vec3 csPosition = vec3( uv * 2.0f - 1.0f, depth * 2.0 - 1.0 );
	vec4 wsPosition = invViewProj * vec4( csPosition, 1.0 );
	wsPosition.xyz /= wsPosition.w;
	return wsPosition.xyz;
}

void main()
{
	vec2 texcoord = ashesTopDownToBottomUp( vtx_texcoord );
	float depth = texture( depthMap, texcoord ).x;
	vec3 worldPosition = computeWorldSpacePosition( depth, texcoord, mtxInvViewProj );
	vec3 normal = texture( normalMap, texcoord ).xyz;
	vec4 specular = texture( specularMap, texcoord );
	vec3 lightDiffuse = vec3( 0.0, 0.0, 0.0 );
	vec3 lightSpecular = vec3( 0.0, 0.0, 0.0 );

	for ( int i = 0; i < lightsCount.x; ++i )
	{
		computeDirectionalLight( i
			, normal
			, specular.w
			, worldPosition
			, lightDiffuse
			, lightSpecular );
	}
	
	pxl_colour = vec4( texture( diffuseMap, texcoord ).xyz * ( lightDiffuse + lightSpecular ), 1.0 );
}
