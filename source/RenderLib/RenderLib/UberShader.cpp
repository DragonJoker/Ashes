#include "UberShader.h"

#include "Material.h"

#include <Renderer/Core/Device.hpp>
#include <Renderer/Shader/ShaderProgram.hpp>

#include <Utils/StringUtils.hpp>

namespace render
{
	namespace
	{
		static std::string const MtxUniforms
		{
			R"(uniform mat4 mtxProjection;
uniform mat4 mtxView;
uniform mat4 mtxModel;
)"
		};

		static std::string const MtxUbo
		{
			R"(layout( std140[UboMatrixBinding] ) uniform Matrices
{
	mat4 mtxProjection;
	mat4 mtxView;
	mat4 mtxModel;
};
)"
		};

		static std::string const MatUniforms
		{
			R"(uniform vec3 matAmbient;
uniform vec3 matEmissive;
#ifdef LIGHTING
uniform vec3 matDiffuse;
uniform vec3 matSpecular;
uniform float matExponent;
#endif
#ifdef TRANSPARENT
uniform float matOpacity;
#endif
)"
		};

		static std::string const MatUbo
		{
			R"(layout( std140[UboMaterialBinding] ) uniform Material
{
	vec3 matAmbient;
	vec3 matEmissive;
#ifdef LIGHTING
	vec3 matDiffuse;
	vec3 matSpecular;
	float matExponent;
#endif
#ifdef TRANSPARENT
	float matOpacity;
#endif
};
)"
		};

		static std::string const BillboardUniforms
		{
			R"(uniform vec3 camera;
uniform vec2 dimensions;
)"
		};

		static std::string const BillboardUbo
		{
			R"(layout( std140[UboBillboardBinding] ) uniform Billboard
{
	vec3 camera;
	vec2 dimensions;
};
)"
		};

		static std::string const PolyLineUniforms
		{
			R"(uniform float lineWidth;
uniform float lineFeather;
uniform float lineScale;
uniform vec3 camera;
)"
		};

		static std::string const PolyLineUbo
		{
			R"(layout( std140[UboPolyLineBinding] ) uniform PolyLine
{
	float lineWidth;
	float lineFeather;
	float lineScale;
	vec3 camera;
};
)"
		};

		static std::string const PickingUniforms
		{
			R"(uniform int drawIndex;
uniform int nodeIndex;
)"
		};

		static std::string const PickingUbo
		{
			R"(layout( std140[UboPickingBinding] ) uniform Picking
{
	int drawIndex;
	int nodeIndex;
};
)"
		};

		static std::string const OverlayUniforms
		{
			R"(uniform mat4 mtxMP;
uniform vec4 colour;
)"
		};

		static std::string const OverlayUbo
		{
			R"(layout( std140[UboOverlayBinding] ) uniform Overlay
{
	mat4 mtxMP;
	vec4 colour;
};
)"
		};

		namespace glsl_vtx
		{
			static std::string const ObjectShader
			{
				R"([attribute] vec3 position;
#ifdef LIGHTING
[attribute] vec3 normal;
#endif
#ifdef TEXTURED
[attribute] vec2 texCoord;
#endif

#ifdef LIGHTING
[varying] vec3 vtx_normal;
#endif
#ifdef TEXTURED
[varying] vec2 vtx_texture;
#endif

void main()
{
	gl_Position = mtxProjection * mtxView * mtxModel * vec4( position, 1.0 );
	[invertY]
#ifdef LIGHTING
	vtx_normal = normal;
#endif
#ifdef TEXTURED
	vtx_texture = texCoord;
#endif
}
)"
			};

			static std::string const PickingObjectShader
			{
				R"([attribute] vec3 position;
#ifdef TEXTURED
[attribute] vec2 texCoord;
#endif

//uniform float scale;

[varying] float vtx_instance;
#ifdef TEXTURED
[varying] vec2 vtx_texture;
#endif

void main()
{
	gl_Position = mtxProjection * mtxView * mtxModel * vec4( position /** vec3( scale, scale, 1.0 )*/, 1.0 );
	[invertY]
#ifdef TEXTURED
	vtx_texture = texCoord + vec2( 0.5, 0.5 );
#endif
	vtx_instance = 0.0;
}
)"
			};

			static std::string const BillboardShader
			{
				R"([attribute] vec3 position;
[attribute] vec2 scale;
[attribute] vec2 texCoord;

#ifdef LIGHTING
[varying] vec3 vtx_normal;
#endif
#ifdef TEXTURED
[varying] vec2 vtx_texture;
#endif

void main()
{
	vec4 mPosition = mtxModel * vec4( position, 1.0 );
#ifdef LIGHTING
	vec3 toCamera = camera - mPosition.xyz;
	toCamera.y = 0.0;
	toCamera = normalize( toCamera );
	vtx_normal = toCamera;
#endif
	vec3 right = vec3( mtxView[0][0], mtxView[1][0], mtxView[2][0] );
	vec3 up = -vec3( mtxView[0][1], mtxView[1][1], mtxView[2][1] );
	float width = dimensions.x;
	float height = dimensions.y;
	mPosition.xyz += ( right * texCoord.x * width * scale.x )
			+ ( up * texCoord.y * height * scale.y );
	vec4 mvPosition = mtxView * mPosition;
	gl_Position = mtxProjection * mvPosition;
	[invertY]
#ifdef TEXTURED
	vtx_texture = texCoord + vec2( 0.5, 0.5 );
#endif
}
)"
			};

			static std::string const PickingBillboardShader
			{
				R"([attribute] vec3 position;
[attribute] vec2 scale;
[attribute] vec2 texCoord;
[attribute] float id;

[varying] float vtx_instance;
#ifdef TEXTURED
[varying] vec2 vtx_texture;
#endif

void main()
{
	vec4 mPosition = mtxModel * vec4( position, 1.0 );
	vec3 right = vec3( mtxView[0][0], mtxView[1][0], mtxView[2][0] );
	vec3 up = -vec3( mtxView[0][1], mtxView[1][1], mtxView[2][1] );
	float width = dimensions.x;
	float height = dimensions.y;
	mPosition.xyz += ( right * texCoord.x * width * scale.x )
			+ ( up * texCoord.y * height * scale.y );
	vec4 mvPosition = mtxView * mPosition;
	gl_Position = mtxProjection * mvPosition;
	[invertY]
#ifdef TEXTURED
	vtx_texture = texCoord + vec2( 0.5, 0.5 );
#endif
	vtx_instance = float( id );
}
)"
			};

			static std::string const PolyLineShader
			{
				R"([attribute] vec3 position;
[attribute] vec3 normal;

[varying] vec3 vtx_normal;

void main()
{
	vec4 mPosition = mtxModel * vec4( position.xyz, 1.0 );
	vec2 delta = normal.xy * lineWidth * lineScale;
	//mPosition.xy += delta;
	vec4 mvPosition = mtxView * mPosition;
	gl_Position = mtxProjection * mvPosition;
	[invertY]
	vtx_normal = normal;
}
)"
			};

			static std::string PanelOverlayShader
			{
				R"([attribute] vec2 position;
#ifdef TEXTURED
[attribute] vec2 texCoord;
#endif

#ifdef TEXTURED
[varying] vec2 vtx_texture;
#endif

void main()
{
#ifdef TEXTURED
	vtx_texture = texCoord;
#endif

	gl_Position = mtxMP * vec4( position, 0.0, 1.0 );
	[invertY]
}
)"
			};

			static std::string TextOverlayShader
			{
				R"([attribute] vec2 position;
[attribute] vec2 texCoord;

[varying] vec2 vtx_texture;

void main()
{
	vtx_texture = texCoord;
	gl_Position = mtxMP * vec4( position, 0.0, 1.0 );
	[invertY]
}
)"
			};

			static std::string TextureShader
			{
				R"([attribute] vec2 position;
[attribute] vec2 texCoord;

[varying] vec2 vtx_texture;

void main()
{
	vtx_texture = texCoord;
	gl_Position = vec4( position, 0.5, 1.0 );
	[invertY]
}
)"
			};

			std::string getDefines( TextureFlags textures )
			{
				std::string ret;
				ret += "#version 310 es\n";
				ret += "precision highp float;\n";

				if ( textures )
				{
					ret += "#define TEXTURED\n";
				}

				return ret;
			}

			std::string getUniforms( ObjectType object)
			{
				std::string ret;

				switch ( object )
				{
				case ObjectType::eObject:
					ret += MtxUbo;
					break;

				case ObjectType::eBillboard:
					ret += MtxUbo;
					ret += BillboardUbo;
					break;

				case ObjectType::ePolyLine:
					ret += MtxUbo;
					ret += PolyLineUbo;
					break;

				case ObjectType::ePanelOverlay:
				case ObjectType::eTextOverlay:
					ret += OverlayUbo;
					break;

				default:
					break;
				}

				return ret;
			}

			std::string getObjectShader( RenderType render )
			{
				std::string ret;

				switch ( render )
				{
				case RenderType::eScene:
					ret += ObjectShader;
					break;

				case RenderType::ePicking:
					ret += PickingObjectShader;
					break;

				default:
					assert( "Unsupported render type" );
					break;
				}

				return ret;
			}

			std::string getBillboardShader( RenderType render )
			{
				std::string ret;

				switch ( render )
				{
				case RenderType::eScene:
					ret += BillboardShader;
					break;

				case RenderType::ePicking:
					ret += PickingBillboardShader;
					break;

				default:
					assert( "Unsupported render type" );
					break;
				}

				return ret;
			}

			std::string getLineShader( RenderType render )
			{
				std::string ret;

				switch ( render )
				{
				case RenderType::eScene:
					ret += PolyLineShader;
					break;

				default:
					assert( "Lines are not supported for picking" );
					break;
				}

				return ret;
			}

			std::string getPanelOverlayShader( RenderType render )
			{
				return PanelOverlayShader;
			}

			std::string getTextOverlayShader( RenderType render )
			{
				return TextOverlayShader;
			}

			std::string getTextureShader( RenderType render )
			{
				return TextureShader;
			}

			std::string getShader( RenderType render
				, ObjectType object )
			{
				std::string ret;

				switch ( object )
				{
				case ObjectType::eObject:
					ret = getObjectShader( render );
					break;

				case ObjectType::eBillboard:
					ret = getBillboardShader( render );
					break;

				case ObjectType::ePolyLine:
					ret = getLineShader( render );
					break;

				case ObjectType::ePanelOverlay:
					ret = getPanelOverlayShader( render );
					break;

				case ObjectType::eTextOverlay:
					ret = getTextOverlayShader( render );
					break;

				case ObjectType::eTexture:
					ret = getTextureShader( render );
					break;

				default:
					assert( "Unsupported object type" );
					break;
				}

				return ret;
			}
		}

		namespace glsl_pxl
		{
			static std::string const PolyLineShader
			{
				R"([varying] vec3 vtx_normal;

void main()
{
	float alpha = ( 1.0 - length( vtx_normal ) ) / lineFeather;
	vec4 pxl_fragColour = vec4( matAmbient.xyz, clamp( alpha, 0.0, 1.0 ) );
	[gl_FragColor] = pxl_fragColour;
}
)"
			};

			static std::string const SceneShader
			{
				R"(#ifdef DIFFUSE_MAP
[TextureDiffuseBinding]uniform sampler2D mapDiffuse;
#endif
#ifdef OPACITY_MAP
[TextureOpacityBinding]uniform sampler2D mapOpacity;
#endif

#ifdef LIGHTING
[varying] vec3 vtx_normal;
#endif
#ifdef TEXTURED
[varying] vec2 vtx_texture;
#endif

void main()
{
	vec4 pxl_fragColour = vec4( matAmbient.xyz, 1.0 );
#ifdef DIFFUSE_MAP
	pxl_fragColour.xyz *= [texture2D]( mapDiffuse, vtx_texture ).xyz;
#endif
#ifdef TRANSPARENT
	pxl_fragColour.a = matOpacity;
#	ifdef OPACITY_MAP
	pxl_fragColour.a *= [texture2D]( mapOpacity, vtx_texture ).r;
#	endif
#	ifdef ALPHA_TEST
	if ( pxl_fragColour.a < 0.5 )
	{
		discard;
	}
	pxl_fragColour.a = 1.0;
#	endif
#endif
	[gl_FragColor] = pxl_fragColour;
}
)"
			};

			static std::string const PickingPackPixelES2
			{
				R"(
int leftShift( int value, int shift )
{
	float mult = pow( 2.0, float( shift ) );
	return int( floor( float( value ) * mult ) );
}
int rightShift( int value, int shift )
{
	float div = pow( 2.0, float( shift ) );
	return int( floor( float( value ) / div ) );
}
vec4 packPixel( int drawIndex, int nodeIndex, int instIndex )
{
	int discarded = leftShift( rightShift( drawIndex, 6 ), 6 );  // ( drawIndex & 0xFFFFFFC0 )
	int hiR = leftShift( drawIndex - discarded, 2 );             // ( drawIndex & 0x0000003F ) << 2
	discarded = leftShift( rightShift( nodeIndex, 8 ), 2 );      // ( ( nodeIndex >> 6 ) & 0xFFFFFFFC )
	int loR = rightShift( nodeIndex, 6 ) - discarded;            // ( ( nodeIndex >> 6 ) & 0x00000003 )
	discarded = leftShift( rightShift( nodeIndex, 6 ), 6 );      // ( nodeIndex & 0xFFFFFFC0 )
	int hiG = leftShift( nodeIndex - discarded, 2 );             // ( nodeIndex & 0x0000003F ) << 2
	discarded = leftShift( rightShift( instIndex, 14 ), 14 );    // ( instIndex & 0xFFFC0000 )
	int loG = rightShift( instIndex - discarded, 16 );           // ( instIndex & 0x00030000 ) >> 16
	discarded = leftShift( rightShift( instIndex, 16 ), 16 );    // ( instIndex & 0xFFFF0000 )
	int hlB = rightShift( instIndex - discarded, 8 );            // ( instIndex & 0x0000FF00 ) >> 8
	discarded = leftShift( rightShift( instIndex, 8 ), 8 );      // ( instIndex & 0xFFFFFF00 )
	int hlA = instIndex - discarded;                             // ( instIndex & 0x000000FF ) >> 0
	return vec4( float( hiR + loR ) / 255.0
		, float( hiG + loG ) / 255.0
		, float( hlB ) / 255.0
		, float( hlA ) / 255.0 );
}
)"
			};

			static std::string const PickingPackPixelES3
			{
				R"(
vec4 packPixel( int drawIndex, int nodeIndex, int instIndex )
{
	int hiR = ( drawIndex & 0x0000003F ) << 2;
	int loR = ( nodeIndex & 0x000000C0 ) >> 6;
	int hiG = ( nodeIndex & 0x0000003F ) << 2;
	int loG = ( instIndex & 0x00030000 ) >> 16;
	int hlB = ( instIndex & 0x0000FF00 ) >> 8;
	int hlA = ( instIndex & 0x000000FF ) >> 0;
	return vec4( float( hiR | loR ) / 255.0
		, float( hiG | loG ) / 255.0
		, float( hlB ) / 255.0
		, float( hlA ) / 255.0 );
}
)"
			};

			static std::string const PickingShader
			{
				R"(
[varying] float vtx_instance;
#ifdef TEXTURED
[varying] vec2 vtx_texture;
#endif

#ifdef OPACITY_MAP
[TextureOpacityBinding]uniform sampler2D mapOpacity;
#endif

void main()
{
#ifdef TRANSPARENT
	float alpha = 1.0;
#	ifdef OPACITY_MAP
	alpha = [texture2D]( mapOpacity, vtx_texture ).r;
#	endif
	if ( alpha < 0.5 )
	{
		discard;
	}
#endif
	[gl_FragColor] = packPixel( drawIndex, nodeIndex, int( vtx_instance + 0.1 ) );
}
)"
			};

			static std::string PanelOverlayShader
			{
				R"(#ifdef DIFFUSE_MAP
[TextureDiffuseBinding]uniform sampler2D mapColour;
#endif
#ifdef OPACITY_MAP
[TextureOpacityBinding]uniform sampler2D mapOpacity;
#endif

#ifdef TEXTURED
[varying] vec2 vtx_texture;
#endif

void main()
{
	vec4 pxl_fragColour = colour;
#ifdef DIFFUSE_MAP
	pxl_fragColour.xyz = [texture2D]( mapColour, vtx_texture ).rgb;
#endif
#ifdef OPACITY_MAP
	pxl_fragColour.a *= [texture2D]( mapOpacity, vtx_texture ).r;
#endif
#ifdef ALPHA_TEST
	if ( pxl_fragColour.a < 0.5 )
	{
		discard;
	}
	pxl_fragColour.a = 1.0;
#endif
	[gl_FragColor] = pxl_fragColour;
}
)"
			};

			static std::string TextOverlayShader
			{
				R"([varying] vec2 vtx_texture;

[TextureOpacityBinding]uniform sampler2D mapOpacity;

void main()
{
	vec4 pxl_fragColour = colour;
	pxl_fragColour.a *= [texture2D]( mapOpacity, vtx_texture ).r;
	[gl_FragColor] = pxl_fragColour;
}
)"
			};

			static std::string TextureShader
			{
				R"([TextureDiffuseBinding]uniform sampler2D mapDiffuse;

[varying] vec2 vtx_texture;

void main()
{
	[gl_FragColor] = [texture2D]( mapDiffuse, vtx_texture );
}
)"
			};

			std::string getTextureDefines( TextureFlags textures )
			{
				std::string ret;

				if ( textures )
				{
					ret += "#define TEXTURED\n";

					if ( checkFlag( textures, TextureFlag::eDiffuse ) )
					{
						ret += "#define DIFFUSE_MAP\n";
					}

					if ( checkFlag( textures, TextureFlag::eOpacity ) )
					{
						ret += "#define OPACITY_MAP\n";
					}
				}

				return ret;
			}

			std::string getOpacityDefines( OpacityType opacity )
			{
				std::string ret;

				switch ( opacity )
				{
				case OpacityType::eOpaque:
					break;

				case OpacityType::eAlphaBlend:
					ret += "#define TRANSPARENT\n";
					break;

				case OpacityType::eAlphaTest:
					ret += "#define TRANSPARENT\n";
					ret += "#define ALPHA_TEST\n";
					break;

				default:
					assert( "Unsupported program type" );
					break;
				}

				return ret;
			}

			std::string getDefines( TextureFlags textures
				, OpacityType opacity )
			{
				std::string ret;
				ret += "#version 310 es\n";
				ret += "precision highp float;\n";
				ret += getTextureDefines( textures );
				ret += getOpacityDefines( opacity );

				return ret;
			}

			std::string getUniforms( RenderType render
				, ObjectType object )
			{
				std::string ret;

				if ( object != ObjectType::ePanelOverlay
					&& object != ObjectType::eTextOverlay
					&& object != ObjectType::eTexture )
				{
					switch ( render )
					{
					case RenderType::ePicking:
						ret += PickingUbo;
						break;

					default:
						ret += MatUbo;
						break;
					}

					switch ( object )
					{
					case ObjectType::ePolyLine:
						ret += PolyLineUbo;
						break;

					default:
						break;
					}
				}
				else if ( object == ObjectType::ePanelOverlay
					|| object == ObjectType::eTextOverlay )
				{
					ret += OverlayUbo;
				}

				ret += "layout( location=0 ) out vec4 out_fragColour;\n";

				return ret;
			}

			std::string getShader( RenderType render
				, ObjectType object )
			{
				std::string ret;

				switch ( render )
				{
				case RenderType::eScene:
					switch ( object )
					{
					case ObjectType::ePolyLine:
						ret += PolyLineShader;
						break;

					case ObjectType::ePanelOverlay:
						ret += PanelOverlayShader;
						break;

					case ObjectType::eTextOverlay:
						ret += TextOverlayShader;
						break;

					case ObjectType::eTexture:
						ret += TextureShader;
						break;

					default:
						ret += SceneShader;
						break;
					}
					break;

				case RenderType::ePicking:
					ret += PickingPackPixelES3;
					ret += PickingShader;
					break;

				default:
					assert( "Unsupported render type" );
					break;
				}

				return ret;
			}
		}
	}

	renderer::ShaderProgramPtr UberShader::createShaderProgram( renderer::Device const & device
		, std::string vtx
		, std::string pxl )
	{
		int count = 0;
		vtx = utils::replace( vtx, "[attribute]", [&count]()
		{
			return std::string{ "layout( location = " } + std::to_string( count++ ) + ") in";
		} );
		vtx = utils::replace( vtx, "[varying]", "out" );
		vtx = utils::replace( vtx, "[texture2D]", "texture" );
		vtx = utils::replace( vtx, "[invertY]", "gl_Position.y = -gl_Position.y;" );

		pxl = utils::replace( pxl, "[varying]", "in" );
		pxl = utils::replace( pxl, "[varying out]", "out" );
		pxl = utils::replace( pxl, "[texture2D]", "texture" );
		pxl = utils::replace( pxl, "[gl_FragColor]", "out_fragColour" );

		vtx = utils::replace( vtx, "[UboMatrixBinding]"
			, ", binding=" + std::to_string( UboMatrixBinding ) );
		vtx = utils::replace( vtx, "[UboMaterialBinding]"
			, ", binding=" + std::to_string( UboMaterialBinding ) );
		vtx = utils::replace( vtx, "[UboBillboardBinding]"
			, ", binding=" + std::to_string( UboBillboardBinding ) );
		vtx = utils::replace( vtx, "[UboPolyLineBinding]"
			, ", binding=" + std::to_string( UboPolyLineBinding ) );
		vtx = utils::replace( vtx, "[UboPickingBinding]"
			, ", binding=" + std::to_string( UboPickingBinding ) );
		vtx = utils::replace( vtx, "[UboOverlayBinding]"
			, ", binding=" + std::to_string( UboOverlayBinding ) );

		pxl = utils::replace( pxl, "[UboMatrixBinding]"
			, ", binding=" + std::to_string( UboMatrixBinding ) );
		pxl = utils::replace( pxl, "[UboMaterialBinding]"
			, ", binding=" + std::to_string( UboMaterialBinding ) );
		pxl = utils::replace( pxl, "[UboBillboardBinding]"
			, ", binding=" + std::to_string( UboBillboardBinding ) );
		pxl = utils::replace( pxl, "[UboPolyLineBinding]"
			, ", binding=" + std::to_string( UboPolyLineBinding ) );
		pxl = utils::replace( pxl, "[UboPickingBinding]"
			, ", binding=" + std::to_string( UboPickingBinding ) );
		pxl = utils::replace( pxl, "[UboOverlayBinding]"
			, ", binding=" + std::to_string( UboOverlayBinding ) );
		pxl = utils::replace( pxl, "[TextureDiffuseBinding]"
			, "layout( binding=" + std::to_string( TextureDiffuseBinding ) + " ) " );
		pxl = utils::replace( pxl, "[TextureOpacityBinding]"
			, "layout( binding=" + std::to_string( TextureOpacityBinding ) + " ) " );


		auto program = device.createShaderProgram();
		program->createModule( vtx, renderer::ShaderStageFlag::eVertex );
		program->createModule( pxl, renderer::ShaderStageFlag::eFragment );
		program->link();
		return program;
	}

	renderer::ShaderProgramPtr UberShader::createShaderProgram( renderer::Device const & device
		, RenderType render
		, TextureFlags textures
		, OpacityType opacity
		, ObjectType object )
	{
		auto vtx = doGetVertexSource( render, textures, opacity, object );
		auto pxl = doGetPixelSource( render, textures, opacity, object );
		return createShaderProgram( device, vtx, pxl );
	}

	NodeType UberShader::nodeType( OpacityType opacity
		, TextureFlags textures )
	{
		NodeType result{ NodeType::eOpaque };

		switch ( opacity )
		{
		case OpacityType::eAlphaBlend:
			result = NodeType::eAlphaBlend;
			break;

		case OpacityType::eAlphaTest:
			result = NodeType::eAlphaTest;
			break;

		default:
			break;
		}

		return NodeType( uint32_t( result ) + textures );
	}

	TextureFlags UberShader::textureFlags( NodeType type )
	{
		TextureFlags result{ TextureFlag::eNone };

		switch ( type )
		{
		case NodeType::eOpaqueNoTex:
		case NodeType::eAlphaTestNoTex:
		case NodeType::eAlphaBlendNoTex:
			break;

		case NodeType::eAlphaBlendOpa:
		case NodeType::eAlphaTestOpa:
			result |= TextureFlag::eOpacity;
			break;

		case NodeType::eOpaqueDiff:
		case NodeType::eAlphaBlendDiff:
		case NodeType::eAlphaTestDiff:
			result |= TextureFlag::eDiffuse;
			break;

		case NodeType::eAlphaBlendOpaDiff:
		case NodeType::eAlphaTestOpaDiff:
			result |= TextureFlag::eOpacity;
			result |= TextureFlag::eDiffuse;
			break;

		default:
			assert( false );
			break;
		}

		return result;
	}

	OpacityType UberShader::opacityType( NodeType type )
	{
		OpacityType result{ OpacityType::eOpaque };

		switch ( type )
		{
		case NodeType::eOpaqueNoTex:
		case NodeType::eOpaqueDiff:
			break;

		case NodeType::eAlphaTestNoTex:
		case NodeType::eAlphaTestOpa:
		case NodeType::eAlphaTestDiff:
		case NodeType::eAlphaTestOpaDiff:
			result = OpacityType::eAlphaTest;
			break;

		case NodeType::eAlphaBlendNoTex:
		case NodeType::eAlphaBlendOpa:
		case NodeType::eAlphaBlendDiff:
		case NodeType::eAlphaBlendOpaDiff:
			result = OpacityType::eAlphaBlend;
			break;

		default:
			assert( false );
			break;
		}

		return result;
	}

	std::string UberShader::doGetVertexSource( RenderType render
		, TextureFlags textures
		, OpacityType opacity
		, ObjectType object )
	{
		std::string vtx = glsl_vtx::getDefines( textures );
		vtx += glsl_vtx::getUniforms( object );
		vtx += glsl_vtx::getShader( render, object );
		return vtx;
	}

	std::string UberShader::doGetPixelSource( RenderType render
		, TextureFlags textures
		, OpacityType opacity
		, ObjectType object )
	{
		std::string pxl = glsl_pxl::getDefines( textures, opacity );
		pxl += glsl_pxl::getUniforms( render, object );
		pxl += glsl_pxl::getShader( render, object );
		return pxl;
	}
}
