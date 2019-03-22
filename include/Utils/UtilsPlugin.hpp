#pragma once

#include "Utils/UtilsPrerequisites.hpp"

#include <Ashes/Core/Instance.hpp>
#include <Ashes/Utils/DynamicLibrary.hpp>

namespace utils
{
	/**
	*\brief
	*	Gère un plugin de rendu (wrappe la fonction de création).
	*/
	class Plugin
	{
	private:
		using CreatorFunction = ashes::Result( * )( ashes::InstanceCreateInfo, ashes::Instance ** );
		using VersionEnumeratorFunction = ashes::Result( * )( uint32_t * );
		using LayerPropertiesEnumeratorFunction = ashes::Result( * )( uint32_t *, ashes::LayerProperties * );
		using ExtensionPropertiesEnumeratorFunction = ashes::Result( * )( char const * const, uint32_t *, ashes::ExtensionProperties * );
		using NamerFunction = char const *( * )();

	public:
		Plugin( Plugin const & ) = delete;
		Plugin( Plugin && ) = default;
		Plugin & operator=( Plugin const & ) = delete;
		Plugin & operator=( Plugin && ) = default;

		Plugin( ashes::DynamicLibrary library );
		uint32_t enumerateVersion()const;
		ashes::LayerPropertiesArray enumerateLayerProperties()const;
		ashes::ExtensionPropertiesArray enumerateExtensionProperties( std::string const & layerName )const;
		ashes::InstancePtr create( ashes::InstanceCreateInfo createInfo )const;

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
		VersionEnumeratorFunction m_enumerateVersion;
		LayerPropertiesEnumeratorFunction m_enumerateLayerProperties;
		ExtensionPropertiesEnumeratorFunction m_enumerateExtensionProperties;
		std::string m_shortName;
		std::string m_fullName;
	};
}
