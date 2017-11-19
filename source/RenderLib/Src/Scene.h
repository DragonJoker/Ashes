/**
*\file
*	Scene.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_Scene_HPP___
#define ___RenderLib_Scene_HPP___
#pragma once

#define DEBUG_PICKING 0

#include "Camera.h"
#include "CameraState.h"
#include "ElementsList.h"
#include "Material.h"
#include "Mesh.h"
#include "Picking.h"
#include "Range.h"
#include "RenderableContainer.h"
#include "TextOverlay.h"
#include "Texture.h"
#include "UberShader.h"
#include "Viewport.h"

#include <GlLib/GlDebug.h>
#include <GlLib/GlUniform.h>
#include <GlLib/GlShaderProgram.h>

#include <functional>

namespace render
{
	/**
	*\brief
	*	Une scène, contenant la caméra et les objets affichés.
	*/
	class Scene
		: public RenderableContainer
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] size
		*	La taille initiale de la zone de rendu.
		*/
		Scene( utils::IVec2 const & size );
		/**
		*\brief
		*	Destructeur.
		*/
		~Scene();
		/**
		*\brief
		*	Met à jour les objets et la caméra.
		*/
		void update();
		/**
		*\brief
		*	Met à jour les incrustations.
		*/
		void updateOverlays();
		/**
		*\brief
		*	Dessine les objets de la scène, à travers la vue de la caméra.
		*/
		void draw()const;
		/**
		*\brief
		*	Redimensionne le viewport de la caméra.
		*\param[in] size
		*	Les nouvelles dimensions.
		*/
		void resize( utils::IVec2 const & size )noexcept;
		/**
		*\brief
		*	Ajoute un objet à la scène.
		*\param[in] object
		*	L'objet à ajouter.
		*/
		void add( ObjectPtr object );
		/**
		*\brief
		*	Supprime un objet de la scène.
		*\param[in] object
		*	L'objet à supprimer.
		*/
		void remove( ObjectPtr object );
		/**
		*\brief
		*	Ajoute un billboard à la scène.
		*\param[in] billboard
		*	Le billboard à ajouter.
		*/
		void add( BillboardPtr billboard );
		/**
		*\brief
		*	Supprime un billboard de la scène.
		*\param[in] billboard
		*	Le billboard à supprimer.
		*/
		void remove( BillboardPtr billboard );
		/**
		*\brief
		*	Ajoute une liste de lignes à la scène.
		*\param[in] lines
		*	La liste de lignes à ajouter.
		*/
		void add( PolyLinePtr lines );
		/**
		*\brief
		*	Supprime une liste de lignes de la scène.
		*\param[in] lines
		*	La liste de lignes à supprimer.
		*/
		void remove( PolyLinePtr lines );
		/**
		*\brief
		*	Ajoute un tampon de billboard à la liste.
		*\param[in] name
		*	Le nom du tampon.
		*\param[in] buffer
		*	Le tampon.
		*/
		void addBillboardBuffer( std::string const & name
			, BillboardBufferPtr buffer );
		/**
		*\return
		*	La caméra.
		*/
		inline Camera const & camera()const noexcept
		{
			return m_camera;
		}
		/**
		*\return
		*	La caméra.
		*/
		inline Camera & camera()noexcept
		{
			return m_camera;
		}
		/**
		*\return
		*	La liste de matériaux.
		*/
		inline MaterialList const & materials()const noexcept
		{
			return m_materials;
		}
		/**
		*\return
		*	La liste de matériaux.
		*/
		inline MaterialList & materials()noexcept
		{
			return m_materials;
		}
		/**
		*\return
		*	La liste de maillages.
		*/
		inline MeshList const & meshes()const noexcept
		{
			return m_meshes;
		}
		/**
		*\return
		*	La liste de maillages.
		*/
		inline MeshList & meshes()noexcept
		{
			return m_meshes;
		}
		/**
		*\return
		*	La liste de textures.
		*/
		inline TextureList const & textures()const noexcept
		{
			return m_textures;
		}
		/**
		*\return
		*	La liste de textures.
		*/
		inline TextureList & textures()noexcept
		{
			return m_textures;
		}
		/**
		*\return
		*	La liste d'incrustations.
		*/
		inline OverlayList const & overlays()const noexcept
		{
			return m_overlays;
		}
		/**
		*\return
		*	La liste d'incrustations.
		*/
		inline OverlayList & overlays()noexcept
		{
			return m_overlays;
		}
		/**
		*\return
		*	Le viewport de la scène.
		*/
		inline Viewport const & viewport()const noexcept
		{
			return m_camera.viewport();
		}
		/**
		*\return
		*	Le viewport de la scène.
		*/
		inline Viewport & viewport()noexcept
		{
			return m_camera.viewport();
		}
		/**
		*\brief
		*	Définit la couleur de fond de la scène.
		*\param[in] colour
		*	La nouvelle valeur.
		*/
		inline void backgroundColour( utils::RgbaColour const & colour )noexcept
		{
			m_backgroundColour = colour;
		}
		/**
		*\return
		*	La couleur de fond de la scène.
		*/
		inline utils::RgbaColour const & backgroundColour()const noexcept
		{
			return m_backgroundColour;
		}
		/**
		*\return
		*	L'état de la caméra.
		*/
		inline CameraState const & state()const noexcept
		{
			return m_state;
		}
		/**
		*\return
		*	L'état de la caméra.
		*/
		inline CameraState & state()noexcept
		{
			return m_state;
		}
		/**
		*\return
		*	L'intervalle de valeurs pour le seuil d'affichage.
		*/
		inline utils::Range< float > const & thresholdBounds()const noexcept
		{
			return m_threshold.range();
		}
		/**
		*\brief
		*	Définit l'intervalle de valeurs pour le seuil d'affichage.
		*/
		inline void thresholdBounds( float min, float max )noexcept
		{
			m_threshold.updateRange( utils::makeRange( min, max ) );
		}
		/**
		*\return
		*	Le seuil d'affichage courant.
		*/
		inline float threshold()const noexcept
		{
			return m_currentThreshold;
		}
		/**
		*\brief
		*	Définit le seuil d'affichage.
		*\param[in] threshold
		*	La nouvelle valeur.
		*/
		inline void threshold( float threshold )noexcept
		{
			m_currentThreshold = threshold;
		}
		/**
		*\return
		*	Les tampons de billboards.
		*/
		inline BillboardList const & billboardsBuffers()const noexcept
		{
			return m_billboardsBuffers;
		}

	private:
		/**
		*\brief
		*	Gestion du signal de changement d'objet déplaçable.
		*/
		void onMovableChanged( Movable & movable );
		/**
		*\brief
		*	Met à jour les billboards.
		*/
		void doUpdateBillboards();

	private:
		//! L'état de la caméra.
		CameraState m_state;
		//! La caméra.
		Camera m_camera;
		//! Les materiaux
		MaterialList m_materials;
		//! Les tampons de billboards.
		BillboardList m_billboardsBuffers;
		//! Les maillages.
		MeshList m_meshes;
		//! Les textures.
		TextureList m_textures;
		//! Les incrustations.
		OverlayList m_overlays;
		//! La couleur du fond.
		utils::RgbaColour m_backgroundColour;
		//! Le seuil précédent d'affichage des objets.
		float m_previousThreshold{ 0.0f };
		//! Le seuil courant d'affichage des objets.
		float m_currentThreshold{ 0.0f };
		//! Le seuil d'affichage des objets.
		utils::RangedValue< float > m_threshold
		{
			std::numeric_limits< float >::infinity(),
			utils::makeRange( -30.0f, 30.0f )
		};
		//! La liste de nouveaux tampons de billboards.
		std::vector< BillboardBufferPtr > m_newBillboardBuffers;
		//! Les Movable qui ont changé.
		std::vector< Movable * > m_changedMovables;
		//! Les connections aux évènements de Movable changé.
		std::map< Movable *, Connection< OnMovableChanged > > m_onMovableChanged;
		//! Dit si la caméra a changé entre 2 updates.
		bool m_cameraChanged{ true };
	};
}

#endif
