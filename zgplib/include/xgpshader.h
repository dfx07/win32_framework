////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2023 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     gp_shader.h
* @create   May 19, 2023
* @brief    Use OpenGL shader
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef XGP_SHADER_H
#define XGP_SHADER_H

#include "xgeotype.h"
#include <GL/glew.h>

#include <glm/gtc/type_ptr.hpp>
#include <xsysutils.h>

____BEGIN_SECTION____(gp)


class Dllexport Shader
{
	template <typename T>
	using Enable_If_Vector = std::enable_if_t<std::is_same_v<T, Vec2D> || std::is_same_v<T, Vec3D>>;

	template <typename T>
	using Enable_If_Mat = std::enable_if_t<std::is_same_v<T, glm::mat2> || std::is_same_v<T, glm::mat3> || std::is_same_v<T, glm::mat4>>;

protected:
	enum class ShaderType
	{
		VERTEX,
		FRAGMENT,
		GEOMETRY,
	};

private:
	static constexpr const wchar_t* strLogFile = L"shader_build.log";
	GLuint          m_programID;

public:
	Shader() : m_programID(0)
	{

	}

	~Shader()
	{

	}

protected:
	/***********************************************************************************
	*! @brief  : Rotate point (degree)
	*! @param  : [in] path : path of the file
	*! @param  : [in] data : data read
	*! @return : unsigned int : number of byte read
	*! @author : thuong.nv              - [Date] : 06/03/2023
	***********************************************************************************/
	static unsigned int read_file(const wchar_t* path, void** data);

	/***********************************************************************************
	*! @brief  : Write log to file
	*! @param  : [in] path : path of the file
	*! @param  : [in] data : write data
	*! @param  : [in] nsize : lenght data (byte)
	*! @param  : [in] bClear : clear content old file
	*! @return : void
	*! @author : thuong.nv              - [Date] : 05/21/2023
	***********************************************************************************/
	static void write_log_file(IN const wchar_t* path, IN const void* data, IN const int& nsize, IN bool bClear = false);

public:
	/***********************************************************************************
	*! @brief  : Create program shader when openGL exist
	*! @param  : [in] pathVertex   : path source vertex shader file
	*! @param  : [in] pathFragment : path source frament shader file
	*! @param  : [in] pathGeometry : path source geometry shader file
	*! @return : void
	*! @author : thuong.nv         - [Date] : 06/03/2023
	***********************************************************************************/
	bool Create(const wchar_t* pathVertex, const wchar_t* pathFragment = NULL, const wchar_t* pathGeometry = NULL);

private:
	/***********************************************************************************
	*! @brief  : write log = print and save file
	*! @param  : [in] message   : message
	*! @param  : [in] length    : lenght message
	*! @param  : [in] bOutFile  : push to file
	*! @return : void
	*! @author : thuong.nv          - [Date] : 06/03/2023
	***********************************************************************************/
	void PrintMessageLog(const char* message, unsigned int length, bool bOutFile = false);

	/***********************************************************************************
	*! @brief  : Create shader when openGL exist
	*! @param  : [in] type   : path source vertex shader file
	*! @param  : [in] source : path source frament shader file
	*! @param  : [in] length : path source geometry shader file
	*! @return : void
	*! @author : thuong.nv          - [Date] : 06/03/2023
	***********************************************************************************/
	bool CheckStatusShader(unsigned int iShaderID, bool bPrintLog = false);

	/***********************************************************************************
	*! @brief  : Create shader when openGL exist
	*! @param  : [in] type   : path source vertex shader file
	*! @param  : [in] source : path source frament shader file
	*! @param  : [in] length : path source geometry shader file
	*! @return : void
	*! @author : thuong.nv          - [Date] : 06/03/2023
	***********************************************************************************/
	bool CheckStatusProgram(unsigned int iProgramID, bool bPrintLog = false);

	/***********************************************************************************
	*! @brief  : Create shader when openGL exist
	*! @param  : [in] type   : path source vertex shader file
	*! @param  : [in] source : path source frament shader file
	*! @param  : [in] length : path source geometry shader file
	*! @return : void
	*! @author : thuong.nv          - [Date] : 06/03/2023
	***********************************************************************************/
	unsigned int CreateShader(ShaderType type, const char* source, unsigned int& length);

	/***********************************************************************************
	*! @brief  : Create shader when openGL exist
	*! @param  : [in] type   : path source vertex shader file
	*! @param  : [in] source : path source frament shader file
	*! @param  : [in] length : path source geometry shader file
	*! @return : void
	*! @author : thuong.nv          - [Date] : 06/03/2023
	***********************************************************************************/
	unsigned int LinkProgam(unsigned int vertexID, unsigned int fragID, unsigned int geoID);

public:
	/***********************************************************************************
	*! @brief  : Check shader created and may be use
	*! @return : void
	*! @author : thuong.nv          - [Date] : 06/03/2023
	***********************************************************************************/
	bool IsOK();

	/***********************************************************************************
	*! @brief  : Use shader
	*! @return : void
	*! @author : thuong.nv          - [Date] : 06/03/2023
	***********************************************************************************/
	void Use();

	/***********************************************************************************
	*! @brief  : UnUse shader
	*! @return : void
	*! @author : thuong.nv          - [Date] : 06/03/2023
	***********************************************************************************/
	void UnUse();

	/***********************************************************************************
	*! @brief  : Set uniform value
	*! @param  : [in] name   : variable name
	*! @param  : [in] value  : variable value
	*! @return : void
	*! @author : thuong.nv          - [Date] : 06/03/2023
	***********************************************************************************/
	template<typename T>
	void SetUniformValue(const char* name, T value);

	/***********************************************************************************
	*! @brief  : Set uniform vector value
	*! @param  : [in] name : variable name
	*! @param  : [in] vec  : vector
	*! @return : void
	*! @author : thuong.nv          - [Date] : 06/03/2023
	***********************************************************************************/
	template<typename T, typename = Enable_If_Vector<T>>
	void SetUniformVec(const char* name, T vec);

	/***********************************************************************************
	*! @brief  : Set uniform variable Maxtrix value
	*! @param  : [in] name  : variable name
	*! @param  : [in] mat   : matrix
	*! @return : void
	*! @author : thuong.nv          - [Date] : 06/03/2023
	***********************************************************************************/
	template<typename T, typename = Enable_If_Mat<T>>
	void SetUniformMat(const char* name, const T& mat);
};


_____END_SECTION_____

#endif // !XGP_SHADER_H

