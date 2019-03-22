#pragma once

#include "FileUtils.hpp"

#include <Ashes/Core/Exception.hpp>
#include <Ashes/Core/Instance.hpp>
#include <Ashes/Utils/DynamicLibrary.hpp>

#include <iostream>

namespace common
{
	class RendererPlugin
	{
		using CreatorFunction = ashes::Result( *)( ashes::InstanceCreateInfo, ashes::Instance ** );
		using VersionEnumeratorFunction = ashes::Result( *)( uint32_t * );
		using LayerPropertiesEnumeratorFunction = ashes::Result( *)( uint32_t *, ashes::LayerProperties * );
		using ExtensionPropertiesEnumeratorFunction = ashes::Result( *)( char const * const, uint32_t *, ashes::ExtensionProperties * );
		using NamerFunction = char const *( *)( );

	public:
		RendererPlugin( ashes::DynamicLibrary library )
			: m_library{ std::move( library ) }
			, m_creator{ nullptr }
		{
			if ( !m_library.getFunction( "createInstance", m_creator ) )
			{
				throw std::runtime_error{ "[" + getFileName( m_library.getPath() ) + "] is not a renderer plugin" };
			}

			if ( !m_library.getFunction( "enumerateVersion", m_enumerateVersion ) )
			{
				throw std::runtime_error{ "[" + getFileName( m_library.getPath() ) + "] is not a renderer plugin" };
			}

			if ( !m_library.getFunction( "enumerateLayerProperties", m_enumerateLayerProperties ) )
			{
				throw std::runtime_error{ "[" + getFileName( m_library.getPath() ) + "] is not a renderer plugin" };
			}

			if ( !m_library.getFunction( "enumerateExtensionProperties", m_enumerateExtensionProperties ) )
			{
				throw std::runtime_error{ "[" + getFileName( m_library.getPath() ) + "] is not a renderer plugin" };
			}

			NamerFunction shortNamer;

			if ( !m_library.getFunction( "getShortName", shortNamer ) )
			{
				throw std::runtime_error{ "[" + getFileName( m_library.getPath() ) + "] is not a renderer plugin" };
			}

			NamerFunction fullNamer;

			if ( !m_library.getFunction( "getFullName", fullNamer ) )
			{
				throw std::runtime_error{ "[" + getFileName( m_library.getPath() ) + "] is not a renderer plugin" };
			}

			m_shortName = shortNamer();
			m_fullName = fullNamer();
		}

		uint32_t enumerateVersion()const
		{
			uint32_t result;
			auto res = m_enumerateVersion( &result );

			if ( res != ashes::Result::eSuccess )
			{
				throw ashes::Exception{ res, "Version retrieval" };
			}

			return result;
		}

		ashes::LayerPropertiesArray enumerateLayerProperties()const
		{
			uint32_t count;
			ashes::LayerPropertiesArray result;
			ashes::Result res;

			do
			{
				res = m_enumerateLayerProperties( &count, nullptr );

				if ( count )
				{
					result.resize( count );
					res = m_enumerateLayerProperties( &count, result.data() );
				}
			}
			while ( res == ashes::Result::eIncomplete );

			if ( res != ashes::Result::eSuccess )
			{
				throw ashes::Exception{ res, "Instance layers retrieval" };
			}

			return result;
		}

		ashes::ExtensionPropertiesArray enumerateExtensionProperties( std::string const & layerName )const
		{
			uint32_t count{};
			ashes::ExtensionPropertiesArray result;
			ashes::Result res{};

			do
			{
				res = m_enumerateExtensionProperties( layerName.empty() ? nullptr : layerName.c_str()
					, &count
					, nullptr );

				if ( count )
				{
					result.resize( count );
					res = m_enumerateExtensionProperties( layerName.empty() ? nullptr : layerName.c_str()
						, &count
						, result.data() );
				}
			}
			while ( res == ashes::Result::eIncomplete );

			if ( res != ashes::Result::eSuccess )
			{
				throw ashes::Exception{ res, "Instance layer [" + layerName + "] extensions retrieval" };
			}

			return result;
		}

		ashes::InstancePtr create( ashes::InstanceCreateInfo createInfo )const
		{
			ashes::Instance * result;
			auto res = m_creator( std::move( createInfo ), &result );

			if ( res != ashes::Result::eSuccess )
			{
				throw ashes::Exception{ res, "Instance creation" };
			}

			return ashes::InstancePtr{ result };
		}

		inline std::string const & getShortName()const
		{
			return m_shortName;
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


	class RendererFactory
	{
	protected:
		using Key = std::string;
		using PtrType = RendererPlugin const *;
		using ObjMap = std::map< Key, PtrType >;

	public:
		void registerType( Key const & key, PtrType plugin )
		{
			m_registered[key] = plugin;
		}

		template< typename ... Parameters >
		PtrType findPlugin( Key const & key )const
		{
			PtrType result{ nullptr };
			auto it = m_registered.find( key );

			if ( it != m_registered.end() )
			{
				result = it->second;
			}
			else
			{
				static std::string const Error = "Unknown object type";
				std::cerr << Error << std::endl;
				throw std::runtime_error{ Error };
			}

			return result;
		}

	private:
		ObjMap m_registered;
	}; 
}
