/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___VkRenderer_ComputePipeline_HPP___
#define ___VkRenderer_ComputePipeline_HPP___
#pragma once

#include "VkRenderer/VkRendererPrerequisites.hpp"

#include <Ashes/Pipeline/ComputePipeline.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Un pipeline de rendu.
	*/
	class ComputePipeline
		: public ashes::ComputePipeline
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le LogicalDevice parent.
		*\param[in] layout
		*	Le layout du pipeline.
		*\param[in] program
		*	Les informations de creation du pipeline.
		*/
		ComputePipeline( Device const & device
			, ashes::PipelineLayout const & layout
			, ashes::ComputePipelineCreateInfo && createInfo );
		/**
		*\~french
		*\brief
		*	Destructeur.
		*\~english
		*\brief
		*	Destructor.
		*/
		~ComputePipeline();
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkPipeline.
		*\~english
		*\brief
		*	VkPipeline implicit cast operator.
		*/
		inline operator VkPipeline const &( )const
		{
			return m_pipeline;
		}

	private:
		Device const & m_device;
		PipelineLayout const & m_layout;
		std::vector< VkSpecializationMapEntry > m_specialisationEntries;
		VkSpecializationInfo m_specialisationInfos;
		VkPipelineShaderStageCreateInfo m_shaderStage;
		VkPipeline m_pipeline{ VK_NULL_HANDLE };
	};
}

#endif
