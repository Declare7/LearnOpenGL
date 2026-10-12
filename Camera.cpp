#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>
#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 up, glm::vec3 front, float yaw, float pitch, float zoom)
    : m_position(position),
    m_worldUp(up),
    m_front(front),
    m_yaw(yaw),
    m_pitch(pitch),
    m_zoom(zoom)
{
    update();
}

float Camera::getZoom() const
{
    return m_zoom;
}

glm::mat4 Camera::getViewMetrix() const
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::vec3 Camera::getPosition() const
{
    return m_position;
}

void Camera::setMovementSpeed(float speed)
{
    m_movementSpeed = speed;
}

void Camera::setScrolledSensitivity(float sensitivity)
{
    m_scrolledSensitivity = sensitivity;
}

void Camera::onMoved(MoveDirection direct, float delta)
{
    float moveFactor = m_movementSpeed * delta;
    switch(direct)
    {
    case MOVE_LEFT:
        m_position += m_right * moveFactor;
        break;

    case MOVE_RIGHT:
        m_position -= m_right * moveFactor;
        break;

    case MOVE_FORWARD:
        m_position += m_front * moveFactor;
        break;

    case MOVE_BACKWARD:
        m_position -= m_front * moveFactor;
        break;

    default:
        break;
    }
}

void Camera::onScrolled(float xOffset, float yOffset)
{
    xOffset *= m_scrolledSensitivity;
    yOffset *= m_scrolledSensitivity;

    m_yaw += xOffset;
    m_pitch += yOffset;

    if(m_pitch> 89.0f)
        m_pitch = 89.0f;
    else if(m_pitch< -89.0f)
        m_pitch = -89.0f;

    update();
}

void Camera::onZoomed(float offset)
{
    m_zoom -= offset;
    if(m_zoom < 1.0f)
        m_zoom = 1.0f;
    else if(m_zoom > 45.0f)
        m_zoom = 45.0f;
}

void Camera::update()
{
    glm::vec3 front;
    front.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
    front.y = sin(glm::radians(m_pitch));
    front.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
    m_front = glm::normalize(front);

    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}
