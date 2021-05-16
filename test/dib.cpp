//	stationaryorbit.test.graphics:/dib
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
#include <iostream>
#include <fstream>
#include <chrono>
#include <memory>
#include "stationaryorbit/graphics-dib.bmpimage.hpp"
#include "stationaryorbit/graphics-core.deformation.hpp"
using namespace zawa_ch::StationaryOrbit;
using namespace zawa_ch::StationaryOrbit::Graphics;

RGB8Pixmap_t image;
DIB::DIBInfoHeader ihead;

void Read();
void Write();
void Write16();
void WriteCoreProfile();
void FripV();
void FripH();
void TurnR();
void TurnL();
void TurnI();
void Crop();
void Resize1();
void Resize2();
void Mono();
void Blend();

void Test_DIB()
{
	std::chrono::duration<double> elapsed;
	auto start = std::chrono::steady_clock::now();
	Read();
	elapsed = std::chrono::steady_clock::now() - start;
	std::cout << "File read: " << elapsed.count() << "sec." << std::endl;

	start = std::chrono::steady_clock::now();
	Write();
	elapsed = std::chrono::steady_clock::now() - start;
	std::cout << "File write: " << elapsed.count() << "sec." << std::endl;

	start = std::chrono::steady_clock::now();
	Write16();
	elapsed = std::chrono::steady_clock::now() - start;
	std::cout << "File write at 16bit: " << elapsed.count() << "sec." << std::endl;

	start = std::chrono::steady_clock::now();
	WriteCoreProfile();
	elapsed = std::chrono::steady_clock::now() - start;
	std::cout << "File write with CoreHeader Profile: " << elapsed.count() << "sec." << std::endl;

	start = std::chrono::steady_clock::now();
	FripV();
	elapsed = std::chrono::steady_clock::now() - start;
	std::cout << "Vertical frip: " << elapsed.count() << "sec." << std::endl;

	start = std::chrono::steady_clock::now();
	FripH();
	elapsed = std::chrono::steady_clock::now() - start;
	std::cout << "Horizonal frip: " << elapsed.count() << "sec." << std::endl;

	start = std::chrono::steady_clock::now();
	TurnR();
	elapsed = std::chrono::steady_clock::now() - start;
	std::cout << "Right turn: " << elapsed.count() << "sec." << std::endl;

	start = std::chrono::steady_clock::now();
	TurnL();
	elapsed = std::chrono::steady_clock::now() - start;
	std::cout << "Left turn: " << elapsed.count() << "sec." << std::endl;

	start = std::chrono::steady_clock::now();
	TurnI();
	elapsed = std::chrono::steady_clock::now() - start;
	std::cout << "Invert turn: " << elapsed.count() << "sec." << std::endl;

	start = std::chrono::steady_clock::now();
	Crop();
	elapsed = std::chrono::steady_clock::now() - start;
	std::cout << "Cropping: " << elapsed.count() << "sec." << std::endl;

	start = std::chrono::steady_clock::now();
	Resize1();
	elapsed = std::chrono::steady_clock::now() - start;
	std::cout << "Shrink: " << elapsed.count() << "sec." << std::endl;

	start = std::chrono::steady_clock::now();
	Resize2();
	elapsed = std::chrono::steady_clock::now() - start;
	std::cout << "Expand: " << elapsed.count() << "sec." << std::endl;

	start = std::chrono::steady_clock::now();
	Mono();
	elapsed = std::chrono::steady_clock::now() - start;
	std::cout << "Monotone: " << elapsed.count() << "sec." << std::endl;

	start = std::chrono::steady_clock::now();
	Blend();
	elapsed = std::chrono::steady_clock::now() - start;
	std::cout << "Colorblend: " << elapsed.count() << "sec." << std::endl;
}

void Read()
{
	///	読み込みを行うWindowsビットマップファイル。
	const char* ifile = "input.bmp";
	// ファイルを開く
	auto loader = DIB::DIBFileLoader(ifile, std::ios_base::in | std::ios_base::binary);
	// ビットマップをロードする
	switch(loader.HeaderSize())
	{
		case DIB::DIBInfoHeader::Size:
		{
			auto bitmap = DIB::DIBInfoBitmap(std::move(loader));
			ihead = bitmap.InfoHead();
			image = bitmap.ToPixmap();
			break;
		}
		default: { throw std::runtime_error("Can't read file."); }
	}
}

void Write()
{
	const char* ofile = "output.bmp";
	// ファイルを開く
	auto loader = DIB::DIBFileLoader(ofile, std::ios_base::out | std::ios_base::binary);
	// ビットマップを書き込む
	DIB::DIBInfoBitmap::Generate(std::move(loader), ihead, image);
}

void Write16()
{
	const char* ofile = "output16.bmp";
	// ヘッダの準備
	auto whead = ihead;
	whead.Compression = DIB::DIBCompressionMethod::RGB;
	whead.BitCount = DIB::DIBBitDepth::Bit16;
	whead.SizeImage = DIB::DIBRGBEncoder::GetImageLength(whead.BitCount, DisplayRectSize(whead.Width, whead.Height));
	// ファイルを開く
	auto loader = DIB::DIBFileLoader(ofile, std::ios_base::out | std::ios_base::binary);
	// ビットマップを書き込む
	DIB::DIBInfoBitmap::Generate(std::move(loader), whead, image);
}

void WriteCoreProfile()
{
	const char* ofile = "output_core.bmp";
	// ヘッダの準備
	auto whead = DIB::DIBCoreHeader();
	whead.BitCount = ihead.BitCount;
	whead.Height = ihead.Height;
	whead.Width = ihead.Width;
	whead.Planes = 1;
	// ファイルを開く
	auto loader = DIB::DIBFileLoader(ofile, std::ios_base::out | std::ios_base::binary);
	// ビットマップを書き込む
	DIB::DIBCoreBitmap::Generate(std::move(loader), whead, image);
}

void FripV()
{
	const char* ofile = "output_fripv.bmp";
	// 画像を上下フリップ
	auto flipedimage = ImageVerticalFlip(image);
	auto alignedimage = ImageAlign(flipedimage);
	// ファイルを開く
	auto loader = DIB::DIBFileLoader(ofile, std::ios_base::out | std::ios_base::binary);
	// ビットマップを書き込む
	DIB::DIBInfoBitmap::Generate(std::move(loader), ihead, alignedimage);
}

void FripH()
{
	const char* ofile = "output_friph.bmp";
	// 画像を左右フリップ
	auto flipedimage = ImageHorizonalFlip(image);
	auto alignedimage = ImageAlign(flipedimage);
	// ファイルを開く
	auto loader = DIB::DIBFileLoader(ofile, std::ios_base::out | std::ios_base::binary);
	// ビットマップを書き込む
	DIB::DIBInfoBitmap::Generate(std::move(loader), ihead, alignedimage);
}

void TurnR()
{
	const char* ofile = "output_turnr.bmp";
	// 画像を右回転
	auto turnedimage = ImageRightTurn(image);
	auto alignedimage = ImageAlign(turnedimage);
	// ヘッダの準備
	auto whead = ihead;
	whead.Width = alignedimage.Size().width();
	whead.Height = alignedimage.Size().height();
	whead.Compression = DIB::DIBCompressionMethod::RGB;
	whead.SizeImage = DIB::DIBRGBEncoder::GetImageLength(whead.BitCount, DisplayRectSize(whead.Width, whead.Height));
	// ファイルを開く
	auto loader = DIB::DIBFileLoader(ofile, std::ios_base::out | std::ios_base::binary);
	// ビットマップを書き込む
	DIB::DIBInfoBitmap::Generate(std::move(loader), whead, alignedimage);
}

void TurnL()
{
	const char* ofile = "output_turnl.bmp";
	// 画像を左回転
	auto turnedimage = ImageLeftTurn(image);
	auto alignedimage = ImageAlign(turnedimage);
	// ヘッダの準備
	auto whead = ihead;
	whead.Width = alignedimage.Size().width();
	whead.Height = alignedimage.Size().height();
	whead.Compression = DIB::DIBCompressionMethod::RGB;
	whead.SizeImage = DIB::DIBRGBEncoder::GetImageLength(whead.BitCount, DisplayRectSize(whead.Width, whead.Height));
	// ファイルを開く
	auto loader = DIB::DIBFileLoader(ofile, std::ios_base::out | std::ios_base::binary);
	// ビットマップを書き込む
	DIB::DIBInfoBitmap::Generate(std::move(loader), whead, alignedimage);
}

void TurnI()
{
	const char* ofile = "output_turni.bmp";
	// 画像を180°回転
	auto turnedimage = ImageInvertTurn(image);
	auto alignedimage = ImageAlign(turnedimage);
	// ファイルを開く
	auto loader = DIB::DIBFileLoader(ofile, std::ios_base::out | std::ios_base::binary);
	// ビットマップを書き込む
	DIB::DIBInfoBitmap::Generate(std::move(loader), ihead, alignedimage);
}

void Crop()
{
	const char* ofile = "output_crop.bmp";
	auto rec = DisplayRectangle(100, 100, image.Size().width() - 200, image.Size().height() - 200);
	// 画像を切り抜き
	auto croppedimage = ImageCropping(image, rec);
	auto alignedimage = ImageAlign(croppedimage);
	// ヘッダの準備
	auto whead = ihead;
	whead.Width = rec.Width();
	whead.Height = rec.Height();
	whead.SizeImage = (((uint16_t(whead.BitCount) * whead.Width) + 31) / 32 * 4) * whead.Height;
	// ファイルを開く
	auto loader = DIB::DIBFileLoader(ofile, std::ios_base::out | std::ios_base::binary);
	// ビットマップを書き込む
	DIB::DIBInfoBitmap::Generate(std::move(loader), whead, alignedimage);
}

void Resize1()
{
	const char* ofile = "output_resize1.bmp";
	const float resizefactor = 0.5f;
	auto newsize = DisplayRectSizeF(image.Size().width() * resizefactor, image.Size().height() * resizefactor);
	// 画像をリサイズ
	auto clampedimage = ImageClamp(image);
	auto resizedimage = ImageScaling<RGB8_t>(clampedimage, ImageInterpolation::Bilinear<RGB8_t>, resizefactor);
	// ヘッダの準備
	auto whead = ihead;
	whead.Width = newsize.width();
	whead.Height = newsize.height();
	whead.SizeImage = (((uint16_t(whead.BitCount) * whead.Width) + 31) / 32 * 4) * whead.Height;
	// ファイルを開く
	auto loader = DIB::DIBFileLoader(ofile, std::ios_base::out | std::ios_base::binary);
	// ビットマップを書き込む
	DIB::DIBInfoBitmap::Generate(std::move(loader), whead, resizedimage);
}

void Resize2()
{
	const char* ofile = "output_resize2.bmp";
	const float resizefactor = 2.0f;
	auto newsize = DisplayRectSizeF(image.Size().width() * resizefactor, image.Size().height() * resizefactor);
	// 画像をリサイズ
	auto clampedimage = ImageClamp(image);
	auto resizedimage = ImageScaling<RGB8_t>(clampedimage, ImageInterpolation::NearestNeighbor<RGB8_t>, resizefactor);
	// ヘッダの準備
	auto whead = ihead;
	whead.Width = newsize.width();
	whead.Height = newsize.height();
	whead.SizeImage = (((uint16_t(whead.BitCount) * whead.Width) + 31) / 32 * 4) * whead.Height;
	// ファイルを開く
	auto loader = DIB::DIBFileLoader(ofile, std::ios_base::out | std::ios_base::binary);
	// ビットマップを書き込む
	DIB::DIBInfoBitmap::Generate(std::move(loader), whead, resizedimage);
}

void Mono()
{
	const char* ofile = "output_mono.bmp";
	// 画像をモノトーン化
	auto monoimage = RGB8Pixmap_t::Convert<RGB8_t>(image, [](const RGB8_t from)-> RGB8_t { return ColorConvert::ToRgbFromLuminance(ColorConvert::ToLuminanceFromSrgb(from)); });
	// ファイルを開く
	auto loader = DIB::DIBFileLoader(ofile, std::ios_base::out | std::ios_base::binary);
	// ビットマップを書き込む
	DIB::DIBInfoBitmap::Generate(std::move(loader), ihead, monoimage);
}

void Blend()
{
	const char* ofile = "output_colorblend.bmp";
	// 色ブレンディング
	auto source = ColorFillImage<RGB8_t>(RGB8_t(Proportion8_t(1.0f), Proportion8_t(0.0f), Proportion8_t(0.0f)), DisplayRectangle(DisplayPoint(0, 0), DisplayRectSize(ihead.Width, ihead.Height)));
	auto blendimage = ColorBlendImage(ColorOverlayBlender<RGB8_t>(), image, source);
	// ファイルを開く
	auto loader = DIB::DIBFileLoader(ofile, std::ios_base::out | std::ios_base::binary);
	// ビットマップを書き込む
	DIB::DIBInfoBitmap::Generate(std::move(loader), ihead, blendimage);
}
