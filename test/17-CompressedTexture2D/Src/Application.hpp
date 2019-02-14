#pragma once

#include "Prerequisites.hpp"

#include <Application.hpp>

namespace vkapp
{
	class Application
		: public common::App
	{
	public:
		Application();

	private:
		common::MainFrame * doCreateMainFrame( wxString const & rendererName )override;
	};
}

wxDECLARE_APP( vkapp::Application );
