#include "TrashTheCacheComponent.h"
#include <chrono>
#include <iostream>

//Imgui
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"


#include <SDL.h>
#include "implot.h"
#include <SDL_opengl.h>

using namespace std::chrono;

TrashTheCacheComponent::TrashTheCacheComponent(dae::GameObject& owner, int arraySize)
    : Component(owner), m_ArraySize(arraySize) 
{

}

void TrashTheCacheComponent::Render(glm::vec3 position) 
{
    ImGui::SetNextWindowPos(ImVec2(position.x, position.y), ImGuiCond_Once);
    if (ImGui::Begin("Thrash The Cache", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        if (ImGui::BeginTabBar("Exercises")) {
            if (ImGui::BeginTabItem("Exercise 1")) {
                ImGui::Text("Trash the cache using int!");
                ImGui::PushItemWidth(800);
                ImGui::InputInt("# samples", &m_SampleAmount);
                ImGui::PopItemWidth();
                if (m_SampleAmount < 1) m_SampleAmount = 1;
                if (ImGui::Button("Use Int")) CalculateFunctionTime(true);
                PlotGraph();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Exercise 2")) {
                ImGui::Text("Trash the cache using GameObject3D!");
                ImGui::PushItemWidth(800);
                ImGui::InputInt("# samples", &m_SampleAmount);
                ImGui::PopItemWidth();
                if (m_SampleAmount < 1) m_SampleAmount = 1;
                if (ImGui::Button("Use GameObject")) CalculateFunctionTime(false);
                PlotGraph();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();
    }
}

void TrashTheCacheComponent::CalculateFunctionTime(bool useInt) 
{
    m_GraphsAreReady = false;
    m_IntTimes.clear();
    m_StepSizes.clear();

    if (useInt) {
        std::vector<int*> veryLargeArray(m_ArraySize, nullptr);
        for (int i = 0; i < m_ArraySize; i++) {
            veryLargeArray[i] = new int(5);
        }
        for (int stepSize = 1; stepSize <= 1024; stepSize *= 2) {
            long long totalTime = 0;
            for (int sample = 0; sample < m_SampleAmount; ++sample) {
                auto start = high_resolution_clock::now();
                for (int i = 0; i < m_ArraySize; i += stepSize) {
                    *veryLargeArray[i] *= 2;
                }
                auto end = high_resolution_clock::now();
                totalTime += duration_cast<microseconds>(end - start).count();
            }
            m_StepSizes.push_back(static_cast<float>(stepSize));
            m_IntTimes.push_back(static_cast<float>(totalTime) / m_SampleAmount);
        }
    }
    else 
    {
        std::vector<GameObject3D*> veryLargeArray(m_ArraySize, nullptr);
        for (int i = 0; i < m_ArraySize; i++) {
            veryLargeArray[i] = new GameObject3D();
        }
        for (int stepSize = 1; stepSize <= 1024; stepSize *= 2) {
            long long totalTime = 0;
            for (int sample = 0; sample < m_SampleAmount; ++sample) {
                auto start = high_resolution_clock::now();
                for (int i = 0; i < m_ArraySize; i += stepSize) {
                    veryLargeArray[i]->ID *= 2;
                }
                auto end = high_resolution_clock::now();
                totalTime += duration_cast<microseconds>(end - start).count();
            }
            m_StepSizes.push_back(static_cast<float>(stepSize));
            m_IntTimes.push_back(static_cast<float>(totalTime) / m_SampleAmount);
        }
    }
    m_GraphsAreReady = true;
}

void TrashTheCacheComponent::PlotGraph() 
{
    if (!m_GraphsAreReady || m_StepSizes.empty() || m_IntTimes.empty()) return;
    if (ImPlot::BeginPlot("Execution Time vs Step Size")) {
        ImPlot::SetupAxis(ImAxis_X1, "Step Size");
        ImPlot::SetupAxis(ImAxis_Y1, "Execution Time (µs)");
        ImPlot::SetupAxisScale(ImAxis_X1, ImPlotScale_Linear);
        ImPlot::SetupAxisScale(ImAxis_Y1, ImPlotScale_Linear);
        ImPlot::PlotLine("Execution Time", m_StepSizes.data(), m_IntTimes.data(), static_cast<int>(m_StepSizes.size()));
        ImPlot::EndPlot();
    }
}






