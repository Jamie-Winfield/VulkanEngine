#include "Camera.h"
#define GLM_FORCE_SWIZZLE
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>


Camera::Camera(ProjectionMode _mode, glm::vec3 _position, glm::vec3 _lookat, float _width, float _height)
{
	projection_mode = _mode;
	view_width = _width;
	view_height = _height;
	SetPosition(_position);
	SetLookAt(_lookat);
	

	SetProjectionMulti();
	UpdateView();
	UpdateProjection();
	
}

void Camera::SetPosition(glm::vec3 _position)
{
	position = _position;
	cam_x = _position.x;
	cam_y = _position.y;
	cam_z = _position.z;
	SetLookAt(cam_x, cam_y, 0);
	UpdateView();
}

void Camera::SetPosition(float _x, float _y, float _z)
{
	cam_x = _x;
	cam_y = _y;
	cam_z = _z;
	position = glm::vec3(cam_x, cam_y, cam_z);
	SetLookAt(cam_x, cam_y, 0);
	UpdateView();
}

void Camera::ChangePosition(float _x, float _y, float _z)
{
	cam_x += _x;
	cam_y += _y;
	cam_z += _z;
	SetPosition(cam_x, cam_y, cam_z);
}

glm::vec3 Camera::GetPosition()
{
	return position;
}

void Camera::SetLookAt(glm::vec3 _lookat)
{
	lookat = _lookat;
	lookat_x = _lookat.x;
	lookat_y = _lookat.y;
	lookat_z = _lookat.z;
	UpdateView();
}

void Camera::SetLookAt(float _x, float _y, float _z)
{
	lookat_x = _x;
	lookat_y = _y;
	lookat_z = _z;
	lookat = glm::vec3(lookat_x, lookat_y, lookat_z);
	UpdateView();
}

void Camera::ChangeLookAt(float _x, float _y, float _z)
{
	lookat_x += _x;
	lookat_y += _y;
	lookat_z += _z;
	lookat = glm::vec3(lookat_x, lookat_y, lookat_z);
	UpdateView();
}

glm::vec3 Camera::GetLookAt()
{
	return lookat;
}

void Camera::SetWidth(float _width)
{
	view_width = _width;
	UpdateProjection();
}

void Camera::SetHeight(float _height)
{
	view_height = _height;
	UpdateProjection();
}

void Camera::SetWidthAndHeight(float _width, float _height)
{
	view_width = _width;
	view_height = _height;
	UpdateProjection();
}

void Camera::SetProjectionMode(ProjectionMode _mode)
{
	projection_mode = _mode;
	SetProjectionMulti();
	UpdateProjection();
}

glm::mat4 Camera::GetView()
{
	return view;
}

glm::mat4 Camera::GetProjection()
{
	return projection;
}

void Camera::SetProjectionMulti()
{
	switch (projection_mode)
	{
	case ORTHOGRATHIC:
	{
		projection_multi = 1;
		break;
	}
	case PERSPECTIVE:
	{
		projection_multi = -1;
		break;
	}
	}
}

void Camera::UpdateProjection()
{
	switch (projection_mode)
	{
	case ORTHOGRATHIC:
	{
		projection = glm::ortho(0.0f, view_width, view_height, 0.0f, -1000.f, 1000.f);
		break;
	}
	case PERSPECTIVE:
	{
		projection = glm::perspective(glm::radians(90.f), view_width / view_height, 0.1f, 10.f);
		break;
	}
	}
	projection[1][1] *= projection_multi;
	updated1 = true;
	updated2 = true;
	updated3 = true;
	
}

void Camera::UpdateView()
{
	view = glm::lookAt(position, lookat, up_vector);
	updated1 = true;
	updated2 = true;
	updated3 = true;
}
