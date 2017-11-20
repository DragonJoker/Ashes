/**
*\file
*	Debug.h
*\author
*	Sylvain Doremus
*/
#ifndef ___Debug_HPP___
#define ___Debug_HPP___
#pragma once

#include "RenderLibPrerequisites.h"

#include <chrono>

namespace render
{
	/**
	*\brief
	*	Affiche des incrustations de débogage.
	*/
	class Debug
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\remarks
		*	Enregistre les incrustations dans la scène donnée.
		*\param[in] enable
		*	Si \p false, désactive les incrustations.
		*\param[in,out] scene
		*	La scène.
		*\param[in] loader
		*	Le loader de police.
		*/
		Debug( bool enable
			, render::Scene & scene
			, render::FontLoader & loader );
		/**
		*\brief
		*	Destructeur.
		*\remarks
		*	Enlève les incrustations de la scène.
		*/
		~Debug();
		/**
		*\brief
		*	Enregistre le temps de début.
		*/
		void start();
		/**
		*\brief
		*	Affiche la différence de temps.
		*/
		void end();
		/**
		*\brief
		*	Met à jour les comptes des billboards et des tampons.
		*\param[in] billboards
		*	Les billboards.
		*\param[in] buffers
		*	Les tampons.
		*/
		void count( BillboardArray const & billboards
			, BillboardList const & buffers );

	private:
		using Clock = std::chrono::high_resolution_clock;
		using TimePoint = Clock::time_point;
		//! Dit si les infromations de débogage sont activées.
		bool const m_enabled{ false };
		//! La scène sur laquelle sont enregistrées les incrustations.
		render::Scene * m_scene{ nullptr };
		//! L'incrustation contenant le temps de rendu d'une frame.
		render::TextOverlayPtr m_time;
		//! L'incrustation contenant le nombre de frame par secondes.
		render::TextOverlayPtr m_fps;
		//! L'incrustation contenant la version d'OpenGL.
		render::TextOverlayPtr m_version;
		//! L'incrustation contenant le nombre de billboards affichés.
		render::TextOverlayPtr m_billboardCount;
		//! L'incrustation contenant le nombre de tampons de billboards utilisés.
		render::TextOverlayPtr m_buffersCount;
		//! L'index de temps de début de frame.
		TimePoint m_startTime{};
		//! La texture de police utilisée par les incrustations.
		render::FontTexturePtr m_fontTexture;
	};
}

#endif
