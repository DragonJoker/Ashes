/**
*\file
*	RenderLibPrerequisites.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "VkConfig.hpp"

#include "VkAccessFlag.hpp"
#include "VkAttributeFormat.hpp"
#include "VkBlendFactor.hpp"
#include "VkBlendOp.hpp"
#include "VkBufferTarget.hpp"
#include "VkColourComponentFlag.hpp"
#include "VkCommandBufferResetFlag.hpp"
#include "VkCommandBufferUsageFlag.hpp"
#include "VkCommandPoolCreateFlag.hpp"
#include "VkCompareOp.hpp"
#include "VkCullModeFlag.hpp"
#include "VkDepthStencilStateFlag.hpp"
#include "VkDescriptorType.hpp"
#include "VkFenceCreateFlag.hpp"
#include "VkFilter.hpp"
#include "VkFrontFace.hpp"
#include "VkImageAspectFlag.hpp"
#include "VkImageLayout.hpp"
#include "VkImageTiling.hpp"
#include "VkImageUsageFlag.hpp"
#include "VkIndexType.hpp"
#include "VkLogicOp.hpp"
#include "VkMemoryMapFlag.hpp"
#include "VkMemoryPropertyFlag.hpp"
#include "VkMipmapMode.hpp"
#include "VkMultisampleStateFlag.hpp"
#include "VkPipelineBindPoint.hpp"
#include "VkPipelineStageFlag.hpp"
#include "VkPixelFormat.hpp"
#include "VkPolygonMode.hpp"
#include "VkPrimitiveTopology.hpp"
#include "VkQueryControlFlag.hpp"
#include "VkQueryPipelineStatisticFlag.hpp"
#include "VkRasterisationStateFlag.hpp"
#include "VkSampleCountFlag.hpp"
#include "VkShaderStageFlag.hpp"
#include "VkStencilOp.hpp"
#include "VkSubpassContents.hpp"
#include "VkTessellationStateFlag.hpp"
#include "VkWrapMode.hpp"

#include <Renderer/RendererPrerequisites.hpp>


namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un tableau de RendererType en tableau de VkType.
	*\remarks
	*	Un prérequis à cette fonction est que la fonction VkType convert( RendererType ) existe.
	*\param[in] values
	*	Le tableau de RendererType.
	*\return
	*	Le tableau de VkType.
	*/
	template< typename VkType, typename RendererType >
	std::vector< VkType > convert( std::vector< RendererType > const & values )
	{
		std::vector< VkType > result;
		result.reserve( values.size() );

		for ( auto & value : values )
		{
			result.emplace_back( convert( value ) );
		}

		return result;
	}
}
