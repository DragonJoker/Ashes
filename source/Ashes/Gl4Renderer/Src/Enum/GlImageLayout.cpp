#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	std::string getName( GlImageLayout value )
	{
		switch ( value )
		{
		case gl_renderer::GL_LAYOUT_UNDEFINED_EXT:
			return "GL_LAYOUT_UNDEFINED_EXT";

		case gl_renderer::GL_LAYOUT_GENERAL_EXT:
			return "GL_LAYOUT_GENERAL_EXT";

		case gl_renderer::GL_LAYOUT_COLOR_ATTACHMENT_EXT:
			return "GL_LAYOUT_COLOR_ATTACHMENT_EXT";

		case gl_renderer::GL_LAYOUT_DEPTH_STENCIL_ATTACHMENT_EXT:
			return "GL_LAYOUT_DEPTH_STENCIL_ATTACHMENT_EXT";

		case gl_renderer::GL_LAYOUT_DEPTH_STENCIL_READ_ONLY_EXT:
			return "GL_LAYOUT_DEPTH_STENCIL_READ_ONLY_EXT";

		case gl_renderer::GL_LAYOUT_SHADER_READ_ONLY_EXT:
			return "GL_LAYOUT_SHADER_READ_ONLY_EXT";

		case gl_renderer::GL_LAYOUT_TRANSFER_SRC_EXT:
			return "GL_LAYOUT_TRANSFER_SRC_EXT";

		case gl_renderer::GL_LAYOUT_TRANSFER_DST_EXT:
			return "GL_LAYOUT_TRANSFER_DST_EXT";

		default:
			assert( false && "Unsupported GlImageLayout" );
			return "GlImageLayout_UNKNOWN";
		}
	}

	GlImageLayout convert( ashes::ImageLayout const & layout )
	{
		switch ( layout )
		{
		case ashes::ImageLayout::eUndefined:
		case ashes::ImageLayout::ePreinitialised:
		case ashes::ImageLayout::ePresentSrc:
			return GL_LAYOUT_UNDEFINED_EXT;

		case ashes::ImageLayout::eGeneral:
			return GL_LAYOUT_GENERAL_EXT;

		case ashes::ImageLayout::eColourAttachmentOptimal:
			return GL_LAYOUT_COLOR_ATTACHMENT_EXT;

		case ashes::ImageLayout::eDepthStencilAttachmentOptimal:
			return GL_LAYOUT_DEPTH_STENCIL_ATTACHMENT_EXT;

		case ashes::ImageLayout::eDepthStencilReadOnlyOptimal:
			return GL_LAYOUT_DEPTH_STENCIL_READ_ONLY_EXT;

		case ashes::ImageLayout::eShaderReadOnlyOptimal:
			return GL_LAYOUT_SHADER_READ_ONLY_EXT;

		case ashes::ImageLayout::eTransferSrcOptimal:
			return GL_LAYOUT_TRANSFER_SRC_EXT;

		case ashes::ImageLayout::eTransferDstOptimal:
			return GL_LAYOUT_TRANSFER_DST_EXT;

		default:
			assert( false && "Unsupported image layout" );
			return GL_LAYOUT_UNDEFINED_EXT;
		}
	}

	ashes::ImageLayout convertImageLayout( GlImageLayout layout )
	{
		switch ( layout )
		{
		case GL_LAYOUT_UNDEFINED_EXT:
			return ashes::ImageLayout::eUndefined;

		case GL_LAYOUT_GENERAL_EXT:
			return ashes::ImageLayout::eGeneral;

		case GL_LAYOUT_COLOR_ATTACHMENT_EXT:
			return ashes::ImageLayout::eColourAttachmentOptimal;

		case GL_LAYOUT_DEPTH_STENCIL_ATTACHMENT_EXT:
			return ashes::ImageLayout::eDepthStencilAttachmentOptimal;

		case GL_LAYOUT_DEPTH_STENCIL_READ_ONLY_EXT:
			return ashes::ImageLayout::eDepthStencilReadOnlyOptimal;

		case GL_LAYOUT_SHADER_READ_ONLY_EXT:
			return ashes::ImageLayout::eShaderReadOnlyOptimal;

		case GL_LAYOUT_TRANSFER_SRC_EXT:
			return ashes::ImageLayout::eTransferSrcOptimal;

		case GL_LAYOUT_TRANSFER_DST_EXT:
			return ashes::ImageLayout::eTransferDstOptimal;

		default:
			assert( false && "Unsupported image layout" );
			return ashes::ImageLayout::eUndefined;
		}
	}
}

