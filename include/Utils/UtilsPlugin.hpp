#pragma once

#include "Factory.hpp"

#include <Ashes/Core/Instance.hpp>
#include <Ashes/Utils/DynamicLibrary.hpp>

namespace utils
{
	using RendererFactory = Factory< ashes::Instance
		, std::string
		, ashes::InstancePtr
		, std::function< ashes::InstancePtr( ashes::Instance::Configuration const & ) > >;
	/**
	*\brief
	*	Gère un plugin de rendu (wrappe la fonction de création).
	*/
	class Plugin
	{
	private:
		using CreatorFunction = ashes::Instance *( * )( ashes::Instance::Configuration const & );
		using NamerFunction = char const *( * )();

	public:
		Plugin( Plugin const & ) = delete;
		Plugin( Plugin && ) = default;
		Plugin & operator=( Plugin const & ) = delete;
		Plugin & operator=( Plugin && ) = default;

		Plugin( ashes::DynamicLibrary library
			, RendererFactory & factory );
		ashes::InstancePtr create( ashes::Instance::Configuration const & configuration )const;

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
