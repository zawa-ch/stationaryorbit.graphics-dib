# stationaryorbit.graphics-dib:/cmake/Findstationaryorbit.graphics-dib.cmake
# Copyright 2021 zawa-ch.
# GPLv3 (or later) license
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
#
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.
# If not, see <http://www.gnu.org/licenses/>.
#

# stationaryorbit.core の探索
if(NOT stationaryorbit.core_FOUND)
    find_path(stationaryorbit_core cmake/Findstationaryorbit.core.cmake
        PATHS
            dependencies
            ..
        PATH_SUFFIXES
            stationaryorbit-core
    )
    if(EXISTS "${stationaryorbit_core}/cmake/Findstationaryorbit.core.cmake")
        message("found Findstationaryorbit.core.cmake in ${stationaryorbit_core}/cmake")
        set(stationaryorbit.core_ROOT "${stationaryorbit_core}")
        set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${stationaryorbit_core}/cmake")
    endif()
    find_package(stationaryorbit.core REQUIRED)
    set(stationaryorbit_core_MODULE_NAME stationaryorbit.core::stationaryorbit.core)
endif()

# stationaryorbit.graphics-core の探索
if(NOT stationaryorbit.graphics-core_FOUND)
    find_path(stationaryorbit_graphics_core cmake/Findstationaryorbit.graphics-core.cmake
        PATHS
            dependencies
            ..
        PATH_SUFFIXES
            stationaryorbit-core
    )
    if(EXISTS "${stationaryorbit_graphics_core}/cmake/Findstationaryorbit.graphics-core.cmake")
        message("found Findstationaryorbit.graphics-core.cmake in ${stationaryorbit_graphics_core}/cmake")
        set(stationaryorbit.graphics-core_ROOT "${stationaryorbit_graphics_core}")
        set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${stationaryorbit_graphics_core}/cmake")
    endif()
    find_package(stationaryorbit.graphics-core REQUIRED)
    set(stationaryorbit_graphics_core_MODULE_NAME stationaryorbit.graphics-core::stationaryorbit.graphics-core)
endif()

# インクルードディレクトリの探索
find_path(stationaryorbit_graphics_dib_INCLUDE_DIR stationaryorbit/graphics-dib
    PATHS
        ENV stationaryorbit.graphics-dib_ROOT
        ENV stationaryorbit.graphics-dib_INCLUDE_DIR
        ${stationaryorbit.graphics-dib_ROOT}
    PATH_SUFFIXES
        include/
    NO_PACKAGE_ROOT_PATH
)
set(stationaryorbit.graphics-dib_INCLUDE_DIRS
    ${stationaryorbit.graphics-core_INCLUDE_DIRS}
    ${stationaryorbit_graphics_dib_INCLUDE_DIR}
)
unset(stationaryorbit_graphics_dib_INCLUDE_DIR)
message("include dir: ${stationaryorbit.graphics-dib_INCLUDE_DIRS}")
mark_as_advanced(stationaryorbit.graphics-dib_INCLUDE_DIRS)

list(APPEND stationaryorbit.graphics-dib_NAMES stationaryorbit.graphics-dib libstationaryorbit.graphics-dib)

# ライブラリパスの探索
find_library(stationaryorbit_graphics_dib_LIBRARY
    NAMES
        stationaryorbit.graphics-dib
        libstationaryorbit.graphics-dib
    NAMES_PER_DIR
    PATHS
        ENV stationaryorbit.graphics-dib_ROOT
        ENV stationaryorbit.graphics-dib_LIB_DIR
        ${stationaryorbit.graphics-dib_ROOT}
    PATH_SUFFIXES
        lib/
        build/src/
    NO_PACKAGE_ROOT_PATH
)
set(stationaryorbit.graphics-dib_LIBRARIES ${stationaryorbit_graphics_dib_LIBRARY} )
unset(stationaryorbit_graphics_dib_LIBRARY)
message("library dir: ${stationaryorbit.graphics-dib_LIBRARIES}")
mark_as_advanced(stationaryorbit.graphics-dib_LIBRARIES)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(stationaryorbit.graphics-dib
    REQIRED_VARS
        stationaryorbit.graphics-dib_LIBRARIES
        stationaryorbit.graphics-dib_INCLUDE_DIRS
)

if(stationaryorbit.graphics-dib_FOUND AND NOT TARGET stationaryorbit.graphics-dib::stationaryorbit.graphics-dib)
    add_library(stationaryorbit.graphics-dib::stationaryorbit.graphics-dib UNKNOWN IMPORTED)
    set_target_properties(stationaryorbit.graphics-dib::stationaryorbit.graphics-dib
        PROPERTIES
            IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
            IMPORTED_LOCATION "${stationaryorbit.graphics-dib_LIBRARIES}"
            INTERFACE_INCLUDE_DIRECTORIES "${stationaryorbit.graphics-dib_INCLUDE_DIRS}"
    )
    set_property(TARGET stationaryorbit.graphics-dib::stationaryorbit.graphics-dib PROPERTY INTERFACE_LINK_LIBRARIES
        stationaryorbit.core::stationaryorbit.core
        stationaryorbit.graphics-core::stationaryorbit.graphics-core
    )
endif()
