/**
*\file
*	DesktopUtilsPrerequisites.h
*\author
*	Sylvain Doremus
*/
#ifndef ___DesktopUtils_Prerequisites_HPP___
#define ___DesktopUtils_Prerequisites_HPP___
#pragma once

#include <Utils/Converter.hpp>
#include <Utils/StringUtils.hpp>

#include <ElementsList.h>
#include <FontLoader.h>
#include <Glyph.h>
#include <Material.h>
#include <Mesh.h>
#include <Object.h>
#include <Texture.h>

#include <stdlib.h>
#include <string.h>

#include <chrono>
#include <fstream>
#include <sstream>

#if defined( __GNUG__ )
#	define COMPILER_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#	if COMPILER_VERSION > 40900
#		pragma GCC diagnostic ignored "-Wdeprecated-register"
#	endif
#endif

namespace utils
{
	using Clock = std::chrono::high_resolution_clock;

	class FontLoader;
	class ObjLoader;
	/**
	*\brief
	*	Evènement de type redimensionnement de fenêtre.
	*/
	struct SizeEvent
	{
		//! Les nouvelle dimensions
		utils::IVec2 size;
		//! Dit si la fenêtre est maximisée.
		bool maximized;
		//! Dit si la fenêtre est minimisée.
		bool minimized;
		//! Dit si la fenêtre est restaurée.
		bool restored;
	};
	/**
	*\brief
	*	Evènement lié à la souris.
	*/
	struct MouseEvent
	{
		//! La position actuelle de la souris.
		utils::IVec2 position;
		//! \p true si le bouton gauche de la souris est enfoncé.
		bool ldown;
		//! \p true si le bouton milieu de la souris est enfoncé.
		bool mdown;
		//! \p true si le bouton droit de la souris est enfoncé.
		bool rdown;
		//! Le delta de la molette.
		int delta;
	};
}

#endif
