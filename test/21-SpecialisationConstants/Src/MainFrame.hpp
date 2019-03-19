#pragma once

#include "Prerequisites.hpp"

#include <Core/Instance.hpp>

#include <MainFrame.hpp>

namespace vkapp
{
	class MainFrame
		: public common::MainFrame
	{
	public:
		MainFrame( wxString const & rendererName
			, utils::RendererFactory & factory );

	private:
		wxPanel * doCreatePanel( wxSize const & size, ashes::Instance const & instance )override;
	};
}
