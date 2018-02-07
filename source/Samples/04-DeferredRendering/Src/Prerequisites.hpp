#pragma once

#include <Prerequisites.hpp>

namespace vkapp
{
	struct TexturedVertexData
	{
		renderer::Vec4 position;
		renderer::Vec2 uv;
	};

	struct GeometryPassTexture
	{
		renderer::TexturePtr texture;
		renderer::TextureViewPtr view;
	};

	using GeometryPassResult = std::array < GeometryPassTexture, 5u >;

	static wxString const AppName = wxT( "04-DeferredRendering" );

	class Application;
	class MainFrame;
	class OpaqueRendering;
	class RenderPanel;
	class TransparentRendering;
}
