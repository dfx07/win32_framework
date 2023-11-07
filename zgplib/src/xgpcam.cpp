#include "xgpcam.h"

____BEGIN_SECTION____(gp)

template<CameraType T>
Camera<T>::Camera() : m_iWidth(0), m_iHeight(0), m_fFar(0.f), m_fNear(0.f)
{
	m_position = { 0.f, 0.f, 0.f };
	m_direction = { 0.f, 0.f, 1.f };
	m_up = { 0.f, 1.f, 0.f };

	m_viewMat = glm::mat4(1.f);
	m_projMat = glm::mat4(1.f);
	m_modelMat = glm::mat4(1.f);
}

template<CameraType T>
Camera<T>::~Camera()
{

}

template<CameraType T>
CameraType Camera<T>::GetType()
{
	return m_type;
}

/*******************************************************************************
*! @brief  : Chuyển đổi từ tọa độ [Trái trên] sang tọa độ tại [Trung tâm] trên view
*! @param    [in] p : tọa độ trên view
*! @return : glm::vec2 tọa độ thực tế
*! @author : thuong.nv          - [Date] : 22/04/2023
*! @note   : Tọa độ [x, y] đầu vào là tọa độ trên view  (Left Top)
*******************************************************************************/
template<CameraType T>
glm::vec2 Camera<T>::PointLT2Center(const glm::vec2& p) const
{
	glm::vec2 point = { 0, 0 };  // Để mặc định
	point.x = (p.x - m_iWidth / 2);
	point.y = -(p.y - m_iHeight / 2);
	return point;
}

/*******************************************************************************
*! @brief  : Chuyển đổi từ tọa độ [Trái trên] sang tọa độ tại [Trung tâm] trên view   [3D]
*! @param    [in] p : tọa độ trên view
*! @return : glm::vec2 tọa độ thực tế
*! @author : thuong.nv          - [Date] : 22/04/2023
*! @note   : Tọa độ [x, y] đầu vào là tọa độ trên view  (Left Top)
*******************************************************************************/
template<CameraType T>
glm::vec3 Camera<T>::PointLT2Center(const glm::vec3& p) const
{
	glm::vec3 point = { 0, 0 ,  p.z };  // Để mặc định
	point.x =  (p.x - m_iWidth / 2);
	point.y = -(p.y - m_iHeight / 2);
	return point;
}

template<CameraType T>
glm::mat4& Camera<T>::GetViewMatrix()
{
	return m_viewMat;
}

template<CameraType T>
glm::mat4& Camera<T>::GetProjMatrix()
{
	return m_projMat;
}

template<CameraType T>
glm::mat4& Camera<T>::GetModelMatrix()
{
	return m_modelMat;
}

template<CameraType T>
void Camera<T>::InitView(int width, int height, float _near, float _far)
{
	m_fNear   = _near;
	m_fFar    = _far;
	m_iWidth  = width;
	m_iHeight = height;
}

template<CameraType T>
void Camera<T>::SetUpCamera(glm::vec3 pos, glm::vec3 dir, glm::vec3 up)
{
	m_position = pos;
	m_direction = dir;
	m_up = up;
}

template<CameraType T>
void Camera<T>::SetViewSize(int width, int height)
{
	m_iWidth = width;
	m_iHeight = height;
}

template<CameraType T>
void Camera<T>::SetDistPlane(float _near, float _far)
{
	m_fNear = _near;
	m_fFar = _far;
}

template<CameraType T>
glm::vec3 Camera<T>::GetPosition()
{
	return m_position;
}

template<CameraType T>
void Camera<T>::SetPosition(glm::vec3 pos)
{
	m_position = pos;
}

template<CameraType T>
void Camera<T>::SetPosition(float phi, float theta, float distance, glm::vec3 target)
{
	// Tính điểm tại vị trí tâm
	float x = cos(phi) * sin(theta);
	float y = sin(phi);
	float z = cos(phi) * cos(theta);
	glm::vec3 position = glm::vec3(x, y, z) * distance;

	// Dịch chuyển điểm đó đến target
	float dis = glm::distance(target, glm::vec3(0, 0, 0));
	if (fabs(dis) > 0.1)
	{
		glm::vec3 vut = glm::normalize(target - glm::vec3(0, 0, 0));

		m_position = position + (vut * dis);
		m_direction = glm::normalize(m_position - target);
	}
	else
	{
		m_position = position;
	}
}

template<CameraType T>
void Camera<T>::Move(glm::vec3 mov)
{
	m_position += mov;
	this->UpdateViewMatrix();
}


/// <summary>
/// Camera2D ////////////////////////////////////////////////////////////
/// </summary>

Camera2D::Camera2D(): Camera(),
	m_fZoom(1.f)
{
	m_projMatNozoom = glm::mat4(1.f);
}

Camera2D::~Camera2D()
{

}

/*******************************************************************************
*! @brief  : Chuyển đổi tọa độ điểm từ view sang tọa độ thực tế
*! @param    [in] p : tọa độ trên view
*! @return : glm::vec2 tọa độ thực tế
*! @author : thuong.nv          - [Date] : 22/04/2023
*! @note   : Tọa độ [x, y] đầu vào là tọa độ trên view  (Left Top)
*******************************************************************************/
glm::vec2 Camera2D::PointLocal2Global(const glm::vec2& p) const
{
	glm::vec2 point = { 0 , 0 };    // Để mặc định
	point.x = m_position.x + (p.x - float(m_iWidth / 2)) / m_fZoom;
	point.y = m_position.y - (p.y - float(m_iHeight / 2)) / m_fZoom;
	return point;
}

/*******************************************************************************
*! @brief  : Chuyển đổi tọa độ điểm từ thực thế sang tọa độ view
*! @param    [in] p : real position to local view
*! @return : glm::vec2 view position
*! @author : thuong.nv          - [Date] : 22/04/2023
*! @note   : Tọa độ [x, y] đầu vào là tọa độ trên view  (Left Top)
*******************************************************************************/
glm::vec2 Camera2D::PointGlobal2Local(const glm::vec2& p) const
{
	glm::vec2 point = { 0 , 0 };   // Để mặc định
	point.x = (float(m_iWidth / 2) - m_fZoom * (m_position.x - p.x));
	point.y = (m_fZoom * (m_position.y - p.y) + float(m_iHeight / 2));
	return point;
}

glm::mat4 Camera2D::GetMatrixNozoom()
{
	return  m_projMatNozoom;
}

/*******************************************************************************
*! @brief  : Cập nhật view matrix
*! @param    [in] mode : Chế độ zoom hoặc không zoom
*! @return : void
*! @author : thuong.nv          - [Date] : 22/04/2023
*******************************************************************************/
void Camera2D::UpdateViewMatrix()
{
	m_viewMat = glm::lookAt(m_position, m_position - m_direction, m_up);
}

/*******************************************************************************
*! @brief  : Cập nhật projection matrix
*! @param    [in] mode : Chế độ zoom hoặc không zoom
*! @return : void
*! @author : thuong.nv          - [Date] : 22/04/2023
*******************************************************************************/
void Camera2D::UpdateProjMatrix()
{
	// Projection matrix không sử dụng zoom
	float left = -float(m_iWidth / 2);
	float right = float(m_iWidth / 2);
	float bottom = -float(m_iHeight / 2);
	float top = float(m_iHeight / 2);

	m_projMatNozoom = glm::ortho(left, right, bottom, top, float(m_fNear), float(m_fFar));

	// Projection matrix sử dụng thuộc tính zoom
	left = left / m_fZoom;
	right = right / m_fZoom;
	bottom = bottom / m_fZoom;
	top = top / m_fZoom;

	m_projMat = glm::ortho(left, right, bottom, top, float(m_fNear), float(m_fFar));
}

/*******************************************************************************
*! @brief  : Load ma trận cho  GPU
*! @param    [in] mode : Chế độ zoom hoặc không zoom
*! @return : void
*! @author : thuong.nv          - [Date] : 22/04/2023
*******************************************************************************/
void Camera2D::UpdateMatrix()
{
	this->UpdateViewMatrix();
	this->UpdateProjMatrix();
}

/*******************************************************************************
*! @brief  : Load ma trận cho  GPU
*! @param    [in] mode : Chế độ zoom hoặc không zoom
*! @return : void
*! @author : thuong.nv          - [Date] : 22/04/2023
*******************************************************************************/
void Camera2D::UseMatrix(CameraMode mode) const
{
	// Use OpenGL >= 1.1 có thể sử dụng shader
	if (mode == CameraMode::CMODE_NOZOOM)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glm::value_ptr(m_projMatNozoom));

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	else
	{
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glm::value_ptr(m_projMat));

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glm::value_ptr(m_viewMat));
	}
}

void Camera2D::LoadMatrix(GLuint program) const
{
	GLint mvLoc = glGetUniformLocation(program, "mv_matrix");
	GLint projLoc = glGetUniformLocation(program, "proj_matrix");

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(m_viewMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(m_projMat));
}

/*******************************************************************************
*! @brief  : Cập nhật thông số zoom có giới hạn giá trị
*! @return : void
*! @author : thuong.nv          - [Date] : 22/04/2023
*******************************************************************************/
void Camera2D::UpdateZoom(float zDelta)
{
	// [Note] Điều chỉnh zoom này có lợi thế là sẽ zoom đều             
	// Khi zoom càng bé thì giảm zoom càng chậm thay cho việc tuyến tính
	// Tương tự nếu tăng sẽ nhanh nếu nếu zoom càng lớn                 
	if (zDelta > 0)
	{
		m_fZoom /= ZOOM_FACTOR;
	}
	else
	{
		m_fZoom *= ZOOM_FACTOR;
	}

	// Xử lý trong trường hợp giới hạn
	if (m_fZoom < ZOOM_MIN)
	{
		m_fZoom = ZOOM_MIN;
	}

	else if (m_fZoom > ZOOM_MAX)
	{
		m_fZoom = ZOOM_MAX;
	}
}

float Camera2D::GetZoom() const
{
	return m_fZoom;
}

/*******************************************************************************
*! @brief  : Target camera vào một vị trí (tọa độ local) với lượng zoom delta
*! @return : void
*! @author : thuong.nv          - [Date] : 22/04/2023
*******************************************************************************/
void Camera2D::ZoomTo(float x, float y, float delta_z)
{
	// Tính tọa độ thực tế của nó trước và sau khi set zoom
	glm::vec2 pGlobalOld = PointLocal2Global(glm::vec2(x, y));
	UpdateZoom(delta_z);
	glm::vec2 pGlobalNew = PointLocal2Global(glm::vec2(x, y));
	glm::vec2 vMove = pGlobalNew - pGlobalOld;

	// Di chuyển vị trí của camera một đoạn vì vị trí x, y được giữ nguyên
	m_position.x -= vMove.x;
	m_position.y -= vMove.y;
	m_position.z = m_position.z;
}

/*******************************************************************************
*! @brief  : Dịch chuyển tọa độ camera đi một khoảng delta x và delta y
*! @param    [in] delx : tọa độ x trên local view
*! @param    [in] dely : tọa độ y trên local view
*! @return : void
*! @author : thuong.nv          - [Date] : 22/04/2023
*******************************************************************************/
void Camera2D::Move(float delx, float dely)
{
	float deltax_glo = -delx / m_fZoom;
	float deltay_glo = dely / m_fZoom;

	m_position += glm::vec3(deltax_glo, deltay_glo, 0);
}

/// <summary>
/// Camera3D ////////////////////////////////////////////////////////////
/// </summary>

Camera3D::Camera3D() : Camera(), m_fFov(1.f), m_fTheta(0.f), m_fPhi(0.f),
m_fDis(0.f), m_iMode(0)
{
	m_modelMat = glm::mat4(1.f);
	m_vTarget = { 0.f, 0.f, 0.f };
}

/*******************************************************************************
*! @brief  : Cập nhật model matrix
*! @param    [in] mode : Chế độ zoom hoặc không zoom
*! @return : void
*! @author : thuong.nv          - [Date] : 22/04/2023
*******************************************************************************/
void Camera3D::SetModelMatrix(glm::mat4& mat)
{
	m_modelMat = mat;
}

/*******************************************************************************
*! @brief  : Cập nhật view matrix
*! @param    [in] mode : Chế độ zoom hoặc không zoom
*! @return : void
*! @author : thuong.nv          - [Date] : 22/04/2023
*******************************************************************************/
void Camera3D::UpdateViewMatrix()
{
	m_viewMat = glm::lookAt(m_position, m_position - m_direction, m_up);
}

/*******************************************************************************
*! @brief  : Cập nhật projection matrix
*! @param    [in] mode : Chế độ zoom hoặc không zoom
*! @return : void
*! @author : thuong.nv          - [Date] : 22/04/2023
*******************************************************************************/
void Camera3D::UpdateProjMatrix()
{
	float aspect = float(m_iWidth) / float(m_iHeight);
	m_projMat = glm::perspective(glm::radians(m_fFov), aspect, m_fNear, m_fFar);
}

/*******************************************************************************
*! @brief  : Load ma trận cho  GPU
*! @param    [in] mode : Chế độ zoom hoặc không zoom
*! @return : void
*! @author : thuong.nv          - [Date] : 22/04/2023
*******************************************************************************/
void Camera3D::UpdateMatrix()
{
	this->UpdateViewMatrix();
	this->UpdateProjMatrix();
}

/*******************************************************************************
*! @brief  : Load ma trận cho  GPU
*! @param    [in] mode : Chế độ zoom hoặc không zoom
*! @return : void
*! @author : thuong.nv          - [Date] : 22/04/2023
*******************************************************************************/
void Camera3D::UseMatrix(CameraMode mode) const
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(m_projMat));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(m_viewMat * m_modelMat));
}

/*******************************************************************************
*! @brief  : Load ma trận cho  GPU
*! @param    [in] mode : Chế độ zoom hoặc không zoom
*! @return : void
*! @author : thuong.nv          - [Date] : 22/04/2023
*******************************************************************************/
void Camera3D::LoadMatrix(GLuint program) const
{
	GLint mvLoc = glGetUniformLocation(program, "mv_matrix");
	GLint projLoc = glGetUniformLocation(program, "proj_matrix");

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(m_viewMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(m_projMat));
}

void Camera3D::UpdateOrbitTarget(float phi, float theta)
{
	//↓ 2021.09.22 N.V.Thuong [Xử lý ngoại lệ và khoảng xoay]
	//if (fabs(phi) > M3_HPI)
	//{
	//	phi = (phi < 0.0) ? (-M3_HPI + M3_ALG_EPSILON) : (M3_HPI - M3_ALG_EPSILON);
	//}
	//if (theta > M3_2PI)
	//{
	//	int factor = int(theta / M3_2PI);
	//	theta = theta - factor * M3_2PI;
	//}
	//else if (theta < 0)
	//{
	//	int factor = int(theta / M3_2PI);
	//	theta = theta + abs(factor + 1) * M3_2PI;
	//}
	////↑ 2021.09.22 N.V.Thuong [Xử lý ngoại lệ và khoảng xoay]

	////↓ 2021.09.22 N.V.Thuong [Cập nhật thông số camera]
	//m_fPhi = phi;
	//m_fTheta = theta;
	//m_position = M3D_CalPosition(m_vTarget, m_fDis, m_fPhi, m_fTheta);

	//if (m_fDis >= M3_EPSILON)
	//{
	//	m_direction = glm::normalize(m_position - m_vTarget);
	//}
	//else
	//{
	//	m_direction = glm::vec3(0, 0, 0);
	//}
	//↑ 2021.09.22 N.V.Thuong [Cập nhật thông số camera]
}

CameraType Camera3D::GetType()
{
	return CameraType::C3D;
}

void Camera3D::SetFieldOfView(float fov)
{
	m_fFov = fov;
}

void Camera3D::SetUpOrbitTarget(glm::vec3 position, glm::vec3 target)
{
	m_vTarget = target;
	m_fDis = glm::distance(position, target);
	//M3D_GetAnglePositionOxyz(position, target, m_fPhi, m_fTheta);
	this->UpdateOrbitTarget(m_fPhi, m_fTheta);
}

void Camera3D::OrbitTarget(float delPhi, float delTheta)
{
	float phi = m_fPhi + delPhi;
	float theta = m_fTheta + delTheta;
	this->UpdateOrbitTarget(phi, theta);
}

/*******************************************************************************
*! @brief  : Thu vị trí camera vào vị trí target đã được đặt trước
*! @param    [in] mode : Chế độ zoom hoặc không zoom
*! @return : void
*! @author : thuong.nv          - [Date] : 22/04/2023
*******************************************************************************/
void Camera3D::ZoomTo(float delta)
{
	glm::vec3 vUint = glm::normalize(m_direction);
	glm::vec3 vPositionNew = m_position - vUint * delta;
	glm::vec3 vUintNew = glm::normalize(vPositionNew - m_vTarget);

	float fDot = glm::dot(vUint, vUintNew);
	if (fDot <= 0.0)
	{
		m_position = m_vTarget + vUint;
	}
	else
	{
		m_position = vPositionNew;
	}
	m_fDis = glm::distance(m_position, m_vTarget);
}

/*******************************************************************************
*! @brief  : Di chuyển camera theo hướng tạo bưởi direction và up
*! @param    [in] mode : Chế độ zoom hoặc không zoom
*! @return : void
*! @author : thuong.nv          - [Date] : 22/04/2023
*******************************************************************************/
void Camera3D::OrbitMove(float delta)
{
	glm::vec3 vuint_move = glm::cross(m_up, -m_direction);
	m_position += vuint_move * delta;
	m_vTarget += vuint_move * delta;
	this->UpdateViewMatrix();
}

/*******************************************************************************
*! @brief  :  Di chuyển camera theo mặt phẳng của camera
*! @param    [in] mode : Chế độ zoom hoặc không zoom
*! @return : void
*! @author : thuong.nv          - [Date] : 22/04/2023
*******************************************************************************/
void Camera3D::OrbitMove1(float x, float y)
{
	float xtemp = m_position.x + x;
	float ytemp = m_position.y + y;

	//↓ 2021.09.22 N.V.Thuong [Tính tọa độ z của camera mới]
	glm::vec3 vn = glm::normalize(m_direction);
	float d = vn.x * m_position.x + vn.y * m_position.y + vn.z * m_position.z;
	float ztemp = (d - (vn.x * xtemp + vn.y * ytemp)) / vn.z;

	glm::vec3 temp(xtemp, ytemp, ztemp);
	if (glm::distance(temp, m_position) > 0.001f)
	{
		glm::vec3 v = glm::normalize(temp - m_position);
		m_position += v * (0.12f);
		m_vTarget += v * (0.12f);
		this->UpdateViewMatrix();
	}
}

_____END_SECTION_____


