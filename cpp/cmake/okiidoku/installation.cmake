# SPDX-FileCopyrightText: 2020 David Fong
# SPDX-License-Identifier: GPL-3.0-or-later
# cspell:words CMAKEDIR
include_guard(DIRECTORY)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

if(NOT DEFINED OKIIDOKU_INSTALL_CMAKEDIR)
	set(OKIIDOKU_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/okiidoku")
endif()

function(okiidoku_install_target target)
	install(
		TARGETS ${target}
		EXPORT okiidoku_installation_targets
		# RUNTIME_DEPENDENCIES # currently there are none to bundle.
		${ARGN}
		RUNTIME  COMPONENT okiidoku_runtime
		LIBRARY  COMPONENT okiidoku_runtime
		NAMELINK_COMPONENT okiidoku_development
		ARCHIVE  COMPONENT okiidoku_development
		INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
	)
endfunction()

okiidoku_install_target(okiidoku_compile_options_public)
okiidoku_install_target(okiidoku)
okiidoku_install_target(okiidoku_cli OPTIONAL)

install(FILES ../README.md TYPE DOC COMPONENT okiidoku_about)
# install(FILES ../.reuse/dep5 TYPE DOC COMPONENT okiidoku_about RENAME copying) # hm probably not right. need to make a custom one for the installation
install(DIRECTORY ../LICENSES TYPE DOC COMPONENT okiidoku_about)
# TODO copy the LICENSES folder to doc?

if(BUILD_SHARED_LIBS)
	set(_okiidoku_lib_type_name shared)
else()
	set(_okiidoku_lib_type_name static)
endif()

# regular installation import support:
install(
	EXPORT okiidoku_installation_targets
	NAMESPACE "${PROJECT_NAME}::"
	DESTINATION "${OKIIDOKU_INSTALL_CMAKEDIR}"
	FILE "okiidoku-${_okiidoku_lib_type_name}-targets.cmake"
	COMPONENT okiidoku_development
)
# also support importing from this repo's build-tree (no installation):
# https://cmake.org/cmake/help/latest/guide/importing-exporting/index.html#exporting-targets-from-the-build-tree
export(
	EXPORT okiidoku_installation_targets
	NAMESPACE okiidoku::
	FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/okiidoku-targets.cmake"
)


write_basic_package_version_file(
	okiidoku-config-version.cmake
	COMPATIBILITY "${OKIIDOKU_VERSION_COMPATIBILITY}"
)

# support for `find_package`. seems complicated to set up and I'm not sure anyone will want this. I don't.
# https://cmake.org/cmake/help/latest/guide/importing-exporting/index.html#creating-relocatable-packages
# install(FILES
# 	# "${CMAKE_CURRENT_BINARY_DIR}/okiidoku-config.cmake" # TODO
# 	"${CMAKE_CURRENT_BINARY_DIR}/okiidoku-config-version.cmake"
# 	DESTINATION "${OKIIDOKU_INSTALL_CMAKEDIR}"
# 	COMPONENT okiidoku_development
# )