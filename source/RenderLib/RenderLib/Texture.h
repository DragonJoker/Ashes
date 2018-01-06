/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_Texture_HPP___
#define ___RenderLib_Texture_HPP___
#pragma once

#include "RenderLibPrerequisites.h"

#include <Renderer/Sampler.hpp>
#include <Renderer/Texture.hpp>

namespace render
{
	/**
	*\brief
	*	Une texture, avec son image et son échantillonneur.
	*/
	class Texture
	{
	public:
		/**
		*\brief
		*	Constructeur
		*\param[in] resources
		*	Les ressources de rendu.
		*\param[in] wrapS, wrapT
		*	Les modes de wrap de texture.
		*\param[in] minFilter, magFilter
		*	Les filtres de minification et magnification.
		*/
		Texture( renderer::Device const & device
			, renderer::WrapMode wrapS = renderer::WrapMode::eClampToEdge
			, renderer::WrapMode wrapT = renderer::WrapMode::eClampToEdge
			, renderer::WrapMode wrapR = renderer::WrapMode::eClampToEdge
			, renderer::Filter minFilter = renderer::Filter::eLinear
			, renderer::Filter magFilter = renderer::Filter::eLinear );
		/**
		*\brief
		*	Charge l'image de la texture.
		*\param[in] format
		*	Le format de l'image.
		*\param[in] size
		*	Les dimensions de l'image.
		*\param[in] data
		*	Les données de l'image.
		*/
		void image( utils::PixelFormat format
			, utils::IVec2 const & size
			, ByteArray const & data
			, renderer::StagingBuffer const & stagingBuffer
			, renderer::CommandBuffer const & commandBuffer );
		/**
		*\return
		*	Le format des pixels de la texture.
		*/
		inline utils::PixelFormat format()const noexcept
		{
			return m_format;
		}
		/**
		*\return
		*	Les dimensions de la texture.
		*/
		inline utils::IVec2 const & dimensions()const noexcept
		{
			return m_size;
		}
		/**
		*\return
		*	Les dimensions de la texture.
		*/
		inline renderer::Texture const & texture()const noexcept
		{
			return *m_texture;
		}
		/**
		*\return
		*	Les dimensions de la texture.
		*/
		inline renderer::Sampler const & sampler()const noexcept
		{
			return *m_sampler;
		}

	private:
		//! Les dimensions de l'image.
		utils::IVec2 m_size;
		//! Le format des données de l'image.
		utils::PixelFormat m_format{ utils::PixelFormat::eR8G8B8 };
		//! La texture.
		renderer::TexturePtr m_texture;
		//! L'échantillonneur.
		renderer::SamplerPtr m_sampler;
	};
}

#endif
