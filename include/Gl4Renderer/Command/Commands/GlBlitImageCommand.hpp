/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Command/Commands/GlCommandBase.hpp"

#include <Ashes/Miscellaneous/ImageBlit.hpp>

namespace gl_renderer
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

			ashes::ImageViewPtr view;
			GlAttachmentPoint point;
			GLuint object;
			GlAttachmentType type;
		};
		struct LayerCopy
		{
			LayerCopy( ashes::ImageBlit region
				, Image const & srcImage
				, Image const & dstImage
				, uint32_t layer );

			ashes::ImageBlit region;
			Attachment src;
			Attachment dst;
		};

	public:
		BlitImageCommand( Device const & device
			, ashes::Image const & srcImage
			, ashes::Image const & dstImage
			, std::vector< ashes::ImageBlit > const & regions
			, ashes::Filter filter );
		~BlitImageCommand();

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		Image const & m_srcTexture;
		Image const & m_dstTexture;
		std::vector< std::shared_ptr< LayerCopy > > m_layerCopies;
		GLuint m_srcFbo;
		GLuint m_dstFbo;
		GlFilter m_filter;
		GlImageAspectFlags m_mask;
		GLuint m_fbo;
	};
}
