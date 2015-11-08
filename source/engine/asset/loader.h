#pragma once
#include "types.h"
#include "json.h"

namespace engine
{
	namespace asset
	{
		const TBuffer FileLoaderData(const std::string& szFilePath);

		const util::JSON FileLoaderJson(const std::string& szFilePath);
		const util::JSON DataLoaderJson(const TBuffer& data);

		SDL_Texture* const FileLoaderSdlTexture(const std::string& szFilePath);
		SDL_Texture* const DataLoaderSdlTexture(const TBuffer& data);
	}
}