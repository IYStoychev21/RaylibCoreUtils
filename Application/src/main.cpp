#include <raylib.h>
#include "Core.h"
#include <imgui.h>
#include <glm/glm.hpp>

class MoveScript : public Core::NativeScript
{
public:
   void OnUpdate() override
   {
      if(IsKeyDown(KEY_W))
      {
        m_Entity->GetComponent<Core::TransformComponent>().Position.y -= 0.1;
      }

      if(IsKeyDown(KEY_S))
      {
        m_Entity->GetComponent<Core::TransformComponent>().Position.y += 0.1;
      }

      if(IsKeyDown(KEY_D))
      {
        m_Entity->GetComponent<Core::TransformComponent>().Position.x += 0.1;
      }

      if(IsKeyDown(KEY_A))
      {
        m_Entity->GetComponent<Core::TransformComponent>().Position.x -= 0.1;
      }
   }
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
      entityOne.AddComponent<Core::TransformComponent>(glm::vec3((1280 / 2) - 5, (720 / 2) - 5, 0.0f), 2.0f, 0.0f);
      entityOne.AddComponent<Core::ColorComponent>(glm::vec4(180, 75, 100, 255));
      entityOne.AddComponent<Core::NativeScriptComponent>(std::shared_ptr<MoveScript>(new MoveScript), std::make_shared<Core::Entity>(entityOne));

      Core::Entity entityTwo(m_Scene);
      entityTwo.AddComponent<Core::ColorComponent>(glm::vec4(75, 100, 170, 255));
      entityTwo.AddComponent<Core::TransformComponent>(glm::vec3((1280 / 2) - 5, (720 / 2) - 5, 0.0f), 1.0, 0.0f);
   }

   void OnImGuiRender() override
   {
      ImGui::Begin("ViewPort");

      if(m_ViewPortSize.x != ImGui::GetContentRegionAvail().x || m_ViewPortSize.y != ImGui::GetContentRegionAvail().y)
      {
         m_ViewPortSize.x = ImGui::GetContentRegionAvail().x;
         m_ViewPortSize.y = ImGui::GetContentRegionAvail().y;
         
         m_FrameBuffer->Init(m_ViewPortSize.x, m_ViewPortSize.y) ;
      }

      rlImGuiImageRenderTexture(m_FrameBuffer->GetTexture().get());
      
      ImGui::End();
   }

   void OnUpdate() override
   {
      for (auto& entity : m_Scene->GetEntities<Core::NativeScriptComponent>())
      {
        Core::NativeScriptComponent& script = entity->GetComponent<Core::NativeScriptComponent>();
         script.Instance->OnUpdate();
      }
      
      m_FrameBuffer->Bind();

      ClearBackground(PURPLE);

      for (auto& entity : m_Scene->GetEntities<Core::TransformComponent>())
      {
        Core::TransformComponent& transform = entity->GetComponent<Core::TransformComponent>();
        Core::ColorComponent& color = entity->GetComponent<Core::ColorComponent>();

         DrawCircle(transform.Position.x, transform.Position.y, 10 * transform.Scale, Color(color.Color.r, color.Color.g, color.Color.b, color.Color.a));
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
   app->Init({1280, 720}, "Application");
   app->Run();
}