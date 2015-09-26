#include "engine/asset/asset.h"
#include "engine/asset/resource_path.h"
#include "engine/asset/loaders.h"

//#include "proprietary.h"
//#include <string>
//#include <algorithm>
//#include <fstream>


namespace Engine
{
	namespace Asset
	{
		void GetFileString(const std::string& szFilePath, std::string& outPath, std::string& outName, std::string& outExt)
		{
			assert(!szFilePath.empty());
			bool ext = true;
			outPath = outExt = outName = szFilePath;

			for (int i = szFilePath.length() - 1; i >= 0; --i)
			{
				if (ext)
				{
					if (szFilePath[i] == '.')
					{
						ext = false;
						outExt.replace(outExt.begin(), outExt.begin() + i, "");
						outName.replace(outName.begin() + i, outName.end(), "");
					}
				}
				else
				{
					if (szFilePath[i] == '\\' || szFilePath[i] == '/' )
					{
						outPath.replace(outPath.begin() + i + 1, outPath.end(), "");
						outName.replace(outName.begin(), outName.begin() + i + 1, "");
						return;
					}
				}
			}
		}

		std::string GetFilePath(const std::string& szString)
		{
			std::string ret;
			for (int i = szString.length() - 1; i >= 0; --i)
			{
				if (szString[i] == '.')
				{
					return ret;
				}
				ret += szString[i];
			}
			return ret;
		}
		std::string GetFileName(const std::string& szString);
		std::string GetFileExt(const std::string& szString);

		CAsset* CAsset::instance = nullptr;
		CAsset* CAsset::Get(void)
		{
			if (!instance)
				instance = new CAsset();
			return instance;
		}
		void CAsset::DeleteInstance(void)
		{
			if (instance)
			{
				instance->UnloadAll();
				delete instance;
				instance = nullptr;
			}
		}

		/*Private*/
		CAsset::CAsset(void)
		{
			this->m_pRen = NULL;
		}
		CAsset::~CAsset(void)
		{
		}
		slong CAsset::FindAsset(const std::string& szAsset)
		{
			for (ulong i = 0, size = this->m_vAssets.size(); i < size; ++i)
			{
				if (this->m_vAssets[i] && this->m_vAssets[i]->m_szFile == szAsset)
				{
					return i;
				}
			}
			return -1;
		}


		/*Interface*/
		void CAsset::LoadAsset(const std::string& szAsset)
		{
			//Early out
			if (-1 != this->FindAsset(szAsset))
				return;

			std::string prefix = getResourcePath();
			std::string path, name, ext;
			GetFileString(szAsset, path, name, ext);

			//TODO: put switch here to load specific types of data depending on the ext

			void* ptr = null;

			if (ext == ".png")
			{
				ptr = FileLoaderSdlTexture(prefix + szAsset);
			}

			assert(ptr && "Can not find asset.");
			IAsset* pAsset = new IAsset();
			pAsset->m_pData = ptr;
			pAsset->m_szFile = szAsset;

			this->m_vAssets.push_back(pAsset);
		}
		void CAsset::LoadContext(const std::string& szContext)
		{
			assert(false);
		}
		void CAsset::Push(IAsset* const m_pAsset)
		{
			assert(false);
		}
		void CAsset::UnloadAsset(const std::string& szAsset)
		{
			slong index = this->FindAsset(szAsset);

			//Early out
			if (-1 == index)
				return;

			//TODO: Determine unloader
			//Make an array of loaders and unloaders using the asset type

			IAsset* pAsset = this->m_vAssets[index];
			this->m_vAssets[index] = null;
			SDL_DestroyTexture(reinterpret_cast<SDL_Texture*>(pAsset->m_pData));
			delete pAsset;
		}
		void CAsset::UnloadContext(const std::string& szContext)
		{
			assert(false);
		}
		void CAsset::Reload(void)
		{
			assert(false);
		}
		void CAsset::UnloadAll(void)
		{
			for (ulong i = 0, size = this->m_vAssets.size(); i < size; ++i)
			{
				this->UnloadAsset(this->m_vAssets[i]->m_szFile);
			}
		}
		slong CAsset::Aquire(const std::string& szAsset)
		{
			slong ret = this->FindAsset(szAsset);
			assert(ret != -1 && "Asset is not in memory.");
			return ret;
		}
		void CAsset::Release(const std::string& szAsset)
		{
			assert(false);
		}
		void CAsset::Release(const slong& slAsset)
		{
			assert(false);
		}
		void* CAsset::Get(const slong& slAsset)
		{
			assert(slAsset < slong(this->m_vAssets.size()));
			return this->m_vAssets[slAsset]->m_pData;
		}


		///*Interface*/
		//bool CAsset::Init(const char* szPakName)
		//{
		//	std::string Pak_File = getResourcePath() + std::string(szPakName);
		//	if (!Pak::Load(Pak_File.c_str()))
		//	{
		//		SDL_Log("Error - Pak::Load - Unable to load pak file.\n");
		//		return false;
		//	}
		//	return true;
		//}
		//slong CAsset::LoadAsset(const char* szAsset, const EAssetType eAssetType, bool bUsePak)
		//{
		//	slong ret = -1;
		//	switch (eAssetType)
		//	{
		//	case EAssetType::IMAGE:
		//	{
		//		ret = this->LoadImage_List(szAsset);
		//		if (ret == -1)
		//			ret = (bUsePak) ? this->LoadImage_Pak(szAsset) : this->LoadImage_File(szAsset);
		//		break;
		//	}
		//	case EAssetType::SFX:
		//	case EAssetType::MUS:
		//	case EAssetType::VOX:
		//		break;
		//	case EAssetType::FONT:
		//		ret = this->LoadFont_List(szAsset);
		//		if (ret == -1)
		//			ret = (bUsePak) ? this->LoadFont_Pak(szAsset) : this->LoadFont_File(szAsset);
		//		break;
		//	}
		//	return ret;
		//}
		//void CAsset::UnloadAsset(const slong slIdentifier, const EAssetType eAssetType)
		//{
		//	switch (eAssetType)
		//	{
		//	case EAssetType::IMAGE:
		//	{
		//		//Error check
		//		if (slIdentifier < 0 || slIdentifier > slong(this->m_vImages.size()))
		//		{
		//			SDL_Log("Error - CAsset::UnloadAsset - index out of range.\n");
		//			return;
		//		}
		//		this->m_vImages[slIdentifier].m_slRefs--;
		//		if (this->m_vImages[slIdentifier].m_slRefs <= 0)
		//			UnloadImage(slIdentifier);
		//		break;
		//	}
		//	case EAssetType::FONT:
		//	{
		//		//Error check
		//		if (slIdentifier < 0 || slIdentifier > slong(this->m_vFonts.size()))
		//		{
		//			SDL_Log("Error - CAsset::UnloadAsset - index out of range.\n");
		//			return;
		//		}
		//		this->m_vFonts[slIdentifier].m_slRefs--;
		//		if (this->m_vFonts[slIdentifier].m_slRefs <= 0)
		//			UnloadFont(slIdentifier);
		//		break;
		//	}
		//	}
		//}
		//slong CAsset::PushAsset(IAsset pAsset, const EAssetType eAssetType)
		//{
		//	SDL_Log("Error - CAsset::PushAsset - func not written yet.\n");
		//	return -1;
		//}
		//void CAsset::ReloadAssets(const EAssetType eAssetType)
		//{
		//}

		///*Shared*/
		//slong CAsset::FindOpenSlot(const EAssetType eAssetType)
		//{
		//	slong ret = -1;
		//	switch (eAssetType)
		//	{
		//	case EAssetType::IMAGE:
		//	{
		//		ulong size = this->m_vImages.size();
		//		for (ulong i = 0; i < size; ++i)
		//		{
		//			if (this->m_vImages[i].m_slRefs <= 0)
		//				ret = i;
		//		}
		//		break;
		//	}
		//	}
		//	return ret;
		//}

		///*Image*/
		//slong CAsset::LoadImage_List(const char* szAsset)
		//{
		//	//Look for texture in list
		//	ulong size = this->m_vImages.size();
		//	for (ulong i = 0; i < size; ++i)
		//	{
		//		if (strcmp(szAsset, this->m_vImages[i].m_szFile.c_str()) == 0)
		//		{
		//			this->m_vImages[i].m_slRefs++;
		//			return i;
		//		}
		//	}
		//	return -1;
		//}
		//slong CAsset::LoadImage_Pak(const char* szAsset)
		//{
		//	//Check that the renderer is valid
		//	if (this->m_pRen == NULL)
		//	{
		//		SDL_Log("Error - CAsset::LoadImage_Pak - Renderer is invalid.\n");
		//		return -1;
		//	}

		//	//Get the image from the pak file
		//	char* file_buffer = NULL;
		//	ulong file_size = 0;
		//	if (!Pak::GetAsset(szAsset, &file_buffer, file_size))
		//	{
		//		SDL_Log("Error - Asset ' %s ' is not in pak file.\n", szAsset);
		//		return -1;
		//	}

		//	//Load WOPS
		//	SDL_RWops* wops = NULL;
		//	wops = SDL_RWFromMem(file_buffer, file_size);

		//	//Load surface
		//	SDL_Surface* surface = IMG_Load_RW(wops, 0);
		//	if (surface == NULL)
		//	{
		//		SDL_Log("Error - IMG_Load_RW - %s.\n", SDL_GetError());
		//		return -1;
		//	}

		//	//Load the texture from the surface
		//	SDL_Texture* texture = SDL_CreateTextureFromSurface(this->m_pRen, surface);//IMG_LoadTexture_RW(ren, wops, 1 );
		//	if (texture == NULL)
		//	{
		//		SDL_Log("Error - SDL_CreateTextureFromSurface - %s.\n", SDL_GetError());
		//		return -1;
		//	}

		//	//Free the buffer
		//	delete[] file_buffer;

		//	//Create TSprite
		//	TImage tImg;
		//	tImg.m_szFile = szAsset;
		//	tImg.m_pSurface = surface;
		//	tImg.m_pTex = texture;
		//	tImg.m_slRefs = 1;

		//	//Add to list
		//	slong slot = this->FindOpenSlot(EAssetType::IMAGE);
		//	if (slot == -1) //No available slot
		//	{
		//		this->m_vImages.push_back(tImg);
		//		slot = this->m_vImages.size() - 1;
		//	}
		//	else
		//		this->m_vImages[slot] = tImg;

		//	//Return
		//	return slot;
		//}
		//slong CAsset::LoadImage_File(const char* szAsset)
		//{
		//	//Load surface
		//	SDL_Surface* surface = IMG_Load(szAsset);
		//	if (surface == NULL)
		//	{
		//		SDL_Log("Error - IMG_Load - %s.\n", SDL_GetError());
		//		return -1;
		//	}

		//	//Load the texture
		//	SDL_Texture* texture = IMG_LoadTexture(this->m_pRen, szAsset);
		//	if (texture == NULL)
		//	{
		//		SDL_Log("Error - IMG_LoadTexture - %s.\n", SDL_GetError());
		//		return -1;
		//	}

		//	//Create TSprite
		//	TImage tImg;
		//	tImg.m_szFile = szAsset;
		//	tImg.m_pSurface = surface;
		//	tImg.m_pTex = texture;
		//	tImg.m_slRefs = 1;

		//	//Add to list
		//	slong slot = this->FindOpenSlot(EAssetType::IMAGE);
		//	if (slot == -1) //No available slot
		//	{
		//		this->m_vImages.push_back(tImg);
		//		slot = this->m_vImages.size() - 1;
		//	}
		//	else
		//		this->m_vImages[slot] = tImg;

		//	//Return
		//	return slot;
		//}
		//void CAsset::UnloadImage(const slong slIndex)
		//{
		//	//Grab surface and texture
		//	SDL_Surface* pSurface = this->m_vImages[slIndex].m_pSurface;
		//	SDL_Texture* pTexture = this->m_vImages[slIndex].m_pTex;

		//	//Cleanup the slot
		//	this->m_vImages[slIndex] = TImage();

		//	//Free the assets
		//	SDL_FreeSurface(pSurface);
		//	SDL_DestroyTexture(pTexture);
		//}

		///*Font*/
		//slong CAsset::LoadFont_List(const char* szAsset)
		//{
		//	//Look for font in list
		//	ulong size = this->m_vFonts.size();
		//	for (ulong i = 0; i < size; ++i)
		//	{
		//		if (strcmp(szAsset, this->m_vFonts[i].m_szFile.c_str()) == 0)
		//		{
		//			this->m_vFonts[i].m_slRefs++;
		//			return i;
		//		}
		//	}
		//	return -1;
		//}
		//slong CAsset::LoadFont_Pak(const char* szAsset)
		//{
		//	////Two parts - img and font
		//	//std::string img = std::string(szAsset) + std::string(".png");
		//	//std::string font = std::string(szAsset) + std::string(".font");

		//	////Create TFont
		//	//Proprietary::TFont* pFont = new Proprietary::TFont();

		//	////Get the img
		//	//pFont->m_slTex = this->LoadAsset(img.c_str(), EAssetType::IMAGE);

		//	////Error check
		//	//if (pFont->m_slTex == -1)
		//	//{
		//	//	SDL_Log("Error - CAsset::LoadFont_Pak - can't find img.\n");
		//	//	delete pFont;
		//	//	pFont = NULL;
		//	//	return -1;
		//	//}

		//	////Get the font
		//	//char* file_buffer = NULL;
		//	//ulong file_size = 0;
		//	//if (!Pak::GetAsset(font.c_str(), &file_buffer, file_size))
		//	//{
		//	//	SDL_Log("Error - Pak::GetAsset - can't find font.\n");
		//	//	return -1;
		//	//}

		//	////Parse Font
		//	//pFont->Load(file_buffer, file_size);

		//	////Free the buffer
		//	//delete[] file_buffer;

		//	////Create TRune
		//	//TRune tRune;
		//	//tRune.m_szFile = szAsset;
		//	//tRune.m_pFont = pFont;
		//	//tRune.m_slRefs = 1;

		//	////Add to list
		//	//slong slot = this->FindOpenSlot(EAssetType::FONT);
		//	//if (slot == -1) //No available slot
		//	//{
		//	//	this->m_vFonts.push_back(tRune);
		//	//	slot = this->m_vFonts.size() - 1;
		//	//}
		//	//else
		//	//	this->m_vFonts[slot] = tRune;

		//	////Return
		//	//return slot;

		//	return -1;
		//}
		//slong CAsset::LoadFont_File(const char* szAsset)
		//{
		//	SDL_Log("Error - CAsset::LoadFont_File - FUNCTION NOT FINISHED.\n");
		//	return -1;
		//}
		//void CAsset::UnloadFont(const slong slIndex)
		//{
		//	////Grab TFont
		//	//Proprietary::TFont* pFont = this->m_vFonts[slIndex].m_pFont;

		//	////Unload the img
		//	//this->UnloadAsset(pFont->m_slTex, EAssetType::IMAGE);

		//	////Cleanup the slot
		//	//this->m_vFonts[slIndex] = TRune();

		//	////Free the asset
		//	//delete pFont;
		//}

		//SDL_Texture* LoaderSdlTexture(void* data, slong size)
		//{
		//	return null;
		//}
		//IAsset* FileLoaderSdlTexture(std::string szFilePath)
		//{
		//	//Load the texture
		//	SDL_Texture* texture = IMG_LoadTexture(CAsset::GetInstance()->GetSdlRenderer(), szFilePath.c_str());
		//	if (texture == NULL)
		//	{
		//		SDL_Log("Error - IMG_LoadTexture - %s.\n", SDL_GetError());
		//		assert(false);
		//		return null;
		//	}
		//	return texture;
		//}
	}
}