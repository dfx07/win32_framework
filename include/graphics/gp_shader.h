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
#ifndef GP_SHADER_H
#define GP_SHADER_H

#include "math/xgeotype.h"
#include <GL/glew.h>

#include <glm/gtc/type_ptr.hpp>
#include <xsysutils.h>

___BEGIN_NAMESPACE___

class Dllexport Shader
{
protected:
    enum class ShaderType
    {
        VERTEX  ,
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
    static unsigned int read_file(const wchar_t* path, void** data)
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
    static void write_log_file(IN const wchar_t* path, IN const void* data, IN const int& nsize, IN bool bClear = false)
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
        if (!file) return ;

        fwrite(data, sizeof(char), nsize, file);

        fclose(file);
    }

public:
    /***********************************************************************************
    *! @brief  : Create program shader when openGL exist
    *! @param  : [in] pathVertex   : path source vertex shader file
    *! @param  : [in] pathFragment : path source frament shader file
    *! @param  : [in] pathGeometry : path source geometry shader file
    *! @return : void
    *! @author : thuong.nv         - [Date] : 06/03/2023
    ***********************************************************************************/
    bool Create(const wchar_t* pathVertex, const wchar_t* pathFragment = NULL, const wchar_t* pathGeometry = NULL)
    {
        void* buff;
        char* vertexBuff, *fragBuff, *geoBuff;
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

private:
    /***********************************************************************************
    *! @brief  : write log = print and save file
    *! @param  : [in] message   : message
    *! @param  : [in] length    : lenght message
    *! @param  : [in] bOutFile  : push to file
    *! @return : void
    *! @author : thuong.nv          - [Date] : 06/03/2023
    ***********************************************************************************/
    void PrintMessageLog(const char* message, unsigned int length, bool bOutFile = false)
    {
        std::cout << message << std::endl;

        if (bOutFile)
        {
            int nMesLength = length;
            if (nMesLength <= 0)
            {
                nMesLength = strlen(message);
            }
            std::wstring strPath = get_current_directory_execute();
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
    bool CheckStatusShader(unsigned int iShaderID, bool bPrintLog = false)
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
    bool CheckStatusProgram(unsigned int iProgramID, bool bPrintLog = false)
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
    unsigned int CreateShader(ShaderType type, const char* source, unsigned int& length)
    {
        unsigned int iShaderID = 0;

        bool bFailed     = false;
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
    unsigned int LinkProgam(unsigned int vertexID, unsigned int fragID, unsigned int geoID)
    {
        unsigned int iProgamID = 0;
        bool bOutLogFile = true;

        iProgamID = glCreateProgram();

        if (iProgamID > 0)
        {
            if(vertexID > 0) glAttachShader(iProgamID, vertexID);
            if(fragID > 0)   glAttachShader(iProgamID, fragID);
            if(geoID > 0)    glAttachShader(iProgamID, geoID);

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

public:
    bool IsOK()
    {
        return (m_programID > 0) ? true : false;
    }

    void Use()
    {
        glUseProgram(m_programID);
    }

    void UnUse()
    {
        glUseProgram(0);
    }

    void SetUniformValue(const char* name, float value)
    {
        if (m_programID <= 0)
            return;

        GLint LocID = glGetUniformLocation(m_programID, name);

        if (LocID >= 0)
        {
            glUniform1f(LocID, value);
        }
    }

    void SetUniformMat4(const char* name, const Mat4& mat)
    {
        if (m_programID <= 0)
            return;

        GLint LocID = glGetUniformLocation(m_programID, name);

        if (LocID >= 0)
        {
            glUniformMatrix4fv(LocID, 1, GL_FALSE, glm::value_ptr(mat));
        }
    }
};


____END_NAMESPACE____

#endif // !GP_SHADER_H
