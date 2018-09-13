/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "GlCommandBase.hpp"

#include <Miscellaneous/ImageBlit.hpp>

namespace gl_renderer
{
	class BlitImageCommand
		: public CommandBase
	{
	public:
		struct Attachment
		{
			Attachment( renderer::ImageSubresourceLayers & subresource
				, Texture const & image
				, uint32_t layer );

			renderer::TextureViewPtr view;
			GlAttachmentPoint point;
			GLuint object;
			GlAttachmentType type;
		};
		struct LayerCopy
		{
			LayerCopy( renderer::ImageBlit region
				, Texture const & srcImage
				, Texture const & dstImage
				, uint32_t layer );

			renderer::ImageBlit region;
			Attachment src;
			Attachment dst;
		};

	public:
		BlitImageCommand( Device const & device
			, renderer::Texture const & srcImage
			, renderer::Texture const & dstImage
			, std::vector< renderer::ImageBlit > const & regions
			, renderer::Filter filter );
		~BlitImageCommand();

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		Texture const & m_srcTexture;
		Texture const & m_dstTexture;
		std::vector< std::shared_ptr< LayerCopy > > m_layerCopies;
		GLuint m_srcFbo;
		GLuint m_dstFbo;
		GlFilter m_filter;
		GlImageAspectFlags m_mask;
		GLuint m_fbo;
	};
}
