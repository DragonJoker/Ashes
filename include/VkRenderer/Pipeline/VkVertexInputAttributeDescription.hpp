/**
*\file
*	VertexBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___VkRenderer_VertexInputAttributeDescription_HPP___
#define ___VkRenderer_VertexInputAttributeDescription_HPP___
#pragma once

#include <Ashes/Pipeline/VertexInputAttributeDescription.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::VertexInputAttributeDescription en VkVertexInputAttributeDescription.
	*\param[in] desc
	*	Le ashes::VertexInputAttributeDescription.
	*\return
	*	Le VkVertexInputAttributeDescription.
	*/
	VkVertexInputAttributeDescription convert( ashes::VertexInputAttributeDescription const & desc );
}

#endif
