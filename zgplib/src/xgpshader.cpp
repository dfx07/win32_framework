#include "xgpshader.h"
#include "xsysutils.h"

____BEGIN_SECTION____(gp)

/***********************************************************************************
*! @brief  : Rotate point (degree)
*! @param  : [in] path : path of the file
*! @param  : [in] data : data read
*! @return : unsigned int : number of byte read
*! @author : thuong.nv              - [Date] : 06/03/2023
***********************************************************************************/
unsigned int Shader::read_file(const wchar_t* path, void** data)
{
	if (data) *data = NULL;
	if (path == NULL)  return 0;

	long int nbytes = 0;
	// open and read file share data
	FILE* file = _wfsopen(path, L"rb", _SH_DENYRD);
	if (!file) return nbytes;

	// read number of bytes file size 
	fseek(file, 0L, SEEK_END);
	nbytes = static_cast<long int>(ftell(file));
	fseek(file, 0L, SEEK_SET);

	if (nbytes < 0) // ftell failed
	{
		fclose(file); return 0;
	}

	// 2 null value (char and wide char)
	nbytes = nbytes + 2;

	// read data form file to memory
	auto tbuff = new unsigned char[nbytes];
	memset(tbuff, 0, nbytes);
	nbytes = (long int)fread_s(tbuff, nbytes, sizeof(char), nbytes, file);
	fclose(file);

	// Read content bytes file + nbyte read
	if (data) *data = tbuff;
	else delete[] tbuff;
	return nbytes;
}

/***********************************************************************************
*! @brief  : Write log to file
*! @param  : [in] path : path of the file
*! @param  : [in] data : write data
*! @param  : [in] nsize : lenght data (byte)
*! @param  : [in] bClear : clear content old file
*! @return : void
*! @author : thuong.nv              - [Date] : 05/21/2023
***********************************************************************************/
void Shader::write_log_file(IN const wchar_t* path, IN const void* data, IN const int& nsize, IN bool bClear)
{
	// check exist file | if not exist so create log file
	bool bExist = false;

	if (_waccess_s(path, 0) == 0)
	{
		bExist = true;
	}

	if (bClear || !bExist)
	{
		FILE* file = _wfsopen(path, L"wb", SH_DENYNO);
		if (file)
		{
			fwrite("/////////////////// SHADER LOG ///////////////////////", sizeof(char), NULL, file);
			fclose(file);
		}
	}

	FILE* file = _wfsopen(path, L"ab", SH_DENYNO);
	if (!file) return;

	fwrite(data, sizeof(char), nsize, file);

	fclose(file);
}

/***********************************************************************************
*! @brief  : Create program shader when openGL exist
*! @param  : [in] pathVertex   : path source vertex shader file
*! @param  : [in] pathFragment : path source frament shader file
*! @param  : [in] pathGeometry : path source geometry shader file
*! @return : void
*! @author : thuong.nv         - [Date] : 06/03/2023
***********************************************************************************/
bool Shader::Create(const wchar_t* pathVertex, const wchar_t* pathFragment, const wchar_t* pathGeometry)
{
	void* buff;
	char* vertexBuff, * fragBuff, * geoBuff;
	unsigned int vertexShaderID = 0, fragShaderID = 0, geoShaderID = 0;

	// read vertext shader file
	unsigned int nLength = read_file(pathVertex, &buff);
	vertexBuff = (nLength > 0) ? static_cast<char*>(buff) : NULL;

	vertexShaderID = CreateShader(ShaderType::VERTEX, vertexBuff, nLength);
	if (vertexShaderID <= 0)
	{
		return false;
	}

	// read frament shader file
	nLength = read_file(pathFragment, &buff);
	fragBuff = (nLength > 0) ? static_cast<char*>(buff) : NULL;

	fragShaderID = CreateShader(ShaderType::FRAGMENT, fragBuff, nLength);
	if (fragShaderID <= 0)
	{
		glDeleteShader(vertexShaderID);
		return false;
	}

	// read geometry shader file
	if (pathGeometry != NULL)
	{
		nLength = read_file(pathGeometry, &buff);
		geoBuff = (nLength > 0) ? static_cast<char*>(buff) : NULL;

		geoShaderID = CreateShader(ShaderType::GEOMETRY, geoBuff, nLength);
		if (geoShaderID <= 0)
		{
			glDeleteShader(vertexShaderID);
			glDeleteShader(fragShaderID);
			return false;
		}
	}

	m_programID = LinkProgam(vertexShaderID, fragShaderID, geoShaderID);
	if (m_programID <= 0)
	{
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragShaderID);
		glDeleteShader(geoShaderID);
		return false;
	}

	return true;
}

/***********************************************************************************
*! @brief  : write log = print and save file
*! @param  : [in] message   : message
*! @param  : [in] length    : lenght message
*! @param  : [in] bOutFile  : push to file
*! @return : void
*! @author : thuong.nv          - [Date] : 06/03/2023
***********************************************************************************/
void Shader::PrintMessageLog(const char* message, unsigned int length, bool bOutFile)
{
	//std::cout << message << std::endl;

	if (bOutFile)
	{
		int nMesLength = length;
		if (nMesLength <= 0)
		{
			nMesLength = static_cast<int>(strlen(message));
		}
		std::wstring strPath = fox::get_current_directory_execute();
		strPath.append(strLogFile);

		write_log_file(strPath.c_str(), message, nMesLength, false);
	}
}

/***********************************************************************************
*! @brief  : Create shader when openGL exist
*! @param  : [in] type   : path source vertex shader file
*! @param  : [in] source : path source frament shader file
*! @param  : [in] length : path source geometry shader file
*! @return : void
*! @author : thuong.nv          - [Date] : 06/03/2023
***********************************************************************************/
bool Shader::CheckStatusShader(unsigned int iShaderID, bool bPrintLog)
{
	int bSuccess = 1;
	int iShaderType = 0;

	constexpr const unsigned int nLogBuffSize = 512;
	char LogBuff[nLogBuffSize];

	constexpr const unsigned int nMesLogSize = 1024;
	char MessageLog[nMesLogSize];
	memset(MessageLog, 0, nMesLogSize);

	memset(LogBuff, 0, nLogBuffSize);

	glGetShaderiv(iShaderID, GL_COMPILE_STATUS, &bSuccess);
	glGetShaderiv(iShaderID, GL_SHADER_TYPE, &iShaderType);

	const size_t nLengthShaderType = 20;
	char strShaderType[nLengthShaderType];
	switch (iShaderType)
	{
	case GL_VERTEX_SHADER:
		memcpy_s(strShaderType, nLengthShaderType, "VERTEX", 7);
		break;
	case GL_FRAGMENT_SHADER:
		memcpy_s(strShaderType, nLengthShaderType, "FRAGMENT", 9);
		break;
	case GL_GEOMETRY_SHADER:
		memcpy_s(strShaderType, nLengthShaderType, "GEOMETRY", 9);
		break;
	default:
		memcpy_s(strShaderType, nLengthShaderType, "UNDEFINE", 9);
		break;
	}

	if (!bSuccess)
	{
		glGetShaderInfoLog(iShaderID, nLogBuffSize, NULL, LogBuff);
		snprintf(MessageLog, nMesLogSize, ">>>> ERROR::SHADER::%s::COMPILATION_FAILED\n%s\n ", strShaderType, LogBuff);
		PrintMessageLog(MessageLog, 0, bPrintLog);
	}
	else
	{
		snprintf(MessageLog, nMesLogSize, "[OK] Built shader [%s] done . \n", strShaderType);
		PrintMessageLog(MessageLog, 0, bPrintLog);
	}

	return bSuccess ? true : false;
}

/***********************************************************************************
*! @brief  : Create shader when openGL exist
*! @param  : [in] type   : path source vertex shader file
*! @param  : [in] source : path source frament shader file
*! @param  : [in] length : path source geometry shader file
*! @return : void
*! @author : thuong.nv          - [Date] : 06/03/2023
***********************************************************************************/
bool Shader::CheckStatusProgram(unsigned int iProgramID, bool bPrintLog)
{
	int bSuccess = 1;
	int iShaderType = 0;

	constexpr const unsigned int nLogBuffSize = 512;
	char LogBuff[nLogBuffSize];

	constexpr const unsigned int nMesLogSize = 1024;
	char MessageLog[nMesLogSize];
	memset(MessageLog, 0, nMesLogSize);

	memset(LogBuff, 0, nLogBuffSize);

	glGetProgramiv(iProgramID, GL_LINK_STATUS, &bSuccess);
	if (!bSuccess)
	{
		glGetProgramInfoLog(iProgramID, nLogBuffSize, NULL, LogBuff);
		snprintf(MessageLog, nMesLogSize, ">>>> ERROR::PROGRAM::COMPILATION_FAILED\n%s\n ", LogBuff);

		PrintMessageLog(MessageLog, 0, bPrintLog);
	}
	else
	{
		snprintf(MessageLog, nMesLogSize, "[OK] Built program done . \n");
		PrintMessageLog(MessageLog, 0, bPrintLog);
	}

	return bSuccess ? true : false;
}

/***********************************************************************************
*! @brief  : Create shader when openGL exist
*! @param  : [in] type   : path source vertex shader file
*! @param  : [in] source : path source frament shader file
*! @param  : [in] length : path source geometry shader file
*! @return : void
*! @author : thuong.nv          - [Date] : 06/03/2023
***********************************************************************************/
unsigned int Shader::CreateShader(ShaderType type, const char* source, unsigned int& length)
{
	unsigned int iShaderID = 0;

	bool bFailed = false;
	bool bOutLogFile = true;

	switch (type)
	{
	case ShaderType::VERTEX:
	{
		iShaderID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(iShaderID, 1, &source, NULL);
		glCompileShader(iShaderID);

		// print compile errors if any
		if (!CheckStatusShader(iShaderID, bOutLogFile))
		{
			PrintMessageLog("[FAILED] Create ShaderType::VERTEX Shader failed !", NULL, bOutLogFile);
			bFailed = true;
		}

		break;
	}
	case ShaderType::FRAGMENT:
	{
		iShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(iShaderID, 1, &source, NULL);
		glCompileShader(iShaderID);

		// print compile errors if any
		if (!CheckStatusShader(iShaderID, bOutLogFile))
		{
			PrintMessageLog("[FAILED] Create ShaderType::FRAGMENT Shader failed !", NULL, bOutLogFile);
			bFailed = true;
		}

		break;
	}
	case ShaderType::GEOMETRY:
	{
		iShaderID = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(iShaderID, 1, &source, NULL);
		glCompileShader(iShaderID);

		// print compile errors if any
		if (!CheckStatusShader(iShaderID, bOutLogFile))
		{
			PrintMessageLog("[FAILED] Create ShaderType::GEOMETRY Shader failed !", NULL, bOutLogFile);
			bFailed = true;
		}
		break;
	}
	default:
	{
		assert(0);
		bFailed = true;
		break;
	}
	}

	if (bFailed)
	{
		auto funFreeShader = [](unsigned int& ish)
		{
			if (ish <= 0) return;
			glDeleteShader(ish);
			ish = 0;
		};

		funFreeShader(iShaderID);

		iShaderID = 0;
	}

	return iShaderID;
}

/***********************************************************************************
*! @brief  : Create shader when openGL exist
*! @param  : [in] type   : path source vertex shader file
*! @param  : [in] source : path source frament shader file
*! @param  : [in] length : path source geometry shader file
*! @return : void
*! @author : thuong.nv          - [Date] : 06/03/2023
***********************************************************************************/
unsigned int Shader::LinkProgam(unsigned int vertexID, unsigned int fragID, unsigned int geoID)
{
	unsigned int iProgamID = 0;
	bool bOutLogFile = true;

	iProgamID = glCreateProgram();

	if (iProgamID > 0)
	{
		if (vertexID > 0) glAttachShader(iProgamID, vertexID);
		if (fragID > 0)   glAttachShader(iProgamID, fragID);
		if (geoID > 0)    glAttachShader(iProgamID, geoID);

		glLinkProgram(iProgamID);

		if (!CheckStatusProgram(iProgamID, bOutLogFile))
		{
			glDeleteProgram(iProgamID);
		}
	}
	else
	{
		iProgamID = 0;
		PrintMessageLog("[FAILED] glCreateProgram failed", NULL, true);
	}

	return iProgamID;
}

/***********************************************************************************
*! @brief  : Check shader created and may be use
*! @return : void
*! @author : thuong.nv          - [Date] : 06/03/2023
***********************************************************************************/
bool Shader::IsOK()
{
	return (m_programID > 0) ? true : false;
}

void Shader::Use()
{
	glUseProgram(m_programID);
}

void Shader::UnUse()
{
	glUseProgram(0);
}

/***********************************************************************************
*! @brief  : Set uniform value
*! @param  : [in] name   : variable name
*! @param  : [in] value  : variable value
*! @return : void
*! @author : thuong.nv          - [Date] : 06/03/2023
***********************************************************************************/
template<typename T>
void Shader::SetUniformValue(const char* name, T value)
{
	if (m_programID <= 0)
		return;

	GLint LocID = glGetUniformLocation(m_programID, name);

	if (std::is_floating_point<T>)
	{
		glUniform1f(LocID, static_cast<float>(value));
	}
	else if (std::is_same<T, bool>)
	{

	}
	else
	{
		assert(0);
	}
}

/***********************************************************************************
*! @brief  : Set uniform vector value
*! @param  : [in] name : variable name
*! @param  : [in] vec  : vector
*! @return : void
*! @author : thuong.nv          - [Date] : 06/03/2023
***********************************************************************************/
template<typename T, typename >
void Shader::SetUniformVec(const char* name, T vec)
{
	if (m_programID <= 0)
		return;

	GLint LocID = glGetUniformLocation(m_programID, name);

	if (std::is_same<T, Vec2D>)
	{
		glUniform2f(LocID, vec.x, vec.y);
	}
	else if (std::is_same<T, Vec3D>)
	{
		glUniform3f(LocID, vec.x, vec.y, vec.z);
	}
	else
	{
		assert(0);
	}
}

/***********************************************************************************
*! @brief  : Set uniform variable Maxtrix value
*! @param  : [in] name  : variable name
*! @param  : [in] mat   : matrix
*! @return : void
*! @author : thuong.nv          - [Date] : 06/03/2023
***********************************************************************************/
template<typename T, typename >
void Shader::SetUniformMat(const char* name, const T& mat)
{
	if (m_programID <= 0)
		return;

	GLint LocID = glGetUniformLocation(m_programID, name);

	if (std::is_same<T, glm::mat3>)
	{
		glUniformMatrix3fv(LocID, glm::value_ptr(mat));
	}
	else if (std::is_same<T, glm::mat4>)
	{
		glUniformMatrix4fv(LocID, glm::value_ptr(mat));
	}
	else if (std::is_same<T, glm::mat2>)
	{
		glUniformMatrix2fv(LocID, glm::value_ptr(mat));
	}
	else
	{
		assert(0);
	}
}

_____END_SECTION_____
