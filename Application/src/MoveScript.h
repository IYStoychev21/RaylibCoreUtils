#pragma once
#include "Core.h"

class MoveScript : public Core::NativeScript
{
public:
   void onLateAttach() override;
   void OnUpdate() override;
   
private:
   std::shared_ptr<Core::TransformComponent> m_Transform = nullptr;
   std::shared_ptr<Core::RidgetBody2DComponent> m_RidgetBody = nullptr;
};