#pragma once
#define GLM_SWIZZLE
#include <glm/glm.hpp>
class Camera
{
public:
	enum ProjectionMode
	{
		ORTHOGRATHIC,
		PERSPECTIVE
	};

	Camera(ProjectionMode _mode, glm::vec3 _position, glm::vec3 _lookat, float _width, float _height);

	void SetPosition(glm::vec3 _position);
	void SetPosition(float _x, float _y, float _z);
	void ChangePosition(float _x, float _y, float _z);
	glm::vec3 GetPosition();

	void SetLookAt(glm::vec3 _lookat);
	void SetLookAt(float _x, float _y, float _z);
	void ChangeLookAt(float _x, float _y, float _z);
	glm::vec3 GetLookAt();

	void SetWidth(float _width);
	void SetHeight(float _height);
	void SetWidthAndHeight(float _width, float _height);
	void SetProjectionMode(ProjectionMode _mode);

	glm::mat4 GetView();
	glm::mat4 GetProjection();

	bool updated1 = false;
	bool updated2 = false;
	bool updated3 = false;

private:
	float cam_x = 0;
	float cam_y = 0;
	float cam_z = 0;

	glm::vec3 position;

	float lookat_x = 0;
	float lookat_y = 0;
	float lookat_z = 0;

	float view_width = 0;
	float view_height = 0;

	glm::vec3 lookat;

	glm::vec3 up_vector = glm::vec3(0.f, 1.f, 0.f);

	glm::mat4 projection;
	glm::mat4 view;

	ProjectionMode projection_mode;

	float projection_multi = 0;

	void SetProjectionMulti();

	void UpdateProjection();
	void UpdateView();
};

