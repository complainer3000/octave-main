#pragma once

#include "Nodes/Widgets/Widget.h"
#include "Assets/Texture.h"
#include "Vertex.h"
#include "AssetRef.h"

#include "glm/glm.hpp"

class Quad : public Widget
{
public:

    DECLARE_NODE(Quad, Widget);

    Quad();
    virtual ~Quad();

    virtual void Create() override;
    virtual void Destroy() override;

    QuadResource* GetResource();

    virtual void GatherProperties(std::vector<Property>& outProps) override;

    virtual DrawData GetDrawData() override;

    virtual void Tick(float deltaTime) override;
    virtual void EditorTick(float deltaTime) override;
    virtual void Render() override;

    void SetTexture(class Texture* texture);
    Texture* GetTexture();

    virtual void SetColor(glm::vec4 color) override;

    void SetColor(glm::vec4 colors[4]);
    void SetColor(glm::vec4 topLeft,
                  glm::vec4 topRight,
                  glm::vec4 bottomLeft,
                  glm::vec4 bottomRight);

    void SetUvScale(glm::vec2 scale);
    glm::vec2 GetUvScale() const;

    void SetUvOffset(glm::vec2 offset);
    glm::vec2 GetUvOffset() const;

    VertexUI* GetVertices();

protected:

    static bool HandlePropChange(Datum* datum, uint32_t index, const void* newValue);

    void TickCommon(float deltaTime);
    void InitVertexData();
    void UpdateVertexData();

    TextureRef mTexture;
    VertexUI mVertices[4];

    glm::vec2 mUvScale;
    glm::vec2 mUvOffset;

    // Graphics Resource
    QuadResource mResource;
};