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
		~Sampler();
		/**
		*\name
		*	Getters.
		*/
		/**@{*/
		inline VkSamplerAddressMode getWrapS()const
		{
			return m_createInfo->addressModeU;
		}

		inline VkSamplerAddressMode getWrapT()const
		{
			return m_createInfo->addressModeV;
		}

		inline VkSamplerAddressMode getWrapR()const
		{
			return m_createInfo->addressModeW;
		}

		inline VkFilter getMinFilter()const
		{
			return m_createInfo->minFilter;
		}

		inline VkFilter getMagFilter()const
		{
			return m_createInfo->magFilter;
		}

		inline VkSamplerMipmapMode getMipFilter()const
		{
			return m_createInfo->mipmapMode;
		}

		inline float getMinLod()const
		{
			return m_createInfo->minLod;
		}

		inline float getMaxLod()const
		{
			return m_createInfo->maxLod;
		}

		inline float getLodBias()const
		{
			return m_createInfo->mipLodBias;
		}

		inline VkBorderColor getBorderColour()const
		{
			return m_createInfo->borderColor;
		}

		inline float getMaxAnisotropy()const
		{
			return m_createInfo->maxAnisotropy;
		}

		inline VkCompareOp getCompareOp()const
		{
			return m_createInfo->compareOp;
		}
		/**@}*/
		/**
		*\brief
		*	VkSampler implicit cast operator.
		*/
		inline operator VkSampler const & ()const
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
