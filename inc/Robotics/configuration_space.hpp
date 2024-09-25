/***********************************************************************
 * @file	:	configuration_space.hpp
 * @brief 	:	Configuration space
 *              Definition of the configuration space of a robot.
 * @author	:	Marco Valdez @mvaldezc
 *
 ***********************************************************************/

#pragma once
#include <numbers>

namespace Robotics {

    inline constexpr float PI = std::numbers::pi_v<float>;


    /**
     * @brief Enum class to define the topology of the generalized coordinates.
     */
    enum class Topology
    {
        R2,     // Euclidean space
        R3,     // Euclidean space
        SO2,    // Special orthogonal group
        SO3,    // Special orthogonal group
        SE2,    // Special Euclidean group
        SE3     // Special Euclidean group
    };

    /**
     * @brief Struct to define a generalized coordinate.
     */
    struct GeneralizedCoordinate_t
    {
        // Coordinate value
        float value;
        // Topology of the coordinate
        Topology topology;
        // Coordinate limits
        float min;
        float max;
        // Overload operator() to return the value
        auto operator()() -> float { return value; }
        // Overload operator= to a float
        auto operator=(const float & val) -> GeneralizedCoordinate_t & { value = val; return *this; }
        // Overload operator= to a GeneralizedCoordinate_t
        auto operator=(const GeneralizedCoordinate_t & val) -> GeneralizedCoordinate_t & 
        {   
            value = val.value;
            topology = val.topology;
            min = val.min;
            max = val.max; 
            return *this; 
        }
    };

    /**
     * @brief Struct to define the configuration space of a robot.
     */
    struct ConfigurationSpace_t
    {
        GeneralizedCoordinate_t q1 {0.0f, Topology::SO2, -PI, PI};
        GeneralizedCoordinate_t q2 {0.0f, Topology::SO2, -PI, PI};
        GeneralizedCoordinate_t q3 {0.0f, Topology::SO2, -PI, PI};
        GeneralizedCoordinate_t q4 {0.0f, Topology::SO2, -PI, PI};
        GeneralizedCoordinate_t q5 {0.0f, Topology::SO2, -PI, PI};
        GeneralizedCoordinate_t q6 {0.0f, Topology::SO2, -PI, PI};
    };

    /**
     * @brief Struct to define a waypoint in the configuration space.
     */
    typedef struct
    {
        float q1;
        float q2;
        float q3;
        float q4;
        float q5;
        float q6;
    } Waypoint_t;

} // namespace Robotics
