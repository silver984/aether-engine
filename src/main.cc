#include <aether/applications/engine.hh>
#include <aether/applications/setup.hh>

int main() {
	{
		aether::setup setup;
		if (!setup.init()) {
			return 1;
		}
		setup.run();
	}

	{
		aether::engine engine;
		if (!engine.init()) {
			return 1;
		}
		engine.run();
	}

	return 0;
}