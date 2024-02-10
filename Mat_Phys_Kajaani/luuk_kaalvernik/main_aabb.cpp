#include <mikroplot/window.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct Box
{
	glm::vec2 position;
	glm::vec2 halfsize;
	float rotation = 0.0f;
	bool isColliding = false;
	bool isStatic = false;
	glm::vec2 velocity = glm::vec2(0);
	glm::vec2 oldPosition;
};

struct Sphere {
	glm::vec2 position;
	float radius = 0.5f;
	bool isColliding = false;
	bool isStatic = false;
	glm::vec2 velocity = glm::vec2(0);
	glm::vec2 oldPosition;
};

bool isAABBCollision(const Box& a, const Box& b, glm::vec2& normalVec) {
	glm::vec2 d = b.position - a.position;
	glm::vec2 hs = b.halfsize + a.halfsize;
	bool x = std::abs(d.x) < hs.x;
	bool y = std::abs(d.y) < hs.y;

	if (x && y)
	{
		float overlapX = hs.x - std::abs(d.x);
		float overlapY = hs.y - std::abs(d.y);
		// normalVec = glm::normalize(-d);
		if (overlapX > overlapY)
		{
			normalVec.y = overlapY;
			normalVec.x = 0;
			normalVec = glm::normalize(normalVec);
		} else
		{
			normalVec.y = 0;
			normalVec.x = overlapX;
			normalVec = glm::normalize(normalVec);
		}
		return true;
	}

	return false;
}

bool isSphereSphereCollision(const Sphere& a, const Sphere& b, glm::vec2& normalVec) {
	float d = glm::length(b.position - a.position);
	float rTot = b.radius + a.radius;

	if (d < rTot) {
		normalVec = glm::normalize(a.position - b.position);

		return true;
	}

	return false;

}


std::vector<glm::vec2> getVertices(const Box& box, float scale) {
	std::vector<glm::vec2> vertices;

	vertices.push_back(glm::vec2(-box.halfsize.x, box.halfsize.y)); // Top left
	vertices.push_back(glm::vec2(-box.halfsize.x, -box.halfsize.y)); // Bottem left
	vertices.push_back(glm::vec2(box.halfsize.x, -box.halfsize.y)); // Bottem right
	vertices.push_back(glm::vec2(box.halfsize.x, box.halfsize.y)); // Top right
	// Top left (again because we want to close "line loop")
	vertices.push_back(glm::vec2(-box.halfsize.x, box.halfsize.y));

	// Transform vertices using matrices
	glm::mat4 matTranslation = glm::translate(glm::vec3(box.position /*x,y*/, 0.0f /*z == 0*/));
	glm::mat4 matScale = glm::scale(glm::vec3(scale, scale, scale));
	glm::mat4 matRotation = glm::rotate(box.rotation, glm::vec3(0, 0, 1));

	glm::mat4 mathModel = matTranslation * matRotation * matScale;
	// matModel is the model matrix for our object.
	for (auto& vert : vertices)
	{
		// M *<vert.x, vert.y, 0, 1>
		vert = mathModel * glm::vec4(vert, 0, 1);
	}

	// glm::mat4 MVP = matProjction * glm::inverse(matview) * matModel,

	return vertices;
}

bool isSphereAABBCollision(const Sphere& sphere, const Box& b, glm::vec2& normalVec) {
	auto verts = getVertices(b, 1.0f); // Get vertices with scale of one
	for (const auto vertex : verts) {

		auto d = glm::length(vertex - sphere.position);

		if (d < sphere.radius)
		{
			normalVec = glm::normalize(sphere.position - b.position);

			return true;
		}
	}

	return false;
}

void simulate(auto& objects, float deltaTime) {
	for (auto& obj : objects){
		obj.oldPosition = obj.position;
		if (obj.isStatic)
		{
			continue;
		}
		auto oldPosition = obj.position;
		glm::vec2 gravity(0, -9.81f);
		
		obj.velocity += gravity * deltaTime;
		obj.position += obj.velocity * deltaTime;
	}
}

void reactCollision(auto& a, auto& b, const glm::vec2& normalVec) {
	a.velocity = 0.9f * glm::reflect(a.velocity, normalVec);
	b.velocity = 0.9f * glm::reflect(b.velocity, -normalVec);
}

int main() {
	mikroplot::Window window(900, 900, "AABB Points");
	
	std::vector<Sphere> spheres;
	std::vector<Box> boxes;

	// static ground
	boxes.push_back({ {0, -9.0f}, {9.0f, 0.5f} });
	boxes.back().isStatic = true;
	boxes.push_back({ {-9.0, 0.0f}, {0.5f, 7.5f} });
	boxes.back().isStatic = true;
	boxes.push_back({ {9.0, 0.0f}, {0.5f, 7.5f} });
	boxes.back().isStatic = true;
	boxes.push_back({ {0, 9.0f}, {9.0f, 0.5f} });

	boxes.back().isStatic = true;

	Box box1;
	box1.position.x = 5.0;
	box1.position.y = 8.0;
	box1.halfsize.x = 1.0f;
	box1.halfsize.y = 0.5f;
	box1.rotation = 0.0f; // in radiants

	boxes.push_back(box1);

	Box box2;
	box2.position.x = -5.0;
	box2.position.y = 4.0;
	box2.halfsize.x = 3.0f;
	box2.halfsize.y = 3.5f;
	box2.rotation = 0.0f; // in radiants

	boxes.push_back(box2);

	spheres.push_back({ Sphere { {-2.0f, 1.0f}, 1.0f } });
	spheres.push_back({ Sphere { {1.0f, -1.0f}, 0.5f } });

	mikroplot::Timer timer;
	float totalTime = 0;
	while (window.shouldClose() == false)
	{
		float deltaTime = timer.getDeltaTime();
		if (deltaTime > 0.1f)
		{
			deltaTime = 0.1f;
		}
		totalTime += deltaTime;

		// Move box[0]:
		float moveX = window.getKeyState(mikroplot::KEY_RIGHT) - window.getKeyState(mikroplot::KEY_LEFT);
		float moveY = window.getKeyState(mikroplot::KEY_UP) - window.getKeyState(mikroplot::KEY_DOWN);
		spheres[0].position += glm::vec2(moveX, moveY) * deltaTime;

		simulate(boxes, deltaTime);
		simulate(spheres, deltaTime);

		// Reset collision flags from previous frame
		for (size_t i = 0; i < boxes.size(); ++i)
		{
			boxes[i].isColliding = false;
		}

		for (size_t i = 0; i < spheres.size(); ++i)
		{
			spheres[i].isColliding = false;
		}

		// Check collisions between each object:
		for (size_t ia = 0; ia < boxes.size(); ++ia)
		{
			for (size_t ib = ia + 1; ib < boxes.size(); ++ib)
			{
				glm::vec2 normal;
				if (isAABBCollision(boxes[ia], boxes[ib], normal))
				{
					boxes[ia].isColliding = true;
					boxes[ib].isColliding = true;
					boxes[ia].position = boxes[ia].oldPosition;
					boxes[ib].position = boxes[ib].oldPosition;
					reactCollision(boxes[ia], boxes[ib], normal);
				}
			}
		}
		
		for (size_t ia = 0; ia < spheres.size(); ++ia)
		{
			for (size_t ib = ia + 1; ib < spheres.size(); ++ib)
			{
				glm::vec2 normal;
				if (isSphereSphereCollision(spheres[ia], spheres[ib], normal))
				{
					spheres[ia].isColliding = true;
					spheres[ib].isColliding = true;
					spheres[ia].position = spheres[ia].oldPosition;
					spheres[ib].position = spheres[ib].oldPosition;
					reactCollision(spheres[ia], spheres[ib], normal);
				}
			}
		}
		
		for (size_t ia = 0; ia < spheres.size(); ++ia)
		{
			for (size_t ib = 0; ib < boxes.size(); ++ib)
			{
				glm::vec2 normal;
				if (isSphereAABBCollision(spheres[ia], boxes[ib], normal))
				{
					spheres[ia].isColliding = true;
					boxes[ib].isColliding = true;
					spheres[ia].position = spheres[ia].oldPosition;
					boxes[ib].position = boxes[ib].oldPosition;
					reactCollision(spheres[ia], boxes[ib], normal);
				}
			}
		}

		window.setScreen(-10, 10, -10, 10);
		window.drawAxis();

		// sin goes from 0.5 - 1.0
		// float rpm = 1.0f;
		// float angularspeed = rpm * 2.0f * 3.141592635f;
		// Note: angularspeed, in this case is the frequency of "pulsating scale"

		// float scalingFactor = 0.75f + (0.25f*std::sinf(totalTime*angularspeed));

		// float w is angular velocity of rigid body:
		// float w = 1.0f;
		// box1.rotation = box1.rotation + 0.5f * w * deltaTime;

		for (const auto& box : boxes)
		{
			// getVertices from box and construct mikroplot::vec2 vertices
			std::vector<mikroplot::vec2> points;
			for (auto point : getVertices(box, 1.0))
			{
				points.push_back({ point.x, point.y });
			}
			window.drawLines(points, box.isColliding ? 8 : 11, 5);
		}

		for (const auto& sphere : spheres) {
			window.drawCircle({ sphere.position.x, sphere.position.y }, sphere.radius, sphere.isColliding ? 8 : 11);
		}

		window.update();
	}

	return 0;
}