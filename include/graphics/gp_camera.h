////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2023 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     gp_camera.h
* @create   April 15, 2023
* @brief    Camera define
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef GP_CAMERA_H
#define GP_CAMERA_H

#include "math/xgeotype.h"
#include <GL/glew.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


___BEGIN_NAMESPACE___


enum class CameraType
{
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
class Camera
{
protected:
    Vec3D           m_position;     // Vị trí camera
    Vec3D           m_direction;    // Hướng camera trỏ tới (ngược hướng với hướng camera thực tế) - Nó nên là vector đơn vị (normal)
    Vec3D           m_up;           // Hướng lên camera

    int             m_iWidth;       // Chiều rộng của view
    int             m_iHeight;      // Chiều cao của view
    float           m_fNear;        // Mặt phẳng gần
    float           m_fFar;         // Mặt phẳng xa

    Mat4            m_viewMat;      // view matrix
    Mat4            m_projMat;      // projection matrix 
    Mat4            m_modelMat;     // model matrix (sử dụng OpenGL 1.1)

public:
    Camera() : m_iWidth(0), m_iHeight(0), m_fFar(0.f), m_fNear(0.f)
    {
        m_position  = { 0.f, 0.f, 0.f };
        m_direction = { 0.f, 0.f, 1.f };
        m_up        = { 0.f, 1.f, 0.f };

        m_viewMat  = Mat4(1.f);
        m_projMat  = Mat4(1.f);
        m_modelMat = Mat4(1.f);
    }

    virtual ~Camera()
    {

    }

public:
    virtual CameraType GetType() = 0;

    virtual void UpdateViewMatrix() = 0;
    virtual void UpdateProjMatrix() = 0;

    virtual void UpdateMatrix() = 0;
    virtual void UseMatrix(CameraMode mode = CameraMode::CMODE_NORMAL) const = 0;
    virtual void LoadMatrix(GLuint program) const = 0;

    virtual Mat4& GetViewMatrix() { return m_viewMat; }
    virtual Mat4& GetProjMatrix() { return m_projMat; }
    virtual Mat4& GetModelMatrix(){ return m_modelMat;}

public:
    /*******************************************************************************
    *! @brief  : Chuyển đổi từ tọa độ [Trái trên] sang tọa độ tại [Trung tâm] trên view
    *! @param    [in] p : tọa độ trên view
    *! @return : Vec2D tọa độ thực tế
    *! @author : thuong.nv          - [Date] : 22/04/2023
    *! @note   : Tọa độ [x, y] đầu vào là tọa độ trên view  (Left Top)
    *******************************************************************************/
    Vec2D PointLT2Center(const Vec2D& p) const
    {
        Vec2D point = { 0, 0 };  // Để mặc định
        point.x     =  (p.x - m_iWidth /2);
        point.y     = -(p.y - m_iHeight/2);
        return point;
    }

    /*******************************************************************************
    *! @brief  : Chuyển đổi từ tọa độ [Trái trên] sang tọa độ tại [Trung tâm] trên view   [3D]
    *! @param    [in] p : tọa độ trên view
    *! @return : Vec2D tọa độ thực tế
    *! @author : thuong.nv          - [Date] : 22/04/2023
    *! @note   : Tọa độ [x, y] đầu vào là tọa độ trên view  (Left Top)
    *******************************************************************************/
    Vec3D PointLT2Center(const Vec3D& p) const
    {
        Vec3D point = { 0, 0 ,  p.z};  // Để mặc định
        point.x     =  (p.x - m_iWidth /2);
        point.y     = -(p.y - m_iHeight/2);
        return point;
    }

public:

    void InitView(int width, int height, float _near, float _far)
    {
        m_fNear   = _near;
        m_fFar    = _far;
        m_iWidth  = width;
        m_iHeight = height;
    }

    void SetUpCamera(Vec3D pos, Vec3D dir, Vec3D up = Vec3D(0, 1, 0))
    {
        m_position  = pos;
        m_direction = dir;
        m_up        = up;
    }

    void SetViewSize(int width, int height)
    {
        m_iWidth  = width;
        m_iHeight = height;
    }

    void SetDistPlane(float _near, float _far)
    {
        m_fNear = _near;
        m_fFar  = _far;
    }

    Vec3D GetPosition()
    {
        return m_position;
    }

    void SetPosition(Vec3D pos)
    {
        m_position  = pos;
    }

    // Vị trí camera dự vào vị trí target
    void SetPosition(float phi, float theta, float distance, Vec3D target)
    {
        // Tính điểm tại vị trí tâm
        float x = cos(phi) * sin(theta);
        float y = sin(phi);
        float z = cos(phi) * cos(theta);
        Vec3D position = Vec3D(x, y, z) * distance;

        // Dịch chuyển điểm đó đến target
        float dis  = glm::distance(target, Vec3D(0, 0, 0));
        if (fabs(dis) > 0.1)
        {
            Vec3D vut   = glm::normalize(target - Vec3D(0, 0, 0));

            m_position  = position + (vut * dis);
            m_direction = glm::normalize(m_position - target);
        }
        else
        {
            m_position = position;
        }
    }

    void Move(Vec3D mov)
    {
        m_position += mov;
        this->UpdateViewMatrix();
    }
};

/**********************************************************************************
* ⮟⮟ Class name: Camera2D
* Base class for window handle inheritance
***********************************************************************************/
class Camera2D : public Camera
{
protected:
    static constexpr float ZOOM_MIN    = 0.01f;
    static constexpr float ZOOM_MAX    = 100.f;
    static constexpr float ZOOM_FACTOR =  0.9f;

private:
    float           m_fZoom;         // Tỷ lệ zoom
    Mat4            m_projMatNozoom; //projection matrix nozoom

public:
    Camera2D() : m_fZoom(1.f)
    {
        m_projMatNozoom = Mat4(1.f);
    }

public:
    /*******************************************************************************
    *! @brief  : Chuyển đổi tọa độ điểm từ view sang tọa độ thực tế
    *! @param    [in] p : tọa độ trên view
    *! @return : Vec2D tọa độ thực tế
    *! @author : thuong.nv          - [Date] : 22/04/2023
    *! @note   : Tọa độ [x, y] đầu vào là tọa độ trên view  (Left Top)
    *******************************************************************************/
    Vec2D PointLocal2Global(const Vec2D& p)
    {
        Vec2D point = {0 , 0};    // Để mặc định
        point.x = m_position.x + (p.x - float(m_iWidth /2))/m_fZoom;
        point.y = m_position.y - (p.y - float(m_iHeight/2))/m_fZoom;
        return point;
    }

    /*******************************************************************************
    *! @brief  : Chuyển đổi tọa độ điểm từ thực thế sang tọa độ view
    *! @param    [in] p : real position to local view
    *! @return : Vec2D view position
    *! @author : thuong.nv          - [Date] : 22/04/2023
    *! @note   : Tọa độ [x, y] đầu vào là tọa độ trên view  (Left Top)
    *******************************************************************************/
    Vec2D PointGlobal2Local(const Vec2D& p)
    {
        Vec2D point = {0 , 0};   // Để mặc định
        point.x = (float(m_iWidth /2) - m_fZoom*(m_position.x -p.x));
        point.y = (m_fZoom*(m_position.y -p.y) + float(m_iHeight/2));
        return point;
    }

public:
    Mat4 GetMatrixNozoom()
    {
        return  m_projMatNozoom;
    }

    /*******************************************************************************
    *! @brief  : Cập nhật view matrix 
    *! @param    [in] mode : Chế độ zoom hoặc không zoom
    *! @return : void
    *! @author : thuong.nv          - [Date] : 22/04/2023
    *******************************************************************************/
    virtual void UpdateViewMatrix()
    {
        m_viewMat = glm::lookAt(m_position, m_position - m_direction, m_up);
    }

    /*******************************************************************************
    *! @brief  : Cập nhật projection matrix 
    *! @param    [in] mode : Chế độ zoom hoặc không zoom
    *! @return : void
    *! @author : thuong.nv          - [Date] : 22/04/2023
    *******************************************************************************/
    virtual void UpdateProjMatrix()
    {
        // Projection matrix không sử dụng zoom
        float left   = -float(m_iWidth  / 2);
        float right  =  float(m_iWidth  / 2);
        float bottom = -float(m_iHeight / 2);
        float top    =  float(m_iHeight / 2);

        m_projMatNozoom  = glm::ortho(left, right, bottom, top, float(m_fNear), float(m_fFar));

        // Projection matrix sử dụng thuộc tính zoom
        left         =  left  / m_fZoom;
        right        =  right / m_fZoom;
        bottom       =  bottom/ m_fZoom;
        top          =  top   / m_fZoom;

        m_projMat    = glm::ortho(left, right, bottom, top, float(m_fNear), float(m_fFar));
    }

    /*******************************************************************************
    *! @brief  : Load ma trận cho  GPU
    *! @param    [in] mode : Chế độ zoom hoặc không zoom
    *! @return : void
    *! @author : thuong.nv          - [Date] : 22/04/2023
    *******************************************************************************/
    virtual void UpdateMatrix()
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
    void UseMatrix(CameraMode mode = CameraMode::CMODE_NORMAL) const
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

    void LoadMatrix(GLuint program) const
    {
        GLint mvLoc   = glGetUniformLocation(program, "mv_matrix");
        GLint projLoc = glGetUniformLocation(program, "proj_matrix");

        glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(m_viewMat));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(m_projMat));
    }

protected:
    /*******************************************************************************
    *! @brief  : Cập nhật thông số zoom có giới hạn giá trị
    *! @return : void
    *! @author : thuong.nv          - [Date] : 22/04/2023
    *******************************************************************************/
    void UpdateZoom(float zDelta)
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

public:
    virtual CameraType GetType() { return CameraType::C2D; }

    float GetZoom() const
    {
        return m_fZoom;
    }

    /*******************************************************************************
    *! @brief  : Target camera vào một vị trí (tọa độ local) với lượng zoom delta   
    *! @return : void
    *! @author : thuong.nv          - [Date] : 22/04/2023
    *******************************************************************************/
    void ZoomTo(float x, float y, float delta_z)
    {
        // Tính tọa độ thực tế của nó trước và sau khi set zoom
        Vec2D pGlobalOld = PointLocal2Global(Vec2D(x, y));
        UpdateZoom(delta_z);
        Vec2D pGlobalNew = PointLocal2Global(Vec2D(x, y));
        Vec2D vMove = pGlobalNew - pGlobalOld;

        // Di chuyển vị trí của camera một đoạn vì vị trí x, y được giữ nguyên
        m_position.x -= vMove.x;
        m_position.y -= vMove.y;
        m_position.z  = m_position.z;
    }

    /*******************************************************************************
    *! @brief  : Dịch chuyển tọa độ camera đi một khoảng delta x và delta y
    *! @param    [in] delx : tọa độ x trên local view
    *! @param    [in] dely : tọa độ y trên local view
    *! @return : void
    *! @author : thuong.nv          - [Date] : 22/04/2023
    *******************************************************************************/
    void Move(float delx, float dely)
    {
        float deltax_glo = -delx / m_fZoom;
        float deltay_glo =  dely / m_fZoom;

        m_position += Vec3D(deltax_glo, deltay_glo, 0);
    }
};

/**********************************************************************************
* ⮟⮟ Class name: Camera3D
* Base class for window handle inheritance
***********************************************************************************/
class Camera3D : public Camera
{
private:
    float           m_fFov;    // Góc nhìn

    // 2021.09.22 N.V.Thuong [Tính toán cho xoay quanh mục tiêu]
    float           m_fTheta;  // Góc với trục Oz
    float           m_fPhi;    // Góc phụ với trục Oy
    Vec3D           m_vTarget; // Vị trí camera Target
    float           m_fDis;    // Vị trí camera Target

    int             m_iMode;   // Chế độ Camera đang hoạt động

public:
    Camera3D() : m_fFov(1.f), m_fTheta(0.f), m_fPhi(0.f),
        m_fDis(0.f), m_iMode(0)
    {
        m_modelMat = Mat4(1.f);
        m_vTarget = { 0.f, 0.f, 0.f };
    }

    /*******************************************************************************
    *! @brief  : Cập nhật model matrix
    *! @param    [in] mode : Chế độ zoom hoặc không zoom
    *! @return : void
    *! @author : thuong.nv          - [Date] : 22/04/2023
    *******************************************************************************/
    virtual void SetModelMatrix(Mat4& mat)
    {
        m_modelMat = mat;
    }

    /*******************************************************************************
    *! @brief  : Cập nhật view matrix
    *! @param    [in] mode : Chế độ zoom hoặc không zoom
    *! @return : void
    *! @author : thuong.nv          - [Date] : 22/04/2023
    *******************************************************************************/
    virtual void UpdateViewMatrix()
    {
        m_viewMat = glm::lookAt(m_position, m_position - m_direction, m_up);
    }

    /*******************************************************************************
    *! @brief  : Cập nhật projection matrix
    *! @param    [in] mode : Chế độ zoom hoặc không zoom
    *! @return : void
    *! @author : thuong.nv          - [Date] : 22/04/2023
    *******************************************************************************/
    virtual void UpdateProjMatrix()
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
    virtual void UpdateMatrix()
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
    void UseMatrix(CameraMode mode = CameraMode::CMODE_NORMAL) const
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
    void LoadMatrix(GLuint program) const
    {
        GLint mvLoc = glGetUniformLocation(program, "mv_matrix");
        GLint projLoc = glGetUniformLocation(program, "proj_matrix");

        glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(m_viewMat));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(m_projMat));
    }

private:
    void UpdateOrbitTarget(float phi, float theta)
    {
        //↓ 2021.09.22 N.V.Thuong [Xử lý ngoại lệ và khoảng xoay]
        //if (fabs(phi) > M3_HPI)
        //{
        //    phi = (phi < 0.0)?(-M3_HPI+M3_ALG_EPSILON):(M3_HPI-M3_ALG_EPSILON);
        //}
        //if (theta > M3_2PI)
        //{
        //    int factor = int(theta / M3_2PI);
        //    theta = theta - factor* M3_2PI;
        //}
        //else if (theta < 0)
        //{
        //    int factor = int(theta / M3_2PI);
        //    theta = theta + abs(factor+1)* M3_2PI;
        //}
        ////↑ 2021.09.22 N.V.Thuong [Xử lý ngoại lệ và khoảng xoay]

        ////↓ 2021.09.22 N.V.Thuong [Cập nhật thông số camera]
        //m_fPhi     = phi;
        //m_fTheta   = theta;
        //m_position = M3D_CalPosition(m_vTarget, m_fDis, m_fPhi, m_fTheta);

        //if (m_fDis >= M3_EPSILON)
        //{
        //    m_direction = glm::normalize(m_position - m_vTarget);
        //}
        //else
        //{
        //    m_direction = Vec3D(0, 0, 0);
        //}
        //↑ 2021.09.22 N.V.Thuong [Cập nhật thông số camera]
    }
public:
    virtual CameraType GetType() { return CameraType::C3D; }

    void SetFieldOfView(float fov)
    {
        m_fFov  = fov;
    }

    // Thiết lập thông số cho chế độ xoay target
    void SetUpOrbitTarget(Vec3D position, Vec3D target)
    {
        m_vTarget = target;
        m_fDis    = glm::distance(position, target);
        //M3D_GetAnglePositionOxyz(position, target, m_fPhi, m_fTheta);
        this->UpdateOrbitTarget(m_fPhi, m_fTheta);
    }

    void OrbitTarget(float delPhi, float delTheta)
    {
        float phi   = m_fPhi   + delPhi;
        float theta = m_fTheta + delTheta;
        this->UpdateOrbitTarget(phi, theta);
    }

    //==============================================================================
    // Thu vị trí camera vào vị trí target đã được đặt trước                        
    //==============================================================================
    void ZoomTo(float delta)
    {
        Vec3D vUint        = glm::normalize(m_direction);
        Vec3D vPositionNew = m_position - vUint*delta;
        Vec3D vUintNew     = glm::normalize(vPositionNew - m_vTarget);

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

    //==============================================================================
    // Di chuyển camera theo hướng tạo bưởi direction và up                         
    //==============================================================================
    void OrbitMove(float delta)
    {
        Vec3D vuint_move = glm::cross(m_up, -m_direction);
        m_position += vuint_move * delta;
        m_vTarget  += vuint_move * delta;
        this->UpdateViewMatrix();
    }

    //==============================================================================
    // Di chuyển camera theo mặt phẳng của camera                                   
    //==============================================================================
    void OrbitMove1(float x , float y)
    {
        float xtemp = m_position.x + x;
        float ytemp = m_position.y + y;

        //↓ 2021.09.22 N.V.Thuong [Tính tọa độ z của camera mới]
        Vec3D vn = glm::normalize(m_direction);
        float d = vn.x* m_position.x + vn.y*m_position.y + vn.z *m_position.z;
        float ztemp = (d - (vn.x*xtemp + vn.y*ytemp))/vn.z;

        Vec3D temp(xtemp, ytemp ,ztemp);
        if (glm::distance(temp, m_position) > 0.001f)
        {
            Vec3D v = glm::normalize(temp - m_position);
            m_position += v*(0.12f) ;
            m_vTarget  += v*(0.12f) ;
            this->UpdateViewMatrix();
        }
    }
};

____END_NAMESPACE____

#endif // !GPCAMERA_H

