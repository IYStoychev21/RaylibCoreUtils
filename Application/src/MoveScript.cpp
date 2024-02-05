#include "MoveScript.h"

void MoveScript::onLateAttach()
{
   m_Transform = std::make_shared<Core::TransformComponent>(m_Entity->GetComponent<Core::TransformComponent>());
   m_RidgetBody = std::make_shared<Core::RidgetBody2DComponent>(m_Entity->GetComponent<Core::RidgetBody2DComponent>());
}

void MoveScript::OnUpdate()
{
   float speed = 5.0f;
   
   if(IsKeyDown(KEY_W))
   {
      m_RidgetBody->Body->SetLinearVelocity(b2Vec2(0.0f, -speed));
   }

   if(IsKeyDown(KEY_S))
   {
      m_RidgetBody->Body->SetLinearVelocity(b2Vec2(0.0f, speed));
   }

   if(IsKeyDown(KEY_D))
   {
      m_RidgetBody->Body->SetLinearVelocity(b2Vec2(speed, 0.0f));
   }

   if(IsKeyDown(KEY_A))
   {
      m_RidgetBody->Body->SetLinearVelocity(b2Vec2(-speed, 0.0f));
   }
}
