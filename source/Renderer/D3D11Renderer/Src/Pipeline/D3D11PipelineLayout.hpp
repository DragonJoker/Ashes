/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11RendererPrerequisites.hpp"

#include <Pipeline/PipelineLayout.hpp>

namespace d3d11_renderer
{
	/**
	*\brief
	*	Wrapper de TestPipelineLayout.
	*/
	class PipelineLayout
		: public renderer::PipelineLayout
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
			, renderer::DescriptorSetLayoutCRefArray const & setLayouts
			, renderer::PushConstantRangeCRefArray const & pushConstantRanges );
		/**
		*\brief
		*	Destructeur.
		*/
		~PipelineLayout();
		/**
		*\copydoc	renderer::PipelineLayout::createPipeline
		*/
		renderer::PipelinePtr createPipeline( renderer::GraphicsPipelineCreateInfo createInfo )const override;
		/**
		*\copydoc	renderer::PipelineLayout::createPipeline
		*/
		renderer::ComputePipelinePtr createPipeline( renderer::ComputePipelineCreateInfo createInfo )const override;

	private:
		Device const & m_device;
	};
}
