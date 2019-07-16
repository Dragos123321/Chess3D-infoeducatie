#pragma once

#include <iostream>
#include <math.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera {
public:
    static const float TO_RADS;

    Camera() = default;
    Camera(float windowWidth, float windowHeight);
    ~Camera();

    void handleMouseMove(int mouseX, int mouseY);
    const float toRads(const float& angleInDegrees) const;
    void move(int fps);

    float getPitchSensitivity() { return m_pitchSensitivity; }
    void setPitchSensitivity(float value) { m_pitchSensitivity = value; }
    float getYawSensitivity() { return m_yawSensitivity; }
    void setYawSensitivity(float value) { m_yawSensitivity = value; }

    glm::vec3 getPosition() const { return m_position; }
    float getXPos() const { return m_position[0]; }
    float getYPos() const { return m_position[1]; }
    float getZPos() const { return m_position[2]; }

    glm::vec4 getRotation() const { return m_rotation; }
    float getXRot() const { return m_rotation[0]; }
    float getYRot() const { return m_rotation[1]; }
    float getZRot() const { return m_rotation[2]; }

    glm::vec3 getDirection() const { return m_direction; }
    void setDirection(glm::vec3 direction) { m_direction = direction; }

    bool getFW() { return m_holdingForward; }
    bool getBW() { return m_holdingBackward; }
    bool getLS() { return m_holdingLeftStrafe; }
    bool getRS() { return m_holdingRightStrafe; }
    bool getZP() { return m_holdingZoomPositive; }
    bool getZN() { return m_holdingZoomNegative; }

    void setFW(bool value) { m_holdingForward = value; }
    void setBW(bool value) { m_holdingBackward = value; }
    void setLS(bool value) { m_holdingLeftStrafe = value; }
    void setRS(bool value) { m_holdingRightStrafe = value; }
    void setZP(bool value) { m_holdingZoomPositive = value; }
    void setZN(bool value) { m_holdingZoomNegative = value; }

	void setPosition(glm::vec3 position) { m_position = position; }

private:
    glm::vec3 m_position;
    glm::vec3 m_direction;
    glm::vec4 m_rotation;
    glm::vec4 m_speed;

    float m_movementSpeedFactor; 
    float m_pitchSensitivity;
	float m_yawSensitivity;
    float m_angleH;
    float m_angleV;
    float m_distance;

    int m_windowWidth;
    int m_windowHeight;
    int m_windowMidX;
    int m_windowMidY;

    void initCamera();
    glm::vec3 polar2Cartesian (float phi, float theta, float d);
    glm::vec3 cartesian2Polar (glm::vec3);

    bool m_holdingForward;
    bool m_holdingBackward;
    bool m_holdingLeftStrafe;
    bool m_holdingRightStrafe;
    bool m_holdingZoomPositive;
    bool m_holdingZoomNegative;
};
