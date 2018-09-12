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
		using CreatorFunction = renderer::Renderer *( * )( renderer::Renderer::Configuration const & );
		using NamerFunction = char const *( * )();

	public:
		RendererPlugin( RendererPlugin const & ) = delete;
		RendererPlugin( RendererPlugin && ) = default;
		RendererPlugin & operator=( RendererPlugin const & ) = delete;
		RendererPlugin & operator=( RendererPlugin && ) = default;

		RendererPlugin( renderer::DynamicLibrary && library
			, RendererFactory & factory );
		renderer::RendererPtr create( renderer::Renderer::Configuration const & configuration );

		std::string const & getShortName()
		{
			return m_shortName;
		}

		std::string const & getFullName()
		{
			return m_fullName;
		}

	private:
		renderer::DynamicLibrary m_library;
		CreatorFunction m_creator;
		std::string m_shortName;
		std::string m_fullName;
	};
}
