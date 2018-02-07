SPIRVIncludeDir = path.join( VulkanSDKDir, "include" )
SPIRVEnabled = false

if ( os.isdir( SPIRVIncludeDir ) )
then
	if ( os.istarget( "Windows" ) )
	then
		SPIRVLibDirDebug = path.join( VulkanSDKDir, "lib", "%{cfg.architecture:gsub('x86_64', 'x64')}", "Debug" )
		SPIRVLibDirRelease = path.join( VulkanSDKDir, "lib", "%{cfg.architecture:gsub('x86_64', 'x64')}", "Release" )
		includedirs{
			SPIRVIncludeDir,
		}
		filter( "configurations:Debug" )
			libdirs { SPIRVLibDirDebug }
			links{ "SPIRV" }
		filter( "configurations:Release" )
			libdirs { SPIRVLibDirRelease }
			links{ "SPIRV" }
		SPIRVEnabled = true
	else
		SPIRVLibDir = path.join( VulkanSDKDir, "lib" )
		includedirs{
			SPIRVIncludeDir,
		}
		libdirs { SPIRVLibDir }
		links{ "SPIRV" }
		SPIRVEnabled = true
	end
end
