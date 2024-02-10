#include <mikroplot/window.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

struct Point {
    glm::vec2 root; // The root the position will rotate around
    glm::vec2 position;
    float angularVelocity = 1.0f;
};

template<typename Body, typename S>
void simulate(Body& body, S dt) {
    auto negativeRoot = glm::vec2({ -body.root.x, -body.root.y });

    body.position += negativeRoot;

    // Update position with rotation
    body.position = glm::rotate(body.position, body.angularVelocity * dt);

    body.position += body.root;
}

int main() {
    using namespace mikroplot;

    Window window(900, 900, "Rotating Vector");
    Timer timer;

    // The body that is rotating around point 2, 3
    Point body;
    body.root = glm::vec2(2, 3);
    body.position = glm::vec2(2, 6);
    body.angularVelocity = 1.0f;

    while (!window.shouldClose()) {
        float dt = timer.getDeltaTime();

        // Set the increments to be 1
        if (window.getKeyPressed(mikroplot::KEY_RIGHT))
        {
            body.angularVelocity += 1;
        }

        // Set the increments to be 1
        if (window.getKeyPressed(mikroplot::KEY_LEFT))
        {
            body.angularVelocity -= 1;
        }
        
        // Check for user input to point towards -5, 3 and stop moving
        if (window.getKeyPressed(mikroplot::KEY_SPACE)) {
            // Normalize the direction and subtract the root to make sure the targetDirection is calculated from the root position.
            glm::vec2 targetDirection = glm::normalize(glm::vec2(-5, 3) - body.root);

            // Calculate the diameter of the circle using the length of the body minus the root
            auto length = glm::length(body.position - body.root);

            // Move the targetDirection back to the root after pointing towards it.
            body.position = body.root + targetDirection * length;
            body.angularVelocity = 0.0f;
        }

        // Update simulation
        simulate(body, dt);

        // Render
        window.setScreen(-10, 10, -10, 10);
        window.drawAxis();
        window.drawPoints({ mikroplot::vec2( body.position.x, body.position.y ) }, 10, 10);
        window.drawPoints({ mikroplot::vec2( -5, 3 ) }, 10, 10);
        window.update();
    }

    return 0;
}
