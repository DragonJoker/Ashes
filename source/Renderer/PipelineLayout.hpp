/*
This file belongs to VkLib.
See LICENSE file in root folder
*/
#pragma once

#include "RendererPrerequisites.hpp"

#include <VkLib/PipelineLayout.hpp>

namespace renderer
{
	/**
	*\brief
	*	Wrapper de vk::PipelineLayout.
	*/
	class PipelineLayout
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le LogicalDevice parent.
		*\param[in] layout
		*	Le layout des descripteurs du pipeline.
		*/
		PipelineLayout( Device const & device
			, DescriptorSetLayout const * layout );
		/**
		*@~french
		*@brief
		*	Conversion implicite vers VkPipelineLayout.
		*@~english
		*@brief
		*	VkPipelineLayout implicit cast operator.
		*/
		inline vk::PipelineLayout getLayout()const
		{
			return *m_layout;
		}

	private:
		vk::PipelineLayoutPtr m_layout;
	};
}
