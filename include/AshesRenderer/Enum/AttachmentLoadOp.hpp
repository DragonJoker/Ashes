/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_AttachmentLoadOp_HPP___
#define ___Ashes_AttachmentLoadOp_HPP___
#pragma once

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Gets the name of the given element type.
	*\param[in] value
	*	The element type.
	*\return
	*	The name.
	*\~french
	*\brief
	*	Récupère le nom du type d'élément donné.
	*\param[in] value
	*	Le type d'élément.
	*\return
	*	Le nom.
	*/
	inline std::string getName( VkAttachmentLoadOp value )
	{
		switch ( value )
		{
		case VK_ATTACHMENT_LOAD_OP_LOAD:
			return "load";

		case VK_ATTACHMENT_LOAD_OP_CLEAR:
			return "clear";

		case VK_ATTACHMENT_LOAD_OP_DONT_CARE:
			return "dont_care";

		default:
			assert( false && "Unsupported VkAttachmentLoadOp." );
			throw std::runtime_error{ "Unsupported VkAttachmentLoadOp" };
		}

		return 0;
	}
}

#endif
