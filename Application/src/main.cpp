#include "Core.h"

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
   }

   void OnImGuiRender() override
   {
      ImGuiStyle& style = ImGui::GetStyle();
      style.WindowPadding = ImVec2(0, 0);
      
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
      m_FrameBuffer->Bind();

      ClearBackground(GetColor(0x312b47ff));

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