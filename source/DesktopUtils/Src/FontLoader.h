/**
*\file
*	FontLoader.h
*\author
*	Sylvain Doremus
*/
#ifndef ___DesktopUtils_FontLoader_HPP___
#define ___DesktopUtils_FontLoader_HPP___
#pragma once

#include <RenderLib/FontLoader.h>

struct  FT_LibraryRec_;
typedef FT_LibraryRec_ * FT_Library;
struct  FT_FaceRec_;
typedef FT_FaceRec_ *  FT_Face;

namespace utils
{
	/**
	*\brief
	*	Classe chargée de charger une police depuis un fichier.
	*/
	class FontLoader
		: public render::FontLoader
	{
	public:
		/**
		*\brief
		*	Constructeur, charge le fichier de la police en RAM.
		*\param[in] pathFile
		*	Le chemin d'accès au fichier.
		*/
		FontLoader( std::string const & pathFile );
		/**
		*\brief
		*	Destructeur, décharge le fichier.
		*/
		~FontLoader();
		/**
		*\brief
		*	Initialise le loader pour la taille de police donnée.
		*\param[in] height
		*	La hauteur de police.
		*/
		void initialise( uint32_t height )override;
		/**
		*\brief
		*	Charge la glyphe associée au caractère donné.
		*\param[in] c
		*	Le caractère.
		*\return
		*	La glyphe chargée.
		*/
		render::Glyph loadGlyph( char c )const override;

	private:
		void doCleanup();

	private:
		FT_Library m_library{ nullptr };
		FT_Face m_face{ nullptr };
		std::string m_pathFile;
	};
}

#endif
