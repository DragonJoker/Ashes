#pragma once

#include <Prerequisites.hpp>

namespace vkapp
{
	struct GeometryPassTexture
	{
		ashes::ImagePtr texture;
		ashes::ImageView view;
	};

	using GeometryPassResult = std::array < GeometryPassTexture, 5u >;

	static wxString const AppName{ wxT( SAMPLE_FOLDER ) };
	static wxString const AppDesc = wxT( "Deferred Rendering" );

	class Application;
	class MainFrame;
	class OpaqueRendering;
	class RenderPanel;
	class TransparentRendering;
}
