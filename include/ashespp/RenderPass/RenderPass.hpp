/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_RenderPass_HPP___
#define ___AshesPP_RenderPass_HPP___
#pragma once

#include "RenderPassCreateInfo.hpp"

namespace ashes
{
	/**
	*\brief
	*	Describes a render pass (which can contain one or more render subpasses).
	*/
	class RenderPass
		: public VkObject
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] createInfo
		*	The creation informations.
		*/
		RenderPass( Device const & device
			, RenderPassCreateInfo createInfo );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] createInfo
		*	The creation informations.
		*/
		RenderPass( Device const & device
			, std::string const & debugName
			, RenderPassCreateInfo createInfo );
		/**
		*\brief
		*	Destructor.
		*/
		~RenderPass()noexcept;
		/**
		*\brief
		*	Creates a frame buffer compatible with this render pass.
		*\remarks
		*	If the compatibility between wanted views and the render pass' formats
		*	is not possible, a std::runtime_error will be thrown.
		*\param[in] dimensions
		*	The frame buffer's dimensions.
		*\param[in] views
		*	The views for the frame buffer to create.
		*\param[in] layers
		*	The layers count for the frame buffer to create.
		*\return
		*	The created frame buffer.
		*/
		FrameBufferPtr createFrameBuffer( VkFramebufferCreateInfo const & info )const;
		/**
		*\brief
		*	Creates a frame buffer compatible with this render pass.
		*\remarks
		*	If the compatibility between wanted views and the render pass' formats
		*	is not possible, a std::runtime_error will be thrown.
		*\param[in] dimensions
		*	The frame buffer's dimensions.
		*\param[in] views
		*	The views for the frame buffer to create.
		*\param[in] layers
		*	The layers count for the frame buffer to create.
		*\return
		*	The created frame buffer.
		*/
		FrameBufferPtr createFrameBuffer( std::string const & debugName
			, VkFramebufferCreateInfo info )const;
		/**
		*\brief
		*	Creates a frame buffer compatible with this render pass.
		*\remarks
		*	If the compatibility between wanted views and the render pass' formats
		*	is not possible, a std::runtime_error will be thrown.
		*\param[in] dimensions
		*	The frame buffer's dimensions.
		*\param[in] views
		*	The views for the frame buffer to create.
		*\param[in] layers
		*	The layers count for the frame buffer to create.
		*\return
		*	The created frame buffer.
		*/
		FrameBufferPtr createFrameBuffer( VkExtent2D const & dimensions
			, ImageViewCRefArray views
			, uint32_t layers = 1u )const;
		/**
		*\brief
		*	Creates a frame buffer compatible with this render pass.
		*\remarks
		*	If the compatibility between wanted views and the render pass' formats
		*	is not possible, a std::runtime_error will be thrown.
		*\param[in] dimensions
		*	The frame buffer's dimensions.
		*\param[in] views
		*	The views for the frame buffer to create.
		*\param[in] layers
		*	The layers count for the frame buffer to create.
		*\return
		*	The created frame buffer.
		*/
		FrameBufferPtr createFrameBuffer( std::string const & debugName
			, VkExtent2D const & dimensions
			, ImageViewCRefArray views
			, uint32_t layers = 1u )const;
		/**
		*name
		*	Getters.
		*/
		/**@{*/
		size_t getAttachmentCount()const
		{
			return m_createInfo.attachments.size();
		}

		VkAttachmentDescriptionArray const & getAttachments()const
		{
			return m_createInfo.attachments;
		}

		Device const & getDevice()const
		{
			return m_device;
		}

		size_t getSubpassCount()const
		{
			return m_createInfo.subpasses.size();
		}

		SubpassDescriptionArray const & getSubpasses()const
		{
			return m_createInfo.subpasses;
		}
		/**@}*/
		/**
		*\brief
		*	VkRenderPass implicit cast operator.
		*/
		operator VkRenderPass const & ()const
		{
			return m_internal;
		}

	private:
		Device const & m_device;
		RenderPassCreateInfo m_createInfo;
		VkRenderPass m_internal{};
	};
}

#endif
