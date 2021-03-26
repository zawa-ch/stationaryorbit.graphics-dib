//	stationaryorbit/graphics-dib/invaliddibformat
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
#ifndef __stationaryorbit_graphics_dib_invaliddibformatexception_
#define __stationaryorbit_graphics_dib_invaliddibformatexception_
#include "stationaryorbit/core.exception.hpp"
namespace zawa_ch::StationaryOrbit::Graphics::DIB
{
	///	Windowsビットマップとしてのアクセスができないデータに対してパースを行おうとした際にスローされる例外。
	class InvalidDIBFormatException
		: public InvalidOperationException
	{
	public:
		explicit InvalidDIBFormatException(const std::string& what_arg);
		explicit InvalidDIBFormatException(const char* what_arg);
	};
}
#endif // __stationaryorbit_graphics_dib_invaliddibformatexception_
