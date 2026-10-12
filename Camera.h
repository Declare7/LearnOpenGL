#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:
    enum MoveDirection
    {
        MOVE_LEFT = 0,
        MOVE_RIGHT,
        MOVE_FORWARD,
        MOVE_BACKWARD
    };

public:
    explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
                    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f),
                    float yaw = -90.0f,
                    float pitch = 0.0f,
                    float zoom = 45.0f);

    float getZoom() const;

    glm::mat4 getViewMetrix() const;

    glm::vec3 getPosition() const;

    void setMovementSpeed(float speed);

    void setScrolledSensitivity(float sensitivity);

    void onMoved(MoveDirection direct, float delta);

    void onScrolled(float xOffset, float yOffset);

    void onZoomed(float offset);

private:
    void update();

private:
    glm::vec3 m_position;
    glm::vec3 m_worldUp;
    glm::vec3 m_up;
    glm::vec3 m_front;
    glm::vec3 m_right;

    float m_yaw{0.0};
    float m_pitch{0.0};
    float m_zoom{0.0};

    float m_movementSpeed{2.5f};
    float m_scrolledSensitivity{0.1f};
};

#endif // CAMERA_H
