dofile( "../Premake/FindwxWidgets.lua" )

folders = os.matchdirs( "*" )

for i, folder in ipairs( folders ) do
	if ( folder ~= "Assets" ) then
		currentSourceDir = path.join( sourceDir, "Samples", folder )
		currentBinaryDir = path.join( binaryDir, "Samples", folder )

		project( "Sample-" .. folder )

		location( currentBinaryDir )

		if ( folder == "00-Common" ) then
			kind( "StaticLib" )
			targetdir( path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", staticLibDir ) )
			includedirs{
				path.join( sourceDir, "Utils", "Src" ),
				path.join( sourceDir, "Renderer", "Renderer", "Src" ),
				path.join( binaryDir, "Renderer", "Renderer", "Src" ),
				path.join( currentBinaryDir, "Src" ),
				path.join( currentSourceDir, "Src" ),
				path.join( sourceDir, "external", "imgui" )
			}
			links{
				"Utils",
				"Renderer"
			}
			postbuildcommands {
				"{COPY} " .. path.join( sourceDir, "Samples", "Assets" ) .. " " .. path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", assetsDir, "Assets" )
			}
			shadersFolder = path.join( sourceDir, "Samples", folder, "Shaders" )
			if ( os.isdir( shadersFolder ) ) then
				postbuildcommands {
					"{COPY} " .. shadersFolder .. " " .. path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", assetsDir, "Sample-" .. folder, "Shaders" )
				}
			end
			forceincludes( "PrecompiledHeader.hpp" )
			pchheader( "PrecompiledHeader.hpp" )
			pchsource( folder .. "/Src/PrecompiledHeader.cpp" )
			files{ currentSourceDir .. "/Src/**.hpp",
				currentSourceDir .. "/Src/**.inl",
				currentSourceDir .. "/Src/**.cpp",
				path.join( sourceDir, "external", "imgui", "imgui.cpp" ),
				path.join( sourceDir, "external", "imgui", "imgui_draw.cpp" ),
			}

		else
			kind( "WindowedApp" )
			targetdir( path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", executableDir ) )
			includedirs{
				path.join( sourceDir, "Utils", "Src" ),
				path.join( sourceDir, "Renderer", "Renderer", "Src" ),
				path.join( binaryDir, "Renderer", "Renderer", "Src" ),
				path.join( sourceDir, "Samples", "00-Common", "Src" ),
				path.join( currentBinaryDir, "Src" ),
				path.join( currentSourceDir, "Src" ),
				path.join( sourceDir, "external", "imgui" )
			}
			links{
				"Utils",
				"Renderer",
				"Sample-00-Common",
				binaryLinks
			}
			shadersFolder = path.join( sourceDir, "Samples", folder, "Shaders" )
			if ( os.isdir( shadersFolder ) ) then
				postbuildcommands {
					"{COPY} " .. shadersFolder .. " " .. path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", assetsDir, folder, "Shaders" )
				}
			end
			forceincludes( "PrecompiledHeader.hpp" )
			pchheader( "PrecompiledHeader.hpp" )
			pchsource( folder .. "/Src/PrecompiledHeader.cpp" )
			files{ currentSourceDir .. "/Src/**.hpp",
				currentSourceDir .. "/Src/**.inl",
				currentSourceDir .. "/Src/**.cpp"
			}

		end

		vpaths{ ["Header Files"] = "**.hpp" }
		vpaths{ ["Header Files"] = "**.inl" }
		vpaths{ ["Source Files"] = "**.cpp" }

		dofile( "../Premake/FindAssimp.lua" )

		filter( "configurations:Debug" )
			wx_config{
				Unicode="yes",
				Version="3.1",
				Libs="core,aui",
				Static="yes",
				Debug="yes"
			}
		filter( "configurations:Release" )
			wx_config{
				Unicode="yes",
				Version="3.1",
				Libs="core,aui",
				Static="yes"
			}
		filter {}
	end
end
