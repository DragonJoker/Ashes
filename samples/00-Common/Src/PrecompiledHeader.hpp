#pragma once

#include "Prerequisites.hpp"

#include <AshesPP/Buffer/Buffer.hpp>
#include <AshesPP/Buffer/PushConstantsBuffer.hpp>
#include <AshesPP/Buffer/StagingBuffer.hpp>
#include <AshesPP/Buffer/UniformBuffer.hpp>
#include <AshesPP/Buffer/VertexBuffer.hpp>
#include <AshesPP/Command/CommandBuffer.hpp>
#include <AshesPP/Command/CommandPool.hpp>
#include <AshesPP/Command/Queue.hpp>
#include <AshesPP/Core/Surface.hpp>
#include <AshesPP/Core/Device.hpp>
#include <AshesPP/Core/PlatformWindowHandle.hpp>
#include <AshesPP/Core/Instance.hpp>
#include <AshesPP/Core/SwapChain.hpp>
#include <AshesPP/Descriptor/DescriptorSet.hpp>
#include <AshesPP/Descriptor/DescriptorSetLayout.hpp>
#include <AshesPP/Descriptor/DescriptorSetPool.hpp>
#include <AshesPP/Image/Sampler.hpp>
#include <AshesPP/Image/Image.hpp>
#include <AshesPP/Image/ImageView.hpp>
#include <AshesPP/Miscellaneous/QueryPool.hpp>
#include <AshesPP/Pipeline/GraphicsPipeline.hpp>
#include <AshesPP/Pipeline/PipelineLayout.hpp>
#include <AshesPP/RenderPass/FrameBuffer.hpp>
#include <AshesPP/RenderPass/RenderPass.hpp>

#include <Utils/StringUtils.hpp>
#include <Utils/Transform.hpp>
#include <Utils/GlslToSpv.hpp>

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <numeric>
#include <sstream>

#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/Importer.hpp>

#pragma warning( disable: 4996 )
#include <wx/app.h>
#include <wx/cmdline.h>
#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/windowptr.h>
#include <wx/wx.h>

#if defined( __WXGTK__ )
#	include <gdk/gdkx.h>
#	include <gtk/gtk.h>
#elif defined( __WXMSW__ )
#	include <wx/msw/private.h>
#endif


#if ASHES_WIN32

#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>

#include <direct.h>
#include <Shlobj.h>
#include <windows.h>

#elif defined( __linux__ )

#include <sys/stat.h>

#include <unistd.h>
#include <dirent.h>
#include <pwd.h>

#endif

#if defined( near )
#	undef near
#	undef far
#endif
