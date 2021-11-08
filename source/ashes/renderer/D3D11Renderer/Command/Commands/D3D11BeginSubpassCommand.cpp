/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11BeginSubpassCommand.hpp"

#include "Buffer/D3D11Buffer.hpp"
#include "Image/D3D11ImageView.hpp"
#include "RenderPass/D3D11FrameBuffer.hpp"
#include "RenderPass/D3D11RenderPass.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	namespace
	{
		std::vector< ID3D11UnorderedAccessView * > doListUavs( LayoutBindingWritesArray uavs )
		{
			std::vector< ID3D11UnorderedAccessView * > result;

			for ( auto & writes : uavs )
			{
				for ( auto & write : writes->writes )
				{
					if ( write.pBufferInfo )
					{
						for ( auto & uav : makeArrayView( write.pBufferInfo, write.descriptorCount ) )
						{
							auto buffer = uav.buffer;
							result.push_back( get( buffer )->getUnorderedAccessView() );
						}
					}
					else
					{
						assert( write.pImageInfo );
						for ( auto & uav : makeArrayView( write.pImageInfo, write.descriptorCount ) )
						{
							auto view = uav.imageView;
							result.push_back( get( view )->getUnorderedAccessView() );
						}
					}
				}
			}

			return result;
		}
	}

	BeginSubpassCommand::BeginSubpassCommand( VkDevice device
		, VkRenderPass renderPass
		, VkFramebuffer frameBuffer
		, VkSubpassDescription const & subpass )
		: CommandBase{ device }
		, m_renderPass{ renderPass }
		, m_subpass{ subpass }
		, m_frameBuffer{ frameBuffer }
	{
		auto & allViews = get( m_frameBuffer )->getAllViews();
		auto attaches = makeArrayView( m_subpass.pColorAttachments
			, m_subpass.colorAttachmentCount );

		for ( auto & attach : attaches )
		{
			m_attaches.push_back( static_cast< ID3D11RenderTargetView * >( allViews[attach.attachment]->view ) );
		}

		if ( m_subpass.pDepthStencilAttachment )
		{
			auto & attach = *m_subpass.pDepthStencilAttachment;
			m_depthAttach = static_cast< ID3D11DepthStencilView * >( allViews[attach.attachment]->view );
		}
	}

	void BeginSubpassCommand::apply( Context const & context )const
	{
		auto uavStart = UINT( m_attaches.size() );
		context.context->OMSetRenderTargetsAndUnorderedAccessViews( UINT( m_attaches.size() )
			, m_attaches.data()
			, m_depthAttach
			, uavStart
			, D3D11_KEEP_UNORDERED_ACCESS_VIEWS
			, nullptr
			, nullptr );

		if ( context.uavStart != uavStart )
		{
			context.uavStart = uavStart;

			if ( !context.rawUavs.empty() )
			{
				context.context->OMSetRenderTargetsAndUnorderedAccessViews( D3D11_KEEP_RENDER_TARGETS_AND_DEPTH_STENCIL
					, nullptr
					, nullptr
					, context.uavStart
					, UINT( context.rawUavs.size() )
					, context.rawUavs.data()
					, nullptr );
			}
		}
	}

	CommandPtr BeginSubpassCommand::clone()const
	{
		return std::make_unique< BeginSubpassCommand >( *this );
	}
}
