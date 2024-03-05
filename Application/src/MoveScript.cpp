#include "MoveScript.h"
#include <box2d/box2d.h>

void MoveScript::onLateAttach()
{
   m_Transform = std::make_shared<Core::TransformComponent>(m_Entity->GetComponent<Core::TransformComponent>());
   m_RidgetBody = std::make_shared<Core::RidgetBody2DComponent>(m_Entity->GetComponent<Core::RidgetBody2DComponent>());
}

void MoveScript::OnUpdate()
{
   float speed = 2000.0f;
   b2Vec2 velocity = {0.0f, 0.0f};

   if(IsKeyDown(KEY_W))
      velocity.y = -1.0f;
   else if(IsKeyDown(KEY_S))
      velocity.y = 1.0f;

   if(IsKeyDown(KEY_D))
      velocity.x = 1.0f;
   else if(IsKeyDown(KEY_A))
      velocity.x = -1.0f;

   velocity *= speed;

   m_RidgetBody->Body->ApplyLinearImpulse(velocity, b2Vec2(m_Transform->Position.x, m_Transform->Position.y), true);
}
