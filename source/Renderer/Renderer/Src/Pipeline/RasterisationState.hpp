/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_RasterisationState_HPP___
#define ___Renderer_RasterisationState_HPP___
#pragma once

#include "RendererPrerequisites.hpp"

namespace renderer
{
	/**
	*\brief
	*	Etat de rastérisation.
	*/
	class RasterisationState
	{
	public:
		RasterisationState( RasterisationState const & rhs );
		RasterisationState( RasterisationState && rhs ) = default;
		RasterisationState & operator=( RasterisationState const & rhs );
		RasterisationState & operator=( RasterisationState && rhs ) = default;
		~RasterisationState() = default;
		/**
		*\brief
		*	Constructeur.
		*\param[in] flags
		*	Les indicateurs de l'état.
		*\param[in] depthClampEnable
		*	Le statut d'activation du bornage en profondeur.
		*\param[in] rasteriserDiscardEnable
		*	Le statut de désactivation du rastériseur.
		*\param[in] polygonMode
		*	Le mode d'affichage des polygones.
		*\param[in] cullMode
		*	Le mode de culling.
		*\param[in] frontFace
		*	L'orientation des faces qui seront considérées comme faisant face à la caméra.
		*\param[in] depthBiasEnable
		*	Le statut d'activation du biais de profondeur.
		*\param[in] depthBiasConstantFactor
		*	Le facteur du biais de profondeur.
		*\param[in] depthBiasClamp
		*	La borne maximale du biais de profondeur.
		*\param[in] depthBiasSlopeFactor
		*	Le facteur de biais de profondeur, par rapport à la pente.
		*\param[in] lineWidth
		*	La largeur des lignes.
		*/
		RasterisationState( float lineWidth
			, RasterisationStateFlags flags = 0
			, bool depthClampEnable = false
			, bool rasteriserDiscardEnable = false
			, PolygonMode polygonMode = PolygonMode::eFill
			, CullModeFlags cullMode = CullModeFlag::eBack
			, FrontFace frontFace = FrontFace::eCounterClockwise
			, bool depthBiasEnable = false
			, float depthBiasConstantFactor = 0.0f
			, float depthBiasClamp = 0.0f
			, float depthBiasSlopeFactor = 0.0f );
		/**
		*\brief
		*	Constructeur.
		*\param[in] flags
		*	Les indicateurs de l'état.
		*\param[in] depthClampEnable
		*	Le statut d'activation du bornage en profondeur.
		*\param[in] rasteriserDiscardEnable
		*	Le statut de désactivation du rastériseur.
		*\param[in] polygonMode
		*	Le mode d'affichage des polygones.
		*\param[in] cullMode
		*	Le mode de culling.
		*\param[in] frontFace
		*	L'orientation des faces qui seront considérées comme faisant face à la caméra.
		*\param[in] depthBiasEnable
		*	Le statut d'activation du biais de profondeur.
		*\param[in] depthBiasConstantFactor
		*	Le facteur du biais de profondeur.
		*\param[in] depthBiasClamp
		*	La borne maximale du biais de profondeur.
		*\param[in] depthBiasSlopeFactor
		*	Le facteur de biais de profondeur, par rapport à la pente.
		*/
		RasterisationState( RasterisationStateFlags flags = 0
			, bool depthClampEnable = false
			, bool rasteriserDiscardEnable = false
			, PolygonMode polygonMode = PolygonMode::eFill
			, CullModeFlags cullMode = CullModeFlag::eBack
			, FrontFace frontFace = FrontFace::eCounterClockwise
			, bool depthBiasEnable = false
			, float depthBiasConstantFactor = 0.0f
			, float depthBiasClamp = 0.0f
			, float depthBiasSlopeFactor = 0.0f );
		/**
		*\~english
		*name
		*	Getters.
		*\~french
		*name
		*	Accesseurs.
		*/
		/**@{*/
		inline uint16_t getHash()const
		{
			return m_hash;
		}

		inline RasterisationStateFlags getFlags()const
		{
			return m_flags;
		}

		inline bool isDepthClampEnabled()const
		{
			return m_depthClampEnable;
		}

		inline bool isRasteriserDiscardEnabled()const
		{
			return m_rasteriserDiscardEnable;
		}

		inline PolygonMode getPolygonMode()const
		{
			return m_polygonMode;
		}

		inline CullModeFlags getCullMode()const
		{
			return m_cullMode;
		}

		inline FrontFace getFrontFace()const
		{
			return m_frontFace;
		}

		inline bool isDepthBiasEnabled()const
		{
			return m_depthBiasEnable;
		}

		inline float getDepthBiasConstantFactor()const
		{
			return m_depthBiasConstantFactor;
		}

		inline float getDepthBiasClamp()const
		{
			return m_depthBiasClamp;
		}

		inline float getDepthBiasSlopeFactor()const
		{
			return m_depthBiasSlopeFactor;
		}

		inline bool hasLineWidth()const
		{
			return m_lineWidth != nullptr;
		}

		inline float getLineWidth()const
		{
			assert( m_lineWidth );
			return *m_lineWidth;
		}
		/**@}*/

	private:
		RasterisationStateFlags m_flags;
		bool m_depthClampEnable;
		bool m_rasteriserDiscardEnable;
		PolygonMode m_polygonMode;
		CullModeFlags m_cullMode;
		FrontFace m_frontFace;
		bool m_depthBiasEnable;
		uint16_t m_hash;
		float m_depthBiasConstantFactor;
		float m_depthBiasClamp;
		float m_depthBiasSlopeFactor;
		std::unique_ptr< float > m_lineWidth;
		friend bool operator==( RasterisationState const & lhs, RasterisationState const & rhs );
	};

	inline bool operator==( RasterisationState const & lhs, RasterisationState const & rhs )
	{
		return lhs.m_hash == rhs.m_hash
			&& lhs.m_depthBiasConstantFactor == rhs.m_depthBiasConstantFactor
			&& lhs.m_depthBiasClamp == rhs.m_depthBiasClamp
			&& lhs.m_depthBiasSlopeFactor == rhs.m_depthBiasSlopeFactor
			&& (lhs.m_lineWidth == rhs.m_lineWidth
				&& ( lhs.m_lineWidth
					? *lhs.m_lineWidth == *rhs.m_lineWidth
					: true ) );
	}

	inline bool operator!=( RasterisationState const & lhs, RasterisationState const & rhs )
	{
		return !( lhs == rhs );
	}
}

#endif
