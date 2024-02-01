#include "raylib.h"
#include "Core.h"
#include "imgui.h"

class SandBoxLayer : public Core::Layer
{
public:
   void OnAttach() override
   {
      m_FrameBuffer.reset(new Core::FrameBuffer);
      m_FrameBuffer->Init(1280, 720);
   }

   void OnImGuiRender() override
   {
      ImGui::Begin("ViewPort");

      rlImGuiImageRenderTexture(m_FrameBuffer->GetTexture().get());

      ImGui::End();
   }

   void OnUpdate() override
   {
      m_FrameBuffer->Bind();

      ClearBackground(PURPLE);

      m_FrameBuffer->UnBind();
   }

private:
   std::shared_ptr<Core::FrameBuffer> m_FrameBuffer;
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