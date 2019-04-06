/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_Renderer_HPP___
#define ___AshesPP_Renderer_HPP___
#pragma once

#include "AshesPP/Core/Instance.hpp"

#include <AshesCommon/DynamicLibrary.hpp>

namespace ashes
{
	class RendererList
	{
	public:
		RendererList();
		~RendererList();

		AshPluginDescription selectPlugin( std::string const & name )const;

	private:
		ashes::DynamicLibrary m_library;
		std::vector< AshPluginDescription > m_plugins;
	};
}

#endif
