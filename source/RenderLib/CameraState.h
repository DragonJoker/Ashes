/**
*\file
*	CameraState.h
*\author
*	Sylvain Doremus
*/
#ifndef ___DesktopUtils_CameraState_HPP___
#define ___DesktopUtils_CameraState_HPP___
#pragma once

#include <GlLib/Angle.h>
#include "RangedValue.h"

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
		gl::Quaternion rotation()const noexcept;
		/**
		*\brief
		*	Définit les angles d'ouverture minimal et maximal.
		*\param[in] fovyMin, fovyMax
		*	Les bornes.
		*/
		inline void zoomBounds( gl::Angle const & fovyMin
			, gl::Angle const & fovyMax )noexcept
		{
			m_fovy.updateRange( makeRange( fovyMin, fovyMax ) );
		}
		/**
		*\return
		*	Les bornes du zoom.
		*/
		inline Range< gl::Angle > const & zoomBounds()const noexcept
		{
			return m_fovy.range();
		}
		/**
		*\brief
		*	Définit la vitesse de rotation de la caméra.
		*param[in] value
		*	La nouvelle valeur.
		*/
		inline void velocity( gl::Vec2 const & value )noexcept
		{
			m_velocityX = gl::Degrees{ value.x };
			m_velocityY = gl::Degrees{ value.y };
		}
		/**
		*\brief
		*	Définit la vitesse de zoom de la caméra.
		*param[in] value
		*	La nouvelle valeur.
		*/
		inline void zoomVelocity( float value )noexcept
		{
			m_zoomVelocity = gl::Degrees{ value };
		}
		/**
		*\return
		*	La valeur du zoom.
		*/
		inline gl::Angle const & zoom()const noexcept
		{
			return m_fovy.value();
		}

	private:
		//! L'intervalle d'angles de rotation.
		Range< gl::Radians > m_angleRange
		{
			gl::Radians{ float( -gl::PiDiv2 ) },
			gl::Radians{ float( gl::PiDiv2 ) }
		};
		//! La rotation sur l'axe X.
		RangedValue< gl::Radians > m_angleX
		{
			0.0_radians,
			m_angleRange
		};
		//! La rotation sur l'axe Y.
		RangedValue< gl::Radians > m_angleY
		{
			0.0_radians,
			m_angleRange
		};
		//! La rotation sur l'axe X.
		gl::Quaternion m_quatX;
		//! La rotation sur l'axe Y.
		gl::Quaternion m_quatY;
		//! L'intervalle de vitesse de rotation.
		Range< gl::Degrees > m_velocityRange
		{
			-5.0_degrees,
			5.0_degrees
		};
		//! La vitesse de rotation sur l'axe X.
		RangedValue< gl::Degrees > m_velocityX
		{
			0.0_degrees,
			m_velocityRange
		};
		//! La vitesse de rotation sur l'axe Y.
		RangedValue< gl::Degrees > m_velocityY
		{
			0.0_degrees,
			m_velocityRange
		};
		//! La vitesse de zoom.
		RangedValue< gl::Degrees > m_zoomVelocity
		{
			0.0_degrees,
			makeRange( -2.0_degrees, 2.0_degrees )
		};
		//! Le FovY utilisé pour le zoom, borné dans son intervalle.
		RangedValue< gl::Angle > m_fovy
		{
			gl::Angle{ 45.0_degrees },
			makeRange( gl::Angle{ 0.1_degrees }, gl::Angle{ 45.0_degrees } )
		};
	};
}

#endif
