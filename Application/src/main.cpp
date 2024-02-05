#include <raylib.h>
#include "Core.h"
#include <imgui.h>
#include <glm/glm.hpp>

class MoveScript : public Core::NativeScript
{
public:
   void onLateAttach() override
   {
      m_Transform = std::make_shared<Core::TransformComponent>(m_Entity->GetComponent<Core::TransformComponent>());
      m_RidgetBody = std::make_shared<Core::RidgetBody2DComponent>(m_Entity->GetComponent<Core::RidgetBody2DComponent>());
   }
   
   void OnUpdate() override
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

private:
   std::shared_ptr<Core::TransformComponent> m_Transform = nullptr;
   std::shared_ptr<Core::RidgetBody2DComponent> m_RidgetBody = nullptr;
};

class SandBoxLayer : public Core::Layer
{
public:
   void OnAttach() override
   {
      m_FrameBuffer.reset(new Core::FrameBuffer);
      m_FrameBuffer->Init(m_ViewPortSize.x, m_ViewPortSize.y);
      
      m_Scene.reset(new Core::Scene);

      Core::Entity entityOne(m_Scene);
      entityOne.AddComponent<Core::TransformComponent>(glm::vec3(720, 0, 0.0f), glm::vec3(80.0f, 80.0f, 0.0f), 0.0f);
      entityOne.AddComponent<Core::ColorComponent>(glm::vec4(0.8f, 0.2f, 0.3f, 1.0f));
      entityOne.AddComponent<Core::NativeScriptComponent>(std::shared_ptr<MoveScript>(new MoveScript), std::make_shared<Core::Entity>(entityOne));
      entityOne.AddComponent<Core::Box2DColliderComponent>();
      entityOne.AddComponent<Core::RidgetBody2DComponent>(b2_dynamicBody, false);

      Core::Entity entityTwo(m_Scene);
      entityTwo.AddComponent<Core::ColorComponent>(glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));
      entityTwo.AddComponent<Core::TransformComponent>(glm::vec3(0.0f, 300, 0.0f), glm::vec3(1280.0f, 100.0f, 0), 0.0f);
      entityTwo.AddComponent<Core::Box2DColliderComponent>();
      entityTwo.AddComponent<Core::RidgetBody2DComponent>(b2_staticBody, false);
      
      m_Scene->InitPhysicsWorld(0.0f, 9.8f);

      for(auto& entity: m_Scene->GetEntities<Core::RidgetBody2DComponent>())
      {
         Core::RidgetBody2DComponent& ridgetBody = entity->GetComponent<Core::RidgetBody2DComponent>();
         Core::TransformComponent& transform = entity->GetComponent<Core::TransformComponent>();

         b2BodyDef bodyDef;
         
         bodyDef.type = ridgetBody.Type;
         bodyDef.position.Set(transform.Position.x, transform.Position.y);
         
         bodyDef.angle = transform.Rotation;
         bodyDef.fixedRotation = ridgetBody.FixedRotation;
         
         ridgetBody.Body = m_Scene->GetPhysicsWorld()->CreateBody(&bodyDef);

         if(entity->HasComponent<Core::Box2DColliderComponent>())
         {
            Core::Box2DColliderComponent& collider = entity->GetComponent<Core::Box2DColliderComponent>();

            b2PolygonShape shape;
            shape.SetAsBox(collider.Size.x * transform.Scale.x / 2, collider.Size.y * transform.Scale.y / 2, b2Vec2(transform.Scale.x / 2, transform.Scale.y / 2), 0.0f);
            
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &shape;
            fixtureDef.density = collider.Density;
            fixtureDef.friction = collider.Friction;
            ridgetBody.Body->CreateFixture(&fixtureDef);
         }
      }

      for(auto& entity : m_Scene->GetEntities<Core::NativeScriptComponent>())
      {
         Core::NativeScriptComponent& script = entity->GetComponent<Core::NativeScriptComponent>();
         script.Instance->onLateAttach();
      }
   }

   void OnImGuiRender() override
   {
      ImGui::Begin("ViewPort");

      if(m_ViewPortSize.x != ImGui::GetContentRegionAvail().x || m_ViewPortSize.y != ImGui::GetContentRegionAvail().y)
      {
         m_ViewPortSize.x = ImGui::GetContentRegionAvail().x;
         m_ViewPortSize.y = ImGui::GetContentRegionAvail().y;
         
         m_FrameBuffer->Init(m_ViewPortSize.x, m_ViewPortSize.y);
      }

      rlImGuiImageRenderTexture(m_FrameBuffer->GetTexture().get());
      
      ImGui::End();
   }

   void OnUpdate() override
   {
      m_Scene->GetPhysicsWorld()->Step(1.0f / 60.0f, 6, 2);

      for(auto& entity : m_Scene->GetEntities<Core::RidgetBody2DComponent>())
      {
         Core::RidgetBody2DComponent& ridgetBody = entity->GetComponent<Core::RidgetBody2DComponent>();
         Core::TransformComponent& transform = entity->GetComponent<Core::TransformComponent>();

         transform.Position.x = ridgetBody.Body->GetPosition().x;
         transform.Position.y = ridgetBody.Body->GetPosition().y;
         transform.Rotation = ridgetBody.Body->GetAngle();
      }
      
      for (auto& entity : m_Scene->GetEntities<Core::NativeScriptComponent>())
      {
        Core::NativeScriptComponent& script = entity->GetComponent<Core::NativeScriptComponent>();
         script.Instance->OnUpdate();
      }
      
      m_FrameBuffer->Bind();

      ClearBackground(GetColor(0x312b47ff));

      for (auto& entity : m_Scene->GetEntities<Core::TransformComponent>())
      {
         Core::TransformComponent& transform = entity->GetComponent<Core::TransformComponent>();
         Core::ColorComponent& color = entity->GetComponent<Core::ColorComponent>();
      
         DrawRectanglePro(Rectangle({transform.Position.x, transform.Position.y, transform.Scale.x, transform.Scale.y}), Vector2({0, 0}), transform.Rotation, Color(color.Color.r, color.Color.g, color.Color.b, color.Color.a));
         
      }

      m_FrameBuffer->UnBind();
   }

private:
   std::shared_ptr<Core::FrameBuffer> m_FrameBuffer = nullptr;
   glm::vec2 m_ViewPortSize = {GetScreenWidth(), GetScreenHeight() };
   
   std::shared_ptr<Core::Scene> m_Scene = nullptr;
};

class SandBoxApplication : public Core::Application
{
public:
   SandBoxApplication()
   {
      PushLayer(std::shared_ptr<SandBoxLayer>(new SandBoxLayer));
   }

   ~SandBoxApplication()
   {
      
   }
};

int main()
{
   SandBoxApplication* app = new SandBoxApplication;
   app->Init({1280, 720}, "Sandbox");
   app->Run();
}