#include <mikroplot/window.h>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp> // Include this header for glm::linearRand
#include <glm/gtx/rotate_vector.hpp> // Include this header for glm::rotate

///
/// \brief The Point class
///
struct Point {
	glm::vec2 position;
	glm::vec2 velocity = glm::vec2(0,0);
	float lifeSpan = 0;
	float aliveTime = 0;

	bool isAlive(float dt) {
		aliveTime += dt;
		return lifeSpan >= aliveTime;
	}
};

struct ParticleEmitter {
	glm::vec2 position;
	// Make it have an initial velocity of 10.0f
	glm::vec2 direction = glm::vec2(0, 10);
	float coneAngle = 1;
	float angleRadiants;
	float minSpeed;
	float maxSpeed;
	float spawnedLifeSpan = 10;
	float particlesPerSecond = 15;
	float internalTimer = 0;

	void emitParticle(float dt, std::vector<Point>& points) {
		internalTimer += dt;

		if (internalTimer > (1.0f / particlesPerSecond)) {
			Point point;
			point.position = position;

			// Calculate random angle within the specified cone angle
			float randomAngle = glm::linearRand(-coneAngle / 2.0f, coneAngle / 2.0f);

			// Combine the emitter's direction and rotate it by angleRadiants
			glm::vec2 initialDirection = glm::rotate(direction, angleRadiants);

			// Rotate the initial direction vector by the random angle
			glm::mat2 ConeRotationMatrix(cos(randomAngle), -sin(randomAngle), sin(randomAngle), cos(randomAngle));
			glm::vec2 rotatedDirection = ConeRotationMatrix * initialDirection;

			// Set the velocity to the rotated direction
			point.velocity = glm::normalize(rotatedDirection) * maxSpeed;

			point.lifeSpan = spawnedLifeSpan;

			points.push_back(point);
			internalTimer = 0;
		}
	}

};

///
/// \brief Simulates body a single step forward in time.
/// \param rb = Rigid body to simulate.
/// \param dt = delta time.
/// \return Updated rigid body
///
template<typename Body, typename S>
Body simulate(Body body, S dt, glm::vec2 windForce, const ParticleEmitter& emitter) {
	float mass = 1.0f;
	glm::vec2 acceleration = glm::vec2(0, -9.81f / mass) + windForce / mass;
	Body oldBody = body;

	// Eulers integration: Integrate new velocity by adding acceleration*dt to old velocity:
	body.velocity = body.velocity + acceleration * dt;

	// Check and adjust velocity magnitude to ensure it does not drop below minSpeed
	float currentSpeed = glm::length(body.velocity);
	
	// Check if speed is not below max speed
	if (currentSpeed < emitter.minSpeed) {
        body.velocity = glm::normalize(body.velocity) * emitter.minSpeed;
    }

	// Check if speed is not above max speed
	if (currentSpeed > emitter.maxSpeed) {
        body.velocity = glm::normalize(body.velocity) * emitter.maxSpeed;
    }
	
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

	glm::vec2 windForce(0.0f, 0.0f);

	std::vector<Point> points;
	std::vector<vec2> lines;

	ParticleEmitter emitter;
	emitter.particlesPerSecond = 1;
	emitter.position.x = 1.0f;
	emitter.position.y = 1.0f;
	emitter.angleRadiants = 0.0f;
	emitter.spawnedLifeSpan = 5.0f;
	emitter.maxSpeed = 10.0f;
	emitter.minSpeed = 2.0f;

	/*Point body;
	body.position.x = 1.0f;
	body.position.y = 1.0f;
	body.velocity.x = 5.0f;
	body.velocity.y = 10.0f;
	body.velocity *= 0.5f;
	float startDelay = 2.0f;*/

	while (!window.shouldClose()) {
		float dt = timer.getDeltaTime();
		
		/*startDelay -= dt;
		if (startDelay < 0.0f)
		{
			body = simulate(body, dt);
		}*/

		emitter.emitParticle(dt, points);
		std::vector<mikroplot::vec2> particlePosition;

		// Update simulation
		for  (Point& body : points)
		{
			if (body.isAlive(dt))
			{
				body = simulate(body, dt, windForce, emitter);
				// Construct point(s) to draw from body position(s)
				particlePosition.push_back({ body.position.x, body.position.y });
			}
		}

		// Render
		window.setScreen(-1, 11, -1, 11);
		window.drawAxis();
		
		window.drawPoints(particlePosition, 11, 10);
		window.update();
	}

	return 0;
}
