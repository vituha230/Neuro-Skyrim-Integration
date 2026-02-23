-- Xmake bindings for libneurosdk

add_requires("json.h", "mongoose", "tinycthread")



target("libneurosdk")
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
	
	
	
target("glaze")
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
	
	
