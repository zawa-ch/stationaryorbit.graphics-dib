//	stationaryorbit/graphics-dib/dibv5bitmap
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
#ifndef __stationaryorbit_graphics_dib_dibv5bitmap__
#define __stationaryorbit_graphics_dib_dibv5bitmap__
#include <vector>
#include <functional>
#include <optional>
#include "dibheaders.hpp"
#include "dibloader.hpp"
#include "rgbdecoder.hpp"
namespace zawa_ch::StationaryOrbit::Graphics::DIB
{
	///	@a DIBLoader を使用してV5Headerを持つWindows bitmap 画像を読み込みます。
	class DIBV5Bitmap
	{
	public:
		typedef uint32_t RawDataType;
		typedef RGB8_t ValueType;
		typedef RGB8Pixmap_t Pixmap;
	private:
		DIBLoader&& loader;
		DIBV5Header ihead;
		std::vector<RGB8_t> palette;
	public:
		///	@a DIBLoader を使用して @a DIBV5Bitmap を初期化します。
		///	@param	loader
		///	読み込みに使用する @a DIBLoader 。
		///	このオブジェクトで「消費」されるため、右辺値参照である必要があります。
		DIBV5Bitmap(DIBLoader&& loader);
		DIBV5Bitmap(const DIBV5Bitmap&) = delete;
		DIBV5Bitmap(DIBV5Bitmap&&) = default;
		virtual ~DIBV5Bitmap() = default;

		///	このオブジェクトの情報ヘッダを取得します。
		[[nodiscard]] const DIBV5Header& InfoHead() const { return ihead; }
		///	このオブジェクトの色パレットを取得します。
		[[nodiscard]] std::optional<std::reference_wrapper<const std::vector<RGB8_t>>> Palette() const;

		///	画像の指定された位置にある1ピクセルの色を取得します。
		///	@param	pos
		///	取得する画像上の座標位置。
		[[nodiscard]] ValueType GetPixel(const DisplayPoint& pos);
		///	画像の指定された位置から連続したピクセルの色を取得します。
		///	@param	pos
		///	取得する画像上の座標位置。
		///	@param	count
		///	取得する要素数。
		[[nodiscard]] std::vector<ValueType> GetPixel(const DisplayPoint& pos, size_t count);
		///	画像の指定された位置にある1ピクセルの色を設定します。
		///	@param	pos
		///	設定する画像上の座標位置。
		///	@param	value
		///	設定する値。
		void SetPixel(const DisplayPoint& pos, const ValueType& value);
		///	画像の指定された位置から連続したピクセルの色を設定します。
		///	@param	pos
		///	取得する画像上の座標位置。
		///	@param	value
		///	設定する値の配列。
		void SetPixel(const DisplayPoint& pos, const std::vector<ValueType>& value);
		///	画像の指定された位置にある1ピクセルの生データを取得します。
		///	@param	pos
		///	取得する画像上の座標位置。
		[[nodiscard]] RawDataType GetPixelRaw(const DisplayPoint& pos);
		///	画像の指定された位置から連続したピクセルの生データを取得します。
		///	@param	pos
		///	取得する画像上の座標位置。
		///	@param	count
		///	取得する要素数。
		[[nodiscard]] std::vector<RawDataType> GetPixelRaw(const DisplayPoint& pos, size_t count);
		///	画像の指定された位置にある1ピクセルの生データを設定します。
		///	@param	pos
		///	設定する画像上の座標位置。
		///	@param	value
		///	設定する値。
		void SetPixelRaw(const DisplayPoint& pos, const RawDataType& value);
		///	画像の指定された位置から連続したピクセルの生データを設定します。
		///	@param	pos
		///	取得する画像上の座標位置。
		///	@param	value
		///	設定する値の配列。
		void SetPixelRaw(const DisplayPoint& pos, const std::vector<RawDataType>& value);

		///	画像を @a WritableImage にコピーします。
		///	@param	dest
		///	コピー先の画像バッファ。
		void CopyTo(WritableImage<ValueType>& dest);
		///	画像の指定された領域を @a WritableImage にコピーします。
		///	@param	dest
		///	コピー先の画像バッファ。
		///	@param	area
		///	コピー元の切り抜き範囲。
		///	@param	destorigin
		///	コピー先の貼り付け基準位置。
		void CopyTo(WritableImage<ValueType>& dest, const DisplayRectangle& area, const DisplayPoint& destorigin = DisplayPoint(0, 0));
		///	画像を @a Pixmap として出力します。
		[[nodiscard]] Pixmap ToPixmap();
		///	画像の指定された領域を @a Pixmap として出力します。
		///	@param	area
		///	画像の切り抜き範囲。
		[[nodiscard]] Pixmap ToPixmap(const DisplayRectangle& area);

		///	指定された @a DIBLoader に画像データを書き込み、 @a DIBV5Bitmap を生成します。
		///	@param	loader
		///	書き込み先の @a DIBLoader オブジェクト。
		///	このオブジェクトで「消費」されるため、右辺値参照である必要があります。
		///	@param	header
		///	生成時に格納する画像ヘッダデータ。
		///	@param	image
		///	生成時に格納する画像データ。
		static std::optional<DIBV5Bitmap> Generate(DIBLoader&& loader, const DIBV5Header& header, const Image<RGB8_t>& image);
		///	指定された @a DIBLoader に画像データを書き込み、 @a DIBV5Bitmap を生成します。
		///	@param	loader
		///	書き込み先の @a DIBLoader オブジェクト。
		///	このオブジェクトで「消費」されるため、右辺値参照である必要があります。
		///	@param	header
		///	生成時に格納する画像ヘッダデータ。
		///	@param	palette
		///	生成時に格納する色パレット。
		///	@param	image
		///	生成時に格納する画像データ。
		static std::optional<DIBV5Bitmap> Generate(DIBLoader&& loader, const DIBV5Header& header, const std::vector<RGB8_t> palette, const Image<RGB8_t>& image);
	private:
		[[nodiscard]] ValueType ConvertToRGB(const DIBRGBDecoder::ValueType& data) const;
		[[nodiscard]] RawDataType ConvertToRawData(const DIBRGBDecoder::ValueType& data) const;
		[[nodiscard]] DIBRGBDecoder::ValueType ConvertToRGBDecoderValue(const ValueType& value) const;
		[[nodiscard]] DIBRGBDecoder::ValueType ConvertToRGBDecoderValue(const RawDataType& value) const;
	};
}
#endif // __stationaryorbit_graphics_dib_dibv5bitmap__
