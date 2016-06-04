////////////////////////
// CUSTOM
////////////////////////
#include "cameraInterface.hpp"



namespace OpenGLEngine
{

namespace camera
{
	///////////////////////////////////////////
	//	CONSTUCTOR & DESTRUCTOR
	///////////////////////////////////////////
	Camera::Camera(float aspect_ratio, glm::vec3 pos, glm::vec3 focus, float fov , std::pair<float, float> viewingFrustum)
	{
		this->aspect_ratio = aspect_ratio;
		this->cameraPos = pos;

		// NOTE: fov = atan(h * (do - f) / (2 * do * f) ) * 2 
		// were f  : camera focal length
		//		h  : sensor size
		//		do : focus distance
		// or <=> fov = 2 * atan( frame_size/(f * 2 * (m +1) )
		//	were m : maginfication = f / (d0 - f) 
		this->fov = fov;

		lookAt(focus);

		near_plane = viewingFrustum.first;
		far_plane = viewingFrustum.second;

		perspective(aspect_ratio);
	}



	///////////////////////////////////////////
	//	GETTERS
	///////////////////////////////////////////
	glm::vec3 Camera::getXAxis()
	{
		return this->xAxis;
	}
	glm::vec3 Camera::getYAxis()
	{
		return this->yAxis;
	}
	glm::vec3 Camera::getZAxis()
	{
		return this->zAxis;
	}


	
	glm::vec3 Camera::getCameraPosition()
	{
		return cameraPos;
	}

	glm::vec3 Camera::getCameraFocus()
	{
		return cameraFocus;
	}


	float Camera::getCameraFOV(bool rad)
	{
		if (rad)
		{
			float DEGTORAD = 0.017453292520;
			return DEGTORAD * fov;
		}

		return fov;
	}
	
	glm::mat4 Camera::getViewMatrix()
	{
		return this->viewMatrix;
	}

	glm::mat4 Camera::getProjectionMatrix()
	{
		return this->projectionMatrix;
	}



	std::pair<float, float> Camera::getNearFarPlane()
	{
		return std::make_pair(near_plane, far_plane);
	}






	///////////////////////////////////////////
	//	SETTERS
	///////////////////////////////////////////
	void Camera::setFOV(float fov)
	{
		this->fov = fov;
	}

	void Camera::setViewFurstum(std::pair<float, float> viewingFrustum)
	{
		this->near_plane = viewingFrustum.first;
		this->far_plane = viewingFrustum.second;
	}
	
	void Camera::setAspectRatio(float aspect_ratio)
	{
		this->aspect_ratio = aspect_ratio;
	}
	
	void Camera::setPositon(glm::vec3 pos)
	{
		this->cameraPos = pos;
	}
	
	void Camera::setFocus(glm::vec3 focus)
	{
		this->cameraFocus = focus;
	}

	void Camera::setViewMatrix(glm::mat4 viewMatrix)
	{
		this->viewMatrix = viewMatrix;
	}



	///////////////////////////////////////////
	//	UTILITY
	///////////////////////////////////////////
	void Camera::lookAt(glm::vec3 focus)
	{
		this->cameraFocus = focus;
		//glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraFocus);

		// Gram-Schmit to build camera local space
		// zAxis = target -> pos
		this->zAxis = glm::normalize(cameraPos - cameraFocus);
		// xAxis
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->xAxis = glm::normalize(glm::cross(up, zAxis));
		// zAxis
		this->yAxis = glm::cross(zAxis, xAxis);

		// change of basis matrix
		// Create a 4x4 orientation matrix from the right, up, and forward vectors
		// This is transposed which is equivalent to performing an inverse 
		// if the matrix is orthonormalized (in this case, it is).
		glm::mat4 orientation(
			xAxis.x, yAxis.x, zAxis.x, 0.0,
			xAxis.y, yAxis.y, zAxis.y, 0.0,
			xAxis.z, yAxis.z, zAxis.z, 0.0,
			0.0, 0.0, 0.0, 1.0);

		// Create a 4x4 translation matrix.
		// The eye position is negated which is equivalent
		// to the inverse of the translation matrix. 
		// T(v)^-1 == T(-v)
		glm::mat4 translation(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			-cameraPos.x, -cameraPos.y, -cameraPos.z, 1.0);



		//this->viewMatrix = orientation * translation;
		this->viewMatrix = glm::mat4(
			xAxis.x, yAxis.x, zAxis.x, 0.0,
			xAxis.y, yAxis.y, zAxis.y, 0.0,
			xAxis.z, yAxis.z, zAxis.z, 0.0,
			-glm::dot(xAxis, cameraPos), -glm::dot(yAxis, cameraPos), -glm::dot(zAxis, cameraPos), 1.0
			);

		//this->viewMatrix = glm::lookAt(this->cameraPos, this->cameraFocus, up);
	}



	
	void Camera::perspective(float aspectRatio)
	{
		projectionMatrix = glm::perspective(getCameraFOV(), aspectRatio, near_plane, far_plane);
	}

	glm::mat4 Camera::perspectiveProjection(float aspectRatio)
	{
		this->perspective(aspectRatio);
		return this->projectionMatrix;
	}




	void Camera::moveTo(glm::vec3 position)
	{
		this->cameraPos = position;
		lookAt(this->cameraFocus);
	}


}


}