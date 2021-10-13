//	stationaryorbit.graphics-dib:/dibcorebitmap
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
#include "stationaryorbit/graphics-dib/dibcorebitmap.hpp"
using namespace zawa_ch::StationaryOrbit;
using namespace zawa_ch::StationaryOrbit::Graphics::DIB;

DIBCoreBitmapDecoder::DIBCoreBitmapDecoder(DIBLoader& loader, size_t offset, DIBBitDepth bitdepth, const DisplayRectSize& size)
	: loader(loader), offset(offset), bitdepth(bitdepth), size(size), length(size.width() * size.height()), pixellength(DIBCoreBitmapEncoder::GetPxLength(bitdepth)), stridelength(DIBCoreBitmapEncoder::GetStrideLength(bitdepth, size))
{
	Reset();
}
bool DIBCoreBitmapDecoder::Next()
{
	if (IsAfterEnd()) { return false; }
	++current;
	if (IsAfterEnd()) { current = length; return false; }
	current_value = Get(current);
	return true;
}
bool DIBCoreBitmapDecoder::Next(const IteratorTraits::IteratorDiff_t& count)
{
	if (IsAfterEnd()) { return false; }
	current += count;
	if (IsAfterEnd()) { current = length; return false; }
	current_value = Get(current);
	return true;
}
bool DIBCoreBitmapDecoder::Previous()
{
	if (IsBeforeBegin()) { return false; }
	--current;
	if (IsBeforeBegin()) { current = -1; return false; }
	current_value = Get(current);
	return true;
}
bool DIBCoreBitmapDecoder::Previous(const IteratorTraits::IteratorDiff_t& count)
{
	if (IsBeforeBegin()) { return false; }
	current -= count;
	if (IsBeforeBegin()) { current = -1; return false; }
	current_value = Get(current);
	return true;
}
void DIBCoreBitmapDecoder::JumpTo(const DisplayPoint& pos)
{
	if ( (pos.x() < 0)||(pos.y() < 0) ) { throw std::invalid_argument("posに指定されている座標が無効です。"); }
	current = ResolveIndex(pos);
	current_value = Get(ResolveIndex(pos));
}
void DIBCoreBitmapDecoder::Reset() { Reset(IteratorOrigin::Begin); }
void DIBCoreBitmapDecoder::Reset(const IteratorOrigin& origin)
{
	switch(origin)
	{
		default:
		case IteratorOrigin::Begin: { current = 0; break; }
		case IteratorOrigin::End: { current = length - 1; break; }
	}
	current_value = Get(current);
}
bool DIBCoreBitmapDecoder::HasValue() const { return (0 <= current)&&(current < length); }
bool DIBCoreBitmapDecoder::IsBeforeBegin() const { return current < 0; }
bool DIBCoreBitmapDecoder::IsAfterEnd() const { return length <= current; }
Graphics::DisplayPoint DIBCoreBitmapDecoder::CurrentPos() const { return ResolvePos(current); }
DIBCoreBitmapDecoder::ValueType DIBCoreBitmapDecoder::Current() const { if (HasValue()) { return current_value; } else { throw InvalidOperationException("このイテレータは領域の範囲外を指しています。"); } }
void DIBCoreBitmapDecoder::Write(const ValueType& value)
{
	size_t tgt = offset + ResolveOffset(current);
	switch(bitdepth)
	{
		case DIBBitDepth::Bit1: { DIBLoaderHelper::Write(loader, std::get<DIBPixelData<DIBBitDepth::Bit1>>(value), tgt); break; }
		case DIBBitDepth::Bit4: { DIBLoaderHelper::Write(loader, std::get<DIBPixelData<DIBBitDepth::Bit4>>(value), tgt); break; }
		case DIBBitDepth::Bit8: { DIBLoaderHelper::Write(loader, std::get<DIBPixelData<DIBBitDepth::Bit8>>(value), tgt); break; }
		case DIBBitDepth::Bit24: { DIBLoaderHelper::Write(loader, std::get<DIBPixelData<DIBBitDepth::Bit24>>(value), tgt); break; }
		default: { throw InvalidOperationException("情報ヘッダのBitCountの内容が無効です。"); }
	}
	current_value = value;
}
IteratorTraits::IteratorDiff_t DIBCoreBitmapDecoder::Distance(const DIBCoreBitmapDecoder& other) const { return current - other.current; }
bool DIBCoreBitmapDecoder::Equals(const DIBCoreBitmapDecoder& other) const { return current == other.current; }
int DIBCoreBitmapDecoder::Compare(const DIBCoreBitmapDecoder& other) const
{
	if (current == other.current) { return 0; }
	else if (other.current < current) { return 1; }
	else { return -1; }
}
DIBCoreBitmapDecoder::ValueType DIBCoreBitmapDecoder::Get(size_t index)
{
	size_t tgt = offset + ResolveOffset(index);
	switch(bitdepth)
	{
		case DIBBitDepth::Bit1:
		{
			auto result = DIBPixelData<DIBBitDepth::Bit1>();
			DIBLoaderHelper::Read(loader, result, tgt);
			return ValueType(result);
		}
		case DIBBitDepth::Bit4:
		{
			auto result = DIBPixelData<DIBBitDepth::Bit4>();
			DIBLoaderHelper::Read(loader, result, tgt);
			return ValueType(result);
		}
		case DIBBitDepth::Bit8:
		{
			auto result = DIBPixelData<DIBBitDepth::Bit8>();
			DIBLoaderHelper::Read(loader, result, tgt);
			return ValueType(result);
		}
		case DIBBitDepth::Bit24:
		{
			auto result = DIBPixelData<DIBBitDepth::Bit24>();
			DIBLoaderHelper::Read(loader, result, tgt);
			return ValueType(result);
		}
		default: { throw InvalidOperationException("情報ヘッダのBitCountの内容が無効です。"); }
	}
}
size_t DIBCoreBitmapDecoder::ResolveIndex(const DisplayPoint& pos) const { return ((size.height() - 1 - pos.y()) * size.width()) + pos.x(); }
Graphics::DisplayPoint DIBCoreBitmapDecoder::ResolvePos(size_t index) const { return DisplayPoint(index % size.width(), size.height() - 1 - (index / size.width())); }
size_t DIBCoreBitmapDecoder::ResolveOffset(const DisplayPoint& pos) const
{
	if ( (pos.x() < 0)||(pos.y() < 0) ) { throw std::invalid_argument("posに指定されている座標が無効です。"); }
	if ( (size.width() <= pos.x())||(size.height() <= pos.y()) ) { throw std::out_of_range("指定された座標はこの画像領域を超えています。"); }
	return (stridelength * (size.height() - 1 - pos.y())) + (pixellength * pos.x());
}
size_t DIBCoreBitmapDecoder::ResolveOffset(size_t index) const
{
	if (length <= index) { throw std::out_of_range("指定されたインデックスはこの画像領域を超えています。"); }
	return (stridelength * (index / size.width())) + (pixellength * (index % size.width()));
}

DIBCoreBitmapEncoder::DIBCoreBitmapEncoder(DIBLoader& loader, size_t offset, DIBBitDepth bitdepth, const DisplayRectSize& size)
	: loader(loader), offset(offset), bitdepth(bitdepth), size(size), length(size.width() * size.height()), current(0), pixellength(DIBCoreBitmapEncoder::GetPxLength(bitdepth)), stridelength(DIBCoreBitmapEncoder::GetStrideLength(bitdepth, size))
{}
void DIBCoreBitmapEncoder::Write(const ValueType& value)
{
	size_t tgt = offset + ResolveOffset(ResolvePos(current));
	switch(bitdepth)
	{
		case DIBBitDepth::Bit1: { DIBLoaderHelper::Write(loader, std::get<DIBPixelData<DIBBitDepth::Bit1>>(value), tgt); break; }
		case DIBBitDepth::Bit4: { DIBLoaderHelper::Write(loader, std::get<DIBPixelData<DIBBitDepth::Bit4>>(value), tgt); break; }
		case DIBBitDepth::Bit8: { DIBLoaderHelper::Write(loader, std::get<DIBPixelData<DIBBitDepth::Bit8>>(value), tgt); break; }
		case DIBBitDepth::Bit24: { DIBLoaderHelper::Write(loader, std::get<DIBPixelData<DIBBitDepth::Bit24>>(value), tgt); break; }
		default: { throw InvalidOperationException("情報ヘッダのBitCountの内容が無効です。"); }
	}
	if (CurrentPos().x() == (size.width() - 1))
	{
		size_t ci = size.width() * ((uint16_t(bitdepth) + 7) / 8);
		size_t stride = (((size.width() * uint16_t(bitdepth)) + 31) % 32) / 8;
		for (auto i: Range<size_t>(ci, stride).get_std_iterator()) { DIBLoaderHelper::Write(loader, char(), offset + i); }
	}
	++current;
}
void DIBCoreBitmapEncoder::Reset() { current = 0; }
bool DIBCoreBitmapEncoder::HasValue() const { return (0 <= current)&&(current < length); }
Graphics::DisplayPoint DIBCoreBitmapEncoder::CurrentPos() const { return ResolvePos(current); }
bool DIBCoreBitmapEncoder::Equals(const DIBCoreBitmapEncoder& other) const { return current == other.current; }
int DIBCoreBitmapEncoder::Compare(const DIBCoreBitmapEncoder& other) const 
{
	if (current == other.current) { return 0; }
	else if (other.current < current) { return 1; }
	else { return -1; }
}
size_t DIBCoreBitmapEncoder::ResolveIndex(const DisplayPoint& pos) const { return ((size.height() - 1 - pos.y()) * size.width()) + pos.x(); }
Graphics::DisplayPoint DIBCoreBitmapEncoder::ResolvePos(size_t index) const { return DisplayPoint(index % size.width(), size.height() - 1 - (index / size.width())); }
size_t DIBCoreBitmapEncoder::ResolveOffset(const DisplayPoint& pos) const
{
	if ( (pos.x() < 0)||(pos.y() < 0) ) { throw std::invalid_argument("posに指定されている座標が無効です。"); }
	if ( (size.width() <= pos.x())||(size.height() <= pos.y()) ) { throw std::out_of_range("指定された座標はこの画像領域を超えています。"); }
	return (stridelength * (size.height() - 1 - pos.y())) + (pixellength * pos.x());
}
size_t DIBCoreBitmapEncoder::GetPxLength(DIBBitDepth bitdepth) { return (uint16_t(bitdepth) + 7) / 8; }
size_t DIBCoreBitmapEncoder::GetStrideLength(DIBBitDepth bitdepth, const DisplayRectSize& size) { return (((GetPxLength(bitdepth) * size.width()) + 3) / 4) * 4; }
size_t DIBCoreBitmapEncoder::GetImageLength(DIBBitDepth bitdepth, const DisplayRectSize& size) { return GetStrideLength(bitdepth, size) * size.height(); }

DIBCoreBitmap::DIBCoreBitmap(DIBLoader&& loader) : loader(std::forward<DIBLoader>(loader))
{
	if (this->loader.IsEnable()) { throw InvalidOperationException("無効な状態のloaderが渡されました。"); }
	if (this->loader.HeaderSize() < DIBCoreHeader::Size) { throw InvalidDIBFormatException("情報ヘッダの長さはCoreHeaderでサポートされる最小の長さよりも短いです。"); }
	DIBLoaderHelper::Read(this->loader, ihead, sizeof(DIBFileHeader) + sizeof(int32_t));
	size_t palettesize = 0;
	switch(ihead.BitCount)
	{
		case DIBBitDepth::Bit1: { palettesize = 2; break; }
		case DIBBitDepth::Bit4: { palettesize = 16; break; }
		case DIBBitDepth::Bit8: { palettesize = 256; break; }
		case DIBBitDepth::Bit24: { palettesize = 0; break; }
		default: { throw InvalidDIBFormatException("情報ヘッダのBitCountの内容が無効です。"); }
	}
	if (palettesize != 0)
	{
		auto p = std::vector<RGBTriple_t>(palettesize);
		DIBLoaderHelper::Read(this->loader, p.data(), sizeof(DIBFileHeader) + DIBCoreHeader::Size, palettesize);
		for (auto i: p) { palette.push_back(RGB8_t(i)); }
	}
}
std::optional<std::reference_wrapper<const std::vector<Graphics::RGB8_t>>> DIBCoreBitmap::Palette() const
{
	switch(ihead.BitCount)
	{
		case DIBBitDepth::Bit1:
		case DIBBitDepth::Bit4:
		case DIBBitDepth::Bit8:
		{ return palette; }
		case DIBBitDepth::Bit24:
		{ return std::nullopt; }
		default: { throw InvalidDIBFormatException("情報ヘッダのBitCountの内容が無効です。"); }
	}
}
DIBCoreBitmap::ValueType DIBCoreBitmap::GetPixel(const DisplayPoint& pos)
{
	auto decoder = DIBCoreBitmapDecoder(loader, sizeof(DIBFileHeader) + loader.FileHead().Offset(), ihead.BitCount, DisplayRectSize(ihead.Width, ihead.Height));
	decoder.JumpTo(pos);
	return ConvertToRGB(decoder.Current());
}
std::vector<DIBCoreBitmap::ValueType> DIBCoreBitmap::GetPixel(const DisplayPoint& pos, size_t count)
{
	auto decoder = DIBCoreBitmapDecoder(loader, sizeof(DIBFileHeader) + loader.FileHead().Offset(), ihead.BitCount, DisplayRectSize(ihead.Width, ihead.Height));
	decoder.JumpTo(pos);
	auto result = std::vector<DIBCoreBitmap::ValueType>();
	result.reserve(count);
	for (auto _: Range<size_t>(0, count).get_std_iterator()) { result.push_back(ConvertToRGB(decoder.Current())); }
	return result;
}
void DIBCoreBitmap::SetPixel(const DisplayPoint& pos, const ValueType& value)
{
	auto decoder = DIBCoreBitmapDecoder(loader, sizeof(DIBFileHeader) + loader.FileHead().Offset(), ihead.BitCount, DisplayRectSize(ihead.Width, ihead.Height));
	decoder.JumpTo(pos);
	decoder.Write(ConvertToDecoderValue(value));
}
void DIBCoreBitmap::SetPixel(const DisplayPoint& pos, const std::vector<ValueType>& value)
{
	auto decoder = DIBCoreBitmapDecoder(loader, sizeof(DIBFileHeader) + loader.FileHead().Offset(), ihead.BitCount, DisplayRectSize(ihead.Width, ihead.Height));
	decoder.JumpTo(pos);
	for(auto i: value) { decoder.Write(ConvertToDecoderValue(i)); }
}
DIBCoreBitmap::RawDataType DIBCoreBitmap::GetPixelRaw(const DisplayPoint& pos)
{
	auto decoder = DIBCoreBitmapDecoder(loader, sizeof(DIBFileHeader) + loader.FileHead().Offset(), ihead.BitCount, DisplayRectSize(ihead.Width, ihead.Height));
	decoder.JumpTo(pos);
	return ConvertToRawData(decoder.Current());
}
std::vector<DIBCoreBitmap::RawDataType> DIBCoreBitmap::GetPixelRaw(const DisplayPoint& pos, size_t count)
{
	auto decoder = DIBCoreBitmapDecoder(loader, sizeof(DIBFileHeader) + loader.FileHead().Offset(), ihead.BitCount, DisplayRectSize(ihead.Width, ihead.Height));
	decoder.JumpTo(pos);
	auto result = std::vector<RawDataType>();
	result.reserve(count);
	for (auto _: Range<size_t>(0, count).get_std_iterator()) { result.push_back(ConvertToRawData(decoder.Current())); }
	return result;
}
void DIBCoreBitmap::SetPixelRaw(const DisplayPoint& pos, const RawDataType& value)
{
	auto decoder = DIBCoreBitmapDecoder(loader, sizeof(DIBFileHeader) + loader.FileHead().Offset(), ihead.BitCount, DisplayRectSize(ihead.Width, ihead.Height));
	decoder.JumpTo(pos);
	decoder.Write(ConvertToDecoderValue(value));
}
void DIBCoreBitmap::SetPixelRaw(const DisplayPoint& pos, const std::vector<RawDataType>& value)
{
	auto decoder = DIBCoreBitmapDecoder(loader, sizeof(DIBFileHeader) + loader.FileHead().Offset(), ihead.BitCount, DisplayRectSize(ihead.Width, ihead.Height));
	decoder.JumpTo(pos);
	for(auto i: value) { decoder.Write(ConvertToDecoderValue(i)); }
}
void DIBCoreBitmap::CopyTo(WritableImage<RGB8_t>& dest)
{
	auto decoder = DIBCoreBitmapDecoder(loader, sizeof(DIBFileHeader) + loader.FileHead().Offset(), ihead.BitCount, DisplayRectSize(ihead.Width, ihead.Height));
	for (decoder.Reset(); decoder.HasValue(); decoder.Next()) { dest.At(decoder.CurrentPos()) = ConvertToRGB(decoder.Current()); }
}
void DIBCoreBitmap::CopyTo(WritableImage<RGB8_t>& dest, const DisplayRectangle& area, const DisplayPoint& destorigin)
{
	if ((area.left() < 0)||(area.top() < 0)||(ihead.Width < area.right())||(ihead.Height < area.bottom())) { throw std::out_of_range("areaで指定された領域がビットマップの画像領域を超えています。"); }
	auto decoder = DIBCoreBitmapDecoder(loader, sizeof(DIBFileHeader) + loader.FileHead().Offset(), ihead.BitCount, DisplayRectSize(ihead.Width, ihead.Height));
	for (decoder.JumpTo(DisplayPoint(area.left(), area.bottom())); decoder.HasValue(); decoder.Next())
	{
		if (area.contains(decoder.CurrentPos())) { decoder.Next(ihead.Width - area.width() - 1); continue; }
		dest.At(decoder.CurrentPos() - area.origin() + destorigin) = ConvertToRGB(decoder.Current());
	}
}
DIBCoreBitmap::Pixmap DIBCoreBitmap::ToPixmap()
{
	auto result = Pixmap(DisplayRectSize(ihead.Width, ihead.Height));
	CopyTo(result);
	return result;
}
DIBCoreBitmap::Pixmap DIBCoreBitmap::ToPixmap(const DisplayRectangle& area)
{
	auto result = Pixmap(area.size());
	CopyTo(result, area);
	return result;
}
std::optional<DIBCoreBitmap> DIBCoreBitmap::Generate(DIBLoader&& loader, const DIBCoreHeader& header, const Image<RGB8_t>& image) { return Generate(std::forward<DIBLoader>(loader), header, std::vector<RGB8_t>(), image); }
std::optional<DIBCoreBitmap> DIBCoreBitmap::Generate(DIBLoader&& loader, const DIBCoreHeader& header, const std::vector<RGB8_t> palette, const Image<RGB8_t>& image)
{
	auto fhead = DIBFileHeader();
	std::copy(&(fhead.FileType_Signature[0]), &(fhead.FileType_Signature[2]), &(fhead.FileType[0]));
	size_t palsize;
	switch(header.BitCount)
	{
		case DIBBitDepth::Bit1: { throw NotImplementedException(); palsize = 2; break; }
		case DIBBitDepth::Bit4: { throw NotImplementedException(); palsize = 16; break; }
		case DIBBitDepth::Bit8: { throw NotImplementedException(); palsize = 256; break; }
		case DIBBitDepth::Bit24: { palsize = 0; break; }
		default: { throw std::invalid_argument("BitCountの内容が無効です。"); }
	}
	fhead.Offset(int32_t(sizeof(DIBFileHeader) + DIBCoreHeader::Size) + (sizeof(RGBTriple_t) * palsize));
	fhead.FileSize(int32_t(sizeof(DIBFileHeader) + DIBCoreHeader::Size + (sizeof(RGBTriple_t) * palsize) + DIBCoreBitmapEncoder::GetImageLength(header.BitCount, DisplayRectSize(header.Width, header.Height))));
	DIBLoaderHelper::Write(loader, fhead, 0);
	DIBLoaderHelper::Write(loader, DIBCoreHeader::Size, sizeof(DIBFileHeader));
	DIBLoaderHelper::Write(loader, header, sizeof(DIBFileHeader) + sizeof(uint32_t));
	for (auto i: Range<size_t>(0, palsize).get_std_iterator())
	{
		if (palette.size() < i) { DIBLoaderHelper::Write(loader, RGBTriple_t(palette[i]), sizeof(DIBFileHeader) + DIBCoreHeader::Size + (sizeof(RGBTriple_t) * i)); }
		else { DIBLoaderHelper::Write(loader, RGBTriple_t(), sizeof(DIBFileHeader) + DIBCoreHeader::Size + (sizeof(RGBTriple_t) * i)); }
	}
	auto encoder = DIBCoreBitmapEncoder(loader, fhead.Offset(), header.BitCount, DisplayRectSize(header.Width, header.Height));
	switch(header.BitCount)
	{
		case DIBBitDepth::Bit1: { while (encoder.HasValue()) { encoder.Write(DIBPixelData<DIBBitDepth::Bit1>()); } break; }
		case DIBBitDepth::Bit4: { while (encoder.HasValue()) { encoder.Write(DIBPixelData<DIBBitDepth::Bit4>()); } break; }
		case DIBBitDepth::Bit8: { while (encoder.HasValue()) { encoder.Write(DIBPixelData<DIBBitDepth::Bit8>()); } break; }
		case DIBBitDepth::Bit24: { while (encoder.HasValue()) { encoder.Write(DIBPixelPerser::ToPixel24(image.At(encoder.CurrentPos()))); } break; }
		default: { throw std::invalid_argument("BitCountの内容が無効です。"); }
	}
	try
	{
		loader.Sync();
		return DIBCoreBitmap(std::forward<DIBLoader>(loader));
	}
	catch (std::exception e)
	{
		return std::nullopt;
	}
}
DIBCoreBitmap::ValueType DIBCoreBitmap::ConvertToRGB(const DIBCoreBitmapDecoder::ValueType& data) const
{
	switch(ihead.BitCount)
	{
		case DIBBitDepth::Bit1:
		case DIBBitDepth::Bit4:
		case DIBBitDepth::Bit8:
		{
			return palette.at(std::visit([](auto i)->uint32_t { return uint32_t(i); }, data));
		}
		case DIBBitDepth::Bit24:
		{
			return DIBPixelPerser::ToRGB(std::get<DIBPixelData<DIBBitDepth::Bit24>>(data));
		}
		default: { throw InvalidDIBFormatException("情報ヘッダのBitCountの内容が無効です。"); }
	}
}
DIBCoreBitmap::RawDataType DIBCoreBitmap::ConvertToRawData(const DIBCoreBitmapDecoder::ValueType& data) const
{
	switch(ihead.BitCount)
	{
		case DIBBitDepth::Bit1:
		case DIBBitDepth::Bit4:
		case DIBBitDepth::Bit8:
		case DIBBitDepth::Bit24:
		{
			return std::visit([](auto i)->RawDataType { return RawDataType(i); }, data);
		}
		default: { throw InvalidDIBFormatException("情報ヘッダのBitCountの内容が無効です。"); }
	}
}
DIBCoreBitmapDecoder::ValueType DIBCoreBitmap::ConvertToDecoderValue(const ValueType& value) const
{
	switch(ihead.BitCount)
	{
		case DIBBitDepth::Bit1:
		case DIBBitDepth::Bit4:
		case DIBBitDepth::Bit8:
		//	TODO: Implement
		{ throw NotImplementedException(); }
		case DIBBitDepth::Bit24: { return DIBPixelPerser::ToPixel24(value); }
		default: { throw InvalidOperationException("情報ヘッダのBitCountの内容が無効です。"); }
	}
}
DIBCoreBitmapDecoder::ValueType DIBCoreBitmap::ConvertToDecoderValue(const RawDataType& value) const
{
	switch(ihead.BitCount)
	{
		case DIBBitDepth::Bit1: { return DIBPixelData<DIBBitDepth::Bit1>(value); }
		case DIBBitDepth::Bit4: { return DIBPixelData<DIBBitDepth::Bit4>(value); }
		case DIBBitDepth::Bit8: { return DIBPixelData<DIBBitDepth::Bit8>(value); }
		case DIBBitDepth::Bit24: { return DIBPixelData<DIBBitDepth::Bit24>(value); }
		default: { throw InvalidOperationException("情報ヘッダのBitCountの内容が無効です。"); }
	}
}
