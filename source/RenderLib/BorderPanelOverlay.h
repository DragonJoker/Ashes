/**
*\file
*	BorderPanelOverlay.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_BorderPanelOverlay_H___
#define ___RenderLib_BorderPanelOverlay_H___
#pragma once

#include "OverlayCategory.h"

namespace render
{
	/**
	*\brief
	*	Modes de découpe du texte, gère la manière dont le texte est dessiné
	*	quand il dépasse la largeur de l'incrustation.
	*/
	enum class BorderPosition
	{
		//! La bordure est à l'intérieur de l'incrustation
		eInternal,
		//! La bordure est à moitié à l'intérieur, à moitié à l'extérieur de l'incrustation.
		eMiddle,
		//! La bordure est à l'extérieur de l'incrustation.
		eExternal,
	};
	/**
	*\brief
	*	Une incrustation rectangulaire avec une bordure.
	*remarks
	*	Utilise un matériau spécifique pour la bordure.
	*/
	class BorderPanelOverlay
		: public Overlay
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*/
		BorderPanelOverlay();
		/**
		*\brief
		*	Destructeur.
		*/
		~BorderPanelOverlay();
		/**
		*\brief
		*	Dessine l'incrustation.
		*\param[in] renderer
		*	Le renderer utilisé pour dessiner cette incrustation.
		*/
		void render( OverlayRenderer & renderer )const override;
		/**
		*\return
		*	Définit la taille de l'incrustation.
		*\param[in] size
		*	La nouvelle valeur.
		*/
		inline void size( gl::IVec2 const & size )noexcept
		{
			m_sizeChanged |= ( size != m_size );
			m_size = size;
		}
		/**
		*\brief
		*	Définit le matériau de la bordure.
		*\param[in] material
		*	La nouvelle valeur.
		*/
		inline void borderMaterial( MaterialPtr material )
		{
			m_borderMaterial = material;
		}
		/**
		*\return
		*	Le matériau de la bordure.
		*/
		inline Material const & borderMaterial()const
		{
			assert( m_borderMaterial );
			return *m_borderMaterial;
		}
		/**
		*\return
		*	Les dimensions de la bordure
		*/
		gl::IVec4 borderSize()const
		{
			return m_borderSize;
		}
		/**
		*\brief
		*	Définit l'épaisseur de la bordure.
		*\param[in] size
		*	La nouvelle valeur
		*/
		inline void borderSize( gl::IVec4 const & size )
		{
			m_borderSize = size;
			m_borderChanged = true;
		}
		/**
		*\return
		*	L'épaisseur du bord gauche.
		*/
		inline int leftBorderSize()const
		{
			return m_borderSize.x;
		}
		/**
		*\brief
		*	Définit l'épaisseur du bord gauche.
		*\param[in] size
		*	La nouvelle valeur
		*/
		inline void leftBorderSize( int size )
		{
			m_borderSize.x = size;
			m_borderChanged = true;
		}
		/**
		*\return
		*	L'épaisseur du bord haut.
		*/
		inline int topBorderSize()const
		{
			return m_borderSize.y;
		}
		/**
		*\brief
		*	Définit l'épaisseur du bord haut.
		*\param[in] size
		*	La nouvelle valeur
		*/
		inline void topBorderSize( int size )
		{
			m_borderSize.y = size;
			m_borderChanged = true;
		}
		/**
		*\return
		*	L'épaisseur du bord droit.
		*/
		inline int rightBorderSize()const
		{
			return m_borderSize.z;
		}
		/**
		*\brief
		*	Définit l'épaisseur du bord droit.
		*\param[in] size
		*	La nouvelle valeur
		*/
		inline void rightBorderSize( int size )
		{
			m_borderSize.z = size;
			m_borderChanged = true;
		}
		/**
		*\return
		*	L'épaisseur du bord bas.
		*/
		inline int bottomBorderSize()const
		{
			return m_borderSize.w;
		}
		/**
		*\brief
		*	Définit l'épaisseur du bord bas.
		*\param[in] size
		*	La nouvelle valeur
		*/
		inline void bottomBorderSize( int size )
		{
			m_borderSize.w = size;
			m_borderChanged = true;
		}
		/**
		*\return
		*	La position de la bordure.
		*/
		BorderPosition borderPosition()const
		{
			return m_borderPosition;
		}
		/**
		*\brief
		*	Définit la position de la bordure.
		*\param[in] position
		*	La nouvelle valeur.
		*/
		void borderPosition( BorderPosition position )
		{
			m_borderPosition = position;
		}
		/**
		*\return
		*	Le tampon de sommets du panneau
		*/
		inline Overlay::QuadArray const & panelVertex()const
		{
			return m_panelQuads;
		}
		/**
		*\return
		*	Le tampon de sommets de la bordure.
		*/
		inline Overlay::QuadArray const & borderVertex()const
		{
			return m_borderQuads;
		}

	public:
		using Overlay::size;

	protected:
		/**
		*\brief
		*	Met à jour le tampon de sommets si nécessaire.
		*/
		void doUpdate()override;
		/**
		*\brief
		*	Met à jour le tampon de sommets.
		*/
		void doUpdateBuffer();

	protected:
		//! Le matériau de la bordure.
		MaterialPtr m_borderMaterial;
		//! La taille de la bordure.
		gl::IVec4 m_borderSize;
		//! Le nom du matériau de la bordure.
		BorderPosition m_borderPosition;
		//! Les données du tampon de sommets.
		QuadArray m_panelQuads;
		//! Les données du tampon de sommets pour la bordure.
		QuadArray m_borderQuads;
		//!Les UV du panneau.
		gl::Vec4 m_uv{ 0.33f, 0.33f, 0.66f, 0.66f };
		//! Les UV de la partie extérieure de la bordure.
		gl::Vec4 m_borderOuterUv{ 0.0f, 0.0f, 1.0f, 1.0f };
		//! Les UV de la partie intérieure de la bordure.
		gl::Vec4 m_borderInnerUv{ 0.33f, 0.33f, 0.66f, 0.66f };
		//! Dit si la bordure a changé, de quelque manière que ce soit.
		bool m_borderChanged{ true };
		//! Dit si la taille de l'incrustation a changé.
		bool m_sizeChanged{ true };
	};
}

#endif
