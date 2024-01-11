/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_Sampler_HPP___
#define ___AshesPP_Sampler_HPP___
#pragma once

#include "ashespp/Image/SamplerCreateInfo.hpp"

namespace ashes
{
	/**
	*\brief
	*	Sampler wrapper.
	*/
	class Sampler
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
		Sampler( Device const & device
			, SamplerCreateInfo createInfo );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] createInfo
		*	The creation informations.
		*/
		Sampler( Device const & device
			, VkSampler sampler
			, SamplerCreateInfo createInfo );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] createInfo
		*	The creation informations.
		*/
		Sampler( Device const & device
			, std::string const & debugName
			, SamplerCreateInfo createInfo );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] createInfo
		*	The creation informations.
		*/
		Sampler( Device const & device
			, VkSamplerCreateInfo createInfo );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] createInfo
		*	The creation informations.
		*/
		Sampler( Device const & device
			, std::string const & debugName
			, VkSamplerCreateInfo createInfo );
		/**
		*\brief
		*	Destructor.
		*/
		~Sampler()noexcept;
		/**
		*\name
		*	Getters.
		*/
		/**@{*/
		VkSamplerAddressMode getWrapS()const noexcept
		{
			return m_createInfo->addressModeU;
		}

		VkSamplerAddressMode getWrapT()const noexcept
		{
			return m_createInfo->addressModeV;
		}

		VkSamplerAddressMode getWrapR()const noexcept
		{
			return m_createInfo->addressModeW;
		}

		VkFilter getMinFilter()const noexcept
		{
			return m_createInfo->minFilter;
		}

		VkFilter getMagFilter()const noexcept
		{
			return m_createInfo->magFilter;
		}

		VkSamplerMipmapMode getMipFilter()const noexcept
		{
			return m_createInfo->mipmapMode;
		}

		float getMinLod()const noexcept
		{
			return m_createInfo->minLod;
		}

		float getMaxLod()const noexcept
		{
			return m_createInfo->maxLod;
		}

		float getLodBias()const noexcept
		{
			return m_createInfo->mipLodBias;
		}

		VkBorderColor getBorderColour()const noexcept
		{
			return m_createInfo->borderColor;
		}

		float getMaxAnisotropy()const noexcept
		{
			return m_createInfo->maxAnisotropy;
		}

		VkCompareOp getCompareOp()const noexcept
		{
			return m_createInfo->compareOp;
		}
		/**@}*/
		/**
		*\brief
		*	VkSampler implicit cast operator.
		*/
		operator VkSampler const & ()const noexcept
		{
			return m_internal;
		}

	private:
		Device const & m_device;
		SamplerCreateInfo m_createInfo;
		VkSampler m_ownInternal{};
		VkSampler m_internal{};
	};
}

#endif
