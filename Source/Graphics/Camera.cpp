#include "Camera.h"
#include "Graphics.h"
#include "Matrix.h"

Camera::Camera()
{
}

void Camera::dispose()
{
}

pgn::Camera* Graphics::createCamera()
{
	return debug_new Camera;
}

void Camera::_free()
{
	delete this;
}

void Camera::setFrustumLH(float w, float h, float n, float f)
{
	buildPerspectiveProjMatLH(&projMat, w, h, n, f);
}

void Camera::setFrustumFovLH(float fovY, float aspectRatio, float n, float f)
{
	buildPerspectiveProjMatFovLH(&projMat, fovY, aspectRatio, n, f);
}

void Camera::setOrthoLH(float w, float h, float n, float f)
{
	buildOrthogonalProjMatLH(&projMat.float4x3, w, h, n, f);
	projMat[3][0] = 0;
	projMat[3][1] = 0;
	projMat[3][2] = 0;
	projMat[3][3] = 1;
}

void Camera::setViewport(int left, int top, int width, int height, int fullHeight)
{
	viewport.left = left;
	viewport.top = top;
	viewport.width = width;
	viewport.height = height;
	viewport.fullHeight = fullHeight;
}

void Camera::setViewMat(pgn::Float4x3* mat)
{
	viewMat = *mat;
}
