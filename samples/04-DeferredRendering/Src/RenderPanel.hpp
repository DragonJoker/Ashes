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
		void doInitialise( utils::Device const & device
			, VkExtent2D const & size )override;
		void doUpdateOverlays( common::Gui const & overlay )override;
	};
}
