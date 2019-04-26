/*
This file belongs to GlInstance.
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

		for ( auto & attach : makeArrayView( m_subpass.pColorAttachments, subpass.colorAttachmentCount ) )
		{
			m_attaches.push_back( reinterpret_cast< ID3D11RenderTargetView * >( allViews[attach.attachment] ) );
		}

		if ( subpass.pDepthStencilAttachment )
		{
			m_depthAttach = get( m_frameBuffer )->getDSView();
		}
	}

	void BeginSubpassCommand::apply( Context const & context )const
	{
		if ( context.uavs.empty() )
		{
			context.context->OMSetRenderTargets( UINT( m_attaches.size() )
				, m_attaches.data()
				, m_depthAttach );
		}
		else
		{
			std::vector< ID3D11UnorderedAccessView * > uavs;

			for ( auto & writes : context.uavs )
			{
				for ( auto & write : writes->writes )
				{
					for ( auto & uav : makeArrayView( write.pBufferInfo, write.descriptorCount ) )
					{
						auto buffer = uav.buffer;
						uavs.push_back( get( buffer )->getUnorderedAccessView() );
					}

					for ( auto & uav : makeArrayView( write.pImageInfo, write.descriptorCount ) )
					{
						auto view = uav.imageView;
						uavs.push_back( get( view )->getUnorderedAccessView() );
					}
				}
			}

			context.context->OMSetRenderTargetsAndUnorderedAccessViews( UINT( m_attaches.size() )
				, m_attaches.data()
				, m_depthAttach
				, UINT( m_attaches.size() )
				, UINT( uavs.size() )
				, uavs.data()
				, nullptr );
		}
	}

	CommandPtr BeginSubpassCommand::clone()const
	{
		return std::make_unique< BeginSubpassCommand >( *this );
	}
}
