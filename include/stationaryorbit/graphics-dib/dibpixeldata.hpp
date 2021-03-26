//	stationaryorbit/graphics-dib/dibpixeldata
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
#ifndef __stationaryorbit_graphics_dib_dibpixeldata__
#define __stationaryorbit_graphics_dib_dibpixeldata__
#include "stationaryorbit/core.bitoperation.hpp"
#include "stationaryorbit/graphics-core.color.hpp"
#include "dibheaders.hpp"
namespace zawa_ch::StationaryOrbit::Graphics::DIB
{
	///	Windowsビットマップ画像のピクセルデータを表します。
	template<DIBBitDepth Depth>
	struct DIBPixelData final
	{
	private:
		static constexpr size_t data_length = (size_t(Depth) / BitWidth<uint8_t>) + ((size_t(Depth) % BitWidth<uint8_t> != 0)?(1):(0));
	public:
		typedef std::array<uint8_t, data_length> ValueType;
		typedef EndianValueType<ValueType, Endians::little> DataType;
	private:
		DataType _data;
	public:
		DIBPixelData() = default;
		constexpr explicit DIBPixelData(const uint32_t& value) :
		_data
		(
			[](const uint32_t& v)
			{
				auto result = ValueType();
				for (auto i: Range<size_t>(0, data_length).GetStdIterator()) { result[i] = (v >> (i * BitWidth<uint8_t>)) & 0xFF; }
				return result;
			}(value)
		)
		{}

		[[nodiscard]] constexpr const DataType& Data() const { return _data; }
		[[nodiscard]] constexpr ValueType Value() const { return _data.Value(); }

		constexpr explicit operator uint32_t() const
		{
			auto result = uint32_t();
			auto v = _data.Value();
			for (auto i: Range<size_t>(0, data_length).GetStdIterator()) { result |= v[i] << (i * BitWidth<uint8_t>); }
			return result;
		}
	};

	extern template struct DIBPixelData<DIBBitDepth::Bit1>;
	extern template struct DIBPixelData<DIBBitDepth::Bit4>;
	extern template struct DIBPixelData<DIBBitDepth::Bit8>;
	extern template struct DIBPixelData<DIBBitDepth::Bit16>;
	extern template struct DIBPixelData<DIBBitDepth::Bit24>;
	extern template struct DIBPixelData<DIBBitDepth::Bit32>;

	class DIBPixelPerser
	{
		DIBPixelPerser() = delete;
		DIBPixelPerser(const DIBPixelPerser&) = delete;
		DIBPixelPerser(DIBPixelPerser&&) = delete;
		~DIBPixelPerser() = delete;
	public:
		[[nodiscard]] static constexpr RGB8_t ToRGB(const DIBPixelData<DIBBitDepth::Bit16>& data)
		{
			return RGB8_t
			(
				Proportion8_t((uint32_t(data) >> 10) & 0x1F, 0x1F),
				Proportion8_t((uint32_t(data) >> 5) & 0x1F, 0x1F),
				Proportion8_t(uint32_t(data) & 0x1F, 0x1F)
			);
		}
		[[nodiscard]] static constexpr RGB8_t ToRGB(const DIBPixelData<DIBBitDepth::Bit24>& data)
		{
			return RGB8_t
			(
				Proportion8_t((uint32_t(data) >> 16) & 0xFF, 0xFF),
				Proportion8_t((uint32_t(data) >> 8) & 0xFF, 0xFF),
				Proportion8_t(uint32_t(data) & 0xFF, 0xFF)
			);
		}
		[[nodiscard]] static constexpr RGB8_t ToRGB(const DIBPixelData<DIBBitDepth::Bit32>& data)
		{
			return RGB8_t
			(
				Proportion8_t((uint32_t(data) >> 16) & 0xFF, 0xFF),
				Proportion8_t((uint32_t(data) >> 8) & 0xFF, 0xFF),
				Proportion8_t(uint32_t(data) & 0xFF, 0xFF)
			);
		}
		[[nodiscard]] static constexpr DIBPixelData<DIBBitDepth::Bit16> ToPixel16(const RGB8_t& color) { return DIBPixelData<DIBBitDepth::Bit16>( ((color.R().Data().Data() & 0xF8) << 7) | ((color.G().Data().Data() & 0xF8) << 2) | ((color.B().Data().Data() & 0xF8) >> 3) ); }
		[[nodiscard]] static constexpr DIBPixelData<DIBBitDepth::Bit24> ToPixel24(const RGB8_t& color) { return DIBPixelData<DIBBitDepth::Bit24>( (color.R().Data().Data() << 16) | (color.G().Data().Data() << 8) | (color.B().Data().Data()) ); }
		[[nodiscard]] static constexpr DIBPixelData<DIBBitDepth::Bit32> ToPixel32(const RGB8_t& color) { return DIBPixelData<DIBBitDepth::Bit32>( (color.R().Data().Data() << 16) | (color.G().Data().Data() << 8) | (color.B().Data().Data()) ); }
	};
}
#endif // __stationaryorbit_graphics_dib_dibpixeldata__
