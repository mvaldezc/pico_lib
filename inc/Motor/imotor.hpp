/***********************************************************************
 * @file	:	imotor.hpp
 * @brief 	:	Motor Interface
 * 				Definition of a motor interface to be independent of
 *              motor type.
 * @author	:	Marco Valdez @mvaldezc
 *
 ***********************************************************************/

#pragma once
#include <cstdint>
#include <cstring>

namespace Motor {
    
    enum class Direction : bool
    {
        CounterClockwise = false,
        Clockwise = true
    };

    enum class ControlMode
    {
        Position = 0,
        Speed = 1
    };

    /**
     * @interface IMotor
     * @brief Motor control driver interface.
     */
    class IMotor
    {
        public:
            explicit IMotor() = default;
            virtual ~IMotor() = default;
            virtual void enable() = 0;
            virtual void disable() = 0;
            virtual void setZero() = 0;
            virtual void setControlMode(ControlMode mode) = 0;

            virtual int_fast32_t getAbsPosition() const = 0;
            virtual void setAbsPosition(int_fast32_t position) = 0;
            
            virtual void setSpeed(uint_fast32_t speed) = 0;
            virtual uint_fast32_t getSpeed() const = 0;

            bool isEnabled() const
            {
                return enabledFlag;
            }

            virtual void setDirection(Direction direction) = 0;
            Direction getDirection() const
            {
                return direction;
            }

        protected:
            volatile bool enabledFlag = false;
            volatile int_fast32_t position;
            volatile Direction direction = Direction::CounterClockwise;
            const int id = 0;
    };

} // namespace Motor
