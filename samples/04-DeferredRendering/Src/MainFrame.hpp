#pragma once

#include "Prerequisites.hpp"

#include <ashespp/Core/Instance.hpp>

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
		wxWindowPtr< common::RenderPanel > doCreatePanel( wxSize const & size, utils::Instance const & instance )override;
	};
}
