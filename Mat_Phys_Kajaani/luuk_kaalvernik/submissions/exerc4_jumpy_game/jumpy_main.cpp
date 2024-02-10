#include <mikroplot/window.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct Box
{
	glm::vec2 position;
	glm::vec2 halfsize;
	bool isColliding = false;
	bool isStatic = false;
	bool isPlayer = false;
	bool isJumpReset = false;
	bool isBouncy = false;
	glm::vec2 velocity = glm::vec2(0);
	glm::vec2 oldPosition;
};

// Create the map, maybe replace this with a level loader in the future?
void createMap(std::vector<Box>& boxes) {
	Box floor = { {0, -9.0f}, {9.0f, 0.5f} };
	floor.isStatic = true;
	floor.isJumpReset = true;
	boxes.push_back(floor);

	Box middlePlatform = { {0, -3.0f}, {4.5f, 0.5f} };
	middlePlatform.isStatic = true;
	middlePlatform.isJumpReset = true;
	boxes.push_back(middlePlatform);

	Box leftWall = { {-9.0, -4.0f}, {0.5f, 2.0f} };
	leftWall.isStatic = true; 
	leftWall.isJumpReset = true;
	boxes.push_back(leftWall);
	
	Box rightWall = { {9.0, -4.0f}, {0.5f, 2.0f} };
	rightWall.isStatic = true;
	rightWall.isBouncy = true;
	rightWall.isJumpReset = true;
	boxes.push_back(rightWall);

	Box topLeftWall = { {-3.0, 0}, {1.0f, 2.0f} };
	topLeftWall.isStatic = true;
	topLeftWall.isJumpReset = true;
	boxes.push_back(topLeftWall);
	
	Box topRightWall = { {3.0, 0}, {1.0f, 2.0f} };
	topRightWall.isStatic = true;
	boxes.push_back(topRightWall);
}

// We only have AABB collisions in the game by design.
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

	glm::mat4 mathModel = matTranslation * matScale;
	// matModel is the model matrix for our object.
	for (auto& vert : vertices)
	{
		// M *<vert.x, vert.y, 0, 1>
		vert = mathModel * glm::vec4(vert, 0, 1);
	}

	return vertices;
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

// TODO:
// Implement better collisions and different collisions based on the a/b
void reactCollision(auto& a, auto& b, const glm::vec2& normalVec) {
	// A normal surface will remove a lot of energy
	float dissapation = 0.1f;

	// If a surface is bouncy it will not remove a lot of energy/velocity on impact
	if (a.isBouncy || b.isBouncy)
	{
		dissapation = 1.5f;
	}

	a.velocity = dissapation * glm::reflect(a.velocity, normalVec);
	b.velocity = dissapation * glm::reflect(b.velocity, -normalVec);
}


int main() {
	mikroplot::Window window(900, 900, "AABB Points");
	
	std::vector<Box> boxes;

	// Create player first so it always ends up being on spot [0] in the vector
	Box player;
	player.position.x = 0.0;
	player.position.y = -8.0;
	player.halfsize.x = 0.5f;
	player.halfsize.y = 0.5f;
	player.isPlayer = true;

	int totalJumps = 2;
	int currentJumps = 2;

	boxes.push_back(player);

	createMap(boxes);

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

		// Move the player:
		float moveX = window.getKeyState(mikroplot::KEY_RIGHT) - window.getKeyState(mikroplot::KEY_LEFT);
		float jump = 0;

		// Implement jumping
		if (window.getKeyPressed(mikroplot::KEY_SPACE) && currentJumps > 0)
		{
			jump = 7.5f;
			currentJumps--;
		}
		
		// Position [0] is always the player
		// Update velocity instead of position to smooth out jump and to make sure hitboxes are working
		boxes[0].velocity += glm::vec2(moveX * deltaTime * 5, jump);

		simulate(boxes, deltaTime);

		// Reset collision flags from previous frame
		for (size_t i = 0; i < boxes.size(); ++i)
		{
			boxes[i].isColliding = false;
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

					if (boxes[ia].isPlayer && boxes[ib].isJumpReset)
					{
						currentJumps = totalJumps;
					}
					else if (boxes[ib].isPlayer && boxes[ia].isJumpReset) {
						currentJumps = totalJumps;
					}
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

			// Ugly but works
			if (box.isPlayer)
			{
				window.drawLines(points, 8, 5);
			}
			else if (box.isBouncy)
			{
				window.drawLines(points, 9, 5);
			}
			else if (box.isJumpReset)
			{
				window.drawLines(points, 10, 5);
			}
			else
			{
				window.drawLines(points, 11, 5);
			}
			// window.drawLines(points, box.isPlayer ? 8 : 11, 5);
		}

		window.update();
	}

	return 0;
}