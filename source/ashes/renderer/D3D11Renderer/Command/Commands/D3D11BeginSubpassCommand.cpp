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
					for ( auto & uav : makeArrayView( write.pBufferInfo, write.descriptorCount ) )
					{
						auto buffer = uav.buffer;
						result.push_back( get( buffer )->getUnorderedAccessView() );
					}

					for ( auto & uav : makeArrayView( write.pImageInfo, write.descriptorCount ) )
					{
						auto view = uav.imageView;
						result.push_back( get( view )->getUnorderedAccessView() );
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
			m_attaches.push_back( reinterpret_cast< ID3D11RenderTargetView * >( allViews[attach.attachment]->view ) );
		}

		if ( m_subpass.pDepthStencilAttachment )
		{
			auto & attach = *m_subpass.pDepthStencilAttachment;
			m_depthAttach = reinterpret_cast< ID3D11DepthStencilView * >( allViews[attach.attachment]->view );
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
			auto uavs = doListUavs( context.uavs );
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
