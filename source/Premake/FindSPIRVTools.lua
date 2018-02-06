SPIRVToolsIncludeDir = path.join( VulkanSDKDir, "include" )
SPIRVToolsEnabled = false

if ( os.isdir( SPIRVToolsIncludeDir ) )
then
	if ( os.istarget( "Windows" ) )
	then
		SPIRVToolsLibDirDebug = path.join( VulkanSDKDir, "lib", "%{cfg.architecture:gsub('x86_64', 'x64')}", "Debug" )
		SPIRVToolsLibDirRelease = path.join( VulkanSDKDir, "lib", "%{cfg.architecture:gsub('x86_64', 'x64')}", "Release" )
		if ( os.isdir( SPIRVToolsLibDirDebug )
			and os.isdir( SPIRVToolsLibDirRelease )
			and os.isfile( path.join( SPIRVToolsLibDirDebug, "SPIRV-Tools.lib" ) )
			and os.isfile( path.join( SPIRVToolsLibDirRelease, "SPIRV-Tools.lib" ) ) )
		then
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
		end
	else
		SPIRVToolsLibDir = path.join( VulkanSDKDir, "lib" )

		if ( os.isdir( SPIRVToolsLibDir )
			and os.isfile( path.join( SPIRVToolsLibDir, "libSPIRV-Tools.so" ) ) )
		then
			includedirs{
				SPIRVToolsIncludeDir,
			}
			libdirs { SPIRVToolsLibDir }
			links{ "SPIRV-Tools" }
			SPIRVToolsEnabled = true
		end
	end
end
