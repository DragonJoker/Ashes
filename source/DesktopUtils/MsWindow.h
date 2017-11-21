/**
*\file
*	MsWindow.h
*\author
*	Sylvain Doremus
*/
#ifndef ___DesktopUtils_MsWindow_HPP___
#define ___DesktopUtils_MsWindow_HPP___
#pragma once

#include "DesktopUtilsPrerequisites.h"

struct HINSTANCE__;
typedef struct HINSTANCE__ *HINSTANCE;
struct HWND__;
typedef struct HWND__ *HWND;
struct HDC__;
typedef struct HDC__ *HDC;
struct HGLRC__;
typedef struct HGLRC__ *HGLRC;

#if defined( _WIN64 )
typedef int64_t longptr_t;
#else
typedef long longptr_t;
#endif

namespace utils
{
	/**
	*\brief
	*	Fenêtre Win32.
	*/
	class MsWindow
	{
	public:
		MsWindow();
		/**
		*\brief
		*	Crée la fenêtre.
		*/
		void create( HINSTANCE instance
			, std::string const & className
			, int iconResourceID
			, int smallIconResourceID
			, std::string const & title );
		/**
		*\return
		*	Les dimensions de la fenêtre.
		*/
		inline renderer::IVec2 size()const
		{
			return m_size;
		}

	private:
		/**
		*\brief
		*	Création de la fenêtre.
		*/
		virtual void onCreate() = 0;
		/**
		*\brief
		*	Destruction de la fenêtre.
		*/
		virtual void onDestroy() = 0;
		/**
		*\brief
		*	Mise à jour du contenu.
		*/
		virtual void onDraw() = 0;
		/**
		*\brief
		*	Minimisation de la fenêtre.
		*/
		virtual void onMinimise() = 0;
		/**
		*\brief
		*	Réaffichage de la fenêtre après une minimisation.
		*/
		virtual void onRestore( renderer::IVec2 const & event ) = 0;
		/**
		*\brief
		*	Redimensionnement de la fenêtre.
		*/
		virtual void onResize( renderer::IVec2 const & event ) = 0;
		/**
		*\brief
		*	D�placement de la souris.
		*/
		virtual void onMouseMove( MouseEvent const & event ) = 0;
		/**
		*\brief
		*	Bouton gauche de la souris cliqué.
		*/
		virtual void onLButtonDown( MouseEvent const & event ) = 0;
		/**
		*\brief
		*	Bouton gauche de la souris cliqué.
		*/
		virtual void onLButtonClick( MouseEvent const & event ) = 0;
		/**
		*\brief
		*	Bouton gauche de la souris double cliqué.
		*/
		virtual void onLButtonDblClick( MouseEvent const & event ) = 0;
		/**
		*\brief
		*	Un évènement de molette.
		*/
		virtual void onMouseWheel( MouseEvent const & event ) = 0;

	protected:
		/**
		*\brief
		*	Echange les tampons du double buffering.
		*/
		void doSwapBuffers();

	private:
		void doCreate();
		void doDestroy();
		void doMinimise();
		void doRestore( renderer::IVec2 const & size );
		bool doPrepareDC( HDC hdc );
		HGLRC doCreateContext( HDC hdc );
		void doRegisterClass( HINSTANCE hInstance
			, std::string const & className
			, int iconResourceID
			, int smallIconResourceID );
		longptr_t processMessage( uint32_t message
			, size_t wParam
			, longptr_t lParam );
		static longptr_t CALLBACK WndProc( HWND hWnd
			, uint32_t message
			, size_t wParam
			, longptr_t lParam );
		static void doRegisterInstance( MsWindow * window );
		static void doUnregisterInstance( MsWindow * window );
		static MsWindow * doGetInstance( HWND hWnd );

	private:
		HWND m_hwnd{ nullptr };
		HDC m_hdc{ nullptr };
		HGLRC m_context{ nullptr };
		renderer::IVec2 m_size;
		int m_timer{ -1 };
		static std::map< HWND, MsWindow * > sm_instances;
		bool m_minimised{ false };
	};
}

#endif
