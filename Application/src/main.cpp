#include "Core.h"
#include  "MoveScript.h"

#include <raylib.h>
#include <imgui.h>
#include <glm/glm.hpp>

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
      entityOne.AddComponent<Core::RidgetBody2DComponent>(b2_dynamicBody, true);

      Core::Entity entityThree(m_Scene);

      std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(LoadTexture("assets/rider-icon.png"));
      
      entityThree.AddComponent<Core::TransformComponent>(glm::vec3(820, 0, 0.0f), glm::vec3(80.0f, 80.0f, 0.0f), 0.0f);
      entityThree.AddComponent<Core::SpriteComponent>(texture);
      entityThree.AddComponent<Core::Box2DColliderComponent>();
      entityThree.AddComponent<Core::RidgetBody2DComponent>(b2_dynamicBody, true);

      Core::Entity entityTwo(m_Scene);
      entityTwo.AddComponent<Core::ColorComponent>(glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));
      entityTwo.AddComponent<Core::TransformComponent>(glm::vec3(0.0f, 300, 0.0f), glm::vec3(1280.0f, 100.0f, 0), 0.0f);
      entityTwo.AddComponent<Core::Box2DColliderComponent>();
      entityTwo.AddComponent<Core::RidgetBody2DComponent>(b2_staticBody, true);
      
      m_Scene->InitPhysicsWorld(0.0f, 9.8f);

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
      m_Scene->PhysicsUpdate();

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

         if(entity->HasComponent<Core::ColorComponent>())
         {
            Core::ColorComponent& color = entity->GetComponent<Core::ColorComponent>();
         
            DrawRectanglePro(Rectangle({transform.Position.x, transform.Position.y, transform.Scale.x, transform.Scale.y}), Vector2({0, 0}), transform.Rotation, Color(color.Color.r, color.Color.g, color.Color.b, color.Color.a));
         }

         if(entity->HasComponent<Core::SpriteComponent>())
         {
            Core::SpriteComponent& sprite = entity->GetComponent<Core::SpriteComponent>();
            
            DrawTexturePro(*sprite.Texture.get(), Rectangle({0.0f, 0.0f, (float)sprite.Texture->width, (float)sprite.Texture->height}), Rectangle({transform.Position.x, transform.Position.y, transform.Scale.x, transform.Scale.y}), Vector2({0, 0}), transform.Rotation, WHITE);
         }
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