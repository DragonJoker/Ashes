#pragma once

#include <Core/Renderer.hpp>

#include <Utils/DynamicLibrary.hpp>

#include <iostream>

namespace common
{
	class RendererFactory
	{
	protected:
		using Key = std::string;
		using Creator = std::function< ashes::RendererPtr( ashes::Renderer::Configuration const & ) >;
		using ObjPtr = ashes::RendererPtr;
		using ObjMap = std::map< Key, Creator >;

	public:
		void registerType( Key const & key, Creator creator )
		{
			m_registered[key] = creator;
		}

		template< typename ... Parameters >
		ObjPtr create( Key const & key, Parameters && ... params )const
		{
			ObjPtr result;
			auto it = m_registered.find( key );

			if ( it != m_registered.end() )
			{
				result = it->second( std::forward< Parameters >( params )... );
			}
			else
			{
				static std::string const Error = "Unknown object type";
				std::cerr << Error << ": [" << key << "]" << std::endl;
				throw std::runtime_error{ Error };
			}

			return result;
		}

	private:
		ObjMap m_registered;
	};
	/**
	*\brief
	*	Gère un plugin de rendu (wrappe la fonction de création).
	*/
	class RendererPlugin
	{
	private:
		using CreatorFunction = ashes::Renderer *( *)( ashes::Renderer::Configuration const & );
		using NamerFunction = char const *( *)( );

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
