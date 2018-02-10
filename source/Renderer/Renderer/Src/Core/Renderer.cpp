/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "Core/Renderer.hpp"

namespace renderer
{
	Renderer::Renderer( ClipDirection clipDirection
		, std::string const & name )
		: m_clipDirection{ clipDirection }
		, m_name{ name }
	{
	}
}
