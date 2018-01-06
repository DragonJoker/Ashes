/**
*\file
*	CameraState.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_CameraState_HPP___
#define ___RenderLib_CameraState_HPP___
#pragma once

#include <Utils/Angle.hpp>
#include <Utils/Quaternion.hpp>
#include <Utils/RangedValue.hpp>

namespace render
{
	/**
	*\brief
	*	Classe de gestion des déplacements de la caméra.
	*/
	class CameraState
	{
	public:
		/**
		*\brief
		*	Remet les vitesses et angles à 0.
		*/
		void reset();
		/**
		*\brief
		*	Met à jour l'angle et le zoom en fonction des vitesses.
		*	
		*/
		void update();
		/**
		*\return
		*	La rotation voulue pour la caméra.
		*/
		utils::Quaternion rotation()const noexcept;
		/**
		*\brief
		*	Définit les angles d'ouverture minimal et maximal.
		*\param[in] fovyMin, fovyMax
		*	Les bornes.
		*/
		inline void zoomBounds( utils::Angle const & fovyMin
			, utils::Angle const & fovyMax )noexcept
		{
			m_fovy.updateRange( makeRange( fovyMin, fovyMax ) );
		}
		/**
		*\return
		*	Les bornes du zoom.
		*/
		inline utils::Range< utils::Angle > const & zoomBounds()const noexcept
		{
			return m_fovy.range();
		}
		/**
		*\brief
		*	Définit la vitesse de rotation de la caméra.
		*param[in] value
		*	La nouvelle valeur.
		*/
		inline void velocity( utils::Vec2 const & value )noexcept
		{
			m_velocityX = utils::Degrees{ value.x };
			m_velocityY = utils::Degrees{ value.y };
		}
		/**
		*\brief
		*	Définit la vitesse de zoom de la caméra.
		*param[in] value
		*	La nouvelle valeur.
		*/
		inline void zoomVelocity( float value )noexcept
		{
			m_zoomVelocity = utils::Degrees{ value };
		}
		/**
		*\return
		*	La valeur du zoom.
		*/
		inline utils::Angle const & zoom()const noexcept
		{
			return m_fovy.value();
		}

	private:
		//! L'intervalle d'angles de rotation.
		utils::Range< utils::Radians > m_angleRange
		{
			utils::Radians{ float( -utils::PiDiv2 ) },
			utils::Radians{ float( utils::PiDiv2 ) }
		};
		//! La rotation sur l'axe X.
		utils::RangedValue< utils::Radians > m_angleX
		{
			0.0_radians,
			m_angleRange
		};
		//! La rotation sur l'axe Y.
		utils::RangedValue< utils::Radians > m_angleY
		{
			0.0_radians,
			m_angleRange
		};
		//! La rotation sur l'axe X.
		utils::Quaternion m_quatX;
		//! La rotation sur l'axe Y.
		utils::Quaternion m_quatY;
		//! L'intervalle de vitesse de rotation.
		utils::Range< utils::Degrees > m_velocityRange
		{
			-5.0_degrees,
			5.0_degrees
		};
		//! La vitesse de rotation sur l'axe X.
		utils::RangedValue< utils::Degrees > m_velocityX
		{
			0.0_degrees,
			m_velocityRange
		};
		//! La vitesse de rotation sur l'axe Y.
		utils::RangedValue< utils::Degrees > m_velocityY
		{
			0.0_degrees,
			m_velocityRange
		};
		//! La vitesse de zoom.
		utils::RangedValue< utils::Degrees > m_zoomVelocity
		{
			0.0_degrees,
			utils::makeRange( -2.0_degrees, 2.0_degrees )
		};
		//! Le FovY utilisé pour le zoom, borné dans son intervalle.
		utils::RangedValue< utils::Angle > m_fovy
		{
			utils::Angle{ 45.0_degrees },
			utils::makeRange( utils::Angle{ 0.1_degrees }, utils::Angle{ 45.0_degrees } )
		};
	};
}

#endif
