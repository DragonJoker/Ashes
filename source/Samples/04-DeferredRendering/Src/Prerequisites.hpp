#pragma once

#include <Prerequisites.hpp>

namespace vkapp
{
	struct GeometryPassTexture
	{
		renderer::TexturePtr texture;
		renderer::TextureViewPtr view;
	};

	using GeometryPassResult = std::array < GeometryPassTexture, 5u >;

	static wxString const AppName = wxT( "04-DeferredRendering" );
	static wxString const AppDesc = wxT( "Deferred Rendering" );

	class Application;
	class MainFrame;
	class OpaqueRendering;
	class RenderPanel;
	class TransparentRendering;
}
