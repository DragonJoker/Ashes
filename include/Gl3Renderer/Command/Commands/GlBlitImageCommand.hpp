/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl3Renderer/Command/Commands/GlCommandBase.hpp"

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
				, Texture const & image
				, uint32_t layer );

			GlAttachmentPoint point;
			GLuint object;
			GlAttachmentType type;
		};
		struct LayerCopy
		{
			LayerCopy( ashes::ImageBlit region
				, Texture const & srcImage
				, Texture const & dstImage
				, uint32_t layer );

			ashes::ImageBlit region;
			Attachment src;
			Attachment dst;
		};

	public:
		BlitImageCommand( Device const & device
			, ashes::Texture const & srcImage
			, ashes::Texture const & dstImage
			, std::vector< ashes::ImageBlit > const & regions
			, ashes::Filter filter );
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
		GLenum m_srcTarget;
		GLenum m_dstTarget;
	};
}
