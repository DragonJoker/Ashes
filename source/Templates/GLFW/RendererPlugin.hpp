#pragma once

#include <Core/Renderer.hpp>

#include <Utils/DynamicLibrary.hpp>
#include <Utils/Factory.hpp>

namespace common
{
	using RendererFactory = utils::Factory< renderer::Renderer
		, std::string
		, renderer::RendererPtr
		, std::function< renderer::RendererPtr( renderer::Renderer::Configuration const & ) > >;
	/**
	*\brief
	*	Gère un plugin de rendu (wrappe la fonction de création).
	*/
	class RendererPlugin
	{
	private:
		using CreatorFunction = renderer::Renderer *( * )( renderer::Renderer::Configuration const & );

	public:
		RendererPlugin( RendererPlugin const & ) = delete;
		RendererPlugin( RendererPlugin && ) = default;
		RendererPlugin & operator=( RendererPlugin const & ) = delete;
		RendererPlugin & operator=( RendererPlugin && ) = default;

		RendererPlugin( renderer::DynamicLibrary && library
			, RendererFactory & factory );
		renderer::RendererPtr create( renderer::Renderer::Configuration const & configuration );

	private:
		renderer::DynamicLibrary m_library;
		CreatorFunction m_creator;
	};
}
