/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_PhysicalDeviceType_HPP___
#define ___Renderer_PhysicalDeviceType_HPP___
#pragma once

namespace renderer
{
	/**
	*\brief
	*	Enumération des types de périphérique physique.
	*/
	enum class PhysicalDeviceType
		: uint32_t
	{
		eOther = 0,
		eIntegratedGpu = 1,
		eDiscreteGpu = 2,
		eVirtualGpu = 3,
		eCpu = 4,
	};
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
	inline std::string getName( PhysicalDeviceType value )
	{
		switch ( value )
		{
		case PhysicalDeviceType::eOther:
			return "other";

		case PhysicalDeviceType::eIntegratedGpu:
			return "integrated_gpu";

		case PhysicalDeviceType::eDiscreteGpu:
			return "discrete_gpu";

		case PhysicalDeviceType::eVirtualGpu:
			return "virtual_gpu";

		case PhysicalDeviceType::eCpu:
			return "cpu";

		default:
			assert( false && "Unsupported PhysicalDeviceType." );
			throw std::runtime_error{ "Unsupported PhysicalDeviceType" };
		}

		return std::string{};
	}
}

#endif
