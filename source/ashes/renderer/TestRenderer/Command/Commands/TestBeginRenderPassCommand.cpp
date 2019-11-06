/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestBeginRenderPassCommand.hpp"

#include "Buffer/TestBufferView.hpp"
#include "Core/TestDevice.hpp"
#include "RenderPass/TestFrameBuffer.hpp"
#include "RenderPass/TestRenderPass.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	BeginRenderPassCommand::BeginRenderPassCommand( VkDevice device
		, VkRenderPass renderPass
		, VkFramebuffer frameBuffer
		, VkClearValueArray const & clearValues )
		: CommandBase{ device }
		, m_renderPass{ renderPass }
		, m_frameBuffer{ frameBuffer }
	{
		auto rp = get( renderPass );
		assert( clearValues.size() == rp->size() );
		for ( auto & attach : *rp )
		{
			auto & clearValue = clearValues[attach.attachment];

			if ( ashes::isDepthOrStencilFormat( rp->getAttachment( attach ).format ) )
			{
				m_dsClearValue = clearValue;
			}
			else
			{
				m_rtClearValues.push_back( clearValue );
			}
		}
	}

	void BeginRenderPassCommand::apply()const
	{
		auto & views = get( m_frameBuffer )->getAllViews();
		uint32_t clearIndex = 0u;
		auto it = get( m_renderPass )->begin();

		for ( auto viewIndex = 0u; viewIndex < views.size(); ++viewIndex )
		{
			auto & attachDesc = get( m_renderPass )->getAttachment( *it );

			if ( attachDesc.loadOp == VK_ATTACHMENT_LOAD_OP_CLEAR )
			{
				if ( getAspectMask( attachDesc.format ) == VK_IMAGE_ASPECT_COLOR_BIT )
				{
					++clearIndex;
				}
			}

			++it;
		}
	}

	CommandPtr BeginRenderPassCommand::clone()const
	{
		return std::make_unique< BeginRenderPassCommand >( *this );
	}
}
