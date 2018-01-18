#include "GlTextureView.hpp"

#include "GlDevice.hpp"
#include "GlTexture.hpp"

namespace gl_renderer
{
	TextureView::TextureView( Device const & device
		, Texture const & texture
		, renderer::PixelFormat format
		, uint32_t baseMipLevel
		, uint32_t levelCount
		, uint32_t baseArrayLayer
		, uint32_t layerCount )
		: renderer::TextureView{ device, texture, format, baseMipLevel, levelCount, baseArrayLayer, layerCount }
		, m_device{ device }
	{
	}

	TextureView::~TextureView()
	{
	}
}
