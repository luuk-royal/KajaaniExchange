#include <mikroplot/window.h>
#include <math_utils.h>

///
/// \brief The Point class
///
struct Point {
	glm::vec2 s;
	glm::vec2 v = glm::vec2(0,0);
};


///
/// \brief Simulates body a single step forward in time.
/// \param rb = Rigid body to simulate.
/// \param dt = delta time.
/// \return Updated rigid body
///
template<typename Body, typename S>
Body simulate(Body body, S dt) {
	auto dPos = [&body](const decltype(body.s)& x0, S dt) {
		// Derivative of position is velocity
		return body.v;
	};
	body.s = integrate::eulerl<S>(body.s, dt, dPos);
	return body;
}

int main() {
	using namespace mikroplot;

	Window window(900, 900, "Integrate point");
	Timer timer;

	std::vector<vec2> lines;
	Point body = {
		{0,10},
		{0,0}
	};


	while (!window.shouldClose()) {
		float dt = timer.getDeltaTime();

		// Update simulation
		body = simulate(body, dt);

		// Render
		window.setScreen(-1, 11, -1, 11);
		window.drawAxis();
		window.drawLines(lines, 12, 3);
		window.drawPoints({vec2(body.s[0],body.s[1])}, 11, 10);
		window.update();
	}

	return 0;
}














