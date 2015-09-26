#include "img.h"
#include "engine/asset/asset.h"
#include "engine/component/object.h"
#include <cmath>
#include <box2d/Collision/Shapes/b2PolygonShape.h>

namespace Engine
{
	namespace Script
	{
		DEFINE_PART_TYPE_INFO(CImg);

		CImg::CImg(void)
			: m_aImg(-1)
			, m_tAnchor(0.5f, 0.5f)
			, m_eBlendMode(SDL_BLENDMODE_BLEND)
			, m_eRenderFlip(SDL_FLIP_NONE)
		{
			//this->m_szType = "CImg";
			this->m_priority = Part::EPriorities::IMG;
		}
		/*virtual*/ CImg::~CImg(void)
		{
			this->m_cSprite.Unregister();
		/*	assert(this->m_pAsset);
			if (this->m_aImg != -1)
				this->m_pAsset->UnloadAsset(this->m_aImg, Asset::EAssetType::IMAGE);*/
		}
		CImg& CImg::operator= (const CImg& that)
		{
			IPart::operator=(that);
			this->m_aImg = that.m_aImg;
			this->m_tAnchor = that.m_tAnchor;
			return *this;
		}

		/*virtual*/ void CImg::SetOwner(Object::CObject* const pOwner)
		{
			//assert(!pOwner->IsGroup() && "");
			Part::IPart::SetOwner(pOwner);
		}

		void CImg::SetRBG(const float& r, const float& g, const float& b)
		{
			this->m_tColor.r = r;
			this->m_tColor.g = g;
			this->m_tColor.b = b;
		}
		void CImg::SetAlpha(const float& a)
		{
			this->m_tColor.a = a;
		}

		/*virtual*/ void CImg::Copy(Part::IPart* const that)
		{
			auto cast = dynamic_cast<CImg* const>(that);
			assert(cast && "COPY MAY ONLY BE PERFORMED ON PARTS OF THE SAME TYPE");
			*this = *cast;
		}

		/*virtual*/ void CImg::LoadJson(const Json::JSON& json)
		{
			IRenderPart::LoadJson(json);
			this->m_tAnchor = Type2<float>(json["anchor"]["x"].GetDouble(), json["anchor"]["y"].GetDouble());
			this->m_tColor = json["color"].GetColor();
			std::string filePath = json["img"].GetString();
			Asset::CAsset::Get()->LoadAsset(filePath);
			this->m_aImg = Asset::CAsset::Get()->Aquire(filePath);
			std::string blend_mode = json["blend_mode"].GetString();
			if ("none" == blend_mode)
				this->m_eBlendMode = SDL_BLENDMODE_NONE;
			else if ("blend" == blend_mode)
				this->m_eBlendMode = SDL_BLENDMODE_BLEND;
			else if ("mod" == blend_mode)
				this->m_eBlendMode = SDL_BLENDMODE_MOD;
			else if ("add" == blend_mode)
				this->m_eBlendMode = SDL_BLENDMODE_ADD;
			this->m_szLayer = json["layer"].GetString();
			this->m_cSprite.Register(this->m_szLayer);
		}

		/*virtual*/ void CImg::OnAttach(void)
		{
			this->GetRenderNode();
		}

		/*virtual*/ void CImg::OnMatrixChanged(void)
		{
			this->GetRenderNode();
		}

		/*virtual*/ void CImg::OnVisibilityChanged(const bool visible)
		{
			this->GetRenderNode();
			if (visible)
				this->m_cSprite.Register(this->m_szLayer);
			else
				this->m_cSprite.Unregister();
		}

		/*virtual*/ Render::IRenderNode* CImg::GetRenderNode(void)
		{
			//const CMatrix2D& wmat = this->m_pOwner->CalcWorldMatrix();
			//float zed = this->m_pOwner->CalcWorldZed();
			//this->m_cSprite.position = wmat.GetPosition();
			//this->m_cSprite.scale = wmat.GetScale();
			//this->m_cSprite.rotation = wmat.GetRotationZ();
			//this->m_cSprite.position.z = zed;

			this->m_cSprite.texture			= reinterpret_cast<SDL_Texture*>(Asset::CAsset::Get()->Get(this->m_aImg));
			this->m_cSprite.anchor			= this->m_tAnchor;
			this->m_cSprite.blend_mode		= this->m_eBlendMode;
			this->m_cSprite.clr				= this->m_tColor.SDL();
			this->m_cSprite.flip			= this->m_eRenderFlip;
			this->m_cSprite.realAlpha = this->m_tColor.a;

			memset(&this->m_cSprite.src_rect, null, sizeof(SDL_Rect));
			Uint32 format;
			int access;
			SDL_QueryTexture(this->m_cSprite.texture, &format, &access, &this->m_cSprite.src_rect.w, &this->m_cSprite.src_rect.h);

			return &this->m_cSprite;
		}
	
		/*virtual*/ Engine::Math::AABB CImg::GetAABB(void) const
		{
			Type2<int> size;
			Uint32 format;
			int access;
			SDL_QueryTexture(reinterpret_cast<SDL_Texture*>(Asset::CAsset::Get()->Get(this->m_aImg)), &format, &access, &size.w, &size.h);

			Engine::Math::CMatrix2D mat = this->m_pOwner->CalcWorldMatrix();
			vec2 scale = mat.GetScale();
			float rot = mat.GetRotationZ();
			vec2 pos = mat.GetPosition();

			b2PolygonShape shape;
			shape.SetAsBox(scale.x * size.x / 2.0f, scale.y * size.y / 2.0f, b2Vec2(pos.x, pos.y), rot * DEG_TO_RAD);

			return Math::Convertb2PolygonShapeToAabb(shape);
		}

		const Type2<int> CImg::GetSourceSize(void) const
		{
			Type2<int> size;
			Uint32 format;
			int access;
			SDL_QueryTexture(reinterpret_cast<SDL_Texture*>(Asset::CAsset::Get()->Get(this->m_aImg)), &format, &access, &size.w, &size.h);
			return size;
		}
	}
}