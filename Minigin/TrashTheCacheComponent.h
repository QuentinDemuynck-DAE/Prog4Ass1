#pragma once
#include "Component.h"
#include "GameObject.h"
class TrashTheCacheComponent final : public Component {
public:
    struct Transform {
        float matrix[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    };

    class GameObject3D {
    public:
        Transform transform;
        int ID = 0;
    };

    TrashTheCacheComponent(dae::GameObject& owner, int arraySize);
    void Render(glm::vec3 position) override;

private:
    void CalculateFunctionTime(bool useInt);
    void PlotGraph();
    int m_ArraySize{};
    int m_SampleAmount{ 100 };
    bool m_GraphsAreReady{ false };

    std::vector<float> m_StepSizes;
    std::vector<float> m_IntTimes;
};

