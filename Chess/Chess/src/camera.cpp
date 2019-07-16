#include "camera.h"

const float Camera::TO_RADS = 3.141592654f / 180.0f;

Camera::Camera(float windowWidth, float windowHeight) :
	m_windowWidth(static_cast<int>(windowWidth)),
	m_windowHeight(static_cast<int>(windowHeight)),
	m_windowMidX(static_cast<int>(windowWidth / 2.0f)),
	m_windowMidY(static_cast<int>(windowHeight / 2.0f)),
	m_position (glm::vec3(700.f, 300.f, 200.f)),
	m_direction (glm::vec3(0.0f, 0.0f, 0.0f)),
	m_rotation (glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)),
	m_speed (glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)),
	m_movementSpeedFactor (100.0),
	m_pitchSensitivity (0.2f),
	m_yawSensitivity (0.2f),
	m_holdingForward (false),
	m_holdingBackward (false),
	m_holdingLeftStrafe (false),
	m_holdingRightStrafe (false),
	m_holdingZoomNegative (false),
	m_holdingZoomPositive (false)
{
    initCamera();
}

Camera::~Camera() 
{

}

void Camera::initCamera() 
{
    m_position = glm::vec3(700.f, 300.f, 200.f);
    m_direction = glm::vec3(0.0f, 0.0f, 0.0f);
    m_rotation = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    m_speed = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    m_movementSpeedFactor = 100.0;
    glm::vec3 initialPosition = cartesian2Polar(m_position);
    m_angleH = initialPosition[0];
    m_angleV = initialPosition[1];
    m_distance = initialPosition[2];

    m_pitchSensitivity = 0.2f;
    m_yawSensitivity = 0.2f;

    m_holdingForward = false;
    m_holdingBackward = false;
    m_holdingLeftStrafe = false;
    m_holdingRightStrafe = false;
    m_holdingZoomNegative = false;
    m_holdingZoomPositive = false;
}

const float Camera::toRads(const float& angleInDegrees) const 
{
    return angleInDegrees * TO_RADS;
}

void Camera::handleMouseMove(int mouseX, int mouseY) 
{
    float horizMovement = (mouseX - m_windowMidX + 1) * m_yawSensitivity;
    float vertMovement = (mouseY - m_windowMidY) * m_pitchSensitivity;

    m_rotation[0] += vertMovement;
    m_rotation[1] += horizMovement;

    if (m_rotation[0] < -90) 
	{
        m_rotation[0] = -90;
    }

    if (m_rotation[0] > 90) 
	{
        m_rotation[0] = 90;
    }

    if (m_rotation[1] < 0) 
	{
        m_rotation[1] += 360;
    }

    if (m_rotation[1] > 360) 
	{
        m_rotation[1] -= 360;
    }
}

glm::vec3 Camera::polar2Cartesian (float phi, float theta, float d) 
{
    glm::vec3 tmpPosition;
    tmpPosition.x = d * sin (theta) * cos (phi);
    tmpPosition.y = d * cos (theta);
    tmpPosition.z = d * sin (theta) * sin (phi);
    return tmpPosition;
}

glm::vec3 Camera::cartesian2Polar(glm::vec3 initPos) 
{
    float x = initPos.x;
    float y = initPos.y;
    float z = initPos.z;
    float phi = atan(y / x);
    float r = sqrt(x * x + y * y + z * z);
    float theta = acos(z / r);
    glm::vec3 tmpPosition = glm::vec3(phi, theta, r);
    return tmpPosition;
}

void Camera::move(int fps) 
{
    if (m_holdingForward) 
	{
        m_angleV -= toRads(45.f) / fps;
        if (m_angleV < toRads(0.001f)) 
		{
            m_angleV = toRads(0.001f);
        }
    }

    if (m_holdingBackward) 
	{
        m_angleV += toRads(45) / fps;
        if (m_angleV > toRads(90.0))
		{
            m_angleV = toRads(90.0);
        }
    }

    if (m_holdingLeftStrafe) 
	{
        m_angleH += toRads(180) / fps;
        if (m_angleH > toRads(360)) 
		{
            m_angleH = toRads(0);
        }
    }

    if (m_holdingRightStrafe)
	{
        m_angleH -= toRads(180) / fps;
        if (m_angleH < toRads(0)) 
		{
            m_angleH = toRads(360);
        }
    }

    if (m_holdingZoomNegative)
	{
        m_distance += static_cast<float>(500) / fps;
        if (m_distance >3000) 
		{
            m_distance -= static_cast<float>(500) / fps;
        }
    }

    if (m_holdingZoomPositive) 
	{
        m_distance -= static_cast<float>(500) / fps;
        if (m_distance < 0.001) 
		{
            m_distance += static_cast<float>(500) / fps;
        }
    }

    m_position = polar2Cartesian(m_angleH, m_angleV, m_distance);
}
