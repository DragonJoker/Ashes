#pragma once

#include "Prerequisites.hpp"

#include <Ashes/Core/Instance.hpp>

#include <Ashes/Utils/DynamicLibrary.hpp>

namespace common
{
	/**
	*\brief
	*	Gère un plugin de rendu (wrappe la fonction de création).
	*/
	class RendererPlugin
	{
	private:
		using CreatorFunction = ashes::Instance *( * )( ashes::Instance::Configuration const & );
		using NamerFunction = char const *( * )();

	public:
		RendererPlugin( RendererPlugin const & ) = delete;
		RendererPlugin( RendererPlugin && ) = default;
		RendererPlugin & operator=( RendererPlugin const & ) = delete;
		RendererPlugin & operator=( RendererPlugin && ) = default;

		RendererPlugin( ashes::DynamicLibrary library
			, RendererFactory & factory );
		ashes::InstancePtr create( ashes::Instance::Configuration const & configuration );

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
