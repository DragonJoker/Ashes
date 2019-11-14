#include "renderer/Gl4Renderer/Enum/GlConstantFormat.hpp"

namespace ashes::gl4
{
	std::string getName( GlConstantFormatBaseType value )
	{
		switch ( value )
		{
		case GL_CONSTANT_FORMAT_BASE_TYPE_UNSIGNED_BYTE:
			return "GL_UNSIGNED_BYTE";

		case GL_CONSTANT_FORMAT_BASE_TYPE_INT:
			return "GL_INT";

		case GL_CONSTANT_FORMAT_BASE_TYPE_UNSIGNED_INT:
			return "GL_UNSIGNED_INT";

		case GL_CONSTANT_FORMAT_BASE_TYPE_FLOAT:
			return "GL_FLOAT";

		default:
			assert( false && "Unsupported GlConstantFormat" );
			return "GlConstantFormat_UNKNOWN";
		}
	}

	GlConstantFormatBaseType getType( ConstantFormat format )
	{
		switch ( format )
		{
		case ConstantFormat::eFloat:
		case ConstantFormat::eVec2f:
		case ConstantFormat::eVec3f:
		case ConstantFormat::eVec4f:
		case ConstantFormat::eMat2f:
		case ConstantFormat::eMat3f:
		case ConstantFormat::eMat4f:
			return GL_CONSTANT_FORMAT_BASE_TYPE_FLOAT;

		case ConstantFormat::eInt:
		case ConstantFormat::eVec2i:
		case ConstantFormat::eVec3i:
		case ConstantFormat::eVec4i:
			return GL_CONSTANT_FORMAT_BASE_TYPE_INT;

		case ConstantFormat::eUInt:
		case ConstantFormat::eVec2ui:
		case ConstantFormat::eVec3ui:
		case ConstantFormat::eVec4ui:
			return GL_CONSTANT_FORMAT_BASE_TYPE_UNSIGNED_INT;

		case ConstantFormat::eColour:
			return GL_CONSTANT_FORMAT_BASE_TYPE_UNSIGNED_BYTE;

		default:
			assert( false && "Unsupported ConstantFormat." );
			return GL_CONSTANT_FORMAT_BASE_TYPE_FLOAT;
		}
	}

	uint32_t getCount( ConstantFormat format )
	{
		switch ( format )
		{
		case ConstantFormat::eFloat:
		case ConstantFormat::eInt:
		case ConstantFormat::eUInt:
			return 1u;

		case ConstantFormat::eVec2f:
		case ConstantFormat::eVec2i:
		case ConstantFormat::eVec2ui:
			return 2u;

		case ConstantFormat::eVec3f:
		case ConstantFormat::eVec3i:
		case ConstantFormat::eVec3ui:
			return 3u;

		case ConstantFormat::eVec4f:
		case ConstantFormat::eVec4i:
		case ConstantFormat::eVec4ui:
		case ConstantFormat::eMat2f:
		case ConstantFormat::eColour:
			return 4u;

		case ConstantFormat::eMat3f:
			return 9u;

		case ConstantFormat::eMat4f:
			return 16u;

		default:
			assert( false && "Unsupported ConstantFormat." );
			return 4u;
		}
	}
}
