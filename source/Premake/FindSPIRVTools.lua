SPIRVToolsIncludeDir = path.join( VulkanSDKDir, "include" )
SPIRVToolsEnabled = false

if ( os.isdir( SPIRVToolsIncludeDir ) )
then
	if ( os.istarget( "Windows" ) )
	then
		SPIRVToolsLibDirDebug = path.join( VulkanSDKDir, "lib", "%{cfg.architecture:gsub('x86_64', 'x64')}", "Debug" )
		SPIRVToolsLibDirRelease = path.join( VulkanSDKDir, "lib", "%{cfg.architecture:gsub('x86_64', 'x64')}", "Release" )
		includedirs{
			SPIRVToolsIncludeDir,
		}
		filter( "configurations:Debug" )
			libdirs { SPIRVToolsLibDirDebug }
			links{ "SPIRV-Tools" }
		filter( "configurations:Release" )
			libdirs { SPIRVToolsLibDirRelease }
			links{ "SPIRV-Tools" }
		SPIRVToolsEnabled = true
	else
		SPIRVToolsLibDir = path.join( VulkanSDKDir, "lib" )
		includedirs{
			SPIRVToolsIncludeDir,
		}
		libdirs { SPIRVToolsLibDir }
		links{ "SPIRV-Tools" }
		SPIRVToolsEnabled = true
	end
end
