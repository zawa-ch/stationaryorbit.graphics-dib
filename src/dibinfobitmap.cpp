//	stationaryorbit.graphics-dib:/dibinfobitmap
//	Copyright 2020-2021 zawa-ch.
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
#include "stationaryorbit/graphics-dib/dibinfobitmap.hpp"
using namespace zawa_ch::StationaryOrbit;
using namespace zawa_ch::StationaryOrbit::Graphics::DIB;

DIBInfoBitmap::DIBInfoBitmap(DIBLoader&& loader) : loader(std::forward<DIBLoader>(loader))
{
	if (!this->loader.IsEnable()) { throw InvalidOperationException("無効な状態のloaderが渡されました。"); }
	if (this->loader.HeaderSize() < DIBInfoHeader::Size) { throw InvalidDIBFormatException("情報ヘッダの長さはInfoHeaderでサポートされる最小の長さよりも短いです。"); }
	DIBLoaderHelper::Read(this->loader, &ihead, sizeof(DIBFileHeader) + sizeof(int32_t));
	if (ihead.Compression == DIBCompressionMethod::BITFIELDS)
	{
		auto colormaskdata = DIBRGBColorMask();
		DIBLoaderHelper::Read(this->loader, colormaskdata, sizeof(DIBFileHeader) + DIBInfoHeader::Size);
		colormask = DIBColorMask(colormaskdata);
	}
	else if (ihead.Compression == DIBCompressionMethod::ALPHABITFIELDS)
	{
		auto colormaskdata = DIBRGBAColorMask();
		DIBLoaderHelper::Read(this->loader, colormaskdata, sizeof(DIBFileHeader) + DIBInfoHeader::Size);
		colormask = DIBColorMask(colormaskdata);
	}
	if ((ihead.Compression == DIBCompressionMethod::RGB)&&(uint16_t(ihead.BitCount) <= uint16_t(DIBBitDepth::Bit8)))
	{
		size_t palsize = ihead.ClrUsed;
		if (palsize == 0) { palsize = 1 << uint16_t(ihead.BitCount); }
		auto lpal = std::vector<RGBQuad_t>(palsize);
		DIBLoaderHelper::Read(this->loader, lpal.data(), sizeof(DIBFileHeader) + sizeof(DIBInfoHeader), palsize);
		palette.reserve(palsize);
		for(auto i: lpal) { palette.push_back(RGB8_t(i)); }
	}
}
std::optional<std::reference_wrapper<const DIBColorMask>> DIBInfoBitmap::ColorMask() const
{
	switch(ihead.Compression)
	{
		case DIBCompressionMethod::BITFIELDS:
		case DIBCompressionMethod::ALPHABITFIELDS:
		{ return colormask; }
		case DIBCompressionMethod::RGB:
		case DIBCompressionMethod::RLE4:
		case DIBCompressionMethod::RLE8:
		case DIBCompressionMethod::JPEG:
		case DIBCompressionMethod::PNG:
		{ return std::nullopt; }
		default: { throw InvalidOperationException("情報ヘッダのComplessionMethodの内容が無効です。"); }
	}
}
std::optional<std::reference_wrapper<const std::vector<Graphics::RGB8_t>>> DIBInfoBitmap::Palette() const
{
	if (ihead.ClrUsed != 0) { return palette; }
	else { return std::nullopt; }
}
DIBInfoBitmap::ValueType DIBInfoBitmap::GetPixel(const DisplayPoint& pos)
{
	switch(ihead.Compression)
	{
		case DIBCompressionMethod::RGB:
		{
			auto decoder = DIBRGBDecoder(loader, loader.FileHead().Offset(), ihead.BitCount, DisplayRectSize(ihead.Width, ihead.Height));
			decoder.JumpTo(pos);
			return ConvertToRGB(decoder.Current());
			break;
		}
		case DIBCompressionMethod::RLE4:
		case DIBCompressionMethod::RLE8:
		case DIBCompressionMethod::BITFIELDS:
		case DIBCompressionMethod::JPEG:
		case DIBCompressionMethod::PNG:
		case DIBCompressionMethod::ALPHABITFIELDS:
		//	TODO: Implement
		{ throw NotImplementedException(); }
		default: { throw InvalidOperationException("情報ヘッダのComplessionMethodの内容が無効です。"); }
	}
}
std::vector<DIBInfoBitmap::ValueType> DIBInfoBitmap::GetPixel(const DisplayPoint& pos, size_t count)
{
	switch(ihead.Compression)
	{
		case DIBCompressionMethod::RGB:
		{
			auto decoder = DIBRGBDecoder(loader, loader.FileHead().Offset(), ihead.BitCount, DisplayRectSize(ihead.Width, ihead.Height));
			decoder.JumpTo(pos);
			auto result = std::vector<DIBInfoBitmap::ValueType>();
			result.reserve(count);
			for (auto _: Range<size_t>(0, count).get_std_iterator()) { result.push_back(ConvertToRGB(decoder.Current())); }
			return result;
		}
		case DIBCompressionMethod::RLE4:
		case DIBCompressionMethod::RLE8:
		case DIBCompressionMethod::BITFIELDS:
		case DIBCompressionMethod::JPEG:
		case DIBCompressionMethod::PNG:
		case DIBCompressionMethod::ALPHABITFIELDS:
		//	TODO: Implement
		{ throw NotImplementedException(); }
		default: { throw InvalidOperationException("情報ヘッダのComplessionMethodの内容が無効です。"); }
	}
}
void DIBInfoBitmap::SetPixel(const DisplayPoint& pos, const ValueType& value)
{
	switch(ihead.Compression)
	{
		case DIBCompressionMethod::RGB:
		{
			auto decoder = DIBRGBDecoder(loader, loader.FileHead().Offset(), ihead.BitCount, DisplayRectSize(ihead.Width, ihead.Height));
			decoder.JumpTo(pos);
			decoder.Write(ConvertToRGBDecoderValue(value));
			break;
		}
		case DIBCompressionMethod::RLE4: { throw InvalidOperationException("現在のComplessionMethodでの書き込みはサポートされていません。"); }
		case DIBCompressionMethod::RLE8: { throw InvalidOperationException("現在のComplessionMethodでの書き込みはサポートされていません。"); }
		case DIBCompressionMethod::BITFIELDS:
		//	TODO: Implement
		{ throw NotImplementedException(); }
		case DIBCompressionMethod::JPEG: { throw InvalidOperationException("現在のComplessionMethodでの書き込みはサポートされていません。"); }
		case DIBCompressionMethod::PNG: { throw InvalidOperationException("現在のComplessionMethodでの書き込みはサポートされていません。"); }
		case DIBCompressionMethod::ALPHABITFIELDS:
		//	TODO: Implement
		{ throw NotImplementedException(); }
		default: { throw InvalidOperationException("情報ヘッダのComplessionMethodの内容が無効です。"); }
	}
}
void DIBInfoBitmap::SetPixel(const DisplayPoint& pos, const std::vector<ValueType>& value)
{
	switch(ihead.Compression)
	{
		case DIBCompressionMethod::RGB:
		{
			auto decoder = DIBRGBDecoder(loader, loader.FileHead().Offset(), ihead.BitCount, DisplayRectSize(ihead.Width, ihead.Height));
			decoder.JumpTo(pos);
			for(auto i: value) { decoder.Write(ConvertToRGBDecoderValue(i)); }
			break;
		}
		case DIBCompressionMethod::RLE4: { throw InvalidOperationException("現在のComplessionMethodでの書き込みはサポートされていません。"); }
		case DIBCompressionMethod::RLE8: { throw InvalidOperationException("現在のComplessionMethodでの書き込みはサポートされていません。"); }
		case DIBCompressionMethod::BITFIELDS:
		//	TODO: Implement
		{ throw NotImplementedException(); }
		case DIBCompressionMethod::JPEG: { throw InvalidOperationException("現在のComplessionMethodでの書き込みはサポートされていません。"); }
		case DIBCompressionMethod::PNG: { throw InvalidOperationException("現在のComplessionMethodでの書き込みはサポートされていません。"); }
		case DIBCompressionMethod::ALPHABITFIELDS:
		//	TODO: Implement
		{ throw NotImplementedException(); }
		default: { throw InvalidOperationException("情報ヘッダのComplessionMethodの内容が無効です。"); }
	}
}
DIBInfoBitmap::RawDataType DIBInfoBitmap::GetPixelRaw(const DisplayPoint& pos)
{
	switch(ihead.Compression)
	{
		case DIBCompressionMethod::RGB:
		case DIBCompressionMethod::BITFIELDS:
		case DIBCompressionMethod::ALPHABITFIELDS:
		{
			auto decoder = DIBRGBDecoder(loader, loader.FileHead().Offset(), ihead.BitCount, DisplayRectSize(ihead.Width, ihead.Height));
			decoder.JumpTo(pos);
			return ConvertToRawData(decoder.Current());
		}
		case DIBCompressionMethod::RLE4:
		case DIBCompressionMethod::RLE8:
		case DIBCompressionMethod::JPEG:
		case DIBCompressionMethod::PNG:
		//	TODO: Implement
		{ throw NotImplementedException(); }
		default: { throw InvalidOperationException("情報ヘッダのComplessionMethodの内容が無効です。"); }
	}
}
std::vector<DIBInfoBitmap::RawDataType> DIBInfoBitmap::GetPixelRaw(const DisplayPoint& pos, size_t count)
{
	switch(ihead.Compression)
	{
		case DIBCompressionMethod::RGB:
		case DIBCompressionMethod::BITFIELDS:
		case DIBCompressionMethod::ALPHABITFIELDS:
		{
			auto decoder = DIBRGBDecoder(loader, loader.FileHead().Offset(), ihead.BitCount, DisplayRectSize(ihead.Width, ihead.Height));
			decoder.JumpTo(pos);
			auto result = std::vector<RawDataType>();
			result.reserve(count);
			for (auto _: Range<size_t>(0, count).get_std_iterator()) { result.push_back(ConvertToRawData(decoder.Current())); }
			return result;
		}
		case DIBCompressionMethod::RLE4:
		case DIBCompressionMethod::RLE8:
		case DIBCompressionMethod::JPEG:
		case DIBCompressionMethod::PNG:
		//	TODO: Implement
		{ throw NotImplementedException(); }
		default: { throw InvalidOperationException("情報ヘッダのComplessionMethodの内容が無効です。"); }
	}
}
void DIBInfoBitmap::SetPixelRaw(const DisplayPoint& pos, const RawDataType& value)
{
	switch(ihead.Compression)
	{
		case DIBCompressionMethod::RGB:
		case DIBCompressionMethod::BITFIELDS:
		case DIBCompressionMethod::ALPHABITFIELDS:
		{
			auto decoder = DIBRGBDecoder(loader, loader.FileHead().Offset(), ihead.BitCount, DisplayRectSize(ihead.Width, ihead.Height));
			decoder.JumpTo(pos);
			decoder.Write(ConvertToRGBDecoderValue(value));
			break;
		}
		case DIBCompressionMethod::RLE4: { throw InvalidOperationException("現在のComplessionMethodでの書き込みはサポートされていません。"); }
		case DIBCompressionMethod::RLE8: { throw InvalidOperationException("現在のComplessionMethodでの書き込みはサポートされていません。"); }
		case DIBCompressionMethod::JPEG: { throw InvalidOperationException("現在のComplessionMethodでの書き込みはサポートされていません。"); }
		case DIBCompressionMethod::PNG: { throw InvalidOperationException("現在のComplessionMethodでの書き込みはサポートされていません。"); }
		default: { throw InvalidOperationException("情報ヘッダのComplessionMethodの内容が無効です。"); }
	}
}
void DIBInfoBitmap::SetPixelRaw(const DisplayPoint& pos, const std::vector<RawDataType>& value)
{
	switch(ihead.Compression)
	{
		case DIBCompressionMethod::RGB:
		case DIBCompressionMethod::BITFIELDS:
		case DIBCompressionMethod::ALPHABITFIELDS:
		{
			auto decoder = DIBRGBDecoder(loader, loader.FileHead().Offset(), ihead.BitCount, DisplayRectSize(ihead.Width, ihead.Height));
			decoder.JumpTo(pos);
			for(auto i: value) { decoder.Write(ConvertToRGBDecoderValue(i)); }
			break;
		}
		case DIBCompressionMethod::RLE4: { throw InvalidOperationException("現在のComplessionMethodでの書き込みはサポートされていません。"); }
		case DIBCompressionMethod::RLE8: { throw InvalidOperationException("現在のComplessionMethodでの書き込みはサポートされていません。"); }
		case DIBCompressionMethod::JPEG: { throw InvalidOperationException("現在のComplessionMethodでの書き込みはサポートされていません。"); }
		case DIBCompressionMethod::PNG: { throw InvalidOperationException("現在のComplessionMethodでの書き込みはサポートされていません。"); }
		default: { throw InvalidOperationException("情報ヘッダのComplessionMethodの内容が無効です。"); }
	}
}
void DIBInfoBitmap::CopyTo(WritableImage<RGB8_t>& dest)
{
	switch(ihead.Compression)
	{
		case DIBCompressionMethod::RGB:
		{
			auto decoder = DIBRGBDecoder(loader, loader.FileHead().Offset(), ihead.BitCount, DisplayRectSize(ihead.Width, ihead.Height));
			for (decoder.Reset(); decoder.HasValue(); decoder.Next()) { dest.At(decoder.CurrentPos()) = ConvertToRGB(decoder.Current()); }
			break;
		}
		case DIBCompressionMethod::RLE4:
		case DIBCompressionMethod::RLE8:
		case DIBCompressionMethod::BITFIELDS:
		case DIBCompressionMethod::JPEG:
		case DIBCompressionMethod::PNG:
		case DIBCompressionMethod::ALPHABITFIELDS:
		//	TODO: Implement
		{ throw NotImplementedException(); }
		default: { throw InvalidOperationException("情報ヘッダのComplessionMethodの内容が無効です。"); }
	}
}
void DIBInfoBitmap::CopyTo(WritableImage<RGB8_t>& dest, const DisplayRectangle& area, const DisplayPoint& destorigin)
{
	if ((area.left() < 0)||(area.top() < 0)||(ihead.Width < area.right())||(ihead.Height < area.bottom())) { throw std::out_of_range("areaで指定された領域がビットマップの画像領域を超えています。"); }
	switch(ihead.Compression)
	{
		case DIBCompressionMethod::RGB:
		{
			auto decoder = DIBRGBDecoder(loader, loader.FileHead().Offset(), ihead.BitCount, DisplayRectSize(ihead.Width, ihead.Height));
			for (decoder.JumpTo(DisplayPoint(area.left(), area.bottom())); decoder.HasValue(); decoder.Next())
			{
				if (area.contains(decoder.CurrentPos())) { decoder.Next(ihead.Width - area.width() - 1); continue; }
				dest.At(decoder.CurrentPos() - area.origin() + destorigin) = ConvertToRGB(decoder.Current());
			}
			break;
		}
		case DIBCompressionMethod::RLE4:
		case DIBCompressionMethod::RLE8:
		case DIBCompressionMethod::BITFIELDS:
		case DIBCompressionMethod::JPEG:
		case DIBCompressionMethod::PNG:
		case DIBCompressionMethod::ALPHABITFIELDS:
		//	TODO: Implement
		{ throw NotImplementedException(); }
		default: { throw InvalidOperationException("情報ヘッダのComplessionMethodの内容が無効です。"); }
	}
}
DIBInfoBitmap::Pixmap DIBInfoBitmap::ToPixmap()
{
	auto result = Pixmap(DisplayRectSize(ihead.Width, ihead.Height));
	CopyTo(result);
	return result;
}
DIBInfoBitmap::Pixmap DIBInfoBitmap::ToPixmap(const DisplayRectangle& area)
{
	auto result = Pixmap(area.size());
	CopyTo(result, area);
	return result;
}
std::optional<DIBInfoBitmap> DIBInfoBitmap::Generate(DIBLoader&& loader, const DIBInfoHeader& header, const Image<RGB8_t>& image) { return Generate(std::forward<DIBLoader>(loader), header, std::vector<RGB8_t>(), image); }
std::optional<DIBInfoBitmap> DIBInfoBitmap::Generate(DIBLoader&& loader, const DIBInfoHeader& header, const std::vector<RGB8_t> palette, const Image<RGB8_t>& image)
{
	auto fhead = DIBFileHeader();
	std::copy(&(fhead.FileType_Signature[0]), &(fhead.FileType_Signature[2]), &(fhead.FileType[0]));
	switch(header.Compression)
	{
		case DIBCompressionMethod::RGB:
		{
			size_t palsize;
			switch(header.BitCount)
			{
				case DIBBitDepth::Bit1:
				case DIBBitDepth::Bit4:
				case DIBBitDepth::Bit8:
				{
					throw NotImplementedException();
					palsize = header.ClrUsed;
					if (palsize == 0) { palsize = 1 << uint16_t(header.BitCount); }
					if ((1 << uint16_t(header.BitCount)) < palsize) { throw std::invalid_argument("biClrUsedの値が指定されたbiBitCountでサポートされている値を超えています。"); }
					break;
				}
				case DIBBitDepth::Bit16:
				case DIBBitDepth::Bit24:
				case DIBBitDepth::Bit32:
				{ palsize = 0; break; }
				default: { throw std::invalid_argument("BitCountの内容が無効です。"); }
			}
			fhead.Offset(int32_t(sizeof(DIBFileHeader) + DIBInfoHeader::Size) + (sizeof(RGBQuad_t) * palsize));
			fhead.FileSize(int32_t(sizeof(DIBFileHeader) + DIBInfoHeader::Size + (sizeof(RGBQuad_t) * palsize) + (DIBRGBEncoder::GetStrideLength(header.BitCount, DisplayRectSize(header.Width, header.Height)) * header.Height)));
			DIBLoaderHelper::Write(loader, fhead, 0);
			DIBLoaderHelper::Write(loader, DIBInfoHeader::Size, sizeof(DIBFileHeader));
			DIBLoaderHelper::Write(loader, header, sizeof(DIBFileHeader) + sizeof(uint32_t));
			for (auto i: Range<size_t>(0, palsize).get_std_iterator())
			{
				if (palette.size() < i) { DIBLoaderHelper::Write(loader, RGBQuad_t(palette[i]), sizeof(DIBFileHeader) + DIBInfoHeader::Size + (sizeof(RGBQuad_t) * i)); }
				else { DIBLoaderHelper::Write(loader, RGBQuad_t(), sizeof(DIBFileHeader) + DIBInfoHeader::Size + (sizeof(RGBQuad_t) * i)); }
			}
			auto encoder = DIBRGBEncoder(loader, fhead.Offset(), header.BitCount, DisplayRectSize(header.Width, header.Height));
			switch(header.BitCount)
			{
				case DIBBitDepth::Bit1: { while (encoder.HasValue()) { encoder.Write(DIBPixelData<DIBBitDepth::Bit1>()); } break; }
				case DIBBitDepth::Bit4: { while (encoder.HasValue()) { encoder.Write(DIBPixelData<DIBBitDepth::Bit4>()); } break; }
				case DIBBitDepth::Bit8: { while (encoder.HasValue()) { encoder.Write(DIBPixelData<DIBBitDepth::Bit8>()); } break; }
				case DIBBitDepth::Bit16: { while (encoder.HasValue()) { encoder.Write(DIBPixelPerser::ToPixel16(image.At(encoder.CurrentPos()))); } break; }
				case DIBBitDepth::Bit24: { while (encoder.HasValue()) { encoder.Write(DIBPixelPerser::ToPixel24(image.At(encoder.CurrentPos()))); } break; }
				case DIBBitDepth::Bit32: { while (encoder.HasValue()) { encoder.Write(DIBPixelPerser::ToPixel32(image.At(encoder.CurrentPos()))); } break; }
				default: { throw std::invalid_argument("BitCountの内容が無効です。"); }
			}
			try
			{
				loader.Sync();
				return DIBInfoBitmap(std::forward<DIBLoader>(loader));
			}
			catch (std::exception e)
			{
				return std::nullopt;
			}
		}
		case DIBCompressionMethod::RLE8:
		case DIBCompressionMethod::RLE4:
		case DIBCompressionMethod::BITFIELDS:
		case DIBCompressionMethod::JPEG:
		case DIBCompressionMethod::PNG:
		case DIBCompressionMethod::ALPHABITFIELDS:
		{ throw NotImplementedException(); }
		default: { throw std::invalid_argument("CompressionMethodの内容が無効です。"); }
	}
}
DIBInfoBitmap::ValueType DIBInfoBitmap::ConvertToRGB(const DIBRGBDecoder::ValueType& data) const
{
	switch(ihead.Compression)
	{
		case DIBCompressionMethod::RGB:
			switch(ihead.BitCount)
			{
				case DIBBitDepth::Bit1:
				case DIBBitDepth::Bit4:
				case DIBBitDepth::Bit8:
				{
					return palette.at(std::visit([](auto i)->uint32_t { return uint32_t(i); }, data));
				}
				case DIBBitDepth::Bit16: { return DIBPixelPerser::ToRGB(std::get<DIBPixelData<DIBBitDepth::Bit16>>(data)); }
				case DIBBitDepth::Bit24: { return DIBPixelPerser::ToRGB(std::get<DIBPixelData<DIBBitDepth::Bit24>>(data)); }
				case DIBBitDepth::Bit32: { return DIBPixelPerser::ToRGB(std::get<DIBPixelData<DIBBitDepth::Bit32>>(data)); }
				default: { throw InvalidDIBFormatException("情報ヘッダのBitCountの内容が無効です。"); }
			}
			break;
		case DIBCompressionMethod::BITFIELDS:
		case DIBCompressionMethod::ALPHABITFIELDS:
		//	TODO: Implement
		{ throw NotImplementedException(); }
		case DIBCompressionMethod::RLE8:
		case DIBCompressionMethod::RLE4:
		case DIBCompressionMethod::JPEG:
		case DIBCompressionMethod::PNG:
		default: { throw InvalidOperationException("情報ヘッダのComressionMethodの内容が無効です。"); }
	}
}
DIBInfoBitmap::RawDataType DIBInfoBitmap::ConvertToRawData(const DIBRGBDecoder::ValueType& data) const
{
	switch(ihead.BitCount)
	{
		case DIBBitDepth::Bit1:
		case DIBBitDepth::Bit4:
		case DIBBitDepth::Bit8:
		case DIBBitDepth::Bit16:
		case DIBBitDepth::Bit24:
		case DIBBitDepth::Bit32:
		{
			return std::visit([](auto i)->RawDataType { return RawDataType(i); }, data);
		}
		default: { throw InvalidDIBFormatException("情報ヘッダのBitCountの内容が無効です。"); }
	}
}
DIBRGBDecoder::ValueType DIBInfoBitmap::ConvertToRGBDecoderValue(const ValueType& value) const
{
	switch(ihead.Compression)
	{
		case DIBCompressionMethod::RGB:
			switch(ihead.BitCount)
			{
				case DIBBitDepth::Bit1:
				case DIBBitDepth::Bit4:
				case DIBBitDepth::Bit8:
				//	TODO: Implement
				{ throw NotImplementedException(); }
				case DIBBitDepth::Bit16: { return DIBPixelPerser::ToPixel16(value); }
				case DIBBitDepth::Bit24: { return DIBPixelPerser::ToPixel24(value); }
				case DIBBitDepth::Bit32: { return DIBPixelPerser::ToPixel32(value); }
				default: { throw InvalidOperationException("情報ヘッダのBitCountの内容が無効です。"); }
			}
			break;
		case DIBCompressionMethod::BITFIELDS:
		case DIBCompressionMethod::ALPHABITFIELDS:
		//	TODO: Implement
		{ throw NotImplementedException(); }
		case DIBCompressionMethod::RLE8:
		case DIBCompressionMethod::RLE4:
		case DIBCompressionMethod::JPEG:
		case DIBCompressionMethod::PNG:
		default: { throw InvalidOperationException("情報ヘッダのComressionMethodの内容が無効です。"); }
	}
}
DIBRGBDecoder::ValueType DIBInfoBitmap::ConvertToRGBDecoderValue(const RawDataType& value) const
{
	switch(ihead.BitCount)
	{
		case DIBBitDepth::Bit1: { return DIBPixelData<DIBBitDepth::Bit1>(value); }
		case DIBBitDepth::Bit4: { return DIBPixelData<DIBBitDepth::Bit4>(value); }
		case DIBBitDepth::Bit8: { return DIBPixelData<DIBBitDepth::Bit8>(value); }
		case DIBBitDepth::Bit16: { return DIBPixelData<DIBBitDepth::Bit16>(value); }
		case DIBBitDepth::Bit24: { return DIBPixelData<DIBBitDepth::Bit24>(value); }
		case DIBBitDepth::Bit32: { return DIBPixelData<DIBBitDepth::Bit32>(value); }
		default: { throw InvalidOperationException("情報ヘッダのBitCountの内容が無効です。"); }
	}
}
