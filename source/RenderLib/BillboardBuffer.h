/**
*\file
*	BillboardBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_BillboardBuffer_HPP___
#define ___RenderLib_BillboardBuffer_HPP___
#pragma once

#include "RenderSignal.h"

#include <array>

namespace render
{
	/**
	*\brief
	*	Les données de base d'un billboard.
	*/
	struct BillboardData
	{
		//! Sa magnitude.
		float magnitude;
		//! Sa position.
		gl::Vec3 center;
		//! Son échelle.
		gl::Vec2 scale;
	};
	//! Un tableau de données de billboard.
	using BillboardDataArray = std::vector< BillboardData >;
	/**
	*\brief
	*	Données d'une liste de billboards texturables.
	*/
	class BillboardBuffer
	{
	public:
		/**
		*\brief
		*	Les données d'un sommet en VRAM.
		*/
		struct Vertex
		{
			//! Ses données.
			BillboardData data;
			//! Ses coordonnées de texture.
			gl::Vec2 texture;
			//! Son ID de billboard.
			float id;
		};
		//! Un quad est composé de 6 sommets, pour l'afficher en tant que triangle.
		using Quad = std::array< Vertex, 6 >;
		/**
		*\brief
		*	Le stockage des billboards visibles.
		*/
		class Storage
		{
		public:
			/**
			*\brief
			*	Mappe en RAM le tampon VRAM.
			*\return
			*	Le pointeur sur le tampon en RAM.
			*/
			virtual Quad * lock() = 0;
			/**
			*\brief
			*	Démappe le tampon de la RAM.
			*/
			virtual void unlock() = 0;
			/**
			*\return
			*	Le tampon GPU contenant les données.
			*/
			inline gl::Buffer< Quad > const & vbo()const
			{
				assert( m_vbo );
				return *m_vbo;
			}

		protected:
			//! Le VBO contenant les données.
			gl::BufferPtr< BillboardBuffer::Quad > m_vbo;
		};
		//! Un pointeur sur le stockage.
		using StoragePtr = std::unique_ptr< Storage >;

	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] scale
		*	Dit si on veut que les billboards soient mis à l'échelle du zoom, c'est à
		*	dire qu'ils ne varieront pas de taille.
		*/
		BillboardBuffer( bool scale );
		/**
		*\brief
		*	Initialise le VBO afin qu'il puisse contenir les données du tampon.
		*/
		void initialise();
		/**
		*\brief
		*	Nettoie le VBO.
		*/
		void cleanup();
		/**
		*\brief
		*	Met à jour le compte des billboards à afficher en fonction du seuil donné.
		*\param[in] threshold
		*	Le seuil d'affichage.
		*/
		void update( float threshold );
		/**
		*\brief
		*	Cache les billboards qui ne sont pas visibles par la caméra donnée.
		*\param[in] camera
		*	La caméra.
		*\param[in] position
		*	La position.
		*\param[in] scale
		*	L'échelle du zoom.
		*/
		void cull( Camera const & camera
			, gl::Vec3 const & position
			, float scale );
		/**
		*\brief
		*	Retire un point de la liste.
		*\param[in] index
		*	L'index du point.
		*/
		void remove( uint32_t index );
		/**
		*\brief
		*	Ajoute un point à la liste.
		*\param[in] data
		*	Les données du point.
		*/
		void add( BillboardData const & data );
		/**
		*\brief
		*	Ajoute une liste de points à la liste.
		*\param[in] datas
		*	La liste de points.
		*/
		void add( BillboardDataArray const & datas );
		/**
		*\brief
		*	Récupère un point de la liste.
		*\param[in] index
		*	L'index du point.
		*\return
		*	Le point.
		*/
		BillboardData const & at( uint32_t index )const;
		/**
		*\brief
		*	Définit la position d'un point de la liste.
		*\param[in] index
		*	L'index du point.
		*\param[in] data
		*	Le point.
		*/
		void at( uint32_t index, BillboardData const & data );
		/**
		*\return
		*	Le nombre de billboards à afficher (prend en compte le seuil).
		*/
		inline uint32_t count()const
		{
			return m_count;
		}
		/**
		*\brief
		*	Récupère un point de la liste.
		*\param[in] index
		*	L'index du point.
		*\return
		*	Le point.
		*/
		inline BillboardData const & operator[]( uint32_t index )const
		{
			return at( index );
		}
		/**
		*\return
		*	Le tampon GPU contenant les sommets.
		*/
		inline gl::Buffer< Quad > const & vbo()const
		{
			assert( m_visible );
			return m_visible->vbo();
		}

	public:
		//! La notification de tampon changé.
		OnBillboardBufferChanged onBillboardBufferChanged;

	private:
		//! Le compte des objets visibles avant le culling.
		uint32_t m_unculled{ 0u };
		//! Le nombre de billboards à afficher (en fonction du seuil et du culling).
		uint32_t m_count{ 0u };
		//! La liste des sommets.
		std::vector< Quad > m_buffer;
		//! Le stockage des sommets visibles.
		StoragePtr m_visible;
		//! Dit si on veut que les billboards soient mis à l'échelle du zoom.
		bool m_scale{ false };
	};
}

#endif
