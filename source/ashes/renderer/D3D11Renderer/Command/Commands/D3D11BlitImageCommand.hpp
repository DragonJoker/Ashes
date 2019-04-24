/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

#include <Ashes/Miscellaneous/ImageBlit.hpp>

namespace ashes::d3d11
{
	class BlitImageCommand
		: public CommandBase
	{
	public:
		struct Attachment
		{
			Attachment( ashes::ImageSubresourceLayers & subresource
				, Image const & image
				, uint32_t layer );

			ID3D11Resource * image;
			UINT subResourceIndex;
		};
		struct LayerCopy
		{
			LayerCopy( ashes::ImageBlit region
				, Image const & srcImage
				, Image const & dstImage
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
			, std::vector< ashes::ImageBlit > const & regions
			, VkFilter filter );
		~BlitImageCommand();

		void apply( Context const & context )const;
		CommandPtr clone()const;

	private:
		Image const & m_srcTexture;
		Image const & m_dstTexture;
		std::vector< std::shared_ptr< LayerCopy > > m_layerCopies;
		D3D11_FILTER_TYPE m_filter;
	};
}
