#include <aether/context.hh>
#include <aether/node/components/sprite.hh>
#include <aether/node/components/transform.hh>
#include <aether/node/node.hh>
#include <aether/testscene.hh>
#include <aether/window.hh>
#include <aether/zip_archive.hh>

using namespace aether;

bool testscene::init_() {
	if (!scene::init_()) {
		return false;
	}

	zip_archive pak("aether.pak");
	strong_ref<node> boy = node::create(this->ctx_);

	if (!boy) {
		return false;
	}

	{
		strong_ref<sprite> sc = boy->add_component<sprite>();
		sc->set_texture(pak, "boy");
		sc->set_antialiasing(true);

		strong_ref<transform> tc = boy->component<transform>();
		tc->set_scale(0.6f);
		tc->set_position(static_cast<vec2<float>>(window::bounds()) * 0.5f);
		this->add_child(boy);
	}

	return true;
}