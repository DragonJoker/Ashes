/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
{
	class ResolveImageCommand
		: public CommandBase
	{
	public:
		ResolveImageCommand( VkDevice device
			, VkImage srcImage
			, VkImage dstImage
			, ArrayView< VkImageResolve const > regions );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		struct LayerResolve
		{
			LayerResolve( UINT srcSubresource
				, UINT dstSubresource )
				: srcSubresource{ srcSubresource }
				, dstSubresource{ dstSubresource }
			{
			}

			UINT srcSubresource;
			UINT dstSubresource;
		};
		DXGI_FORMAT m_format;
		ID3D11Resource * m_srcResource;
		ID3D11Resource * m_dstResource;
		std::vector< LayerResolve > m_layers;
	};
}
