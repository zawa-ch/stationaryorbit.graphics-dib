//	stationaryorbit/graphics-dib/dibheaders
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
#ifndef __stationaryorbit_graphics_dib_wbmpheaders__
#define __stationaryorbit_graphics_dib_wbmpheaders__
#include <cstdint>
#include "stationaryorbit/core.bitoperation.hpp"
#include "stationaryorbit/graphics-core.color.hpp"
namespace zawa_ch::StationaryOrbit::Graphics::DIB
{
	///	ビットマップの1ピクセルあたりのビット幅。
	enum class DIBBitDepth : uint16_t
	{
		///	ビット幅情報なし。
		///	PNGなどのビット幅可変の場合に用いられます。
		Null = 0,
		Bit1 = 1,
		Bit4 = 4,
		Bit8 = 8,
		Bit16 = 16,
		Bit24 = 24,
		Bit32 = 32,
	};
	///	Windows Bitmap 形式で使用される圧縮形式。
	enum class DIBCompressionMethod : uint32_t
	{
		///	無圧縮RGB。
		RGB,
		///	8ビット/ピクセル 連長圧縮。
		RLE8,
		///	4ビット/ピクセル 連長圧縮。
		RLE4,
		///	ビットフィールド。
		BITFIELDS,
		///	JPEG 圧縮。
		JPEG,
		///	PNG 圧縮。
		PNG,
		///	アルファ付きビットフィールド。
		///	@note
		///	Windoes CE5.0以降で利用可能です。
		ALPHABITFIELDS,
	};
	///	V4ヘッダ・V5ヘッダで使用される色空間保持に関する情報。
	enum class DIBColorSpaceMode
	{
		///	色空間情報はヘッダに記述。
		Header = 0,
		///	sRGBを使用。
		sRGB = 0x73524742,
		Win = 0x57696e20,
		/// プロファイルへのリンクをカラープロファイル領域に記述。
		Link = 0x4c494e4b,
		/// プロファイルの実体をカラープロファイル領域に記述。
		Embeded = 0x4d424544
	};
	///	3バイトで表されるRGB色情報。
	struct RGBTriple_t final
	{
		ChannelValue<Proportion8_t> Blue;
		ChannelValue<Proportion8_t> Green;
		ChannelValue<Proportion8_t> Red;

		RGBTriple_t() = default;
		constexpr explicit RGBTriple_t(const RGB8_t& color) : Blue(color.B()), Green(color.G()), Red(color.R()) {}
		[[nodiscard]] constexpr operator RGB8_t() const { return RGB8_t(Red, Green, Blue); }
	};
	///	4バイトで表されるRGB色情報。
	struct RGBQuad_t final
	{
		ChannelValue<Proportion8_t> Blue;
		ChannelValue<Proportion8_t> Green;
		ChannelValue<Proportion8_t> Red;
		ChannelValue<Proportion8_t> Reserved;

		RGBQuad_t() = default;
		constexpr explicit RGBQuad_t(const RGB8_t& color) : Blue(color.B()), Green(color.G()), Red(color.R()), Reserved() {}
		[[nodiscard]] constexpr explicit operator RGB8_t() const { return RGB8_t(Red, Green, Blue); }
	};
	struct CIEXYZ_t final
	{
		ChannelValue<FixedPoint32q16_t> X;
		ChannelValue<FixedPoint32q16_t> Y;
		ChannelValue<FixedPoint32q16_t> Z;

		[[nodiscard]] constexpr explicit operator XYZColor<FixedPoint32q16_t>() const { return XYZColor<FixedPoint32q16_t>(X, Y, Z); }
	};
	struct CIEXYZTriple_t final
	{
		CIEXYZ_t Red;
		CIEXYZ_t Green;
		CIEXYZ_t Blue;
	};
	struct DIBColorMask final
	{
		BitMask<uint32_t> RedMask;
		BitMask<uint32_t> GreenMask;
		BitMask<uint32_t> BlueMask;
		std::optional<BitMask<uint32_t>> AlphaMask;
	};
	struct DIBRGBColorMask final
	{
		///	赤成分のカラーマスク
		uint32_t RedMask;
		///	緑成分のカラーマスク
		uint32_t GreenMask;
		///	青成分のカラーマスク
		uint32_t BlueMask;

		[[nodiscard]] constexpr operator DIBColorMask() { return DIBColorMask{ BitMask<uint32_t>(RedMask), BitMask<uint32_t>(GreenMask), BitMask<uint32_t>(BlueMask), std::nullopt }; }
	};
	struct DIBRGBAColorMask final
	{
		///	赤成分のカラーマスク
		uint32_t RedMask;
		///	緑成分のカラーマスク
		uint32_t GreenMask;
		///	青成分のカラーマスク
		uint32_t BlueMask;
		///	α成分のカラーマスク
		uint32_t AlphaMask;

		[[nodiscard]] constexpr operator DIBColorMask() { return DIBColorMask{ BitMask<uint32_t>(RedMask), BitMask<uint32_t>(GreenMask), BitMask<uint32_t>(BlueMask), BitMask<uint32_t>(AlphaMask) }; }
	};
	struct DIBFileHeader final
	{
	public:
		///	ファイルタイプ。常に'BM'(0x42, 0x4d)を示します。
		uint8_t FileType[2];
		uint16_t FileSize_L;
		uint16_t FileSize_H;
		int16_t Reserved6;
		int16_t Reserved8;
		uint16_t Offset_L;
		uint16_t Offset_H;

		///	ファイルタイプの識別子。'BM'(0x42, 0x4d)を示します。
		static constexpr uint8_t FileType_Signature[2] = { 'B', 'M' };

		///	ファイルサイズを取得します。
		[[nodiscard]] constexpr int32_t FileSize() const
		{
			auto result = int32_t();
			for (auto i: Range<size_t>(0, sizeof(int32_t)/sizeof(uint16_t)).get_std_iterator()) { ((uint16_t*)&result)[i] = (&FileSize_L)[i]; }
			return result;
		}
		///	ファイルサイズを設定します。
		constexpr void FileSize(int32_t value)
		{
			for (auto i: Range<size_t>(0, sizeof(int32_t)/sizeof(uint16_t)).get_std_iterator()) { (&FileSize_L)[i] = ((uint16_t*)&value)[i]; }
		}
		///	ファイルヘッダの先頭アドレスからビットマップデータの先頭アドレスまでのオフセットを取得します。
		[[nodiscard]] constexpr int32_t Offset() const
		{
			auto result = int32_t();
			for (auto i: Range<size_t>(0, sizeof(int32_t)/sizeof(uint16_t)).get_std_iterator()) { ((uint16_t*)&result)[i] = (&Offset_L)[i]; }
			return result;
		}
		///	ファイルヘッダの先頭アドレスからビットマップデータの先頭アドレスまでのオフセットを設定します。
		constexpr void Offset(int32_t value)
		{
			for (auto i: Range<size_t>(0, sizeof(int32_t)/sizeof(uint16_t)).get_std_iterator()) { (&Offset_L)[i] = ((uint16_t*)&value)[i]; }
		}
		///	BitmapFileHeader構造体の内容を確認し、正しいフォーマットであることをチェックします
		[[nodiscard]] constexpr bool CheckFileHeader() const
		{
			for (size_t i = 0; i < (sizeof(FileType_Signature) / sizeof(uint8_t)); i++) { if (FileType_Signature[i] != FileType[i]) return false; }
			return true;
		}
	};
	struct DIBCoreHeader final
	{
		static const constexpr uint32_t Size = 12;
		///	ビットマップの横幅
		uint16_t Width;
		///	ビットマップの縦幅
		uint16_t Height;
		///	プレーン数
		uint16_t Planes;
		///	1ピクセルあたりのビット数
		DIBBitDepth BitCount;
	};
	struct DIBInfoHeader final
	{
		static const constexpr uint32_t Size = 40;
		///	ビットマップの横幅
		int32_t Width;
		///	ビットマップの縦幅
		int32_t Height;
		///	プレーン数
		uint16_t Planes;
		///	1ピクセルあたりのビット数
		DIBBitDepth BitCount;
		///	圧縮形式
		DIBCompressionMethod Compression;
		///	画像データサイズ (単位はバイト)
		uint32_t SizeImage;
		///	水平方向の解像度 (単位はピクセル/m)
		uint32_t XPelsPerMeter;
		///	垂直方向の解像度 (単位はピクセル/m)
		uint32_t YPelsPerMeter;
		///	使用する色数 ビットマップで実際に使用するカラーパレット内のカラーインデックスの数。
		uint32_t ClrUsed;
		///	重要な色数 ビットマップを表示するために必要なカラーインデックスの数。
		uint32_t ClrImportant;
	};
	struct DIBV4ColorSpace final
	{
		///	色空間 [0(ヘッダ内で定義)]
		DIBColorSpaceMode CSType;
		///	CIEXYZTRIPLE構造体 色空間が0の場合のみ有効
		CIEXYZTriple_t Endpoints;
		///	赤成分のガンマ値 色空間が0の場合のみ有効 16.16の固定小数点数
		FixedPoint32q16_t GammaRed;
		///	緑成分のガンマ値 色空間が0の場合のみ有効 16.16の固定小数点数
		FixedPoint32q16_t GammaGreen;
		///	青成分のガンマ値 色空間が0の場合のみ有効 16.16の固定小数点数
		FixedPoint32q16_t GammaBlue;
	};
	struct DIBV4Header final
	{
		static const constexpr uint32_t Size = 108;
		///	ビットマップの横幅
		int32_t Width;
		///	ビットマップの縦幅
		int32_t Height;
		///	プレーン数
		uint16_t Planes;
		///	1ピクセルあたりのビット数
		DIBBitDepth BitCount;
		///	圧縮形式
		DIBCompressionMethod Compression;
		///	画像データサイズ (単位はバイト)
		uint32_t SizeImage;
		///	水平方向の解像度 (単位はピクセル/m)
		uint32_t XPelsPerMeter;
		///	垂直方向の解像度 (単位はピクセル/m)
		uint32_t YPelsPerMeter;
		///	使用する色数 ビットマップで実際に使用するカラーパレット内のカラーインデックスの数。
		uint32_t ClrUsed;
		///	重要な色数 ビットマップを表示するために必要なカラーインデックスの数。
		uint32_t ClrImportant;
		///	カラーマスク
		DIBRGBAColorMask ColorMask;
		///	色空間
		DIBV4ColorSpace ColorSpace;
	};
	struct DIBV5ColorSpace final
	{
		///	色空間 [0(ヘッダ内で定義), 0x73524742('sRGB'), 0x57696e20('Win '), 0x4c494e4b('LINK'), 0x4d424544('MBED')]
		DIBColorSpaceMode CSType;
		///	CIEXYZTRIPLE構造体 色空間が0の場合のみ有効
		CIEXYZTriple_t Endpoints;
		///	赤成分のガンマ値 色空間が0の場合のみ有効 16.16の固定小数点数
		FixedPoint32q16_t GammaRed;
		///	緑成分のガンマ値 色空間が0の場合のみ有効 16.16の固定小数点数
		FixedPoint32q16_t GammaGreen;
		///	青成分のガンマ値 色空間が0の場合のみ有効 16.16の固定小数点数
		FixedPoint32q16_t GammaBlue;
		///	レンダリングの意図
		uint32_t Intent;
		///	プロファイルデータのオフセット 情報ヘッダの先頭アドレスからプロファイルデータの先頭アドレスまでのオフセット。単位はバイト
		uint32_t ProfileData;
		///	プロファイルデータのサイズ 単位はバイト
		uint32_t ProfileSize;
		uint32_t _Reserved_64;
	};
	struct DIBV5Header final
	{
		static const constexpr uint32_t Size = 124;
		///	ビットマップの横幅
		int32_t Width;
		///	ビットマップの縦幅
		int32_t Height;
		///	プレーン数
		uint16_t Planes;
		///	1ピクセルあたりのビット数
		DIBBitDepth BitCount;
		///	圧縮形式
		DIBCompressionMethod Compression;
		///	画像データサイズ (単位はバイト)
		uint32_t SizeImage;
		///	水平方向の解像度 (単位はピクセル/m)
		uint32_t XPelsPerMeter;
		///	垂直方向の解像度 (単位はピクセル/m)
		uint32_t YPelsPerMeter;
		///	使用する色数 ビットマップで実際に使用するカラーパレット内のカラーインデックスの数。
		uint32_t ClrUsed;
		///	重要な色数 ビットマップを表示するために必要なカラーインデックスの数。
		uint32_t ClrImportant;
		///	カラーマスク
		DIBRGBAColorMask ColorMask;
		///	色空間
		DIBV5ColorSpace ColorSpace;
	};
}
#endif // __stationaryorbit_graphics_dib_wbmpheaders__