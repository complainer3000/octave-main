#pragma once

#include "Datum.h"

enum class ByteExtra
{
    None,
    FlagWidget,
    ExclusiveFlagWidget
};

class Property : public Datum
{
public:
    Property();
    Property(DatumType type,
        const std::string& name,
        void* owner,
        void* data,
        uint32_t count = 1,
        DatumChangeHandlerFP changeHandler = nullptr,
        int32_t extra = 0,
        int32_t enumCount = 0,
        const char** enumStrings = nullptr);

    Property(const Property& src);

    Property& operator=(const Property& src);


    virtual void ReadStream(Stream& stream, bool external) override;
    virtual void WriteStream(Stream& stream) const override;
    virtual uint32_t GetSerializationSize() const override;

    virtual bool IsProperty() const override;
    virtual void DeepCopy(const Datum& src, bool forceInternalStorage) override;

    void PushBackVector(void* value = nullptr);
    void EraseVector(uint32_t index);
    void ResizeVector(uint32_t count);
    Property& MakeVector(uint8_t minCount = 0, uint8_t maxCount = 255);
    bool IsVector() const;
    bool IsArray() const;

#if EDITOR
    static void SetCategory(const char* category);
    static void ClearCategory();
#endif

protected:

    virtual void Reset() override;

public:

    static const char* sCategory;

    std::string mName;
    int32_t mExtra = 0;
    int32_t mEnumCount = 0;
    const char** mEnumStrings = nullptr;
    void* mVector = nullptr;
    uint8_t mMinCount = 0;
    uint8_t mMaxCount = 255;
    bool mIsVector = false;
#if EDITOR
    const char* mCategory = "";
#endif
};

#if EDITOR
struct ScopedPropertyCategory
{
    ScopedPropertyCategory(const char* catName) { Property::SetCategory(catName); }
    ~ScopedPropertyCategory() { Property::ClearCategory(); }
};

#define SCOPED_CATEGORY(CatName) ScopedPropertyCategory scopedCat(CatName);
#else
#define SCOPED_CATEGORY(CatName)  
#endif
