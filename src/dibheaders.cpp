//	stationaryorbit.graphics-dib:/dibheaders
//	Copyright 2020 zawa-ch.
//	GPLv3 (or later) license
//
//	This program is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//	See the GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program.
//	If not, see <http://www.gnu.org/licenses/>.
//
#include "stationaryorbit/graphics-dib/dibheaders.hpp"
using namespace zawa_ch::StationaryOrbit::Graphics::DIB;

static_assert(sizeof(RGBTriple_t) == 3 ,"sizeof(RGBTriple_t) が 3 ではありません。");
static_assert(sizeof(RGBQuad_t) == 4 ,"sizeof(RGBQuad_t) が 4 ではありません。");
static_assert(sizeof(CIEXYZ_t) == 12 ,"sizeof(CIEXYZ_t) が 12 ではありません。");
static_assert(sizeof(CIEXYZTriple_t) == 36 ,"sizeof(CIEXYZTriple_t) が 36 ではありません。");
static_assert(sizeof(DIBRGBAColorMask) == 16 ,"sizeof(DIBRGBAColorMask) が 16 ではありません。");
static_assert(sizeof(DIBFileHeader) == 14 ,"sizeof(DIBFileHeader) が 14 ではありません。");
static_assert(sizeof(DIBCoreHeader) == 8 ,"sizeof(DIBCoreHeader) が 8 ではありません。");
static_assert(sizeof(DIBInfoHeader) == 36 ,"sizeof(DIBInfoHeader) が 36 ではありません。");
static_assert(sizeof(DIBRGBColorMask) == 12 ,"sizeof(DIBRGBColorMask) が 12 ではありません。");
static_assert(sizeof(DIBV4ColorSpace) == 52 ,"sizeof(DIBV4ColorSpace) が 52 ではありません。");
static_assert(sizeof(DIBV4Header) == 104 ,"sizeof(DIBV4Header) が 104 ではありません。");
static_assert(sizeof(DIBV5ColorSpace) == 68 ,"sizeof(DIBV5ColorSpace) が 68 ではありません。");
static_assert(sizeof(DIBV5Header) == 120 ,"sizeof(DIBV5Header) が 120 ではありません。");
