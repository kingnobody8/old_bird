#pragma once
#include "engine/util/util.h"
using namespace Engine::Util;
#include <string>

namespace Engine
{
	namespace Asset
	{
		struct TData
		{
			void* buffer;
			slong size;
			TData(void)
			{
				this->buffer = null;
				this->size = 0;
			}
		};

		struct IAsset
		{
			std::string		m_szFile;
			slong			m_slRefs;
			void*			m_pData;
			IAsset(void)
			{
				m_slRefs = 0;
				m_pData = null;
			}
		};

		enum EAssetType { IMG, FONT, ANIM, SFX, MUS, VOX, NUM_ASSET_TYPES };


		

		//struct AssetSdlTexture : public IAsset
		//{
		//	int width;
		//	int height;
		//	SDL_Texture* pTex;
		//	AssetSdlTexture(void)
		//	{
		//		width = height = 0;
		//		pTex = null;
		//	}
		//};


		//Container for SDL Textures & SDL Surfaces
		//struct TImage : public IAsset
		//{
		//	SDL_Texture*	m_pTex;
		//	SDL_Surface*	m_pSurface;
		//	TImage(void)
		//	{
		//		m_pTex = NULL;
		//		m_pSurface = NULL;
		//	}
		//};
		//
		////Container for TFont
		//struct TRune : public IAsset
		//{
		//	//Proprietary::TFont*		m_pFont;
		//
		//	TRune(void)
		//	{
		//		//m_pFont = NULL;
		//	}
		//};
	}
}