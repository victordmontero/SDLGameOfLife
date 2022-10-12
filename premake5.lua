-- require("cmake")

workspace("SDLGameOfLife")
	configurations({"Debug","Release"})
	platforms ({"Windows","Mingw32","Linux"})
		
	project("SDLGameOfLife")
		kind("ConsoleApp")
		language("C++")
		targetdir("bin/%{cfg.buildcfg}")

		files({
			"**.h",
			"**.cpp",
			"**.lua"
		})

        filter("configurations:Debug")
            defines({"DEBUG"})
            symbols("On")

        filter("configurations:Release")
            defines({"NDEBUG"})
			optimize("On")
			
		filter("platforms:Windows")
            defines({"WINDOWS"})
			system("windows")
			includedirs({
				"../SDL2-2.0.9/include"
			})
	
			libdirs({
				"../SDL2-2.0.9/lib/x86"
			})
			links({
				"SDL2main",
				"SDL2"
			})
		
		filter("platforms:Mingw32")
            defines({"WINDOWS"})
			system("windows")
			buildoptions{"`sdl2-config --cflags`"}
			linkoptions{"-static","`sdl2-config --static-libs`"}
			

		filter("platforms:Linux")
            defines({"LINUX"})
			system("linux")
			buildoptions{"`sdl2-config --cflags`"}
			linkoptions{"`sdl2-config --libs`"}

