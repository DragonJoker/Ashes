#pragma once

#include "Prerequisites.hpp"

#include <RenderPanel.hpp>

namespace vkapp
{
	class RenderPanel
		: public common::RenderPanel
	{
	public:
		RenderPanel( wxWindow * parent
			, wxSize const & size );

	private:
		void doInitialise( renderer::Device const & device
			, renderer::UIVec2 const & size )override;
		void doUpdateOverlays( common::Gui const & overlay )override;
	};
}
