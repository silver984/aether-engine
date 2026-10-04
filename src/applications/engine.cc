#include <aether/applications/engine.hh>
#include <aether/context.hh>
#include <aether/resources.hh>
#include <aether/testscene.hh>

#include <imgui.h>

namespace aether {

void engine::close_() {
	application::close_();
	scene_scheduler_.cleanup_();
	_res_impl::resource_cleaner_::purge_all_();
}

void engine::pre_run_() {
	application::pre_run_();
	context const ctx{.scene_scheduler = &scene_scheduler_};
	scene_scheduler_.replace_scene(scene::create<testscene>(ctx));
}

void engine::update_(float dt) {
	application::update_(dt);
	scene_scheduler_.update_scene_(dt);

	while (time_elapsed_ >= 1.f) {
		time_elapsed_ -= 1.f;
		evaluated_fps_ = frame_count_;
		frame_count_   = 0;
	}

	time_elapsed_ += dt;
}

void engine::draw_() {
	application::draw_();
	scene_scheduler_.draw_scene_();

	ImGui::SetNextWindowSize({75.f, 30.f});
	ImGui::SetNextWindowPos({5.f, 5.f});
	ImGui::Begin("##debug-overlay", nullptr,
	             ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoCollapse |
	                     ImGuiWindowFlags_NoTitleBar);
	ImGui::Text("FPS: %u", evaluated_fps_);
	ImGui::End();

	++frame_count_;
}

} // namespace aether