#pragma once

#include "Prerequisites.hpp"

#include <Core/Renderer.hpp>

#include <Utils/DynamicLibrary.hpp>

namespace common
{
	/**
	*\brief
	*	Gère un plugin de rendu (wrappe la fonction de création).
	*/
	class RendererPlugin
	{
	private:
		using CreatorFunction = ashes::Renderer *( * )( ashes::Renderer::Configuration const & );
		using NamerFunction = char const *( * )();

	public:
		RendererPlugin( RendererPlugin const & ) = delete;
		RendererPlugin( RendererPlugin && ) = default;
		RendererPlugin & operator=( RendererPlugin const & ) = delete;
		RendererPlugin & operator=( RendererPlugin && ) = default;

		RendererPlugin( ashes::DynamicLibrary && library
			, RendererFactory & factory );
		ashes::RendererPtr create( ashes::Renderer::Configuration const & configuration );

		std::string const & getShortName()
		{
			return m_shortName;
		}

		std::string const & getFullName()
		{
			return m_fullName;
		}

	private:
		ashes::DynamicLibrary m_library;
		CreatorFunction m_creator;
		std::string m_shortName;
		std::string m_fullName;
	};
}
