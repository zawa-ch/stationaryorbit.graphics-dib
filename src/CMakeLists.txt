# stationaryorbit.graphics-dib:/src/CMakeLists.txt
# Copyright 2020-2021 zawa-ch.
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

add_library(stationaryorbit.graphics-dib SHARED
    ${Include_Dir}/stationaryorbit/graphics-dib/dibheaders.hpp
    ${Include_Dir}/stationaryorbit/graphics-dib/dibloader.hpp
    ${Include_Dir}/stationaryorbit/graphics-dib/invaliddibformat.hpp
    dibcorebitmap.cpp
    dibheaders.cpp
    dibinfobitmap.cpp
    dibloader.cpp
    dibpixeldata.cpp
    dibv4bitmap.cpp
    dibv5bitmap.cpp
    invaliddibformat.cpp
    rgbdecoder.cpp
)
