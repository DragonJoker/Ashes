/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_Renderer_HPP___
#define ___AshesPP_Renderer_HPP___
#pragma once

#include "ashespp/Core/Instance.hpp"

#include <ashes/common/DynamicLibrary.hpp>

namespace ashes
{
	class RendererList
	{
	public:
		RendererList();

		AshPluginDescription selectPlugin( std::string const & name )const;
		AshPluginDescription getSelectedPlugin()const;

		std::vector< AshPluginDescription >::const_iterator find( std::string const & name )const;

		auto begin()const noexcept
		{
			return m_plugins.begin();
		}

		auto end()const noexcept
		{
			return m_plugins.end();
		}

		bool empty()const noexcept
		{
			return m_plugins.empty();
		}

	private:
		std::unique_ptr< ashes::DynamicLibrary > m_library;
		std::vector< AshPluginDescription > m_plugins;
		PFN_ashSelectPlugin m_selectPlugin{};
		mutable AshPluginDescription m_current{};
	};
}

#endif
