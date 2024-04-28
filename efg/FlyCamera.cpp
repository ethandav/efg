/****************************************************************************
MIT License

Copyright (c) 2023 Guillaume Boissé

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/
#include "FlyCamera.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include <glm/ext/quaternion_trigonometric.hpp>

#include <iostream>

// cam speed
const float rotationSpeed = 0.01f;
const float zoomSpeed = 0.1f;

// mouse
bool rmbDown = false;
int prevMouseX, prevMouseY;

namespace
{

float CalculateHaltonNumber(uint32_t index, uint32_t base)
{
    float f = 1.0f, result = 0.0f;
    for(uint32_t i = index; i > 0;)
    {
        f /= base;
        result = result + f * (i % base);
        i = (uint32_t)(i / (float)base);
    }
    return result;
}

} //! unnamed namespace

FlyCamera CreateFlyCamera(GfxContext gfx, glm::vec3 const &eye, glm::vec3 const &center)
{
    FlyCamera fly_camera = {};

    float const aspect_ratio = gfxGetBackBufferWidth(gfx) / (float)gfxGetBackBufferHeight(gfx);

    fly_camera.eye    = eye;
    fly_camera.center = center;
    fly_camera.up     = glm::vec3(0.0f, 1.0f, 0.0f);

    fly_camera.view      = glm::lookAt(eye, center, fly_camera.up);
    fly_camera.proj      = glm::perspective(0.6f, aspect_ratio, 1e-1f, 1e4f);
    fly_camera.view_proj = fly_camera.proj * fly_camera.view;

    fly_camera.prev_view      = fly_camera.view;
    fly_camera.prev_proj      = fly_camera.proj;
    fly_camera.prev_view_proj = fly_camera.view_proj;

    return fly_camera;
}

void updateView(GfxContext gfx, FlyCamera &cam)
{
    float const aspect_ratio = gfxGetBackBufferWidth(gfx) / (float)gfxGetBackBufferHeight(gfx);

    cam.view      = glm::lookAt(cam.eye, cam.center, cam.up);
    cam.proj      = glm::perspective(0.6f, aspect_ratio, 1e-1f, 1e4f);
    cam.view_proj = cam.proj * cam.view;

    cam.prev_view      = cam.view;
    cam.prev_proj      = cam.proj;
    cam.prev_view_proj = cam.view_proj;
}

void UpdateFlyCamera(GfxContext gfx, GfxWindow window, FlyCamera &fly_camera)
{
    // Update camera history
    fly_camera.prev_view = fly_camera.view;
    fly_camera.prev_proj = fly_camera.proj;

    POINT cursorPos;
    GetCursorPos(&cursorPos);
    ScreenToClient(window, &cursorPos);

    int mouseX = cursorPos.x - (gfxGetBackBufferWidth(gfx) / 2);
    int mouseY = cursorPos.y - (gfxGetBackBufferHeight(gfx) / 2);

    if (rmbDown)
    {
        // Calculate the rotation angles based on mouse movement
        float yaw = -rotationSpeed * static_cast<float>(mouseX - prevMouseX);
        float pitch = rotationSpeed * static_cast<float>(mouseY - prevMouseY);

        float smoothFactor = 0.2f; // Adjust this value for smoother movement
        yaw = smoothFactor * yaw + (1.0f - smoothFactor) * fly_camera.prevYaw;
        pitch = smoothFactor * pitch + (1.0f - smoothFactor) * fly_camera.prevPitch;

        // Create quaternions for the yaw and pitch rotations
        glm::quat yawQuat = glm::angleAxis(yaw, fly_camera.up);
        glm::vec3 right = glm::normalize(glm::cross(fly_camera.up, fly_camera.center - fly_camera.eye));
        glm::quat pitchQuat = glm::angleAxis(pitch, right);

        // Apply the rotations to the direction vector
        glm::vec3 direction = fly_camera.center - fly_camera.eye;
        direction = yawQuat * direction;
        direction = pitchQuat * direction;

        // Update the camera's center position
        fly_camera.center = fly_camera.eye + direction;

        fly_camera.prevYaw = yaw;
        fly_camera.prevPitch = pitch;
    }

    prevMouseX = mouseX;
    prevMouseY = mouseY;

    // Handle camera translation based on keyboard input
    glm::vec3 forward = glm::normalize(fly_camera.center - fly_camera.eye);
    glm::vec3 right = glm::normalize(glm::cross(forward, fly_camera.up));
    if (GetAsyncKeyState('W') & 0x8000)
    {
        fly_camera.eye += forward * zoomSpeed;
        fly_camera.center += forward * zoomSpeed;
    }
    if (GetAsyncKeyState('S') & 0x8000)
    {
        fly_camera.eye -= forward * zoomSpeed;
        fly_camera.center -= forward * zoomSpeed;
    }
    if (GetAsyncKeyState('A') & 0x8000)
    {
        fly_camera.eye -= right * zoomSpeed;
        fly_camera.center -= right * zoomSpeed;
    }
    if (GetAsyncKeyState('D') & 0x8000)
    {
        fly_camera.eye += right * zoomSpeed;
        fly_camera.center += right * zoomSpeed;
    }

    // Handle mouse button states
    if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
    {
        // Right mouse button is held down
        rmbDown = true;
    }
    else
    {
        // Right mouse button is not held down
        rmbDown = false;
    }

    // Update view matrix
    fly_camera.view = glm::lookAt(fly_camera.eye, fly_camera.center, fly_camera.up);

    // Update projection matrix
    float const aspect_ratio = gfxGetBackBufferWidth(gfx) / (float)gfxGetBackBufferHeight(gfx);
    fly_camera.proj = glm::perspective(0.6f, aspect_ratio, 1e-1f, 1e4f);

    fly_camera.view_proj       = fly_camera.proj      * fly_camera.view;
    fly_camera.prev_view_proj  = fly_camera.prev_proj * fly_camera.prev_view;
}

glm::quat clampQuaternion(const glm::quat& q, float maxAngleRadians) {
    float angle = 2 * acos(q.w);
    glm::vec3 axis(q.x, q.y, q.z);

    if (glm::length(axis) > 0) {
        axis = glm::normalize(axis);
    } else {
        // Axis is zero vector, quaternion is already identity or close
        return glm::quat(1.0, 0.0, 0.0, 0.0);
    }

    if (angle > maxAngleRadians) {
        angle = maxAngleRadians;
    }

    return glm::angleAxis(angle, axis);
}

void UpdatePlayerCamera(GfxContext gfx, GfxWindow window, FlyCamera &player_camera)
{
    // Update camera history
    player_camera.prev_view = player_camera.view;
    player_camera.prev_proj = player_camera.proj;

    POINT cursorPos;
    GetCursorPos(&cursorPos);
    ScreenToClient(window, &cursorPos);

    int mouseX = cursorPos.x - (gfxGetBackBufferWidth(gfx) / 2);
    int mouseY = cursorPos.y - (gfxGetBackBufferHeight(gfx) / 2);

    // Calculate the rotation angles based on mouse movement
    float yaw = -rotationSpeed * static_cast<float>(mouseX - prevMouseX);
    float pitch = rotationSpeed * static_cast<float>(mouseY - prevMouseY);

    float smoothFactor = 0.2f; // Adjust this value for smoother movement
    yaw = smoothFactor * yaw + (1.0f - smoothFactor) * player_camera.prevYaw;
    pitch = smoothFactor * pitch + (1.0f - smoothFactor) * player_camera.prevPitch;

    glm::vec3 direction = glm::normalize(player_camera.center - player_camera.eye);

    glm::vec3 right = glm::normalize(glm::cross(player_camera.up, direction));

    // Create quaternions for the yaw and pitch rotations
    glm::quat yawQuat = glm::angleAxis(yaw, player_camera.up);
    glm::quat pitchQuat = glm::angleAxis(pitch, right);

    // Apply the rotations to the direction vector
    direction = yawQuat * direction;
    direction = pitchQuat * direction;
    direction = glm::normalize(direction);

    // Update the camera's center position
    player_camera.center = player_camera.eye + direction;

    player_camera.prevYaw = yaw;
    player_camera.prevPitch = pitch;

    prevMouseX = mouseX;
    prevMouseY = mouseY;

    // Handle camera translation based on keyboard input
    glm::vec3 forward = glm::normalize(glm::cross(right, player_camera.up)); // Forward direction on XZ plane
    glm::vec3 horizontalForward = glm::normalize(glm::vec3(forward.x, 0.0f, forward.z)); // Remove Y component

    if (GetAsyncKeyState('W') & 0x8000)
    {
        player_camera.eye += horizontalForward * zoomSpeed;
        player_camera.center += horizontalForward * zoomSpeed;
    }
    if (GetAsyncKeyState('S') & 0x8000)
    {
        player_camera.eye -= horizontalForward * zoomSpeed;
        player_camera.center -= horizontalForward * zoomSpeed;
    }
    if (GetAsyncKeyState('A') & 0x8000)
    {
        player_camera.eye += right * zoomSpeed;
        player_camera.center += right * zoomSpeed;
    }
    if (GetAsyncKeyState('D') & 0x8000)
    {
        player_camera.eye -= right * zoomSpeed;
        player_camera.center -= right * zoomSpeed;
    }

    // Handle mouse button states
    if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
    {
        // Right mouse button is held down
        rmbDown = true;
    }
    else
    {
        // Right mouse button is not held down
        rmbDown = false;
    }

    // Update view matrix
    player_camera.view = glm::lookAt(player_camera.eye, player_camera.center, player_camera.up);

    // Update projection matrix
    float const aspect_ratio = gfxGetBackBufferWidth(gfx) / (float)gfxGetBackBufferHeight(gfx);
    player_camera.proj = glm::perspective(0.6f, aspect_ratio, 1e-1f, 1e4f);

    player_camera.view_proj = player_camera.proj * player_camera.view;
    player_camera.prev_view_proj = player_camera.prev_proj * player_camera.prev_view;
}
