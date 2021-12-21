/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_PipelineLayout_HPP___
#define ___AshesPP_PipelineLayout_HPP___
#pragma once

#include "ashespp/Pipeline/GraphicsPipeline.hpp"
#include "ashespp/Pipeline/ComputePipeline.hpp"

namespace ashes
{
	/**
	*\brief
	*	The pipeline layout.
	*/
	class PipelineLayout
		: public VkObject
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The parent LogicalDevice.
		*\param[in] setLayouts
		*	The descriptor sets layouts.
		*\param[in] pushConstantRanges
		*	The push constants ranges.
		*/
		PipelineLayout( Device const & device
			, DescriptorSetLayoutCRefArray const & setLayouts
			, VkPushConstantRangeArray const & pushConstantRanges );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The parent LogicalDevice.
		*\param[in] setLayouts
		*	The descriptor sets layouts.
		*\param[in] pushConstantRanges
		*	The push constants ranges.
		*/
		PipelineLayout( Device const & device
			, std::string const & debugName
			, DescriptorSetLayoutCRefArray const & setLayouts
			, VkPushConstantRangeArray const & pushConstantRanges );
		/**
		*\brief
		*	Destructor.
		*/
		~PipelineLayout();
		/**
		*\return
		*	The descriptor sets layouts.
		*/
		inline DescriptorSetLayoutCRefArray const & getDescriptorSetsLayouts()const
		{
			return m_setLayouts;
		}
		/**
		*\return
		*	The push constant ranges.
		*/
		inline VkPushConstantRangeArray const & getPushConstantRanges()const
		{
			return m_pushConstantRanges;
		}
		/**
		*\brief
		*	VkPipelineLayout implicit cast operator.
		*/
		inline operator VkPipelineLayout const & ()const
		{
			return m_internal;
		}

	private:
		Device const & m_device;
		VkPipelineLayout m_internal{};
		DescriptorSetLayoutCRefArray m_setLayouts;
		VkPushConstantRangeArray m_pushConstantRanges;
	};
}

#endif
