set_project("light_or")
set_version("1.0", {build = "%Y%m%d%H%M"})

set_warnings("all", "everything")
add_cflags("-Werror=implicit-function-declaration -Werror=implicit-int -Werror=incompatible-pointer-types -Werror=return-type -Werror=shadow -Werror=return-local-addr -Werror=uninitialized -Werror=format -Werror=int-conversion")
add_cxxflags("-Werror=implicit-function-declaration -Werror=implicit-int -Werror=incompatible-pointer-types -Werror=return-type -Werror=shadow -Werror=return-local-addr -Werror=uninitialized -Werror=format -Werror=int-conversion")
-- -Werror=sign-compare

set_languages("c11","cxx17")

add_rules("mode.release", "mode.debug","mode.check","mode.profile")
-- set_optimize("aggressive")
add_requires("spdlog")
-- add_requires("cmdline")
add_requires("cli11")
add_requires("nlohmann_json") 
add_requires("openmp")

-- includes("src","examples")
includes("examples")


