#include "RenderPanel.hpp"

#include "RenderTarget.hpp"

#include <AssimpLoader.hpp>
#include <FileUtils.hpp>

#include <Ashes/Buffer/UniformBuffer.hpp>

namespace vkapp
{
	RenderPanel::RenderPanel( wxWindow * parent
		, wxSize const & size )
		: common::RenderPanel{ parent
			, size
			, AppName.ToStdString()
			, AppDesc.ToStdString() }
	{
	}
	
	void RenderPanel::doInitialise( ashes::Device const & device
		, ashes::Extent2D const & size )
	{
		common::ImagePtrArray images;
		common::Scene scene;
		scene.object = common::loadObject( common::getPath( common::getExecutableDirectory() ) / "share" / "Assets" / "Nyra", "Nyra_pose.fbx", images );
		m_renderTarget = std::make_unique< RenderTarget >( device
			, size
			, std::move( scene )
			, std::move( images ) );
	}

	void RenderPanel::doUpdateOverlays( common::Gui const & overlay )
	{
		//if ( overlay.header( "Settings" ) )
		//{
		//	if ( overlay.checkBox( "Wireframe", &wireframe ) )
		//	{
		//		buildCommandBuffers();
		//	}
		//}
	}
}
