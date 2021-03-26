//	stationaryorbit.graphics-dib:/dibloader
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
#include "stationaryorbit/graphics-dib/dibloader.hpp"
using namespace zawa_ch::StationaryOrbit;
using namespace zawa_ch::StationaryOrbit::Graphics::DIB;

DIBFileLoader::DIBFileLoader() : stream(), fhead(), headersize() {}
DIBFileLoader::DIBFileLoader(std::fstream&& stream) : stream(std::move(stream)), fhead(), headersize() { LoadHead(); }
DIBFileLoader::DIBFileLoader(const char* filename, std::ios_base::openmode mode) : DIBFileLoader(std::move(std::fstream(filename, mode))) {}
DIBFileLoader::DIBFileLoader(const std::string& filename, std::ios_base::openmode mode) : DIBFileLoader(std::move(std::fstream(filename, mode))) {}
bool DIBFileLoader::IsEnable() const { return fhead.CheckFileHeader(); }
void DIBFileLoader::Sync() noexcept
{
	Flush();
	LoadHead();
}
void DIBFileLoader::Read(char* dest, size_t pos, size_t length)
{
	if (stream.bad()) { throw InvalidOperationException("ストリームの状態が無効です。"); }
	if (stream.tellg() != pos)
	{
		if (stream.seekg(pos).fail()) { stream.clear(); throw std::ios_base::failure("ストリームのシークに失敗しました。"); }
	}
	if (stream.read(dest, length).fail())
	{
		if (stream.eof()) { stream.clear(); throw InvalidDIBFormatException("データの読み取り中にストリーム終端に到達しました。"); }
		stream.clear();
		throw std::ios_base::failure("ストリームの読み取りに失敗しました。");
	}
}
void DIBFileLoader::Write(const char* source, size_t pos, size_t length)
{
	if (stream.bad()) { throw InvalidOperationException("ストリームの状態が無効です。"); }
	if (stream.tellp() != pos)
	{
		if (stream.seekp(pos).fail()) { stream.clear(); throw std::ios_base::failure("ストリームのシークに失敗しました。"); }
	}
	if (stream.write(source, length).fail()) { stream.clear(); throw std::ios_base::failure("ストリームの書き込みに失敗しました。"); }
}
void DIBFileLoader::LoadHead() noexcept
{
	if (stream.bad()) { return; }
	//	読み取り準備
	auto sentry = std::istream::sentry(stream, true);
	if (!sentry) { return; }
	//	ファイル先頭へシーク
	if (stream.seekg(0).fail()) { return; }
	//	ファイルヘッダ・情報ヘッダサイズを読み取り
	if (stream.read((char*)&fhead, sizeof(DIBFileHeader)).fail()) { stream.clear(); return; }
	if (stream.read((char*)&headersize, sizeof(int32_t)).fail()) { stream.clear(); return; }
	stream.clear();
}
void DIBFileLoader::Flush() noexcept
{
	if (stream.bad()) { return; }
	//	出力操作用sentryを構築、その場で破棄する
	(void)std::ostream::sentry(stream);
	stream.clear();
}
