/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#if _WIN32
#	include "Platform/GlMswWindow.hpp"
#elif __linux__
#	include "Platform/GlX11Window.hpp"
#elif __APPLE__
#	include "Platform/CGlWindow.hpp"
#endif
