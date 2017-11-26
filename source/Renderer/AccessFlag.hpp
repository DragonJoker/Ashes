/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <VkLib/FlagCombination.hpp>

namespace renderer
{
	/**
	*\brief
	*	Enumération des types d'accès.
	*/
	enum class AccessFlag
		: uint32_t
	{
		eIndirectCommandRead = 0x00000001,
		eIndexRead = 0x00000002,
		eVertexAttributeRead = 0x00000004,
		eUniformRead = 0x00000008,
		eInputAttachmentRead = 0x00000010,
		eShaderRead = 0x00000020,
		eShaderWrite = 0x00000040,
		eColourAttachmentRead = 0x00000080,
		eColourAttachmentWrite = 0x00000100,
		eDepthStencilAttachmentRead = 0x00000200,
		eDepthStencilAttachmentWrite = 0x00000400,
		eTransferRead = 0x00000800,
		eTransferWrite = 0x00001000,
		eHostRead = 0x00002000,
		eHostWrite = 0x00004000,
		eMemoryRead = 0x00008000,
		eMemoryWrite = 0x00010000,
	};
	VkLib_ImplementFlag( AccessFlag )
	/**
	*\brief
	*	Convertit un renderer::AccessFlags en VkAccessFlags.
	*\param[in] flags
	*	Le renderer::AccessFlags.
	*\return
	*	Le VkAccessFlags.
	*/
	VkAccessFlags convert( AccessFlags const & flags );
}
