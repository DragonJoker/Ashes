/**
*\file
*	TextOverlay.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_TextOverlay_HPP___
#define ___RenderLib_TextOverlay_HPP___
#pragma once

#include "FontTexture.h"
#include "OverlayCategory.h"

namespace render
{
	/**
	*\brief
	*	Modes d'espacement des lignes du texte, gère la manière dont les lignes
	*	sont espacées entre elles, dans un TextOverlay.
	*/
	enum class TextLineSpacingMode
	{
		//! La hauteur d'une ligne est sa propre hauteur.
		eOwnHeight,
		//! La hauteur des lignes est la plus grande hauteur parmi elles.
		eMaxLineHeight,
		//! La hauteur des lignes est la plus grande hauteur des caractères de la police.
		eMaxFontHeight,
		GlLib_EnumBounds( eOwnHeight )
	};
	/**
	*\brief
	*	Alignemens horizontaux pour les incrustations texte.
	*/
	enum class HAlign
	{
		//! Aligné à gauche.
		eLeft,
		//! Centré, horizontalement.
		eCenter,
		//! Aligné à droite.
		eRight,
		GlLib_EnumBounds( eLeft )
	};
	/**
	*\brief
	*	Une incrustation contenant du texte.
	*/
	class TextOverlay
		: public Overlay
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*/
		TextOverlay();
		/**
		*\brief
		*	Destructeur.
		*/
		~TextOverlay();
		/**
		*\brief
		*	Dessine l'incrustation.
		*\param[in] renderer
		*	Le renderer utilisé pour dessiner cette incrustation.
		*/
		void render( OverlayRenderer & renderer )const override;
		/**
		*\brief
		*	Définit la texture de police utilisée pour le dessin de
		*	cette incrustation.
		*\param[in] fontTexture
		*	La texture de police.
		*/
		void fontTexture( FontTexture const & fontTexture )noexcept
		{
			m_textChanged |= ( &fontTexture != m_fontTexture );
			m_fontTexture = &fontTexture;
		}
		/**
		*\return
		*	La texture de police utilisée pour le dessin de cette incrustation.
		*/
		FontTexture const & fontTexture()const noexcept
		{
			assert( m_fontTexture );
			return *m_fontTexture;
		}
		/**
		*\return
		*	Le tampon de sommets du panneau.
		*/
		inline QuadArray const & textVertex()const noexcept
		{
			return m_quads;
		}
		/**
		*\return
		*	Le texte de l'incrustation.
		*/
		inline std::string const & caption()const noexcept
		{
			return m_currentCaption;
		}
		/**
		*\brief
		*	Définit le texte de l'incrustation.
		*\param[in] value
		*	La nouvelle valeur.
		*/
		inline void caption( std::string const & value )
		{
			m_currentCaption = value;
			m_textChanged = true;
		}
		/**
		*\return
		*	L'alignement horizontal.
		*/
		inline HAlign align()const noexcept
		{
			return m_hAlign;
		}
		/**
		*\brief
		*	Définit l'alignement horizontal.
		*\param[in] value
		*	La nouvelle valeur.
		*/
		inline void align( HAlign value )noexcept
		{
			m_textChanged |= m_hAlign != value;
			m_hAlign = value;
		}
		/**
		*\return
		*	Le mode d'espacement des lignes.
		*/
		inline TextLineSpacingMode lineSpacingMode()const noexcept
		{
			return m_lineSpacingMode;
		}
		/**
		*\brief
		*	Définit le mode d'espacement des lignes.
		*\param[in] value
		*	La nouvelle valeur.
		*/
		inline void lineSpacingMode( TextLineSpacingMode value )noexcept
		{
			m_textChanged |= m_lineSpacingMode != value;
			m_lineSpacingMode = value;
		}

	private:
		/**
		*\brief
		*	Met à jour le tampon de sommets.
		*/
		void doUpdate()override;

	private:
		/**
		*\brief
		*	Un caractère, avec ses dimensions et sa position relative.
		*/
		struct DisplayableChar
		{
			//! La position du caractère, relative à sa ligne.
			gl::IVec2 m_position;
			//! Les dimensions du caractère.
			gl::IVec2 m_size;
			//! Le caractère à afficher.
			Glyph const & m_glyph;
			/**
			*\brief
			*	Constructeur.
			*\param[in] position
			*	La position du caractère dans sa ligne.
			*\param[in] size
			*	Les dimensions du caractère.
			*\param[in] glyph
			*	La glyphe du caractère.
			*/
			DisplayableChar( gl::IVec2 const & position
				, gl::IVec2 const & size
				, Glyph const & glyph )
				: m_position{ position }
				, m_size{ size }
				, m_glyph{ glyph }
			{
			}
			/**
			*\brief
			*	Constructeur par copie.
			*\param[in,out] rhs
			*	L'objet à copier.
			*/
			DisplayableChar( DisplayableChar const & rhs )
				: m_position{ rhs.m_position }
				, m_size{ rhs.m_size }
				, m_glyph{ rhs.m_glyph }
			{
			}
			/**
			*\brief
			*	Constructeur par déplacement.
			*\param[in,out] rhs
			*	L'objet à déplacer.
			*/
			DisplayableChar( DisplayableChar && rhs )
				: m_position{ std::move( rhs.m_position ) }
				, m_size{ std::move( rhs.m_size ) }
				, m_glyph{ rhs.m_glyph }
			{
			}
			/**
			*\brief
			*	Opérateur d'affectation par copie.
			*\param[in,out] rhs
			*	L'objet à copier.
			*/
			DisplayableChar & operator=( DisplayableChar const & rhs )
			{
				m_position = rhs.m_position;
				m_size = rhs.m_size;
				return *this;
			}
			/**
			*\brief
			*	Opérateur d'affectation par déplacement.
			*\param[in,out] rhs
			*	L'objet à déplacer.
			*/
			DisplayableChar & operator=( DisplayableChar && rhs )
			{
				if ( &rhs != this )
				{
					m_position = std::move( rhs.m_position );
					m_size = std::move( rhs.m_size );
				}

				return *this;
			}
		};
		/**
		\brief
		*	Une ligne de texte, avec ses dimensions et sa position.
		*/
		struct DisplayableLine
		{
			//! La position de la ligne.
			gl::IVec2 m_position;
			//! La largeur de la ligne.
			uint32_t m_width;
			//! La hauteur de la ligne.
			uint32_t m_height;
			//! Les caractères affichables.
			std::vector< DisplayableChar > m_characters;
		};
		//! Un tableau de lignes affichables.
		using DisplayableLineArray = std::vector< DisplayableLine >;
		/**
		*\brief
		*	Calcule les lignes à afficher.
		*\return
		*	Les lignes.
		*/
		DisplayableLineArray doPrepareText();
		/**
		*\brief
		*	Ajoute un mot au tampon de sommets.
		*\param[in] word
		*	Le mot à ajouter.
		*\param[in] wordWidth
		*	La largeur du mot.
		*\param[out] left
		*	La position à gauche.
		*\param[out] line
		*	La ligne.
		*\param[out] lines
		*	Les lignes.
		*/
		void doPrepareWord( std::string const & word
			, int32_t wordWidth
			, int32_t & left
			, DisplayableLine & line
			, DisplayableLineArray & lines );
		/**
		*\brief
		*	Finit la ligne et passe à la ligne suivante.
		*\param[in,out] line
		*	La ligne, dont la position et la largeur sont complétées.
		*\param[out] left
		*	La position à gauche.
		*/
		void doFinishLine( DisplayableLine & line
			, int32_t & left );
		/**
		*\brief
		*	Aligne horizontalement une ligne.
		*\param[in] width
		*	La largeur de l'incrustation.
		*\param[in] line
		*	La ligne.
		*\param[out] lines
		*	Les lignes.
		*/
		void doAlign( DisplayableLine line
			, DisplayableLineArray & lines );
		/**
		*\brief
		*	Met à jour le tampon de sommets.
		*/
		void doUpdateBuffer();

	private:
		//! Le texte courant de l'incrustation.
		std::string m_currentCaption;
		//! Le texte précédent de l'incrustation.
		std::string m_previousCaption;
		//! Les données du tampon de sommets.
		QuadArray m_quads;
		//! Dit si le texte (contenu, mode de découpe, alignements) a changé.
		bool m_textChanged{ true };
		//! Le mode d'espacement des lignes.
		TextLineSpacingMode m_lineSpacingMode{ TextLineSpacingMode::eOwnHeight };
		//! L'alignement horizontal du texte.
		HAlign m_hAlign{ HAlign::eLeft };
		//! La texture de police utilisée pour dessiner cette incrustation.
		FontTexture const * m_fontTexture{ nullptr };
	};
}

#endif
