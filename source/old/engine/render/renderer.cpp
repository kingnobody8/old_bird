#include "engine/render/renderer.h"
#include "engine/component/object.h"
#include "engine/render/render_node.h"
#include "engine/script/render_part.h"
#include "engine/engine.h"
#include <stack>
#include <algorithm>
#include <list>


namespace Engine
{
	using namespace Object;

	namespace Render
	{
		bool RenderSort(IRenderNode* lhs, IRenderNode* rhs)
		{
			return lhs->GetZed() < rhs->GetZed();
		}

		CRenderer* CRenderer::instance = nullptr;
		CRenderer* CRenderer::Get(void)
		{
			if (!instance)
				instance = new CRenderer();
			return instance;
		}
		void CRenderer::DeleteInstance(void)
		{
			if (instance)
			{
				instance->Exit();
				delete instance;
				instance = nullptr;
			}
		}
		CRenderer::CRenderer()
			: m_pRen(null)
			, m_pWin(null)
		{
		}
		CRenderer::~CRenderer()
		{
		}


		//TODO rename this or render node (too many nodes!)
		typedef std::pair<Object::CObject*, bool> Node;

		IRenderNode* CRenderer::FindRenderNode(Object::CObject* const pObj)
		{
			std::vector<Part::IPart*> vec = pObj->GetParts();
			for (int i = 0; i < (int)vec.size(); ++i)
			{
				if (vec[i]->IsRenderable())
				{
					Script::IRenderPart* pRenderPart = static_cast<Script::IRenderPart*>(vec[i]);
					return pRenderPart->GetRenderNode();
				}
			}
			return null;
		}

		void CRenderer::Init(void)
		{
			//Setup SDL
			this->SetupSDL();

			//Create Cameras
			CCamera* pWorld = new CCamera(Engine::physical_dims);
			CCamera* pUi = new CCamera(Engine::physical_dims);
			m_vCameras.insert(std::pair<std::string, CCamera*>("world", pWorld));
			m_vCameras.insert(std::pair<std::string, CCamera*>("ui", pUi));

			//Create Layers
			m_vLayers.insert(std::pair <std::string, CRenderLayer*>("world", new CRenderLayer(pWorld)));
			m_vLayers.insert(std::pair <std::string, CRenderLayer*>("ui", new CRenderLayer(pUi)));
		}

		void CRenderer::Exit(void)
		{
			for (auto iter = m_vCameras.begin(); iter != m_vCameras.end(); ++iter)
			{
				delete (*iter).second;
			}
			m_vCameras.clear();

			for (auto iter = m_vLayers.begin(); iter != m_vLayers.end(); ++iter)
			{
				delete (*iter).second;
			}
			m_vLayers.clear();
		}

		void CRenderer::SetupSDL(void)
		{
			//Create Window
#ifdef MOBILE
			SDL_DisplayMode mode;
			SDL_GetCurrentDisplayMode(0, &mode);
			//this->m_tPhysicalScreenSize = Type2<slong>(mode.w, mode.h);
			Uint32 win_flag = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN;
			pWin = SDL_CreateWindow("Mako", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mode.w, mode.h, win_flag);
#else
			Uint32 win_flag = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;
			m_pWin = SDL_CreateWindow("Mako", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Engine::physical_dims.x, Engine::physical_dims.y, win_flag);
#endif
			//Error check
			if (!m_pWin)
			{
				SDL_Log("Error - SDL_CreateWindow - %s.\n", SDL_GetError());
				assert(false);
			}

			//TODO: figure out how to use open gl with SDL
			//Create an OpenGL context
			//SDL_GLContext pContext = NULL;// SDL_GL_CreateContext(pWindow);
			//if (!pContext)
			//{
			//	SDL_Log("Error - SDL_GL_CreateContext - %s.\n", SDL_GetError());
			//	return 3;
			//}

			SDL_Log("Initialized SDL Window");

			//Create SDL Renderer
			m_pRen = SDL_CreateRenderer(m_pWin, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

			//Error Check
			if (!m_pRen)
			{
				SDL_Log("Error - SDL_CreateRenderer - %s.\n", SDL_GetError());
				assert(false);
			}

			//TODO we need a real way to handle screen sizes
			//SDL_RenderSetLogicalSize(pRen, 1080, 1920);
		}

		void CRenderer::Update(const Util::Time& dt)
		{
			for (auto iter = m_vCameras.begin(); iter != m_vCameras.end(); ++iter)
			{
				(*iter).second->Update(dt);
			}
		}

		void CRenderer::Render(Object::CGroup* const pRoot)
		{
			if (!pRoot)
			{
				NullRootRender();
				return;
			}


			//TODO
			/*
				go through each layer calling render on it
				each layer should have a list of render nodes
				the layer should do a frustum cull
				then the layer should sort the nodes by z
				then the layer should call render on each node in order
				*/


			//Primer
			SDL_SetRenderDrawColor(this->m_pRen, 0, 0, 0, 255);
			SDL_SetRenderTarget(this->m_pRen, NULL);
			SDL_RenderClear(this->m_pRen);

			//SDL_Rect rect;
			//rect.x = rect.y = 0;
			//SDL_GetRendererOutputSize(this->m_pRen, &rect.w, &rect.h);
			//SDL_SetRenderDrawColor(this->m_pRen, 32, 32, 32, 255);
			//SDL_RenderFillRect(this->m_pRen, &rect);

			//Set all node positions
			std::list<Render::IRenderNode*> vec_sprites;
			Math::CStackMatrix stack_mat;

			//Depth first traversal
			std::stack<Node> stack;
			stack.push(Node(pRoot, false));
			float zed = 0;
			while (!stack.empty())
			{
				Node& node = stack.top();
				if (!node.second)
				{
					stack_mat.Push(node.first->GetLocalMatrix());
					zed += node.first->GetLocalZed();

					//add render nodes (if any)
					std::vector<Part::IPart*> vec = node.first->GetParts();
					for (int i = 0; i < (int)vec.size(); ++i)
					{
						if (vec[i]->IsRenderable())
						{
							Script::IRenderPart* pRenderPart = static_cast<Script::IRenderPart*>(vec[i]);
							IRenderNode* pRNode = pRenderPart->GetRenderNode();
							pRNode->SetMatrix(stack_mat.Mat());
							pRNode->SetZed(zed);
							//pRNode->position = stack_mat.Mat().GetPosition();
							//pRNode->position.z = zed;
							//pRNode->scale = stack_mat.Mat().GetScale();
							//pRNode->rotation = stack_mat.Mat().GetRotationZ();
							vec_sprites.push_back(pRNode);
						}
					}

					/*IRenderNode* pRNode = this->FindRenderNode(node.first);
					if (pRNode)
					{
					pRNode->position = stack_mat.Mat().GetPosition();
					pRNode->position.z = zed;
					pRNode->scale = stack_mat.Mat().GetScale();
					pRNode->rotation = stack_mat.Mat().GetRotationZ();
					vec_sprites.push_back(pRNode);
					}*/

					node.second = true;
					if (node.first->IsGroup())
					{
						CGroup* pGroup = static_cast<CGroup*>(node.first);
						std::vector<CObject*> children = pGroup->GetChildren();
						for (int i = 0, size = children.size(); i < size; ++i)
						{
							stack.push(Node(children[i], false));
						}
					}
					else
					{
						stack.pop();
						stack_mat.Pop();
						zed -= node.first->GetLocalZed();
					}
				}
				else
				{
					stack.pop();
					stack_mat.Pop();
					zed -= node.first->GetLocalZed();
				}
			}




			for (auto iter = this->m_vLayers.begin(); iter != this->m_vLayers.end(); ++iter)
			{
				(*iter).second->DrawLayer(this->m_pRen);
			}

			////Find all renderable parts
			//std::list<Render::IRenderNode*> vec_sprites;
			//Math::CStackMatrix stack_mat;

			////Depth first traversal
			//std::stack<Node> stack;
			//stack.push(Node(pRoot,false));
			//float zed = 0;
			//while (!stack.empty())
			//{
			//	Node& node = stack.top();
			//	if (!node.second)
			//	{
			//		stack_mat.Push(node.first->GetLocalMatrix());
			//		zed += node.first->GetLocalZed();

			//		//add render nodes (if any)
			//		std::vector<Part::IPart*> vec = node.first->GetParts();
			//		for (int i = 0; i < vec.size(); ++i)
			//		{
			//			if (vec[i]->IsRenderable())
			//			{
			//				Script::IRenderPart* pRenderPart = static_cast<Script::IRenderPart*>(vec[i]);
			//				IRenderNode* pRNode = pRenderPart->GetRenderNode();
			//				pRNode->position = stack_mat.Mat().GetPosition();
			//				pRNode->position.z = zed;
			//				pRNode->scale = stack_mat.Mat().GetScale();
			//				pRNode->rotation = stack_mat.Mat().GetRotationZ();
			//				vec_sprites.push_back(pRNode);
			//			}
			//		}

			//		/*IRenderNode* pRNode = this->FindRenderNode(node.first);
			//		if (pRNode)
			//		{
			//			pRNode->position = stack_mat.Mat().GetPosition();
			//			pRNode->position.z = zed;
			//			pRNode->scale = stack_mat.Mat().GetScale();
			//			pRNode->rotation = stack_mat.Mat().GetRotationZ();
			//			vec_sprites.push_back(pRNode);
			//		}*/

			//		node.second = true;
			//		if (node.first->IsGroup())
			//		{
			//			CGroup* pGroup = static_cast<CGroup*>(node.first);
			//			std::vector<CObject*> children = pGroup->GetChildren();
			//			for (int i = 0, size = children.size(); i < size; ++i)
			//			{
			//				stack.push(Node(children[i], false));
			//			}
			//		}
			//		else
			//		{
			//			stack.pop();
			//			stack_mat.Pop();
			//			zed -= node.first->GetLocalZed();
			//		}
			//	}
			//	else
			//	{
			//		stack.pop();
			//		stack_mat.Pop();
			//		zed -= node.first->GetLocalZed();
			//	}
			//}

			//Math::Type2<slong> asdf(1080, 1920);

			////Sort them
			////std::sort(vec_sprites.begin(), vec_sprites.end(), RenderSort);
			//vec_sprites.sort(RenderSort);

			//for (auto iter = vec_sprites.begin(); iter != vec_sprites.end(); ++iter)
			//{
			//	(*iter)->operator()(m_pRen);
			//}

			//Present
			SDL_RenderPresent(this->m_pRen);
		}

		CRenderLayer* CRenderer::GetLayer(const std::string& layer)
		{
			auto find = this->m_vLayers.find(layer);
			if (find == this->m_vLayers.end())
				return null;
			else
				return (*find).second;
		}

		CCamera* CRenderer::GetCamera(const std::string& camera)
		{
			auto find = this->m_vCameras.find(camera);
			if (find == this->m_vCameras.end())
				return null;
			else
				return (*find).second;
		}

		void CRenderer::NullRootRender(void) const
		{
			//Primer
			SDL_SetRenderDrawColor(this->m_pRen, 0, 0, 0, 255);
			SDL_SetRenderTarget(this->m_pRen, NULL);
			SDL_RenderClear(this->m_pRen);

			Type2<int> logical_size;
			SDL_GetRendererOutputSize(this->m_pRen, &logical_size.w, &logical_size.h);
			Type2<float> origin(logical_size.x * 0.5f, logical_size.y * 0.5f);
			SDL_Rect rect;
			rect.x = origin.x - 64;
			rect.y = origin.y - 64;
			rect.w = rect.h = 128;

			CRenderNodeRect node = {};
			node.dstRect = rect;
			node.clr = CLR::CYAN.SDL();
			node.fill = true;
			node(this->m_pRen, CMatrix2D());

			rect.x += 16;
			rect.y += 16;
			rect.w = rect.h = 16;
			node.dstRect = rect;
			node.clr = CLR::GREY.SDL();
			node(this->m_pRen, CMatrix2D());

			rect.x += 80;
			node.dstRect = rect;
			node.clr = CLR::GREY.SDL();
			node(this->m_pRen, CMatrix2D());

			rect.x -= 72;
			rect.y += 80;
			rect.w = 80;
			node.dstRect = rect;
			node.clr = CLR::GREY.SDL();
			node(this->m_pRen, CMatrix2D());

			//Present
			SDL_RenderPresent(this->m_pRen);
		}
	}
}