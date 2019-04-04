/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlBeginSubpassCommand.hpp"

#include "RenderPass/GlFrameBuffer.hpp"
#include "RenderPass/GlRenderPass.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	BeginSubpassCommand::BeginSubpassCommand( VkDevice device
		, VkRenderPass renderPass
		, VkFramebuffer frameBuffer
		, VkSubpassDescription subpass )
		: CommandBase{ device }
		, m_renderPass{ static_cast< VkRenderPass >( renderPass ) }
		, m_subpass{ std::move( subpass ) }
		, m_frameBuffer{ static_cast< VkFramebuffer >( frameBuffer ) }
	{
	}

	void BeginSubpassCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "NextSubpassCommand" );

		if ( get( m_frameBuffer )->getInternal() )
		{
			get( m_frameBuffer )->setDrawBuffers( context
				, VkAttachmentReferenceArray{ m_subpass.pColorAttachments, m_subpass.pColorAttachments + m_subpass.colorAttachmentCount } );
		}
	}

	CommandPtr BeginSubpassCommand::clone()const
	{
		return std::make_unique< BeginSubpassCommand >( *this );
	}
}
