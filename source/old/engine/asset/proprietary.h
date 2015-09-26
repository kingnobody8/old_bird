#pragma once
//TODO add font data type

//namespace Engine
//{
//	namespace Asset
//	{
//		namespace Proprietary
//		{
//			///*Font*/ 
//			////--------------------------------------------------------------------------
//			//const ulong kulNumLetters = 256;
//			//struct TFont
//			//{
//			//	TRect<slong> m_vLetters[kulNumLetters];
//			//	Type2<slong> m_tPix;
//			//	Type2<slong> m_tSet;
//			//	slong		m_slTex;
//
//			//	TFont(void)
//			//	{
//			//		m_slTex = -1;
//			//	}
//			//	void Load(void* const pBuffer, const slong &slSize)
//			//	{
//			//		char* ptr = (char*)(pBuffer);
//			//		ptr += strlen(ptr) + 1;
//
//			//		//Get Pix
//			//		this->m_tPix = *(Type2<slong>*)(ptr);
//			//		ptr += sizeof(Type2<slong>);
//
//			//		//Get Set
//			//		this->m_tSet = *(Type2<slong>*)(ptr);
//			//		ptr += sizeof(Type2<slong>);
//
//			//		//Go through the letters
//			//		while (ptr - (char*)(pBuffer) < slSize)
//			//		{
//			//			//Get index
//			//			slong index = *(slong*)(ptr);
//			//			ptr += sizeof(index);
//
//			//			//Get rect
//			//			TRect<slong> rect = *(TRect<slong>*)(ptr);
//			//			ptr += sizeof(TRect<slong>);
//
//			//			//Set the letter
//			//			this->m_vLetters[index] = rect;
//			//		}
//			//	}
//			//};
//			////--------------------------------------------------------------------------
//		}
//	}
//}