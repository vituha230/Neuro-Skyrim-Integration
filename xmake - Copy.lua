
--------------------------------------------------------------------------------
-- LIBS ------------------------------------------------------------------------
--------------------------------------------------------------------------------
add_requires("json.h", "mongoose", "tinycthread")

target("lib/libneurosdk")
    set_warnings("all")
    set_kind("static")
    set_languages("c17")
    add_defines(
        "NEUROSDK_BUILD_STATIC_LIB",
        "BUILD_WITH_BUILDTOOLS=1",
        "LIB_VERSION=\"0.1.1\"",
        "_CRT_SECURE_NO_WARNINGS",
        "_CRT_RAND_S"
    )
    add_packages("json.h", "mongoose", "tinycthread")

    add_files("libneurosdk/src/neurosdk.c")
    add_includedirs("libneurosdk/include")
    add_includedirs("libneurosdk/include", { public = true })

    on_config(function(target)
        -- Build system is a little cursed, so we replicate script logic :P
        -- Note: this doesn't build on macOS for some reason,
        -- works when I pin mongoose to branch that was used on last passing CI
        import("devel.git")

        -- target:scriptdir() is necessary with my approach
        local sourcedir = path.join(target:scriptdir(), "libneurosdk")
        local commit = git.lastcommit({ repodir = sourcedir })
        target:add("defines", ("LIB_BUILD_HASH=%s"):format(commit))
    end)
	
	
	
target("lib/glaze")
    set_warnings("all")
    set_kind("static")
    set_languages("c++23")
    add_defines(
        "NEUROSDK_BUILD_STATIC_LIB",
        "BUILD_WITH_BUILDTOOLS=1",
        "LIB_VERSION=\"0.1.1\"",
        "_CRT_SECURE_NO_WARNINGS",
        "_CRT_RAND_S"
    )

	add_files("glaze/src/main.cpp")
    add_includedirs("glaze/include")
    add_includedirs("glaze/include", { public = true })

    on_config(function(target)
        -- Build system is a little cursed, so we replicate script logic :P
        -- Note: this doesn't build on macOS for some reason,
        -- works when I pin mongoose to branch that was used on last passing CI
        import("devel.git")

        -- target:scriptdir() is necessary with my approach
        local sourcedir = path.join(target:scriptdir(), "glaze")
        local commit = git.lastcommit({ repodir = sourcedir })
        target:add("defines", ("LIB_BUILD_HASH=%s"):format(commit))
    end)
	
	
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------



-- set minimum xmake version
set_xmakever("2.8.2")

-- includes
includes("lib/commonlibsse")
includes("lib/libneurosdk")
includes("lib/glaze")
includes("lib/xmake.lua")

-- set project
set_project("Neuro-Skyrim-Integration")
set_version("0.0.0")
set_license("GPL-3.0")

-- set defaults
set_languages("c++23")
set_warnings("allextra")

-- add rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- set policies
set_policy("package.requires_lock", true)


local skyrim_path = os.getenv("SKYRIM_PATH")


-- targets
target("Neuro-Skyrim-Integration")
    -- add dependencies to target
    add_deps("commonlibsse")
	add_deps("libneurosdk")
	add_deps("glaze")
	
    -- add commonlibsse plugin
    add_rules("commonlibsse.plugin", {
        name = "Neuro-Skyrim-Integration",
        author = "vituha",
        description = "Skyrim integration for Neuro-Sama"
    })

    -- add src files
    add_files("src/**.cpp") 
    add_headerfiles("src/**.h", "src/**.hpp")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")

	on_install(function (target)
        local target_file = target:targetfile()
        local plugin_folder = path.join(skyrim_path, "Data/SKSE/Plugins")

        os.mkdir(plugin_folder)
        os.cp(target_file, plugin_folder)
        os.cp(
            path.join(
                path.directory(target_file),
                path.basename(target_file) .. ".pdb" -- Evil hack #2
            ),
            plugin_folder
        )

        cprint("${bright green}Installed plugin to " .. plugin_folder)
    end)
	
	
	after_build(function (target)
	
		local target_file = target:targetfile()
        local plugin_folder = path.join(skyrim_path, "Data/SKSE/Plugins")

        os.mkdir(plugin_folder)
        os.cp(target_file, plugin_folder)
        os.cp(
            path.join(
                path.directory(target_file),
                path.basename(target_file) .. ".pdb" -- Evil hack #2
            ),
            plugin_folder
        )

        cprint("${bright green}Installed plugin to " .. plugin_folder)
		
    end)
	
	
	
	
	