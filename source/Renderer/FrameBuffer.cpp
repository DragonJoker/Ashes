/*
This file belongs to VkLib.
See LICENSE file in root folder.
*/
#include "FrameBuffer.hpp"

#include "RenderPass.hpp"
#include "Texture.hpp"

#include <VkLib/Image.hpp>
#include <VkLib/ImageView.hpp>

namespace renderer
{
	namespace
	{
		vk::ImageViewCRefArray convert( TextureCRefArray const & textures )
		{
			vk::ImageViewCRefArray result;
			result.reserve( textures.size() );

			for ( auto & texture : textures )
			{
				result.emplace_back( texture.get().getImage().getView() );
			}

			return result;
		}
	}

	FrameBuffer::FrameBuffer( RenderPass const & renderPass
		, utils::IVec2 const & dimensions
		, TextureCRefArray const & textures )
		: m_frameBuffer{ renderPass.getRenderPass().createCompatibleFrameBuffer( dimensions.x
			, dimensions.y
			, convert( textures ) ) }
	{
	}

	FrameBuffer::FrameBuffer( vk::FrameBufferPtr && frameBuffer )
		: m_frameBuffer{ std::move( frameBuffer ) }
	{
	}
}
