#pragma once

#include "Nodes/Widgets/Button.h"
#include "Nodes/Widgets/Quad.h"

#include "ScriptFunc.h"

typedef void(*TextFieldHandlerFP)(class TextField* textField);

class TextField : public Button
{
public:

    DECLARE_NODE(TextField, Button);

    TextField();
    ~TextField();

    virtual void Tick(float deltaTime) override;
    virtual void SetState(ButtonState newState) override;
    virtual void SetTextString(const std::string& newTextString) override;

    void SetTextEditHandler(TextFieldHandlerFP handler);
    void SetTextConfirmHandler(TextFieldHandlerFP handler);
    void SetScriptTextEditHandler(const ScriptFunc& scriptFunc);
    void SetScriptTextConfirmHandler(const ScriptFunc& scriptFunc);

    static TextField* GetSelectedTextField();
    static void SetSelectedTextField(TextField* newField);
    static void StaticUpdate();

protected:

    static TextField* sSelectedTextField;
    static float sCursorBlinkTime;
    static const float sCursorBlinkPeriod;

    ScriptableFP<TextFieldHandlerFP> mTextEditHandler;
    ScriptableFP<TextFieldHandlerFP> mTextConfirmHandler;

    Quad* mCursorQuad;
    uint32_t mMaxCharacters;
};