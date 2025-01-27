#include "Nodes/3D/StaticMesh3d.h"

#include "Assets/StaticMesh.h"
#include "Renderer.h"
#include "AssetManager.h"
#include "Log.h"
#include "Utilities.h"

#include "Graphics/Graphics.h"

#include <btBulletDynamicsCommon.h>

FORCE_LINK_DEF(StaticMesh3D);
DEFINE_NODE(StaticMesh3D, Mesh3D);

bool StaticMesh3D::HandlePropChange(Datum* datum, uint32_t index, const void* newValue)
{
    Property* prop = static_cast<Property*>(datum);
    OCT_ASSERT(prop != nullptr);
    StaticMesh3D* meshComp = static_cast<StaticMesh3D*>(prop->mOwner);
    bool success = false;

    if (prop->mName == "Static Mesh")
    {
        meshComp->SetStaticMesh(*(StaticMesh**)newValue);
        success = true;
    }
    else if (prop->mName == "Use Triangle Collision")
    {
        meshComp->SetUseTriangleCollision(*(bool*)newValue);
        success = true;
    }
    else if (prop->mName == "Clear Baked Lighting")
    {
        meshComp->ClearInstanceColors();
        success = true;
    }

    return success;
}

static StaticMesh* GetDefaultMesh()
{
    // TODO: replace this with calls to the asset manager.
    static StaticMeshRef defaultMesh = LoadAsset("SM_Cube");
    return defaultMesh.Get<StaticMesh>();
}

StaticMesh3D::StaticMesh3D() :
    mStaticMesh(nullptr),
    mUseTriangleCollision(false),
    mBakeLighting(false),
    mHasBakedLighting(false)
{
    mName = "Static Mesh";
}

StaticMesh3D::~StaticMesh3D()
{

}

const char* StaticMesh3D::GetTypeName() const
{
    return "StaticMesh";
}

void StaticMesh3D::GatherProperties(std::vector<Property>& outProps)
{
    Mesh3D::GatherProperties(outProps);

    SCOPED_CATEGORY("Mesh");

    static bool sFakeBool = false;
    outProps.push_back(Property(DatumType::Asset, "Static Mesh", this, &mStaticMesh, 1, HandlePropChange, int32_t(StaticMesh::GetStaticType())));
    outProps.push_back(Property(DatumType::Bool, "Use Triangle Collision", this, &mUseTriangleCollision, 1, HandlePropChange));
    outProps.push_back(Property(DatumType::Bool, "Bake Lighting", this, &mBakeLighting, 1, HandlePropChange));
    outProps.push_back(Property(DatumType::Bool, "Clear Baked Lighting", this, &sFakeBool, 1, HandlePropChange));
}

void StaticMesh3D::Create()
{
    Mesh3D::Create();
    GFX_CreateStaticMeshCompResource(this);
    SetStaticMesh(GetDefaultMesh());
}

void StaticMesh3D::Destroy()
{
    Mesh3D::Destroy();
    GFX_DestroyStaticMeshCompResource(this);
}

StaticMeshCompResource* StaticMesh3D::GetResource()
{
    return &mResource;
}

void StaticMesh3D::SaveStream(Stream& stream)
{
    Mesh3D::SaveStream(stream);
    stream.WriteAsset(mStaticMesh);
    stream.WriteBool(mUseTriangleCollision);
    stream.WriteBool(mBakeLighting);

    uint32_t numInstanceColors = (uint32_t)mInstanceColors.size();
    stream.WriteUint32(numInstanceColors);
    for (uint32_t i = 0; i < numInstanceColors; ++i)
    {
        stream.WriteUint32(mInstanceColors[i]);
    }
}

void StaticMesh3D::LoadStream(Stream& stream)
{
    Mesh3D::LoadStream(stream);

    AssetRef meshRef;
    stream.ReadAsset(meshRef);
    if (meshRef.Get<StaticMesh>() == nullptr)
        meshRef = GetDefaultMesh();

    mUseTriangleCollision = stream.ReadBool();
    mBakeLighting = stream.ReadBool();

    // Set mesh only after determining if we need to use triangle collision.
    SetStaticMesh(meshRef.Get<StaticMesh>());

    // Load instance colors after setting the static mesh. Otherwise it will clear.
    uint32_t numInstanceColors = stream.ReadUint32();
    mInstanceColors.resize(numInstanceColors);
    for (uint32_t i = 0; i < numInstanceColors; ++i)
    {
        mInstanceColors[i] = stream.ReadUint32();

#if PLATFORM_DOLPHIN
        ReverseColorUint32(mInstanceColors[i]);
#endif
    }

    GFX_UpdateStaticMeshCompResourceColors(this);
}

bool StaticMesh3D::IsStaticMesh3D() const
{
    return true;
}

bool StaticMesh3D::IsSkeletalMesh3D() const
{
    return false;
}

void StaticMesh3D::SetStaticMesh(StaticMesh* staticMesh)
{
    if (mStaticMesh.Get() != staticMesh)
    {
        mStaticMesh = staticMesh;
        RecreateCollisionShape();
        ClearInstanceColors();
    }
}

StaticMesh* StaticMesh3D::GetStaticMesh()
{
    return mStaticMesh.Get<StaticMesh>();
}

void StaticMesh3D::SetUseTriangleCollision(bool triangleCol)
{
    if (mUseTriangleCollision != triangleCol)
    {
        mUseTriangleCollision = triangleCol;
        RecreateCollisionShape();
    }
}

bool StaticMesh3D::GetUseTriangleCollision() const
{
    return mUseTriangleCollision;
}

void StaticMesh3D::SetBakeLighting(bool bake)
{
    mBakeLighting = bake;
}

bool StaticMesh3D::GetBakeLighting() const
{
    return mBakeLighting;
}

Material* StaticMesh3D::GetMaterial()
{
    Material* mat = mMaterialOverride.Get<Material>();

    if (!mat && mStaticMesh.Get())
    {
        mat = mStaticMesh.Get<StaticMesh>()->GetMaterial();
    }

    return mat;
}

void StaticMesh3D::Render()
{
    GFX_DrawStaticMeshComp(this);
}

VertexType StaticMesh3D::GetVertexType() const
{
    VertexType retType = VertexType::Vertex;

    if (mStaticMesh.Get<StaticMesh>() != nullptr && 
        mStaticMesh.Get<StaticMesh>()->HasVertexColor())
    {
        retType = VertexType::VertexColor;
    }

    return retType;
}

void StaticMesh3D::GatherProxyDraws(std::vector<DebugDraw>& inoutDraws)
{
#if DEBUG_DRAW_ENABLED
    Mesh3D::GatherProxyDraws(inoutDraws);

    if (Renderer::Get()->GetDebugMode() == DEBUG_COLLISION)
    {
        StaticMesh* staticMesh = mStaticMesh.Get<StaticMesh>();

        if (staticMesh != nullptr &&
            mCollisionShape != nullptr)
        {
            uint32_t numCollisionShapes = 0;
            btCollisionShape* collisionShapes[MAX_COLLISION_SHAPES] = {};
            glm::mat4 collisionTransforms[MAX_COLLISION_SHAPES] = {};
            uint32_t collisionMeshIndex = 0;
            OCT_UNUSED(collisionMeshIndex); // Only used in EDITOR

            btCompoundShape* compoundShape = nullptr;
            glm::vec3 invScale = 1.0f / GetWorldScale();

            if (mCollisionShape->getShapeType() == COMPOUND_SHAPE_PROXYTYPE)
            {
                compoundShape = static_cast<btCompoundShape*>(mCollisionShape);
                for (uint32_t i = 0; i < uint32_t(compoundShape->getNumChildShapes()); ++i)
                {
                    if (i >= MAX_COLLISION_SHAPES)
                    {
                        LogError("Too many collision shapes");
                        break;
                    }

                    collisionShapes[numCollisionShapes] = compoundShape->getChildShape(i);
                    const btTransform& bTransform = compoundShape->getChildTransform(i);
                    btQuaternion bRotation = bTransform.getRotation();
                    btVector3 bPosition = bTransform.getOrigin();

                    glm::quat rotation = glm::quat(bRotation.w(), bRotation.x(), bRotation.y(), bRotation.z());
                    glm::vec3 position = { bPosition.x(), bPosition.y(), bPosition.z() };

                    collisionTransforms[i] = glm::mat4(1);
                    collisionTransforms[i] = glm::translate(collisionTransforms[i], position);
                    collisionTransforms[i] *= glm::toMat4(rotation);
                        
                    ++numCollisionShapes;
                }
            }
            else
            {
                collisionShapes[0] = mCollisionShape;
                numCollisionShapes = 1;
            }

            for (uint32_t i = 0; i < numCollisionShapes; ++i)
            {
                btCollisionShape* collisionShape = collisionShapes[i];
                int shapeType = collisionShape->getShapeType();

                if (shapeType == EMPTY_SHAPE_PROXYTYPE)
                    continue;

                DebugDraw debugDraw;

                switch (shapeType)
                {
                case BOX_SHAPE_PROXYTYPE:
                {
                    // Assuming that default cube mesh has half extents equal to 1,1,1
                    btBoxShape* boxShape = static_cast<btBoxShape*>(collisionShape);
                    btVector3 halfExtents = boxShape->getHalfExtentsWithMargin();

                    glm::mat4 scale = glm::scale(invScale);
                    collisionTransforms[i] = scale * collisionTransforms[i];
                    collisionTransforms[i] = glm::scale(collisionTransforms[i], { halfExtents.x(), halfExtents.y(), halfExtents.z()});

                    debugDraw.mMesh = LoadAsset<StaticMesh>("SM_Cube");
                    debugDraw.mTransform = mTransform;
                    break;
                }
                case SPHERE_SHAPE_PROXYTYPE:
                {
                    // Assuming that default sphere mesh has a radius of 1
                    btSphereShape* sphereShape = static_cast<btSphereShape*>(collisionShape);
                    float radius = sphereShape->getRadius();

                    glm::mat4 scale = glm::scale(invScale);
                    collisionTransforms[i] = scale * collisionTransforms[i];
                    collisionTransforms[i] = glm::scale(collisionTransforms[i], {radius, radius, radius});

                    debugDraw.mMesh = LoadAsset<StaticMesh>("SM_Sphere");
                    debugDraw.mTransform = mTransform;
                    break;
                }
                case CONVEX_HULL_SHAPE_PROXYTYPE:
                {
#if CREATE_CONVEX_COLLISION_MESH
                    // We only create StaticMesh objects for convex hulls when in editor.
                    debugDraw.mMesh = staticMesh->mCollisionMeshes[collisionMeshIndex];
                    debugDraw.mTransform = mTransform;

                    ++collisionMeshIndex;
#endif
                    break;
                }
                case SCALED_TRIANGLE_MESH_SHAPE_PROXYTYPE:
                {
                    debugDraw.mMesh = staticMesh;
                    debugDraw.mTransform = mTransform;
                    break;
                }
                }

                if (compoundShape != nullptr)
                {
                    debugDraw.mTransform = debugDraw.mTransform * collisionTransforms[i];
                }

                debugDraw.mColor = GetCollisionDebugColor();
                inoutDraws.push_back(debugDraw);
            }
        }
    }
#endif
}

Bounds StaticMesh3D::GetLocalBounds() const
{
    if (mStaticMesh != nullptr)
    {
        return mStaticMesh.Get<StaticMesh>()->GetBounds();
    }
    else
    {
        return Mesh3D::GetLocalBounds();
    }
}

void StaticMesh3D::ClearInstanceColors()
{
    mInstanceColors.clear();
    mHasBakedLighting = false;
    GFX_UpdateStaticMeshCompResourceColors(this);
}

void StaticMesh3D::SetInstanceColors(const std::vector<uint32_t>& colors, bool bakedLighting)
{
    mInstanceColors = colors;
    mHasBakedLighting = bakedLighting;
    GFX_UpdateStaticMeshCompResourceColors(this);
}

std::vector<uint32_t>& StaticMesh3D::GetInstanceColors()
{
    return mInstanceColors;
}

bool StaticMesh3D::HasBakedLighting() const
{
    return mHasBakedLighting;
}

bool StaticMesh3D::HasInstanceColors() const
{
    return (mInstanceColors.size() > 0);
}

void StaticMesh3D::RecreateCollisionShape()
{
    StaticMesh* staticMesh = mStaticMesh.Get<StaticMesh>();

    if (staticMesh != nullptr)
    {
        if (mUseTriangleCollision && staticMesh->GetTriangleCollisionShape())
        {
            glm::vec3 scale = GetWorldScale();
            btVector3 btscale = btVector3(scale.x, scale.y, scale.z);
            btScaledBvhTriangleMeshShape* scaledTriangleShape = new btScaledBvhTriangleMeshShape(staticMesh->GetTriangleCollisionShape(), btscale);
            SetCollisionShape(scaledTriangleShape);
        }
        else
        {
            btCollisionShape* newShape = CloneCollisionShape(staticMesh->GetCollisionShape());
            SetCollisionShape(newShape);
        }
    }
    else
    {
        SetCollisionShape(Primitive3D::GetEmptyCollisionShape());
    }
}