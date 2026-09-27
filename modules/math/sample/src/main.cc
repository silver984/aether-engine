#include <aether/log.hh>
#include <aether/rect.hh>
#include <aether/size.hh>
#include <aether/vec2.hh>

using namespace aether;

int main() {
	vec2<int> a{32};
	vec2<int> b{64};
	a += b;
	size<int> c = static_cast<size<int>>(a);
	log<info>({"({}, {})", c.width, c.height});
	return 0;
}