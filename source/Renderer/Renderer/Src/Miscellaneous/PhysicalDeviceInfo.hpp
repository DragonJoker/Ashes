/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_PhysicalDeviceInfo_HPP___
#define ___Renderer_PhysicalDeviceInfo_HPP___
#pragma once

#include "RendererPrerequisites.hpp"

namespace renderer
{
	/**
	*\~english
	*\brief
	*	Some basic informations about a physical device.
	*\~french
	*\brief
	*	Des informations basiques sur un périphérique physique.
	*/
	struct PhysicalDeviceInfo
	{
		/**
		*\~english
		*\brief
		*	The device name.
		*\~french
		*\brief
		*	Le nom du périphérique.
		*/
		std::string name;
		/**
		*\~english
		*\brief
		*	The device type.
		*\~french
		*\brief
		*	Le type du périphérique.
		*/
		std::string type;
		/**
		*\~english
		*\brief
		*	The supported rendering API version.
		*\~french
		*\brief
		*	La version supportée de l'API de rendu.
		*/
		std::string apiVersion;
		/**
		*\~english
		*\brief
		*	The support of non solid fill mode.
		*\~french
		*\brief
		*	Le support des modes non solide.
		*/
		bool fillModeNonSolid{ false };
		/**
		*\~english
		*\brief
		*	The support of wide lines.
		*\~french
		*\brief
		*	Le support des lignes larges.
		*/
		bool wideLines{ false };
	};
}

#endif
