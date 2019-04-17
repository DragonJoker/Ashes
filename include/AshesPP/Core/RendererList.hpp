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

		auto begin()const
		{
			return m_plugins.begin();
		}

		auto end()const
		{
			return m_plugins.end();
		}

	private:
		ashes::DynamicLibrary m_library;
		std::vector< AshPluginDescription > m_plugins;
		PFN_ashEnumeratePluginsDescriptions m_enumeratePluginDescriptions;;
		PFN_ashSelectPlugin m_selectPlugin;
	};
}

#endif
