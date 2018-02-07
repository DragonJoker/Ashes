dofile( "../Premake/FindwxWidgets.lua" )

folders = os.matchdirs( "*" )

for i, folder in ipairs( folders ) do
	if ( folder ~= "Assets" ) then
		currentSourceDir = path.join( sourceDir, "Samples", folder )
		currentBinaryDir = path.join( binaryDir, "Samples", folder )

		project( "Sample-" .. folder )

		location( currentBinaryDir )

		if ( folder == "00-SamplesCommon" ) then
			kind( "StaticLib" )
			targetdir( path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", staticLibDir ) )
			includedirs{
				path.join( sourceDir, "Core" ),
				path.join( sourceDir, "Renderer", "Renderer", "Src" ),
				path.join( binaryDir, "Renderer", "Renderer", "Src" ),
				path.join( currentBinaryDir, "Src" ),
				path.join( currentSourceDir, "Src" )
			}
			links{
				"Utils",
				"Renderer"
			}
			postbuildcommands {
				"{COPY} " .. path.join( sourceDir, "Samples", "Assets" ) .. " " .. path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", assetsDir, "Assets" )
			}
		else
			kind( "WindowedApp" )
			targetdir( path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", executableDir ) )
			includedirs{
				path.join( sourceDir, "Core" ),
				path.join( sourceDir, "Renderer", "Renderer", "Src" ),
				path.join( binaryDir, "Renderer", "Renderer", "Src" ),
				path.join( sourceDir, "Samples", "00-SamplesCommon", "Src" ),
				path.join( currentBinaryDir, "Src" ),
				path.join( currentSourceDir, "Src" )
			}
			links{
				"Utils",
				"Renderer",
				"Sample-00-SamplesCommon"
			}
			shadersFolder = path.join( sourceDir, "Samples", folder, "Shaders" )
			if ( os.isdir( shadersFolder ) ) then
				postbuildcommands {
					"{COPY} " .. shadersFolder .. " " .. path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", assetsDir, folder, "Shaders" )
				}
			end
		end

		files{ currentSourceDir .. "/Src/**.hpp",
			currentSourceDir .. "/Src/**.inl",
			currentSourceDir .. "/Src/**.cpp"
		}

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
