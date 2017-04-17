#include "update_rules_handler.h"
#include "ui_update_rules_handler.h"

#include <qdebug.h>

#include <imgui.h>
#include "imgui_impl_glfw.h"
#include <stdio.h>
#include <GLFW/glfw3.h>
#include "imguinodegrapheditor.h"   // intellisense only
#include "node_graph_instance.h"
#include <string.h>     //strcpy

#define MAX_ENUM_NAME_LENGTH    84

UpdateRulesHandler::UpdateRulesHandler(QWidget *parent) :
  QWidget(parent),
  mIsEditorOpen(false),
  ui(new Ui::UpdateRulesHandler)
{
  ui->setupUi(this);
}

UpdateRulesHandler::~UpdateRulesHandler()
{
//  if(mIsEditorOpen){
//    ImGui_ImplGlfw_Shutdown();
//    glfwTerminate();
//    glfwDestroyWindow(mNGEWindow);
//  }
  delete ui;
}
static void error_callback(int error, const char* description)
{
    qDebug()<< stderr << "Error %d: %s\n" << error << description;
}

void UpdateRulesHandler::on_pbtn_open_node_graph_editor_released()
{
  // Check if the editor is already open
  if(mIsEditorOpen)
    return;

  mIsEditorOpen = true;

  // Setup window
  glfwSetErrorCallback(error_callback);

  if (!glfwInit())
      qDebug()<< stderr << "GLFW nao conseguiu inicializar, e agora jose?";

  int width  = ui->gridLayout->geometry().width();
  int height = ui->gridLayout->geometry().height();
  mNGEWindow = glfwCreateWindow(width, height, "Update Rules Graph Editor", NULL, NULL);
  glfwMakeContextCurrent(mNGEWindow);

  // Setup ImGui binding
  ImGui_ImplGlfw_Init(mNGEWindow, true);

  // Load Fonts
  // (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
  //ImGuiIO& io = ImGui::GetIO();
  //io.Fonts->AddFontDefault();
  //io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
  //io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f);
  //io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
  //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

  char novoAttr[MAX_ENUM_NAME_LENGTH];
  novoAttr[0] = '\0';
  char addedAttr[MAX_ENUM_NAME_LENGTH];
  addedAttr[0] = '\0';

  ImVec4 clear_color = ImColor(114, 144, 154);

  // Initialize Node graph editor nge (add initial nodes conections and so on)
  InitNGE();

  int resized_width;
  int resized_height;
  // Main loop
  while (!glfwWindowShouldClose(mNGEWindow))
  {
      glfwPollEvents();
      ImGui_ImplGlfw_NewFrame();



      glfwGetWindowSize(mNGEWindow,&resized_width, &resized_height);
      ImGui::SetNextWindowSize(ImVec2(resized_width, resized_height), ImGuiSetCond_Always);
      //ImGui::SetNextWindowPos(ImVec2(0,0));
      //ImGui::SetNextWindowSizeConstraints(ImVec2(resized_width, resized_height), ImVec2(resized_width, resized_height));
      ImGui::Begin("Beringela", nullptr, ImGuiWindowFlags_NoResize |
                                         ImGuiWindowFlags_NoMove |
                                         ImGuiWindowFlags_NoCollapse |
                                         ImGuiWindowFlags_NoTitleBar);
                                         //ImGuiWindowFlags_ShowBorders |
                                         //ImGuiWindowFlags_NoSavedSettings |
      //ImGui::Text("DELICIA DE ABACAXI");
      nge.render();
      ImGui::End();

      // Rendering
      int display_w, display_h;
      glfwGetFramebufferSize(mNGEWindow, &display_w, &display_h);
      glViewport(0, 0, display_w, display_h);
      glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
      glClear(GL_COLOR_BUFFER_BIT);
      ImGui::Render();
      glfwSwapBuffers(mNGEWindow);
  }

  // Cleanup
  ImGui_ImplGlfw_Shutdown();
  glfwTerminate();

  mIsEditorOpen = false;
}
