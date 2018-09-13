#pragma once

#include <Core/Renderer.hpp>

#include <Utils/DynamicLibrary.hpp>
#include <Factory.hpp>

namespace common
{
	using RendererFactory = utils::Factory< ashes::Renderer
		, std::string
		, ashes::RendererPtr
		, std::function< ashes::RendererPtr( ashes::Renderer::Configuration const & ) > >;
	/**
	*\brief
	*	Gère un plugin de rendu (wrappe la fonction de création).
	*/
	class RendererPlugin
	{
	private:
		using CreatorFunction = ashes::Renderer *( * )( ashes::Renderer::Configuration const & );

	public:
		RendererPlugin( RendererPlugin const & ) = delete;
		RendererPlugin( RendererPlugin && ) = default;
		RendererPlugin & operator=( RendererPlugin const & ) = delete;
		RendererPlugin & operator=( RendererPlugin && ) = default;

		RendererPlugin( ashes::DynamicLibrary && library
			, RendererFactory & factory );
		ashes::RendererPtr create( ashes::Renderer::Configuration const & configuration );

	private:
		ashes::DynamicLibrary m_library;
		CreatorFunction m_creator;
	};
}
