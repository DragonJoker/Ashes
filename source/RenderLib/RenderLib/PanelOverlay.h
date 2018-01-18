/**
*\file
*	PanelOverlay.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_PanelOverlay_H___
#define ___RenderLib_PanelOverlay_H___
#pragma once

#include "OverlayCategory.h"

namespace render
{
	/**
	*\brief
	*	Une simple incrustation rectangulaire.
	*/
	class PanelOverlay
		: public Overlay
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*/
		PanelOverlay();
		/**
		*\brief
		*	Destructeur.
		*/
		~PanelOverlay();
		/**
		*\brief
		*	Dessine l'incrustation.
		*\param[in] renderer
		*	Le renderer utilisé pour dessiner cette incrustation.
		*/
		void render( renderer::CommandBuffer const & commandBuffer
			, OverlayRenderer const & renderer )const override;
		/**
		 *\~english
		 *\brief		Retrieves the panel vertex buffer
		 *\return		The buffer
		 *\~french
		 *\brief		Récupère le tampon de sommets du panneau
		 *\return		Le tampon
		 */
		inline Overlay::Quad const & panelVertex()const
		{
			return m_quad;
		}
		/**
		*\return
		*	Définit la taille de l'incrustation.
		*\param[in] size
		*	La nouvelle valeur.
		*/
		inline void size( renderer::IVec2 const & size )noexcept
		{
			m_sizeChanged |= ( size != m_size );
			m_size = size;
			onChanged( *this );
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
		//! Les données du tampon de sommets
		Quad m_quad;
		//!Les UV du panneau.
		renderer::Vec4 m_uv{ 0.0f, 0.0f, 1.0f, 1.0f };
		//! Dit si la taille de l'incrustation a changé.
		bool m_sizeChanged{ true };
	};
}

#endif
