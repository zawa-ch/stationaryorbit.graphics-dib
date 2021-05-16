//	stationaryorbit.graphics-dib:/rgbdecoder
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
#include "stationaryorbit/graphics-dib/rgbdecoder.hpp"
using namespace zawa_ch::StationaryOrbit;
using namespace zawa_ch::StationaryOrbit::Graphics::DIB;

DIBRGBDecoder::DIBRGBDecoder(DIBLoader& loader, size_t offset, DIBBitDepth bitdepth, const DisplayRectSize& size)
	: loader(loader), offset(offset), bitdepth(bitdepth), size(size), length(size.width() * size.height()), pixellength(DIBRGBEncoder::GetPxLength(bitdepth)), stridelength(DIBRGBEncoder::GetStrideLength(bitdepth, size))
{
	Reset();
}
bool DIBRGBDecoder::Next()
{
	if (IsAfterEnd()) { return false; }
	++current;
	if (IsAfterEnd()) { current = length; return false; }
	current_value = Get(current);
	return true;
}
bool DIBRGBDecoder::Next(const IteratorTraits::IteratorDiff_t& count)
{
	if (IsAfterEnd()) { return false; }
	current += count;
	if (IsAfterEnd()) { current = length; return false; }
	current_value = Get(current);
	return true;
}
bool DIBRGBDecoder::Previous()
{
	if (IsBeforeBegin()) { return false; }
	--current;
	if (IsBeforeBegin()) { current = -1; return false; }
	current_value = Get(current);
	return true;
}
bool DIBRGBDecoder::Previous(const IteratorTraits::IteratorDiff_t& count)
{
	if (IsBeforeBegin()) { return false; }
	current -= count;
	if (IsBeforeBegin()) { current = -1; return false; }
	current_value = Get(current);
	return true;
}
void DIBRGBDecoder::JumpTo(const DisplayPoint& pos)
{
	if ( (pos.X() < 0)||(pos.Y() < 0) ) { throw std::invalid_argument("posに指定されている座標が無効です。"); }
	current = ResolveIndex(pos);
	current_value = Get(ResolveIndex(pos));
}
void DIBRGBDecoder::Reset() { Reset(IteratorOrigin::Begin); }
void DIBRGBDecoder::Reset(const IteratorOrigin& origin)
{
	switch(origin)
	{
		default:
		case IteratorOrigin::Begin: { current = 0; break; }
		case IteratorOrigin::End: { current = length - 1; break; }
	}
	current_value = Get(current);
}
bool DIBRGBDecoder::HasValue() const { return (0 <= current)&&(current < length); }
bool DIBRGBDecoder::IsBeforeBegin() const { return current < 0; }
bool DIBRGBDecoder::IsAfterEnd() const { return length <= current; }
Graphics::DisplayPoint DIBRGBDecoder::CurrentPos() const { return ResolvePos(current); }
DIBRGBDecoder::ValueType DIBRGBDecoder::Current() const { if (HasValue()) { return current_value; } else { throw InvalidOperationException("このイテレータは領域の範囲外を指しています。"); } }
void DIBRGBDecoder::Write(const ValueType& value)
{
	size_t tgt = offset + ResolveOffset(current);
	switch(bitdepth)
	{
		case DIBBitDepth::Bit1: { DIBLoaderHelper::Write(loader, std::get<DIBPixelData<DIBBitDepth::Bit1>>(value), tgt); break; }
		case DIBBitDepth::Bit4: { DIBLoaderHelper::Write(loader, std::get<DIBPixelData<DIBBitDepth::Bit4>>(value), tgt); break; }
		case DIBBitDepth::Bit8: { DIBLoaderHelper::Write(loader, std::get<DIBPixelData<DIBBitDepth::Bit8>>(value), tgt); break; }
		case DIBBitDepth::Bit16: { DIBLoaderHelper::Write(loader, std::get<DIBPixelData<DIBBitDepth::Bit24>>(value), tgt); break; }
		case DIBBitDepth::Bit24: { DIBLoaderHelper::Write(loader, std::get<DIBPixelData<DIBBitDepth::Bit24>>(value), tgt); break; }
		case DIBBitDepth::Bit32: { DIBLoaderHelper::Write(loader, std::get<DIBPixelData<DIBBitDepth::Bit32>>(value), tgt); break; }
		default: { throw InvalidOperationException("情報ヘッダのBitCountの内容が無効です。"); }
	}
	current_value = value;
}
IteratorTraits::IteratorDiff_t DIBRGBDecoder::Distance(const DIBRGBDecoder& other) const { return current - other.current; }
bool DIBRGBDecoder::Equals(const DIBRGBDecoder& other) const { return current == other.current; }
int DIBRGBDecoder::Compare(const DIBRGBDecoder& other) const
{
	if (current == other.current) { return 0; }
	else if (other.current < current) { return 1; }
	else { return -1; }
}
DIBRGBDecoder::ValueType DIBRGBDecoder::Get(size_t index)
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
		case DIBBitDepth::Bit16:
		{
			auto result = DIBPixelData<DIBBitDepth::Bit16>();
			DIBLoaderHelper::Read(loader, result, tgt);
			return ValueType(result);
		}
		case DIBBitDepth::Bit24:
		{
			auto result = DIBPixelData<DIBBitDepth::Bit24>();
			DIBLoaderHelper::Read(loader, result, tgt);
			return ValueType(result);
		}
		case DIBBitDepth::Bit32:
		{
			auto result = DIBPixelData<DIBBitDepth::Bit32>();
			DIBLoaderHelper::Read(loader, result, tgt);
			return ValueType(result);
		}
		default: { throw InvalidOperationException("情報ヘッダのBitCountの内容が無効です。"); }
	}
}
size_t DIBRGBDecoder::ResolveIndex(const DisplayPoint& pos) const { return ((size.height() - 1 - pos.Y()) * size.width()) + pos.X(); }
Graphics::DisplayPoint DIBRGBDecoder::ResolvePos(size_t index) const { return DisplayPoint(index % size.width(), size.height() - 1 - (index / size.width())); }
size_t DIBRGBDecoder::ResolveOffset(const DisplayPoint& pos) const
{
	if ( (pos.X() < 0)||(pos.Y() < 0) ) { throw std::invalid_argument("posに指定されている座標が無効です。"); }
	if ( (size.width() <= pos.X())||(size.height() <= pos.Y()) ) { throw std::out_of_range("指定された座標はこの画像領域を超えています。"); }
	return (stridelength * (size.height() - 1 - pos.Y())) + (pixellength * pos.X());
}
size_t DIBRGBDecoder::ResolveOffset(size_t index) const
{
	if (length <= index) { throw std::out_of_range("指定されたインデックスはこの画像領域を超えています。"); }
	return (stridelength * (index / size.width())) + (pixellength * (index % size.width()));
}

DIBRGBEncoder::DIBRGBEncoder(DIBLoader& loader, size_t offset, DIBBitDepth bitdepth, const DisplayRectSize& size)
	: loader(loader), offset(offset), bitdepth(bitdepth), size(size), length(size.width() * size.height()), current(0), pixellength(GetPxLength(bitdepth)), stridelength(GetStrideLength(bitdepth, size))
{}
void DIBRGBEncoder::Write(const ValueType& value)
{
	size_t tgt = offset + ResolveOffset(ResolvePos(current));
	switch(bitdepth)
	{
		case DIBBitDepth::Bit1: { DIBLoaderHelper::Write(loader, std::get<DIBPixelData<DIBBitDepth::Bit1>>(value), tgt); break; }
		case DIBBitDepth::Bit4: { DIBLoaderHelper::Write(loader, std::get<DIBPixelData<DIBBitDepth::Bit4>>(value), tgt); break; }
		case DIBBitDepth::Bit8: { DIBLoaderHelper::Write(loader, std::get<DIBPixelData<DIBBitDepth::Bit8>>(value), tgt); break; }
		case DIBBitDepth::Bit16: { DIBLoaderHelper::Write(loader, std::get<DIBPixelData<DIBBitDepth::Bit16>>(value), tgt); break; }
		case DIBBitDepth::Bit24: { DIBLoaderHelper::Write(loader, std::get<DIBPixelData<DIBBitDepth::Bit24>>(value), tgt); break; }
		case DIBBitDepth::Bit32: { DIBLoaderHelper::Write(loader, std::get<DIBPixelData<DIBBitDepth::Bit32>>(value), tgt); break; }
		default: { throw InvalidOperationException("情報ヘッダのBitCountの内容が無効です。"); }
	}
	if (CurrentPos().X() == (size.width() - 1))
	{
		size_t ci = size.width() * pixellength;
		for (auto i: Range<size_t>(ci, stridelength).GetStdIterator()) { DIBLoaderHelper::Write(loader, char(), offset + i); }
	}
	++current;
}
void DIBRGBEncoder::Reset() { current = 0; }
bool DIBRGBEncoder::HasValue() const { return (0 <= current)&&(current < length); }
Graphics::DisplayPoint DIBRGBEncoder::CurrentPos() const { return ResolvePos(current); }
bool DIBRGBEncoder::Equals(const DIBRGBEncoder& other) const { return current == other.current; }
int DIBRGBEncoder::Compare(const DIBRGBEncoder& other) const 
{
	if (current == other.current) { return 0; }
	else if (other.current < current) { return 1; }
	else { return -1; }
}
size_t DIBRGBEncoder::ResolveIndex(const DisplayPoint& pos) const { return ((size.height() - 1 - pos.Y()) * size.width()) + pos.X(); }
Graphics::DisplayPoint DIBRGBEncoder::ResolvePos(size_t index) const { return DisplayPoint(index % size.width(), size.height() - 1 - (index / size.width())); }
size_t DIBRGBEncoder::ResolveOffset(const DisplayPoint& pos) const
{
	if ( (pos.X() < 0)||(pos.Y() < 0) ) { throw std::invalid_argument("posに指定されている座標が無効です。"); }
	if ( (size.width() <= pos.X())||(size.height() <= pos.Y()) ) { throw std::out_of_range("指定された座標はこの画像領域を超えています。"); }
	return (stridelength * (size.height() - 1 - pos.Y())) + (pixellength * pos.X());
}
size_t DIBRGBEncoder::GetPxLength(DIBBitDepth bitdepth) { return (uint16_t(bitdepth) + 7) / 8; }
size_t DIBRGBEncoder::GetStrideLength(DIBBitDepth bitdepth, const DisplayRectSize& size) { return (((GetPxLength(bitdepth) * size.width()) + 3) / 4) * 4; }
size_t DIBRGBEncoder::GetImageLength(DIBBitDepth bitdepth, const DisplayRectSize& size) { return GetStrideLength(bitdepth, size) * size.height(); }
