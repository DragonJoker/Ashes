OSDependentIncludeDir = path.join( VulkanSDKDir, "include" )
OSDependentEnabled = false

if ( os.isdir( OSDependentIncludeDir ) )
then
	if ( os.istarget( "Windows" ) )
	then
		OSDependentLibDirDebug = path.join( VulkanSDKDir, "lib", "%{cfg.architecture:gsub('x86_64', 'x64')}", "Debug" )
		OSDependentLibDirRelease = path.join( VulkanSDKDir, "lib", "%{cfg.architecture:gsub('x86_64', 'x64')}", "Release" )
		includedirs{
			OSDependentIncludeDir,
		}
		filter( "configurations:Debug" )
			libdirs { OSDependentLibDirDebug }
			links{ "OSDependent" }
		filter( "configurations:Release" )
			libdirs { OSDependentLibDirRelease }
			links{ "OSDependent" }
		OSDependentEnabled = true
	else
		OSDependentLibDir = path.join( VulkanSDKDir, "lib" )
		includedirs{
			OSDependentIncludeDir,
		}
		libdirs { OSDependentLibDir }
		links{ "OSDependent" }
		OSDependentEnabled = true
	end
end
