#include <aether/applications/engine.hh>
#include <aether/context.hh>
#include <aether/resources.hh>
#include <aether/testscene.hh>

namespace aether {

void engine::close_() {
	scene_scheduler_.cleanup_();
	_res_impl::resource_cleaner_::purge_all_();
}

void engine::pre_run_() {
	context const ctx{.scene_scheduler = &scene_scheduler_};
	scene_scheduler_.replace_scene(scene::create<testscene>(ctx));
}

} // namespace aether