#pragma once

#include "Prerequisites.hpp"

#include <AshesPP/Core/Instance.hpp>

#include <MainFrame.hpp>

namespace vkapp
{
	class MainFrame
		: public common::MainFrame
	{
	public:
		MainFrame( wxString const & rendererName
			, ashes::RendererList const & renderers );

	private:
		common::RenderPanel * doCreatePanel( wxSize const & size, utils::Instance const & instance )override;
	};
}
