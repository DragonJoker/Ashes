#include "GlRendererPrerequisites.hpp"

#include "renderer/GlRenderer/Core/GlDevice.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	std::string getName( GlTextureType value )
	{
		switch ( value )
		{
		case GL_TEXTURE_1D:
			return "GL_TEXTURE_1D";

		case GL_TEXTURE_2D:
			return "GL_TEXTURE_2D";

		case GL_TEXTURE_3D:
			return "GL_TEXTURE_3D";

		case GL_TEXTURE_CUBE:
			return "GL_TEXTURE_CUBE_MAP";

		case GL_TEXTURE_1D_ARRAY:
			return "GL_TEXTURE_1D_ARRAY";

		case GL_TEXTURE_2D_ARRAY:
			return "GL_TEXTURE_2D_ARRAY";
			
		case GL_TEXTURE_BUFFER:
			return "GL_TEXTURE_BUFFER";

		case GL_TEXTURE_CUBE_ARRAY:
			return "GL_TEXTURE_CUBE_MAP_ARRAY";

		case GL_TEXTURE_2D_MULTISAMPLE:
			return "GL_TEXTURE_2D_MULTISAMPLE";

		case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
			return "GL_TEXTURE_2D_MULTISAMPLE_ARRAY";

		case GL_TEXTURE_CUBE_POSITIVE_X:
			return "GL_TEXTURE_VIEW_CUBE_MAP_POSITIVE_X";
			
		case GL_TEXTURE_CUBE_NEGATIVE_X:
			return "GL_TEXTURE_VIEW_CUBE_MAP_NEGATIVE_X";

		case GL_TEXTURE_CUBE_POSITIVE_Y:
			return "GL_TEXTURE_VIEW_CUBE_MAP_POSITIVE_Y";

		case GL_TEXTURE_CUBE_NEGATIVE_Y:
			return "GL_TEXTURE_VIEW_CUBE_MAP_NEGATIVE_Y";

		case GL_TEXTURE_CUBE_POSITIVE_Z:
			return "GL_TEXTURE_VIEW_CUBE_MAP_POSITIVE_Z";

		case GL_TEXTURE_CUBE_NEGATIVE_Z:
			return "GL_TEXTURE_VIEW_CUBE_MAP_NEGATIVE_Z";

		default:
			assert( false && "Unsupported GlTextureType" );
			return "GlTextureType_UNKNOWN";
		}
	}

	namespace gl3
	{
		GlTextureType convert( VkImageType const & value
			, uint32_t layerCount
			, VkImageCreateFlags flags )
		{
			switch ( value )
			{
			case VK_IMAGE_TYPE_1D:
				if ( layerCount > 1 )
				{
					return GL_TEXTURE_1D_ARRAY;
				}
				return GL_TEXTURE_1D;

			case VK_IMAGE_TYPE_2D:
				if ( layerCount > 1 )
				{
					if ( checkFlag( flags, VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT ) )
					{
						assert( ( layerCount % 6 ) == 0 );

						if ( layerCount > 6 )
						{
							return GL_TEXTURE_CUBE_ARRAY;
						}

						return GL_TEXTURE_CUBE;
					}

					return GL_TEXTURE_2D_ARRAY;
				}
				return GL_TEXTURE_2D;

			case VK_IMAGE_TYPE_3D:
				return GL_TEXTURE_3D;

			default:
				assert( false && "Unsupported texture type" );
				return GL_TEXTURE_2D;
			}
		}
	}

	namespace gl4
	{
		GlTextureType getTextureType( VkImageViewType const & value
			, uint32_t arraySize
			, VkSampleCountFlagBits samples )
		{
			switch ( value )
			{
			case VK_IMAGE_VIEW_TYPE_1D:
				return GL_TEXTURE_1D;

			case VK_IMAGE_VIEW_TYPE_2D:
				return samples > 1
					? GL_TEXTURE_2D_MULTISAMPLE
					: GL_TEXTURE_2D;

			case VK_IMAGE_VIEW_TYPE_3D:
				return GL_TEXTURE_3D;

			case VK_IMAGE_VIEW_TYPE_CUBE:
				return GL_TEXTURE_CUBE;

			case VK_IMAGE_VIEW_TYPE_1D_ARRAY:
				return arraySize > 1u
					? GL_TEXTURE_1D_ARRAY
					: GL_TEXTURE_1D;

			case VK_IMAGE_VIEW_TYPE_2D_ARRAY:
				return samples > 1
					? ( arraySize > 1u
						? GL_TEXTURE_2D_MULTISAMPLE_ARRAY
						: GL_TEXTURE_2D_MULTISAMPLE )
					: ( arraySize > 1u
						? GL_TEXTURE_2D_ARRAY
						: GL_TEXTURE_2D );

			case VK_IMAGE_VIEW_TYPE_CUBE_ARRAY:
				return GL_TEXTURE_CUBE_ARRAY;

			default:
				assert( false && "Unsupported ImageViewType" );
				return GL_TEXTURE_2D;
			}
		}

		GlTextureType convert( VkImageType const & value
			, uint32_t layerCount )
		{
			switch ( value )
			{
			case VK_IMAGE_TYPE_1D:
				if ( layerCount > 1 )
				{
					return GL_TEXTURE_1D_ARRAY;
				}
				return GL_TEXTURE_1D;

			case VK_IMAGE_TYPE_2D:
				if ( layerCount > 1 )
				{
					return GL_TEXTURE_2D_ARRAY;
				}
				return GL_TEXTURE_2D;

			case VK_IMAGE_TYPE_3D:
				return GL_TEXTURE_3D;

			default:
				assert( false && "Unsupported texture type" );
				return GL_TEXTURE_2D;
			}
		}
	}

	GlTextureType convert( VkDevice device
		, VkImageType const & mode
		, uint32_t layerCount
		, VkImageCreateFlags flags )
	{
		return convert( get( device )->getPhysicalDevice()
			, mode
			, layerCount
			, flags );
	}

	GlTextureType convert( VkPhysicalDevice physicalDevice
		, VkImageType const & mode
		, uint32_t layerCount
		, VkImageCreateFlags flags )
	{
		if ( hasTextureViews( physicalDevice ) )
		{
			return gl4::convert( mode, layerCount );
		}

		return gl3::convert( mode, layerCount, flags );
	}
}
