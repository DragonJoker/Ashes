/**
*\file
*	DesktopUtilsPrerequisites.h
*\author
*	Sylvain Doremus
*/
#ifndef ___DesktopUtils_Prerequisites_HPP___
#define ___DesktopUtils_Prerequisites_HPP___
#pragma once

#include <Renderer/Converter.hpp>
#include <Renderer/StringUtils.hpp>

#include <RenderLib/ElementsList.h>
#include <RenderLib/FontLoader.h>
#include <RenderLib/Glyph.h>
#include <RenderLib/Material.h>
#include <RenderLib/Mesh.h>
#include <RenderLib/Object.h>
#include <Renderer/Texture.hpp>

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
		renderer::IVec2 size;
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
		renderer::IVec2 position;
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
