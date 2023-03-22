////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     xalgorithm.h
* @create   May 6, 2022
* @brief    Function to calculate easing effects (animation)
* @note     For conditions of distribution and use, see copyright notice in readme.txt
* It should only be used with some basic functions, for physical phenomena should
* using physical components to create a real feel
************************************************************************************/
#ifndef XGLEASING_H
#define XGLEASING_H

#include <iostream>
#include <stdarg.h>
#include <vector>
#include <combaseapi.h>
#include <memory>

#include "xalgorithm.h"

#define EASING_PI               3.14159265359f
#define EASING_EPSILON          0.001f

#define EASING_STANDTIME_START  0.f
#define EASING_STANDTIME_END    1.f

___BEGIN_NAMESPACE___


/*==================================================================================
Easing functions : specify the rate of change of a parameter over time.             

Bao gồm tính toán các hiệu ứng easing:
        +   Back                                                                    
        +   Quint                                                                   
        +   Elastic                                                                 
        +   Quart                                                                   

Các phương thức tính toán này dựa vào toán học và thường được áp dụng vào vị trí    

Option:
    EaseIn   : Interpolation follows the mathematical formula associated with the   
               easing function.                                                     
    EaseOut  : Interpolation follows 100% interpolation minus the output of the     
               formula associated with the easing function.                         
    EaseInOut: Interpolation uses EaseIn for the first half of the animation and    
               EaseOut for the second half.                                         
==================================================================================*/

enum class EaseMode
{
    In     ,
    Out    ,
    InOut  ,
};

enum class EaseType
{
    Back   ,
    Quint  ,
    Elastic,
    Quart  ,
    Bounce ,
    Expo   ,
    Circ   ,
    Quad   ,
    Sine   ,
};

// Return : Giá trị trong khoảng [vfrom -> vto] tại thời điểm t
static double CallEasingBack(EaseMode  mode      ,// Chế độ
                             double    t         ,// Thời điểm t từ lúc tính
                             double    vfrom     ,// Giá trị bắt đầu
                             double    vto       ,// Giá trị kết thúc
                             double    duration)  // Khoảng thời gian diễn ra
{
    if (t >= duration) return vto;

    auto t1  = math::hard_map(t, 0.0, duration, EASING_STANDTIME_START, EASING_STANDTIME_END, EASING_EPSILON);

    auto vt1 = 0.0;
    if      (mode == EaseMode::Out)     vt1 = easing::back_out(t1);
    else if (mode == EaseMode::InOut)   vt1 = easing::back_inout(t1);
    else                                vt1 = easing::back_in(t1);

    auto value = math::soft_map(vt1, EASING_STANDTIME_START, EASING_STANDTIME_END, vfrom, vto, EASING_EPSILON);
    return value;
}

// Return : Giá trị trong khoảng [vfrom -> vto] tại thời điểm t
static double CallEasingQuint(EaseMode  mode       ,// Chế độ
                              double    t          ,// Thời điểm t từ lúc tính
                              double    vfrom      ,// Giá trị bắt đầu
                              double    vto        ,// Giá trị kết thúc
                              double    duration)   // Khoảng thời gian diễn ra
{
    if (t >= duration) return vto;

    auto t1  = math::hard_map(t, 0.f, duration, EASING_STANDTIME_START, EASING_STANDTIME_END, EASING_EPSILON);

    auto vt1 = 0.0;
    if      (mode == EaseMode::Out)     vt1 = easing::quint_in(t1);
    else if (mode == EaseMode::InOut)   vt1 = easing::quint_inout(t1);
    else                                vt1 = easing::quint_in(t1);

    auto value = math::soft_map(vt1, EASING_STANDTIME_START, EASING_STANDTIME_END, vfrom, vto, EASING_EPSILON);
    return value;
}

// Return : Giá trị trong khoảng [vfrom -> vto] tại thời điểm t
static double CallEasingElastic(EaseMode  mode      ,// Chế độ
                                double    t         ,// Thời điểm t từ lúc tính
                                double    vfrom     ,// Giá trị bắt đầu
                                double    vto       ,// Giá trị kết thúc
                                double    duration)  // Khoảng thời gian diễn ra
{
    if (t >= duration) return vto;

    auto t1  = math::hard_map(t, 0.f, duration, EASING_STANDTIME_START, EASING_STANDTIME_END, EASING_EPSILON);

    auto vt1 = 0.0;
    if      (mode == EaseMode::Out)     vt1 = easing::elastic_out(t1);
    else if (mode == EaseMode::InOut)   vt1 = easing::elastic_inout(t1);
    else                                vt1 = easing::elastic_in(t1);

    auto value = math::soft_map(vt1, EASING_STANDTIME_START, EASING_STANDTIME_END, vfrom, vto, EASING_EPSILON);
    return value;
}


// Return : Giá trị trong khoảng [vfrom -> vto] tại thời điểm t
static double CallEasingBounce(EaseMode  mode      ,// Chế độ
                               double    t         ,// Thời điểm t từ lúc tính
                               double    vfrom     ,// Giá trị bắt đầu
                               double    vto       ,// Giá trị kết thúc
                               double    duration)  // Khoảng thời gian diễn ra
{
    if (t >= duration) return vto;

    auto t1  = math::hard_map(t, 0.f, duration, EASING_STANDTIME_START, EASING_STANDTIME_END, EASING_EPSILON);

    double vt1 = 0.0;
    if      (mode == EaseMode::Out)     vt1 = easing::bounce_out(t1);
    else if (mode == EaseMode::InOut)   vt1 = easing::bounce_inout(t1);
    else                                vt1 = easing::bounce_in(t1);

    auto value = math::soft_map(vt1, EASING_STANDTIME_START, EASING_STANDTIME_END, vfrom, vto, EASING_EPSILON);
    return value;
}

//===================================================================================
// Class EasingBase : Lớp cơ sở các hiệu ứng Easing liên quan                        
//===================================================================================

interface EasingAction;

interface EasingBase
{
public:
	typedef std::shared_ptr<EasingBase>     easingbase_ptr;
	typedef std::shared_ptr<EasingAction>   easingaction_ptr;
};

interface EasingAction : EasingBase
{
public:
    virtual double  EaseIn(const double t, const double duration, const double from, const double to)   = 0;
    virtual double  EaseOut(const double t, const double duration, const double from, const double to)  = 0;
    virtual double  EaseInOut(const double t, const double duration, const double from, const double to)= 0;
};

class EasingObject : public EasingBase
{
//↓ disable create object EasingObject inheritance - use pointer from CreateIntanse
private:
    //struct secret {};
    //virtual void impossible(secret) = 0; // cannot override this in subclasses
    //                                     // because the parameter type is private

    //template <class X> class AImpl : public X
    //{
    //    void impossible(secret) {}; // a nested class can access private members
    //};
//↑ disable create object EasingObject inheritance - use pointer from CreateIntanse

public:
    virtual EaseType		GetType() = 0;
};

class EasingBack : public EasingAction, EasingObject
{
public:
    virtual EaseType GetType() { return EaseType::Back; }

private:
    virtual double EaseIn(const double t, const double duration, const double from, const double to)
    {
        double value = easing::back_in(t);
        return value;
    }

    virtual double EaseOut(const double t, const double duration, const double from, const double to)
    {
        double value = easing::back_out(t);
        return value;
    }

    virtual double EaseInOut(const double t, const double duration, const double from, const double to)
    {
        double value = easing::back_inout(t);
        return value;
    }
};

class EasingQuint : public EasingAction, EasingObject
{
public:
    virtual EaseType GetType() { return EaseType::Quint; }

private:
    virtual double EaseIn(const double t, const double duration, const double from, const double to)
    {
        double value = easing::quint_in(t);
        return value;
    }

    virtual double EaseOut(const double t, const double duration, const double from, const double to)
    {
        double value = easing::quint_out(t);
        return value;
    }

    virtual double EaseInOut(const double t, const double duration, const double from, const double to)
    {
        double value = easing::quint_inout(t);
        return value;
    }
};

class EasingElastic :public EasingAction, EasingObject
{
public:
    virtual EaseType GetType() { return EaseType::Elastic; }

private:
    virtual double EaseIn(const double t, const double duration, const double from, const double to)
    {
        double value = easing::elastic_in(t);
        return value;
    }

    virtual double EaseOut(const double t, const double duration, const double from, const double to)
    {
        double value = easing::elastic_out(t);
        return value;
    }

    virtual double EaseInOut(const double t, const double duration, const double from, const double to)
    {
        double value = easing::elastic_inout(t);
        return value;
    }
};

class EasingQuart : public EasingAction, EasingObject
{
public:
    virtual EaseType GetType() { return EaseType::Quart; }

private:
    virtual double EaseIn(const double t, const double duration, const double from, const double to)
    {
        double value = easing::quart_in(t);
        return value;
    }

    virtual double EaseOut(const double t, const double duration, const double from, const double to)
    {
        double value = easing::quart_out(t);
        return value;
    }

    virtual double EaseInOut(const double t, const double duration, const double from, const double to)
    {
        double value = easing::quart_inout(t);
        return value;
    }
};

class EasingBounce : public EasingAction, EasingObject
{
public:
    virtual EaseType GetType() { return EaseType::Bounce; }

private:
    virtual double EaseIn(const double t, const double duration, const double from, const double to)
    {
        double value = easing::bounce_in(t);
        return value;
    }

    virtual double EaseOut(const double t, const double duration, const double from, const double to)
    {
        double value = easing::bounce_out(t);
        return value;
    }

    virtual double EaseInOut(const double t, const double duration, const double from, const double to)
    {
        double value = easing::bounce_inout(t);
        return value;
    }
};

class EasingExpo : public EasingAction, EasingObject
{
public:
    virtual EaseType GetType() { return EaseType::Expo; }

private:
    virtual double EaseIn(const double t, const double duration, const double from, const double to)
    {
        double value = easing::expo_in(t);
        return value;
    }

    virtual double EaseOut(const double t, const double duration, const double from, const double to)
    {
        double value = easing::expo_out(t);
        return value;
    }

    virtual double EaseInOut(const double t, const double duration, const double from, const double to)
    {
        double value = easing::expo_inout(t);
        return value;
    }
};

class EasingCirc : public EasingAction, EasingObject
{
public:
    virtual EaseType GetType() { return EaseType::Circ; }

private:
    virtual double EaseIn(const double t, const double duration, const double from, const double to)
    {
        double value = easing::circ_in(t);
        return value;
    }

    virtual double EaseOut(const double t, const double duration, const double from, const double to)
    {
        double value = easing::circ_out(t);
        return value;
    }

    virtual double EaseInOut(const double t, const double duration, const double from, const double to)
    {
        double value = easing::circ_inout(t);
        return value;
    }
};

class EasingQuad : public EasingAction, EasingObject
{
public:
    virtual EaseType GetType() { return EaseType::Quad; }

private:
    virtual double EaseIn(const double t, const double duration, const double from, const double to)
    {
        double value = easing::quad_in(t);
        return value;
    }

    virtual double EaseOut(const double t, const double duration, const double from, const double to)
    {
        double value = easing::quad_out(t);
        return value;
    }

    virtual double EaseInOut(const double t, const double duration, const double from, const double to)
    {
        double value = easing::quad_inout(t);
        return value;
    }
};

class EasingSine : public EasingAction, EasingObject
{
public:
    virtual EaseType GetType() { return EaseType::Sine; }

private:
    virtual double EaseIn(const double t, const double duration, const double from, const double to)
    {
        double value = easing::sine_in(t);
        return value;
    }

    virtual double EaseOut(const double t, const double duration, const double from, const double to)
    {
        double value = easing::sine_out(t);
        return value;
    }

    virtual double EaseInOut(const double t, const double duration, const double from, const double to)
    {
        double value = easing::sine_inout(t);
        return value;
    }
};

//===================================================================================
// Class EasingEngine : Implement execute easing                                     
//===================================================================================

class EasingDataBase : EasingBase
{
public:
	easingbase_ptr    action;
	EaseType          type;
	EaseMode          mode;
	double            from;
	double            to;
    double            duration;

    double            value;
    bool              pause;

public:
	EasingDataBase(easingbase_ptr action, EaseType type,
		EaseMode mode, double duration, double from , double to, double value_begin)
	{
		this->action = action;
		this->type	 = type	 ;
		this->mode	 = mode	 ;
		this->from	 = from	 ;
		this->to	 = to	 ;
        this->duration = duration;

        this->value  = value_begin;
        this->pause  = false;
	}

    void reset()
    {
        this->value = from;
        this->pause = false;
    }
};

class EasingEngine : EasingBase
{
private:
    // common setup property
    double                   m_dCumulativeTime = 0;
    bool                     m_bPause;

    // data list contain
    std::vector<EasingDataBase> m_data_list;

public:
    EasingEngine()
    {
        this->Reset();
    }

public:
    virtual void Start()    { m_bPause = false; }
    virtual void Pause()    { m_bPause = true;  }
    virtual void Continue() { m_bPause = false; }
    virtual bool IsActive() { return !m_bPause; }

    virtual bool AddExec(EaseType type, EaseMode mode, double _duration, double _from, double _to)
    {
        easingaction_ptr action = NULL;

        switch (type)
        {
        case EaseType::Back:
            action = std::make_shared<EasingBack>();
            break;
        case EaseType::Quint:
            action = std::make_shared<EasingQuint>();
            break;
        case EaseType::Elastic:
            action = std::make_shared<EasingElastic>();
            break;
        case EaseType::Quart:
            action = std::make_shared<EasingQuart>();
            break;
        case EaseType::Bounce:
            action = std::make_shared<EasingBounce>();
            break;
        case EaseType::Expo:
            action = std::make_shared<EasingExpo>();
            break;
        case EaseType::Circ:
            action = std::make_shared<EasingCirc>();
            break;
        case EaseType::Quad:
            action = std::make_shared<EasingQuad>();
            break;
        case EaseType::Sine:
            action = std::make_shared<EasingSine>();
            break;
        default:
            break;
        }

        if (!action)
        {
            std::cout << "[err] : type not support !" << std::endl;
            return false;
        }

        m_data_list.emplace_back(EasingDataBase(action, type, mode, _duration, _from, _to , _from));

        return true;
    }

    /***************************************************************************
	*! @brief  : reset data list
	*! @return : number of control created
	*! @author : thuong.nv          - [Date] : 05/03/2023
	***************************************************************************/
    virtual void Reset()
    {
        m_bPause = false;

        m_dCumulativeTime = 0.f;

        for (int i = 0; i < m_data_list.size(); i++)
        {
            m_data_list[i].reset();
        }
    }

    //==================================================================================
    // Thực hiện tính toán giá trị animation easing với đầu vào là thời điểm t          
    // t : Giá trị đầu vào tính theo millisecond
    //==================================================================================
    virtual void Update(double t)
    {
        if (m_bPause) return;

        m_dCumulativeTime += t;

        double cumula_map = 0.0, value = 0.0, from = 0.0, to = 0.0;
        double duration = 0.0;

        m_bPause = true;

        for (int i = 0; i < m_data_list.size(); i++)
        {
            if (m_data_list[i].pause) continue;

            m_bPause = false;

            from     = m_data_list[i].from;
            to       = m_data_list[i].to;
            duration = m_data_list[i].duration;

            auto _action = std::static_pointer_cast<EasingAction>(m_data_list[i].action);
            if (nullptr == _action) continue;

            cumula_map = math::hard_map(m_dCumulativeTime, 0.f, duration, EASING_STANDTIME_START, EASING_STANDTIME_END, EASING_EPSILON);

            // call function caculation data easing
            switch (m_data_list[i].mode)
            {
                case  EaseMode::Out:
                    value = _action->EaseOut(cumula_map, duration, from, to);
                    break;
                case  EaseMode::InOut:
                    value = _action->EaseInOut(cumula_map, duration, from, to);
                    break;
                default:
                    value = _action->EaseIn(cumula_map, duration, from, to);
                    break;
            }

            value = math::soft_map(value, EASING_STANDTIME_START, EASING_STANDTIME_END, from, to, EASING_EPSILON);

            m_data_list[i].value = value;
        }
    }

    virtual double Value(const int& i) const
    {
        return m_data_list[i].value;
    }

    virtual double operator[](const int& i) const
    {
        return this->Value(i);
    }

};

____END_NAMESPACE____

#endif // !XGLEASING_H
