/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_Sampler_HPP___
#define ___AshesPP_Sampler_HPP___
#pragma once

#include "AshesPP/Image/SamplerCreateInfo.hpp"

namespace ashes
{
	/**
	*\brief
	*	Un échantillonneur.
	*/
	class Sampler
	{
	public:
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] createInfo
		*	Les informations de création.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical connection to the GPU.
		*\param[in] createInfo
		*	The creation informations.
		*/
		Sampler( Device const & device
			, SamplerCreateInfo createInfo );
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		~Sampler();
		/**
		*\~english
		*\return
		*	The wrap mode on S axis.
		*\~french
		*\return
		*	Le mode de wrap sur l'axe S.
		*/
		inline VkSamplerAddressMode getWrapS()const
		{
			return static_cast< VkSamplerCreateInfo const & >( m_createInfo ).addressModeU;
		}
		/**
		*\~english
		*\return
		*	The wrap mode on T axis.
		*\~french
		*\return
		*	Le mode de wrap sur l'axe T.
		*/
		inline VkSamplerAddressMode getWrapT()const
		{
			return static_cast< VkSamplerCreateInfo const & >( m_createInfo ).addressModeV;
		}
		/**
		*\~english
		*\return
		*	The wrap mode on R axis.
		*\~french
		*\return
		*	Le mode de wrap sur l'axe R.
		*/
		inline VkSamplerAddressMode getWrapR()const
		{
			return static_cast< VkSamplerCreateInfo const & >( m_createInfo ).addressModeW;
		}
		/**
		*\~english
		*\return
		*	The minification filter.
		*\~french
		*\return
		*	Le filtre de minification.
		*/
		inline VkFilter getMinFilter()const
		{
			return static_cast< VkSamplerCreateInfo const & >( m_createInfo ).minFilter;
		}
		/**
		*\~english
		*\return
		*	The magnification filter.
		*\~french
		*\return
		*	Le filtre de magnification.
		*/
		inline VkFilter getMagFilter()const
		{
			return static_cast< VkSamplerCreateInfo const & >( m_createInfo ).magFilter;
		}
		/**
		*\~english
		*\return
		*	The mipmap filter.
		*\~french
		*\return
		*	Le filtre de mipmap.
		*/
		inline VkSamplerMipmapMode getMipFilter()const
		{
			return static_cast< VkSamplerCreateInfo const & >( m_createInfo ).mipmapMode;
		}
		/**
		*\~english
		*\return
		*	The minimal LOD level.
		*\~french
		*\return
		*	Le niveau minimal pour le LOD.
		*/
		inline float getMinLod()const
		{
			return static_cast< VkSamplerCreateInfo const & >( m_createInfo ).minLod;
		}
		/**
		*\~english
		*\return
		*	The maximal LOD level.
		*\~french
		*\return
		*	Le niveau maximal pour le LOD.
		*/
		inline float getMaxLod()const
		{
			return static_cast< VkSamplerCreateInfo const & >( m_createInfo ).maxLod;
		}
		/**
		*\~english
		*\return
		*	The LOD bias.
		*\~french
		*\return
		*	Le décalage LOD.
		*/
		inline float getLodBias()const
		{
			return static_cast< VkSamplerCreateInfo const & >( m_createInfo ).mipLodBias;
		}
		/**
		*\~english
		*\return
		*	The border colour.
		*\~french
		*\brief
		*\return
		*	La couleur de la bordure.
		*/
		inline VkBorderColor getBorderColour()const
		{
			return static_cast< VkSamplerCreateInfo const & >( m_createInfo ).borderColor;
		}
		/**
		*\~english
		*\return
		*	The maximal anisotropy filtering value.
		*\~french
		*\return
		*	La valeur maximale de filtrage anisotropique.
		*/
		inline float getMaxAnisotropy()const
		{
			return static_cast< VkSamplerCreateInfo const & >( m_createInfo ).maxAnisotropy;
		}
		/**
		*\~english
		*\return
		*	The comparison operator.
		*\~french
		*\brief
		*\return
		*	L'opérateur de comparaison.
		*/
		inline VkCompareOp getCompareOp()const
		{
			return static_cast< VkSamplerCreateInfo const & >( m_createInfo ).compareOp;
		}
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkSampler.
		*\~english
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
		VkSampler m_internal{ VK_NULL_HANDLE };
	};
}

#endif
