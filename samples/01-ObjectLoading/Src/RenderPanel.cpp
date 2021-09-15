#include "RenderPanel.hpp"

#include "RenderTarget.hpp"

#include <AssimpLoader.hpp>
#include <FileUtils.hpp>

#include <ashespp/Buffer/UniformBuffer.hpp>

namespace vkapp
{
	RenderPanel::RenderPanel( wxWindow * parent
		, wxSize const & size
		, std::string const & rndName )
		: common::RenderPanel{ parent
			, size
			, AppName.ToStdString()
			, AppDesc.ToStdString()
			, rndName }
	{
	}
	
	void RenderPanel::doInitialise( utils::Device const & device
		, VkExtent2D const & size )
	{
		common::ImagePtrArray images;
		common::Scene scene;
		scene.object = common::loadObject( ashes::getPath( ashes::getExecutableDirectory() ) / "share" / "Assets" / "Nyra", "Nyra_pose.fbx", images );
		m_renderTarget = std::make_unique< RenderTarget >( device
			, *m_commandPool
			, *m_graphicsQueue
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
