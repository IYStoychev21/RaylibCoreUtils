#include <raylib.h>
#include "Core.h"
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
      entityOne.AddComponent<Core::TransformComponent>(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f);

      Core::Entity entityTwo(m_Scene);
      entityTwo.AddComponent<Core::TransformComponent>(glm::vec3(50.0f, 50.0f, 0.0f), 0.0);
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
      m_FrameBuffer->Bind();

      ClearBackground(PURPLE);

      for (auto& entity : m_Scene->GetEntities<Core::TransformComponent>())
      {
        Core::TransformComponent& transform = entity->GetComponent<Core::TransformComponent>();

         DrawCircle(transform.Position.x, transform.Position.y, 10, RED);
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