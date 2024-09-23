#include "trajectory.hpp"
#include <cstring>
using namespace Robotics;

Waypoint_t Trajectory::getNextWaypoint()
{
    if (waypoints.empty())
    {
        return Waypoint_t();
    }
    Waypoint_t wp = waypoints.front();
    waypoints.pop_front();
    return wp;
}

bool Trajectory::isTrajectoryComplete()
{
    return waypoints.empty();
}

void Trajectory::saveWaypoints(const uint8_t * rawWaypoints, const size_t size)
{
    // get size of Waypoints_t
    size_t wpSize = sizeof(Waypoint_t);

    for (int i = 0; i < size; i += wpSize)
    {
        Waypoint_t wp;
        memcpy(&wp, rawWaypoints + i, wpSize);
        waypoints.push_back(wp);
    }
}

void Trajectory::saveWaypoints(const Waypoint_t * waypoints, const size_t size)
{
    for (int i = 0; i < size; i++)
    {
        this->waypoints.push_back(waypoints[i]);
    }
}

void Trajectory::clearWaypoints()
{
    waypoints.clear();
}

size_t Trajectory::numOfWaypoints()
{
    return waypoints.size();
}