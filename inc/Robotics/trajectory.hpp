#pragma once 
#include <list>
#include <cstdint>
#include "configuration_space.hpp"

namespace Robotics {
    /**
     * @class Trajectory
     * @brief This class is responsible for storing the waypoints of a trajectory.
     * 
     * @details
     * It provides methods to save waypoints, get the next waypoint, check if the trajectory is complete,
     * clear the waypoints and get the number of waypoints. In theory no thread safety is required for this 
     * class as the read and write access are performed from different states.
     * 
     */
    class Trajectory {
        public:
            Trajectory() = default;
            virtual ~Trajectory() = default;

            Waypoint_t getNextWaypoint();
            bool isTrajectoryComplete();
            virtual void saveWaypoints(const uint8_t * rawWaypoints, const size_t size);
            void saveWaypoints(const Waypoint_t * waypoints, const size_t size);
            void clearWaypoints();
            size_t numOfWaypoints();
            
        private:
            std::list<Waypoint_t> waypoints;
    };

} // namespace Robotics
