#include <mikroplot/window.h>
#include <glm/glm.hpp>

///
/// \brief The Point class
///
struct Point {
	glm::vec2 position;
	glm::vec2 velocity = glm::vec2(0,0);
};


///
/// \brief Simulates body a single step forward in time.
/// \param rb = Rigid body to simulate.
/// \param dt = delta time.
/// \return Updated rigid body
///
template<typename Body, typename S>
Body simulate(Body body, S dt) {
	float mass = 1.0f;
	glm::vec2 acceleration = glm::vec2(0, -9.81f / mass);
	Body oldBody = body;

	// Eulers integration: Integrate new velocity by adding acceleration*dt to old velocity:
	body.velocity = body.velocity + acceleration * dt;
	
	// Eulers integration: Integrate new position by adding derivative*dt to old position:
	body.position = body.position + body.velocity * dt;

	// Collision responce to floor

#if 0


	if (body.position.y < 0.0f)
	{
		body.position = oldBody.position;
		body.velocity.y *= -0.9f;
	}
	
	if (body.position.x < -1.0f || body.position.x > 10.0f)
	{
		body.position = oldBody.position;
		body.velocity.x *= -0.9f;
	}
#else
	if (body.position.y < 0.0f) {
		glm::vec2 normal = glm::vec2(0, 1);
		body.velocity = 0.9f * glm::reflect(body.velocity, normal);
		body.position = oldBody.position;
	}
	if (body.position.y > 11.0f) {
		glm::vec2 normal = glm::vec2(0, -1);
		body.velocity = 0.9f * glm::reflect(body.velocity, normal);
		body.position = oldBody.position;
	}
	if (body.position.x < 0.0f) {
		glm::vec2 normal = glm::vec2(1, 0);
		body.velocity = 0.9f * glm::reflect(body.velocity, normal);
		body.position = oldBody.position;
	}
	if (body.position.x > 11.0f) {
		glm::vec2 normal = glm::vec2(-1, 0);
		body.velocity = 0.9f * glm::reflect(body.velocity, normal);
		body.position = oldBody.position;
	}
#endif // 0

	return body;
}

int main() {
	using namespace mikroplot;

	Window window(900, 900, "Particles point");
	Timer timer;


	std::vector<vec2> lines;
	Point body;
	body.position.x = 1.0f;
	body.position.y = 1.0f;
	body.velocity.x = 5.0f;
	body.velocity.y = 10.0f;
	body.velocity *= 0.5f;
	float startDelay = 2.0f;
	while (!window.shouldClose()) {
		float dt = timer.getDeltaTime();
		startDelay -= dt;
		if (startDelay < 0.0f)
		{
			body = simulate(body, dt);
		}

		// Update simulation
		body = simulate(body, dt);

		// Construct point(s) to draw from body position(s)
		std::vector<mikroplot::vec2> particlePosition;
		particlePosition.push_back({ body.position.x, body.position.y });

		// Render
		window.setScreen(-1, 11, -1, 11);
		window.drawAxis();
		
		window.drawPoints({vec2(body.position.x,body.position.y)}, 11, 10);
		window.update();
	}

	return 0;
}
