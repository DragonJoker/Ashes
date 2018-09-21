/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <Pipeline/PipelineLayout.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Wrapper de VkPipelineLayout.
	*/
	class PipelineLayout
		: public ashes::PipelineLayout
	{
	public:
		/**
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The parent LogicalDevice.
		*\param[in] setLayouts
		*	The descriptor sets layouts.
		*\param[in] pushConstantRanges
		*	The push constants ranges.
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le LogicalDevice parent.
		*\param[in] setLayouts
		*	Les layouts des descripteurs du pipeline.
		*\param[in] pushConstantRanges
		*	Les intervalles de push constants.
		*/
		PipelineLayout( Device const & device
			, ashes::DescriptorSetLayoutCRefArray const & setLayouts
			, ashes::PushConstantRangeArray const & pushConstantRanges );
		/**
		*\brief
		*	Destructeur.
		*/
		~PipelineLayout();
		/**
		*\copydoc	ashes::PipelineLayout::createPipeline
		*/
		ashes::PipelinePtr createPipeline( ashes::GraphicsPipelineCreateInfo createInfo )const override;
		/**
		*\copydoc	ashes::PipelineLayout::createPipeline
		*/
		ashes::ComputePipelinePtr createPipeline( ashes::ComputePipelineCreateInfo createInfo )const override;
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkPipelineLayout.
		*\~english
		*\brief
		*	VkPipelineLayout implicit cast operator.
		*/
		inline operator VkPipelineLayout const &( )const
		{
			return m_layout;
		}

	private:
		Device const & m_device;
		VkPipelineLayout m_layout{ VK_NULL_HANDLE };
	};
}
