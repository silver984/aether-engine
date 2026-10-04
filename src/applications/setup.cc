#include <aether/applications/setup.hh>

#include <imgui.h>

namespace aether {

void setup::draw_() {
	application::draw_();
	size<float> bounds = static_cast<size<float>>(window::bounds());
	ImGui::SetNextWindowSize({bounds.width, bounds.height});
	ImGui::SetNextWindowPos({0.f, 0.f});
	ImGui::Begin("##main", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
	ImGui::Text("Aether Setup");
	ImGui::End();
}

} // namespace aether