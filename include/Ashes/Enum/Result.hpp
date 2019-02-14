/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_Result_HPP___
#define ___Ashes_Result_HPP___
#pragma once

namespace ashes
{
	/**
	*\~french
	*\brief
	*	Résultats d'opérations.
	*\~english
	*\brief
	*	Operation results.
	*/
	enum class Result
		: int32_t
	{
		eSuccess = 0,
		eNotReady = 1,
		eTimeout = 2,
		eEventSet = 3,
		eEventReset = 4,
		eIncomplete = 5,
		eErrorOutOfHostMemory = -1,
		eErrorOutOfDeviceMemory = -2,
		eErrorInitializationFailed = -3,
		eErrorDeviceLost = -4,
		eErrorMemoryMapFailed = -5,
		eErrorLayerNotPresent = -6,
		eErrorExtensionNotPresent = -7,
		eErrorFeatureNotPresent = -8,
		eErrorIncompatibleDriver = -9,
		eErrorTooManyObjects = -10,
		eErrorFormatNotSupported = -11,
		eErrorSurfaceLost = -1000000000,
		eErrorNativeWindowInUse = -1000000001,
		eSubOptimal = 1000001003,
		eErrorOutOfDate = -1000001004,
		eErrorIncompatibleDisplay = -1000003001,
		eErrorValidationFailed = -1000011001,
		eErrorInvalidShader = -1000012000,
		eErrorRenderer = -1000012001,
		eBeginRange = eErrorFormatNotSupported,
		eEndRange = eIncomplete,
		eRangeSize = ( eIncomplete - eErrorFormatNotSupported + 1 ),
		eMax = 0x7FFFFFFF
	};
	/**
	*\~english
	*\brief
	*	Gets the name of the given element type.
	*\param[in] value
	*	The element type.
	*\return
	*	The name.
	*\~french
	*\brief
	*	Récupère le nom du type d'élément donné.
	*\param[in] value
	*	Le type d'élément.
	*\return
	*	Le nom.
	*/
	inline std::string getName( Result value )
	{
		switch ( value )
		{
		case Result::eSuccess:
			return "Success";

		case Result::eNotReady:
			return "A fence or query has not yet completed.";

		case Result::eTimeout:
			return "A wait operation has not completed in the specified time.";

		case Result::eEventSet:
			return "An event is signaled.";

		case Result::eEventReset:
			return "An event is unsignaled.";

		case Result::eIncomplete:
			return "A return array was too small for the result.";

		case Result::eErrorOutOfHostMemory:
			return "A host memory allocation has failed.";

		case Result::eErrorOutOfDeviceMemory:
			return "A device memory allocation has failed.";

		case Result::eErrorInitializationFailed:
			return "Initialization of an object could not be completed for implementation-specific reasons.";

		case Result::eErrorDeviceLost:
			return "The logical or physical device has been lost.";

		case Result::eErrorMemoryMapFailed:
			return "Mapping of a memory object has failed.";

		case Result::eErrorLayerNotPresent:
			return "A requested layer is not present or could not be loaded.";

		case Result::eErrorExtensionNotPresent:
			return "A requested extension is not supported.";

		case Result::eErrorFeatureNotPresent:
			return "A requested feature is not supported.";

		case Result::eErrorIncompatibleDriver:
			return "The requested version of the API is not supported by the driver or is otherwise incompatible for implementation-specific reasons.";

		case Result::eErrorTooManyObjects:
			return "Too many objects of the type have already been created.";

		case Result::eErrorFormatNotSupported:
			return "A requested format is not supported on this device.";

		case Result::eErrorSurfaceLost:
			return "A surface is no longer available.";

		case Result::eErrorNativeWindowInUse:
			return "The requested window is already connected to a surface, or to some other API.";

		case Result::eSubOptimal:
			return "An image became available, and the swapchain no longer matches the surface properties exactly, but can still be used to present to the surface successfully.";

		case Result::eErrorOutOfDate:
			return "A surface has changed in such a way that it is no longer compatible with the swapchain, and further presentation requests using the swapchain will fail.";

		case Result::eErrorIncompatibleDisplay:
			return "The display used by a swapchain does not use the same presentable image layout, or is incompatible in a way that prevents sharing an image.";

		case Result::eErrorValidationFailed:
			return "Validation failed.";

		case Result::eErrorInvalidShader:
			return "One or more shaders failed to compile or link.";

		default:
			return "Unknowns error." ;
		}

		return 0;
	}
}

#endif
