#include "renderer/Gl3Renderer/GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	std::string getName( GlConstantFormatBaseType value )
	{
		switch ( value )
		{
		case gl_renderer::GL_CONSTANT_FORMAT_BASE_TYPE_UNSIGNED_BYTE:
			return "GL_UNSIGNED_BYTE";

		case gl_renderer::GL_CONSTANT_FORMAT_BASE_TYPE_INT:
			return "GL_INT";

		case gl_renderer::GL_CONSTANT_FORMAT_BASE_TYPE_UNSIGNED_INT:
			return "GL_UNSIGNED_INT";

		case gl_renderer::GL_CONSTANT_FORMAT_BASE_TYPE_FLOAT:
			return "GL_FLOAT";

		default:
			assert( false && "Unsupported GlConstantFormat" );
			return "GlConstantFormat_UNKNOWN";
		}
	}

	GlConstantFormatBaseType getType( ashes::ConstantFormat format )
	{
		switch ( format )
		{
		case ashes::ConstantFormat::eFloat:
		case ashes::ConstantFormat::eVec2f:
		case ashes::ConstantFormat::eVec3f:
		case ashes::ConstantFormat::eVec4f:
		case ashes::ConstantFormat::eMat2f:
		case ashes::ConstantFormat::eMat3f:
		case ashes::ConstantFormat::eMat4f:
			return GL_CONSTANT_FORMAT_BASE_TYPE_FLOAT;

		case ashes::ConstantFormat::eInt:
		case ashes::ConstantFormat::eVec2i:
		case ashes::ConstantFormat::eVec3i:
		case ashes::ConstantFormat::eVec4i:
			return GL_CONSTANT_FORMAT_BASE_TYPE_INT;

		case ashes::ConstantFormat::eUInt:
		case ashes::ConstantFormat::eVec2ui:
		case ashes::ConstantFormat::eVec3ui:
		case ashes::ConstantFormat::eVec4ui:
			return GL_CONSTANT_FORMAT_BASE_TYPE_UNSIGNED_INT;

		case ashes::ConstantFormat::eColour:
			return GL_CONSTANT_FORMAT_BASE_TYPE_UNSIGNED_BYTE;

		default:
			assert( false && "Unsupported ConstantFormat." );
			return GL_CONSTANT_FORMAT_BASE_TYPE_FLOAT;
		}
	}

	uint32_t getCount( ashes::ConstantFormat format )
	{
		switch ( format )
		{
		case ashes::ConstantFormat::eFloat:
		case ashes::ConstantFormat::eInt:
		case ashes::ConstantFormat::eUInt:
			return 1u;

		case ashes::ConstantFormat::eVec2f:
		case ashes::ConstantFormat::eVec2i:
		case ashes::ConstantFormat::eVec2ui:
			return 2u;

		case ashes::ConstantFormat::eVec3f:
		case ashes::ConstantFormat::eVec3i:
		case ashes::ConstantFormat::eVec3ui:
			return 3u;

		case ashes::ConstantFormat::eVec4f:
		case ashes::ConstantFormat::eVec4i:
		case ashes::ConstantFormat::eVec4ui:
		case ashes::ConstantFormat::eMat2f:
		case ashes::ConstantFormat::eColour:
			return 4u;

		case ashes::ConstantFormat::eMat3f:
			return 9u;

		case ashes::ConstantFormat::eMat4f:
			return 16u;

		default:
			assert( false && "Unsupported ConstantFormat." );
			return 4u;
		}
	}
}
