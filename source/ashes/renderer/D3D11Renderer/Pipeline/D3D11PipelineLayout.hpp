/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/D3D11RendererPrerequisites.hpp"

#include <Ashes/Pipeline/PipelineLayout.hpp>

namespace ashes::d3d11
{
	/**
	*\brief
	*	Wrapper de TestPipelineLayout.
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
		PipelineLayout( VkDevice device
			, ashes::VkDescriptorSetLayoutArray const & setLayouts
			, ashes::VkPushConstantRangeArray const & pushConstantRanges );
		/**
		*\brief
		*	Destructeur.
		*/
		~PipelineLayout();
		/**
		*\copydoc	ashes::PipelineLayout::createPipeline
		*/
		ashes::PipelinePtr createPipeline( ashes::GraphicsPipelineCreateInfo createInfo )const;
		/**
		*\copydoc	ashes::PipelineLayout::createPipeline
		*/
		ashes::ComputePipelinePtr createPipeline( ashes::ComputePipelineCreateInfo createInfo )const;

	private:
		VkDevice m_device;
	};
}
