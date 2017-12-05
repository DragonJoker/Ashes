/**
*\file
*	RenderingResources.h
*\author
*	Sylvain Doremus
*/
#ifndef ___VkRenderer_StagingBuffer_HPP___
#define ___VkRenderer_StagingBuffer_HPP___
#pragma once

#include "VkBuffer.hpp"
#include "VkBuffer.hpp"
#include "VkUniformBuffer.hpp"

#include <VkLib/StagingBuffer.hpp>
#include <VkLib/UniformBuffer.hpp>

#include <Renderer/StagingBuffer.hpp>

namespace vk_renderer
{
	/**
	*\~french
	*\brief
	*	Classe regroupant les ressources de rendu nécessaires au dessin d'une image.
	*/
	class StagingBuffer
		: public renderer::StagingBuffer
	{
	public:
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le LogicalDevice parent.
		*\param[in] size
		*	La taille du tampon.
		*/
		StagingBuffer( renderer::Device const & device
			, uint32_t size = 10000000u );
		/**
		*\~french
		*\brief
		*	Copie les données d'une image dans une texture, en passant par le tampon de transfert.
		*\param[in] data
		*	Les données à copier.
		*\param[out] texture
		*	La texture de destination.
		*/
		void copyTextureData( renderer::CommandBuffer const & commandBuffer
			, renderer::ByteArray const & data
			, renderer::Texture const & texture )const override;

	private:
		virtual void doCopyToStagingBuffer( uint8_t const * const data
			, uint32_t size )const override;
		virtual void doCopyFromStagingBuffer( renderer::CommandBuffer const & commandBuffer
			, uint32_t size
			, uint32_t offset
			, renderer::BufferBase const & buffer )const override;
		virtual void doCopyFromStagingBuffer( renderer::CommandBuffer const & commandBuffer
			, uint32_t size
			, uint32_t offset
			, renderer::VertexBufferBase const & buffer
			, renderer::PipelineStageFlags const & flags )const override;
		virtual void doCopyFromStagingBuffer( renderer::CommandBuffer const & commandBuffer
			, uint32_t size
			, uint32_t offset
			, renderer::UniformBufferBase const & buffer
			, renderer::PipelineStageFlags const & flags )const override;
	};
}

#endif
