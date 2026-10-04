#include <aether/applications/engine.hh>

int main() {
	aether::engine engine;

	if (engine.init()) {
		engine.run();
		return 0;
	}

	return 1;
}