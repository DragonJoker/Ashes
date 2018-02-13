#include "RenderPanel.hpp"

#include "RenderTarget.hpp"

#include <AssimpLoader.hpp>
#include <FileUtils.hpp>

#include <Buffer/UniformBuffer.hpp>

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
	
	void RenderPanel::doInitialise( renderer::Device const & device
		, renderer::UIVec2 const & size )
	{
		common::ImagePtrArray images;
		m_renderTarget = std::make_unique< RenderTarget >( device
			, size
			, common::loadObject( common::getPath( common::getExecutableDirectory() ) / "share" / "Assets" / "Nyra", "Nyra_pose.fbx", images )
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
