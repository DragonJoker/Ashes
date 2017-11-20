/*
This file belongs to VkLib.
See LICENSE file in root folder
*/
namespace utils
{
	namespace details
	{
		/**
		*\brief
		*	Structure d'aide � l'appel vectorialis� d'une fonction.
		*/
		template< typename R, typename T, typename V >
		struct Caller;
		/**
		*\brief
		*	Structure d'aide � l'appel vectorialis� d'une fonction.
		*/
		template< typename R, typename T, typename V >
		struct CallerConstRef;
		/**
		*\brief
		*	Structure d'aide � l'appel vectorialis� d'une fonction.
		*\remarks
		*	Sp�cialisation pour utils::Vec2T
		*/
		template< typename R, typename T >
		struct Caller< R, T, Vec2T< T > >
		{
			/**
			*\brief
			*	Appelle une fonction sur chaque composante du vecteur donn�.
			*\param[in] func
			*	La fonction.
			*\param[in] vector
			*	Le vecteur.
			*\return
			*	Le vecteur r�sultant des appels.
			*/
			static Vec2T< T > call( R( *func )( T ), Vec2T< T > const & vector )
			{
				return Vec2T< T >
				{
					func( vector.x ),
					func( vector.y )
				};
			}
		};
		/**
		*\brief
		*	Structure d'aide � l'appel vectorialis� d'une fonction.
		*\remarks
		*	Sp�cialisation pour utils::Vec2T
		*/
		template< typename R, typename T >
		struct CallerConstRef< R, T, Vec2T< T > >
		{
			/**
			*\brief
			*	Appelle une fonction sur chaque composante du vecteur donn�.
			*\param[in] func
			*	La fonction.
			*\param[in] vector
			*	Le vecteur.
			*\return
			*	Le vecteur r�sultant des appels.
			*/
			static Vec2T< T > call( R( *func )( T const & ), Vec2T< T > const & vector )
			{
				return Vec2T< T >
				{
					func( vector.x ),
					func( vector.y )
				};
			}
		};
		/**
		*\brief
		*	Structure d'aide � l'appel vectorialis� d'une fonction.
		*\remarks
		*	Sp�cialisation pour utils::Vec3T
		*/
		template< typename R, typename T >
		struct Caller< R, T, Vec3T< T > >
		{
			/**
			*\brief
			*	Appelle une fonction sur chaque composante du vecteur donn�.
			*\param[in] func
			*	La fonction.
			*\param[in] vector
			*	Le vecteur.
			*\return
			*	Le vecteur r�sultant des appels.
			*/
			static Vec3T< T > call( R( *func )( T ), Vec3T< T > const & vector )
			{
				return Vec3T< T >
				{
					func( vector.x ),
					func( vector.y ),
					func( vector.z )
				};
			}
		};
		/**
		*\brief
		*	Structure d'aide � l'appel vectorialis� d'une fonction.
		*\remarks
		*	Sp�cialisation pour utils::Vec3T
		*/
		template< typename R, typename T >
		struct CallerConstRef< R, T, Vec3T< T > >
		{
			/**
			*\brief
			*	Appelle une fonction sur chaque composante du vecteur donn�.
			*\param[in] func
			*	La fonction.
			*\param[in] vector
			*	Le vecteur.
			*\return
			*	Le vecteur r�sultant des appels.
			*/
			static Vec3T< T > call( R( *func )( T const & ), Vec3T< T > const & vector )
			{
				return Vec3T< T >
				{
					func( vector.x ),
					func( vector.y ),
					func( vector.z )
				};
			}
		};
		/**
		*\brief
		*	Structure d'aide � l'appel vectorialis� d'une fonction.
		*\remarks
		*	Sp�cialisation pour utils::Vec4T
		*/
		template< typename R, typename T >
		struct Caller< R, T, Vec4T< T > >
		{
			/**
			*\brief
			*	Appelle une fonction sur chaque composante du vecteur donn�.
			*\param[in] func
			*	La fonction.
			*\param[in] vector
			*	Le vecteur.
			*\return
			*	Le vecteur r�sultant des appels.
			*/
			static Vec4T< T > call( R( *func )( T ), Vec4T< T > const & vector )
			{
				return Vec4T< T >
				{
					func( vector.x ),
					func( vector.y ),
					func( vector.z ),
					func( vector.w )
				};
			}
		};
		/**
		*\brief
		*	Structure d'aide � l'appel vectorialis� d'une fonction.
		*\remarks
		*	Sp�cialisation pour utils::Vec4T
		*/
		template< typename R, typename T >
		struct CallerConstRef< R, T, Vec4T< T > >
		{
			/**
			*\brief
			*	Appelle une fonction sur chaque composante du vecteur donn�.
			*\param[in] func
			*	La fonction.
			*\param[in] vector
			*	Le vecteur.
			*\return
			*	Le vecteur r�sultant des appels.
			*/
			static Vec4T< T > call( R( *func )( T const & ), Vec4T< T > const & vector )
			{
				return Vec4T< T >
				{
					func( vector.x ),
					func( vector.y ),
					func( vector.z ),
					func( vector.w )
				};
			}
		};
	}

	template< typename R, typename T, typename V >
	inline V vectorCall( R( *func )( T ), V const & vector )
	{
		return details::Caller< R, T, V >::call( func, vector );
	}

	template< typename R, typename T, typename V >
	inline V vectorCall( R( *func )( T const & ), V const & vector )
	{
		return details::CallerConstRef< R, T, V >::call( func, vector );
	}
}
