/**
*\file
*	RenderableContainer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_RenderableContainer_HPP___
#define ___RenderLib_RenderableContainer_HPP___
#pragma once

#include "SceneRenderer.h"

#include <functional>

namespace render
{
	/**
	*\brief
	*	Un conteneur d'objets pouvant être affichés.
	*/
	class RenderableContainer
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*/
		RenderableContainer();
		/**
		*\brief
		*	Destructeur.
		*/
		~RenderableContainer();
		/**
		*\return
		*	La liste d'objets.
		*/
		inline ObjectArray const & objects()const noexcept
		{
			return m_objects;
		}
		/**
		*\return
		*	La liste d'objets.
		*/
		inline ObjectArray & objects()noexcept
		{
			return m_objects;
		}
		/**
		*\return
		*	La liste de billboards.
		*/
		inline BillboardArray const & billboards()const noexcept
		{
			return m_billboards;
		}
		/**
		*\return
		*	La liste de billboards.
		*/
		inline BillboardArray & billboards()noexcept
		{
			return m_billboards;
		}
		/**
		*\return
		*	La liste de lignes.
		*/
		inline PolyLineArray const & lines()const noexcept
		{
			return m_lines;
		}
		/**
		*\return
		*	La liste de lignes.
		*/
		inline PolyLineArray & lines()noexcept
		{
			return m_lines;
		}
		/**
		*\return
		*	La liste d'objets dessinés.
		*/
		inline RenderSubmeshArray const & renderObjects()const noexcept
		{
			return m_renderObjects;
		}
		/**
		*\return
		*	La liste de billboards dessinés.
		*/
		inline RenderBillboardArray const & renderBillboards()const noexcept
		{
			return m_renderBillboards;
		}

	protected:
		/**
		*\brief
		*	Vide le conteneur.
		*/
		void doCleanup();
		/**
		*\brief
		*	Dessine les objets de la scène, à travers la vue de la caméra.
		*\param[in] camera
		*	La caméra.
		*\param[in] zoomScale
		*	L'échelle calculée par rapport au zoom.
		*/
		void doDraw( Camera const & camera
			, float zoomScale )const;
		/**
		*\brief
		*	Ajoute un objet à la scène.
		*\param[in] object
		*	L'objet à ajouter.
		*/
		void doAdd( ObjectPtr object );
		/**
		*\brief
		*	Supprime un objet de la scène.
		*\param[in] object
		*	L'objet à supprimer.
		*/
		void doRemove( ObjectPtr object );
		/**
		*\brief
		*	Ajoute un billboard à la scène.
		*\param[in] billboard
		*	Le billboard à ajouter.
		*/
		void doAdd( BillboardPtr billboard );
		/**
		*\brief
		*	Supprime un billboard de la scène.
		*\param[in] billboard
		*	Le billboard à supprimer.
		*/
		void doRemove( BillboardPtr billboard );
		/**
		*\brief
		*	Ajoute une liste de lignes à la scène.
		*\param[in] lines
		*	La liste de lignes à ajouter.
		*/
		void doAdd( PolyLinePtr lines );
		/**
		*\brief
		*	Supprime une liste de lignes de la scène.
		*\param[in] lines
		*	La liste de lignes à supprimer.
		*/
		void doRemove( PolyLinePtr lines );

	private:
		//! Le renderer.
		SceneRenderer m_renderer;
		//! Les objets complexes à dessiner.
		ObjectArray m_objects;
		//! Les billboards à dessiner.
		BillboardArray m_billboards;
		//! Les polylignes à dessiner.
		PolyLineArray m_lines;
		//! Les instances de sous-maillages à dessiner, avec leur matériau.
		RenderSubmeshArray m_renderObjects;
		//! Les instances de billboards à dessiner.
		RenderBillboardArray m_renderBillboards;
	};
}

#endif
