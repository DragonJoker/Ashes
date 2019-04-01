/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_Sampler_HPP___
#define ___AshesPP_Sampler_HPP___
#pragma once

#include "AshesPP/AshesPPPrerequisites.hpp"

namespace ashespp
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
			, VkSamplerCreateInfo const & createInfo );
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
			return m_info.addressModeU;
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
			return m_info.addressModeV;
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
			return m_info.addressModeW;
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
			return m_info.minFilter;
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
			return m_info.magFilter;
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
			return m_info.mipmapMode;
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
			return m_info.minLod;
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
			return m_info.maxLod;
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
			return m_info.mipLodBias;
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
			return m_info.borderColor;
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
			return m_info.maxAnisotropy;
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
			return m_info.compareOp;
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
		VkSamplerCreateInfo m_info;
		VkSampler m_internal{ VK_NULL_HANDLE };
	};
}

#endif
