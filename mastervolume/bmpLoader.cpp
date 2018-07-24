#include "stdafx.h"
#include "bmpLoader.h"
#include <stdint.h>

unsigned char* GetBMPPixels(const char *fileName, int width, int height, bool inverse) {
	std::ifstream bmpFile(fileName, std::ifstream::binary);

	if (!bmpFile.is_open())
		return nullptr;

	std::int16_t signature;
	if (!bmpFile.read(reinterpret_cast<char*>(&signature), sizeof(signature)) || signature != 0x4D42)
		return nullptr;

	std::int32_t fileSize;
	if (!bmpFile.read(reinterpret_cast<char*>(&fileSize), sizeof(fileSize)))
		return nullptr;

	std::int32_t pixelsArrayOffset;
	bmpFile.seekg(10);
	if (!bmpFile.read(reinterpret_cast<char*>(&pixelsArrayOffset), sizeof(pixelsArrayOffset)))
		return nullptr;

	if (fileSize - pixelsArrayOffset != width * height * 4)
		return nullptr;

	bmpFile.seekg(pixelsArrayOffset);
	unsigned char *pixels = new (std::nothrow) unsigned char[fileSize - pixelsArrayOffset];
	if (pixels) {
		if (bmpFile.read(reinterpret_cast<char*>(pixels), fileSize - pixelsArrayOffset)) {
			std::int32_t *const pix = reinterpret_cast<std::int32_t*const>(pixels);
			auto getIndex = [](int x, int y, int columns) {return y * columns + x; };

			if (inverse) {
				for (int i = 0; i < width; ++i) {
					for (int j = 0; j < (height + 1) / 2; ++j) {
						std::int32_t &one = pix[getIndex(i, j, width)];
						std::int32_t &other = pix[getIndex(i, height - 1 - j, width)];
						std::swap(one, other);

						if (one == 0xFFFF0000)
							one = 0;
						if (other == 0xFFFF0000)
							other = 0;
					}
				}
			}
		}
	}
	return pixels;
}
