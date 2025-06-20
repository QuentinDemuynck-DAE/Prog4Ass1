#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"

//Imgui
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include <vec4.hpp>

#include "implot.h"



int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	//IMGUI

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init();
}

void dae::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);


	//IMGUI

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	SceneManager::GetInstance().Render();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	
	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	//IMGUI

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	ImPlot::DestroyContext();

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(
	const Texture2D& texture,
	float x, float y,
	glm::vec2 scale,
	glm::ivec4 srcRect,
	float angleDegrees
) const
{
	SDL_Rect dst{};
	dst.x = int(x);
	dst.y = int(y);

	int origW{}, origH{};
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &origW, &origH);

	dst.w = int(origW * scale.x);
	dst.h = int(origH * scale.y);

	SDL_Rect actualSrc{ srcRect.x, srcRect.y, srcRect.z, srcRect.w };
	if (srcRect.x <  0 ||
		srcRect.y <  0 ||
		srcRect.z <= 0 ||
		srcRect.w <= 0 ||
		srcRect.x + srcRect.z > origW ||
		srcRect.y + srcRect.w > origH)
	{
		actualSrc = { 0, 0, origW, origH };
	}
	else
	{
		dst.w = int(actualSrc.w * scale.x);
		dst.h = int(actualSrc.h * scale.y);
	}

	SDL_Point pivot{ 0, 0 };

	SDL_RenderCopyEx(
		GetSDLRenderer(),
		texture.GetSDLTexture(),
		&actualSrc,
		&dst,
		angleDegrees,
		&pivot,
		SDL_FLIP_NONE
	);
}


void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height, glm::ivec4 srcRect) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);

	int origW{}, origH{};
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &origW, &origH);

	//Automatically assign full rect when out of bounds
	SDL_Rect actualSrc{ srcRect.x ,srcRect.y , srcRect.z , srcRect.w };
	if (srcRect.x < 0 || srcRect.x > origW ||
		srcRect.y < 0 || srcRect.y > origH ||
		srcRect.w < 0 || srcRect.z < 0 ||
		srcRect.x + srcRect.w > origW || srcRect.y + srcRect.z > origH)
	{
		actualSrc.x = 0;
		actualSrc.y = 0;
		actualSrc.w = origW;
		actualSrc.h = origH;
	}
	else
	{
		dst.w = static_cast<int>(actualSrc.w);
		dst.h = static_cast<int>(actualSrc.h);

	}
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &actualSrc, &dst);
}

SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }

void dae::Renderer::DrawLine(float x1, float y1, float x2, float y2, SDL_Color color) const
{
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);

	SDL_RenderDrawLine(
		m_renderer,
		static_cast<int>(x1),
		static_cast<int>(y1),
		static_cast<int>(x2),
		static_cast<int>(y2)
	);
}
