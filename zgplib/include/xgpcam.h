////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2023 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     xgpcam.h
* @create   Nov 15, 2023
* @brief    Camera define
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef XGPCAMERA_H
#define XGPCAMERA_H

#include "xsysdef.h"

#include <GL/glew.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


____BEGIN_SECTION____(gp)

enum class CameraType
{
	NONE,
	C2D,
	C3D,
};

enum class CameraMode
{
	CMODE_NORMAL,
	CMODE_NOZOOM,
};

/**********************************************************************************
* ⮟⮟ Class name: Camera
* Base class for window handle inheritance
***********************************************************************************/
template<CameraType T>
class Camera
{
protected:
	glm::vec3       m_position;     // Vị trí camera
	glm::vec3       m_direction;    // Hướng camera trỏ tới (ngược hướng với hướng camera thực tế) - Nó nên là vector đơn vị (normal)
	glm::vec3       m_up;           // Hướng lên camera

	int             m_iWidth;       // Chiều rộng của view
	int             m_iHeight;      // Chiều cao của view
	float           m_fNear;        // Mặt phẳng gần
	float           m_fFar;         // Mặt phẳng xa

	glm::mat4       m_viewMat;      // view matrix
	glm::mat4       m_projMat;      // projection matrix 
	glm::mat4       m_modelMat;     // model matrix (sử dụng OpenGL 1.1)

	CameraType		m_type = T;

public:
	Camera();
	virtual ~Camera();

public:
	virtual CameraType GetType();

	virtual void UpdateViewMatrix() = 0;
	virtual void UpdateProjMatrix() = 0;

	virtual void UpdateMatrix() = 0;
	virtual void UseMatrix(CameraMode mode = CameraMode::CMODE_NORMAL) const = 0;
	virtual void LoadMatrix(GLuint program) const = 0;

	virtual glm::mat4& GetViewMatrix();
	virtual glm::mat4& GetProjMatrix();
	virtual glm::mat4& GetModelMatrix();

public:
	/*******************************************************************************
	*! @brief  : Chuyển đổi từ tọa độ [Trái trên] sang tọa độ tại [Trung tâm] trên view
	*! @param    [in] p : tọa độ trên view
	*! @return : glm::vec2 tọa độ thực tế
	*! @author : thuong.nv          - [Date] : 22/04/2023
	*! @note   : Tọa độ [x, y] đầu vào là tọa độ trên view  (Left Top)
	*******************************************************************************/
	glm::vec2 PointLT2Center(const glm::vec2& p) const;

	/*******************************************************************************
	*! @brief  : Chuyển đổi từ tọa độ [Trái trên] sang tọa độ tại [Trung tâm] trên view   [3D]
	*! @param    [in] p : tọa độ trên view
	*! @return : glm::vec2 tọa độ thực tế
	*! @author : thuong.nv          - [Date] : 22/04/2023
	*! @note   : Tọa độ [x, y] đầu vào là tọa độ trên view  (Left Top)
	*******************************************************************************/
	glm::vec3 PointLT2Center(const glm::vec3& p) const;

public:
	void InitView(int width, int height, float _near, float _far);

	/*******************************************************************************
	*! @brief  : Chuyển đổi từ tọa độ [Trái trên] sang tọa độ tại [Trung tâm] trên view   [3D]
	*! @param    [in] p : tọa độ trên view
	*! @return : glm::vec2 tọa độ thực tế
	*! @author : thuong.nv          - [Date] : 22/04/2023
	*! @note   : Tọa độ [x, y] đầu vào là tọa độ trên view  (Left Top)
	*******************************************************************************/
	void SetUpCamera(glm::vec3 pos, glm::vec3 dir, glm::vec3 up = glm::vec3(0, 1, 0));

	/*******************************************************************************
	*! @brief  : Set view size
	*! @param    [in] width  : width view
	*! @param    [in] height : height view
	*! @author : thuong.nv          - [Date] : 22/04/2023
	*******************************************************************************/
	void SetViewSize(int width, int height);

	/*******************************************************************************
	*! @brief  : Set near and far distance camera
	*! @param    [in] _near : near distace clip plane
	*! @param    [in] _far  : far distance clip plane
	*! @author : thuong.nv          - [Date] : 22/04/2023
	*******************************************************************************/
	void SetDistPlane(float _near, float _far);

	glm::vec3 GetPosition();

	/*******************************************************************************
	*! @brief  : Set camera position use point3D
	*! @param  : [in] pos : point3D
	*! @author : thuong.nv          - [Date] : 22/04/2023
	*******************************************************************************/
	void SetPosition(glm::vec3 pos);

	/*******************************************************************************
	*! @brief  : Vị trí camera dự vào vị trí target góc quay và khoảng cách  [3D]
	*! @param  : [in] p : tọa độ trên view
	*! @return : glm::vec2 tọa độ thực tế
	*! @author : thuong.nv          - [Date] : 22/04/2023
	*! @note   : Tọa độ [x, y] đầu vào là tọa độ trên view  (Left Top)
	*******************************************************************************/
	void SetPosition(float phi, float theta, float distance, glm::vec3 target);

	void Move(glm::vec3 mov);
};

/**********************************************************************************
* ⮟⮟ Class name: Camera2D
* Base class for window handle inheritance
***********************************************************************************/
class Dllexport Camera2D : public Camera<CameraType::C2D>
{
protected:
	static constexpr float ZOOM_MIN = 0.01f;
	static constexpr float ZOOM_MAX = 100.f;
	static constexpr float ZOOM_FACTOR = 0.9f;

private:
	float		m_fZoom;         // Tỷ lệ zoom
	glm::mat4	m_projMatNozoom; //projection matrix nozoom

public:
	Camera2D();
	virtual ~Camera2D();

public:
	/*******************************************************************************
	*! @brief  : Chuyển đổi tọa độ điểm từ view sang tọa độ thực tế
	*! @param    [in] p : tọa độ trên view
	*! @return : glm::vec2 tọa độ thực tế
	*! @author : thuong.nv          - [Date] : 22/04/2023
	*! @note   : Tọa độ [x, y] đầu vào là tọa độ trên view  (Left Top)
	*******************************************************************************/
	glm::vec2 PointLocal2Global(const glm::vec2& p) const;

	/*******************************************************************************
	*! @brief  : Chuyển đổi tọa độ điểm từ thực thế sang tọa độ view
	*! @param    [in] p : real position to local view
	*! @return : glm::vec2 view position
	*! @author : thuong.nv          - [Date] : 22/04/2023
	*! @note   : Tọa độ [x, y] đầu vào là tọa độ trên view  (Left Top)
	*******************************************************************************/
	glm::vec2 PointGlobal2Local(const glm::vec2& p) const;

public:
	glm::mat4 GetMatrixNozoom();

	/*******************************************************************************
	*! @brief  : Cập nhật view matrix
	*! @param    [in] mode : Chế độ zoom hoặc không zoom
	*! @return : void
	*! @author : thuong.nv          - [Date] : 22/04/2023
	*******************************************************************************/
	virtual void UpdateViewMatrix();

	/*******************************************************************************
	*! @brief  : Cập nhật projection matrix
	*! @param    [in] mode : Chế độ zoom hoặc không zoom
	*! @return : void
	*! @author : thuong.nv          - [Date] : 22/04/2023
	*******************************************************************************/
	virtual void UpdateProjMatrix();

	/*******************************************************************************
	*! @brief  : Load ma trận cho  GPU
	*! @param    [in] mode : Chế độ zoom hoặc không zoom
	*! @return : void
	*! @author : thuong.nv          - [Date] : 22/04/2023
	*******************************************************************************/
	virtual void UpdateMatrix();

	/*******************************************************************************
	*! @brief  : Sử dụng matrix 
	*! @param  : [in] mode : Chế độ zoom hoặc không zoom
	*! @return : void
	*! @author : thuong.nv          - [Date] : 22/04/2023
	*******************************************************************************/
	void UseMatrix(CameraMode mode = CameraMode::CMODE_NORMAL) const;

	void LoadMatrix(GLuint program) const;

protected:
	/*******************************************************************************
	*! @brief  : Cập nhật thông số zoom có giới hạn giá trị
	*! @return : void
	*! @author : thuong.nv          - [Date] : 22/04/2023
	*******************************************************************************/
	void UpdateZoom(float zDelta);

public:
	virtual CameraType GetType() { return CameraType::C2D; }

	float GetZoom() const;

	/*******************************************************************************
	*! @brief  : Target camera vào một vị trí (tọa độ local) với lượng zoom delta
	*! @return : void
	*! @author : thuong.nv          - [Date] : 22/04/2023
	*******************************************************************************/
	void ZoomTo(float x, float y, float delta_z);

	/*******************************************************************************
	*! @brief  : Dịch chuyển tọa độ camera đi một khoảng delta x và delta y
	*! @param    [in] delx : tọa độ x trên local view
	*! @param    [in] dely : tọa độ y trên local view
	*! @return : void
	*! @author : thuong.nv          - [Date] : 22/04/2023
	*******************************************************************************/
	void Move(float delx, float dely);
};

/**********************************************************************************
* ⮟⮟ Class name: Camera3D
* Base class for window handle inheritance
***********************************************************************************/
class Dllexport Camera3D : public Camera<CameraType::C3D>
{
private:
	float           m_fFov;    // Góc nhìn

	// 2021.09.22 N.V.Thuong [Tính toán cho xoay quanh mục tiêu]
	float           m_fTheta;  // Góc với trục Oz
	float           m_fPhi;    // Góc phụ với trục Oy
	glm::vec3       m_vTarget; // Vị trí camera Target
	float           m_fDis;    // Vị trí camera Target

	int             m_iMode;   // Chế độ Camera đang hoạt động

public:
	Camera3D();

	/*******************************************************************************
	*! @brief  : Cập nhật model matrix
	*! @param    [in] mode : Chế độ zoom hoặc không zoom
	*! @return : void
	*! @author : thuong.nv          - [Date] : 22/04/2023
	*******************************************************************************/
	virtual void SetModelMatrix(glm::mat4& mat);

	/*******************************************************************************
	*! @brief  : Cập nhật view matrix
	*! @param    [in] mode : Chế độ zoom hoặc không zoom
	*! @return : void
	*! @author : thuong.nv          - [Date] : 22/04/2023
	*******************************************************************************/
	virtual void UpdateViewMatrix();

	/*******************************************************************************
	*! @brief  : Cập nhật projection matrix
	*! @param    [in] mode : Chế độ zoom hoặc không zoom
	*! @return : void
	*! @author : thuong.nv          - [Date] : 22/04/2023
	*******************************************************************************/
	virtual void UpdateProjMatrix();

	/*******************************************************************************
	*! @brief  : Load ma trận cho  GPU
	*! @param    [in] mode : Chế độ zoom hoặc không zoom
	*! @return : void
	*! @author : thuong.nv          - [Date] : 22/04/2023
	*******************************************************************************/
	virtual void UpdateMatrix();

	/*******************************************************************************
	*! @brief  : Load ma trận cho  GPU
	*! @param    [in] mode : Chế độ zoom hoặc không zoom
	*! @return : void
	*! @author : thuong.nv          - [Date] : 22/04/2023
	*******************************************************************************/
	void UseMatrix(CameraMode mode = CameraMode::CMODE_NORMAL) const;

	/*******************************************************************************
	*! @brief  : Load ma trận cho  GPU
	*! @param    [in] mode : Chế độ zoom hoặc không zoom
	*! @return : void
	*! @author : thuong.nv          - [Date] : 22/04/2023
	*******************************************************************************/
	void LoadMatrix(GLuint program) const;

private:
	void UpdateOrbitTarget(float phi, float theta);

public:
	virtual CameraType GetType();

	void SetFieldOfView(float fov);

	/*******************************************************************************
	*! @brief  : Thiết lập thông số cho chế độ xoay target
	*! @param    [in] mode : Chế độ zoom hoặc không zoom
	*! @return : void
	*! @author : thuong.nv          - [Date] : 22/04/2023
	*******************************************************************************/
	void SetUpOrbitTarget(glm::vec3 position, glm::vec3 target);

	void OrbitTarget(float delPhi, float delTheta);

	/*******************************************************************************
	*! @brief  : Thu vị trí camera vào vị trí target đã được đặt trước
	*! @param    [in] mode : Chế độ zoom hoặc không zoom
	*! @return : void
	*! @author : thuong.nv          - [Date] : 22/04/2023
	*******************************************************************************/
	void ZoomTo(float delta);

	/*******************************************************************************
	*! @brief  : Di chuyển camera theo hướng tạo bưởi direction và up
	*! @param    [in] mode : Chế độ zoom hoặc không zoom
	*! @return : void
	*! @author : thuong.nv          - [Date] : 22/04/2023
	*******************************************************************************/
	void OrbitMove(float delta);

	/*******************************************************************************
	*! @brief  :  Di chuyển camera theo mặt phẳng của camera
	*! @param    [in] mode : Chế độ zoom hoặc không zoom
	*! @return : void
	*! @author : thuong.nv          - [Date] : 22/04/2023
	*******************************************************************************/
	void OrbitMove1(float x, float y);
};

_____END_SECTION_____

#endif // !XGPCAMERA_H

