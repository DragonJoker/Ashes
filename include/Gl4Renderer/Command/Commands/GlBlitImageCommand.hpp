/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Command/Commands/GlCommandBase.hpp"

#include "Gl4Renderer/Enum/GlAttachmentPoint.hpp"
#include "Gl4Renderer/Enum/GlAttachmentType.hpp"
#include "Gl4Renderer/Enum/GlFilter.hpp"
#include "Gl4Renderer/Enum/GlImageAspectFlag.hpp"

namespace ashes::gl4
{
	class BlitImageCommand
		: public CommandBase
	{
	public:
		struct Attachment
		{
			Attachment( VkDevice device
				, VkImageSubresourceLayers & subresource
				, VkImage image
				, uint32_t layer );

			VkImageView view;
			GlAttachmentPoint point;
			GLuint object;
			GlAttachmentType type;
		};
		struct LayerCopy
		{
			LayerCopy( VkDevice device
				, VkImageBlit region
				, VkImage srcImage
				, VkImage dstImage
				, uint32_t layer );

			VkImageBlit region;
			Attachment src;
			Attachment dst;
		};

	public:
		BlitImageCommand( VkDevice device
			, VkImage srcImage
			, VkImage dstImage
			, VkImageBlit region
			, VkFilter filter );
		~BlitImageCommand();

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		VkImage m_srcTexture;
		VkImage m_dstTexture;
		std::vector< std::shared_ptr< LayerCopy > > m_layerCopies;
		GLuint m_srcFbo;
		GLuint m_dstFbo;
		GlFilter m_filter;
		GlImageAspectFlags m_mask;
		GLuint m_fbo;
	};
}
