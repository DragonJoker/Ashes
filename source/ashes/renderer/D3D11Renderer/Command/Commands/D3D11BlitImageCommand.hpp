/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
{
	class BlitImageCommand
		: public CommandBase
	{
	public:
		struct Attachment
		{
			Attachment( VkImageSubresourceLayers & subresource
				, VkImage image
				, uint32_t layer );

			ID3D11Resource * image;
			UINT subResourceIndex;
		};
		struct LayerCopy
		{
			LayerCopy( VkImageBlit region
				, VkImage srcImage
				, VkImage dstImage
				, uint32_t layer );

			VkOffset3D dstOffset;
			D3D11_BOX srcBox;
			Attachment src;
			Attachment dst;
		};

	public:
		BlitImageCommand( VkDevice device
			, VkImage srcImage
			, VkImage dstImage
			, VkImageBlitArray const & regions
			, VkFilter filter );
		~BlitImageCommand();

		void apply( Context const & context )const;
		CommandPtr clone()const;

	private:
		VkImage m_srcTexture;
		VkImage m_dstTexture;
		std::vector< std::shared_ptr< LayerCopy > > m_layerCopies;
		D3D11_FILTER_TYPE m_filter;
	};
}
