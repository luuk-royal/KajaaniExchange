#include <mikroplot/window.h>
#include <glm/glm.hpp>

struct Spring {
	float k = 0.1f;  // Spring constant
	float damping = 0.01f;  // Damping factor
	float restLength = 2; // spring length in rest
	glm::vec2 springRoot = glm::vec2(5,10);
	glm::vec2 springEnd = glm::vec2(7, 4);
	//glm::vec2 springEnd = glm::vec2(5, 0);
	glm::vec2 velocity = glm::vec2(0, 0);
};

template<typename Body, typename S>
Body simulate(Body body, S dt) {
	//float mass = 1.0f; 
	// 
	// Mass is consistantly 1 in this world.
	// This makes it so the force of the spring = acceleration.
	// F = m*a | F = 1*a = a

	glm::vec2 gravity = glm::vec2(0, -0.01f);

	// Spring force F = a
	glm::vec2 force = body.springRoot - body.springEnd;
	auto x = glm::length(force) - body.restLength;
	glm::vec2 springForce = glm::normalize(force);
	springForce *= body.k * x;

	// Damping
	glm::vec2 totalForce = springForce + gravity;
	body.velocity += totalForce;

	// Eulers integration
	glm::vec2 totalVelocity = body.velocity * dt;
	body.springEnd += totalVelocity;

	body.velocity *= (1.0f - body.damping);

	return body;
}

int main() {
	using namespace mikroplot;

	Window window(900, 900, "Spring Demo");
	Timer timer;

	// Use default spring
	Spring spring;

	while (!window.shouldClose()) {
		float dt = timer.getDeltaTime();

		// Update simulation
		spring = simulate(spring, dt);

		// Render
		window.setScreen(-5, 10, -5, 15);
		window.drawAxis();

		std::vector<vec2> lines;

		lines.push_back({ spring.springRoot.x, spring.springRoot.y });
		lines.push_back({ spring.springEnd.x, spring.springEnd.y });
		window.drawLines(lines, 5, 10);
		window.update();
	}

	return 0;
}
