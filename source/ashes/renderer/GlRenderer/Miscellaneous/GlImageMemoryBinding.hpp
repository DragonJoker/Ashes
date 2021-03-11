/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Miscellaneous/GlDeviceMemoryBinding.hpp"

namespace ashes::gl
{
	class ImageMemoryBinding
		: public DeviceMemoryBinding
	{
	public:
		ImageMemoryBinding( VkDeviceMemory parent
			, VkDevice device
			, VkImage image
			, VkDeviceSize memoryOffset );
		~ImageMemoryBinding();

		void upload( ContextLock const & context
			, ByteArray const & data
			, VkDeviceSize offset
			, VkDeviceSize size )const override;

	private:
		void setImage1D( ContextLock const & context );
		void setImage2D( ContextLock const & context
			, uint32_t height );
		void setImageCubeFace( ContextLock const & context
			, int face );
		void setImage3D( ContextLock const & context
			, uint32_t depth );
		void setImage2DMS( ContextLock const & context );
		void setImage3DMS( ContextLock const & context
			, uint32_t depth );
		void setupUpdateRegions( VkDeviceSize offset
			, VkDeviceSize size )const;
		void updateRegion( ContextLock const & context
			, VkBufferImageCopy const & copyInfo )const;

	private:
		Image * m_texture;
		std::vector< VkBufferImageCopy > m_updateRegions;
		mutable size_t m_beginRegion{ 0u };
		mutable size_t m_endRegion{ 0u };
	};
}