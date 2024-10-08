-- require("cmake")

workspace("SDLGameOfLife")
	configurations({"Debug","Release"})
	platforms ({"Windows","Mingw32","Linux"})
		
	project("SDLGameOfLife")
		kind("ConsoleApp")
		language("C++")
		targetdir("bin/%{cfg.buildcfg}")

		files({
			"main.cpp",
			"**.lua"
		})

		includedirs({
      "depend/sdl/build/include",
      "depend/sdl/include"
    })

    libdirs {
      "depend/sdl/build"
    }

    filter("configurations:Debug")
     defines({"DEBUG"})
     symbols("On")
     targetsuffix("d")

    filter("configurations:Release")
      defines({"NDEBUG"})
      optimize("On")
			
		filter("platforms:Windows")
      defines({"WINDOWS"})
			system("windows")
			--includedirs({
			--	"../SDL2-2.0.9/include"
			--})
	
			--libdirs({
			--	"../SDL2-2.0.9/lib/x86"
			--})
			links({
        "SDL2lib",
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

      links {
        "SDL2lib",
				"SDL2main%{cfg.linktarget.suffix}",
        "SDL2%{cfg.linktarget.suffix}"
      }

      libdirs {
        "depend/sdl/build"
      }

  project "SDL2lib"
     kind "Makefile"
     objdir()
   
     location("depend/sdl")
     --includedirs{"./include"}
     --targetname "SDL2"
		 targetdir "%{prj.location}/build"

     cleancommands {
	     "{RMDIR} %{prj.location}/build/"
     }
   
    buildcommands {
      "cmake -DCMAKE_BUILD_TYPE=%{cfg.buildcfg} %{prj.location} -B %{cfg.targetdir}",
	    "cmake --build %{cfg.targetdir} --config %{cfg.buildcfg} --parallel $(nproc --ignore=1)"

    }
   
    rebuildcommands {
	    "{RMDIR} %{prj.location}/build/",
      "cmake -DCMAKE_BUILD_TYPE=%{cfg.buildcfg} %{prj.location} -B %{cfg.targetdir}",
	    "cmake --build %{cfg.targetdir} --config %{cfg.buildcfg} --parallel $(nproc --ignore=1)"
 
    }
