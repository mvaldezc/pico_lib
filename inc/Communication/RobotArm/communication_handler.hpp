/***********************************************************************
 * @file	:	communication_handler.hpp
 * @brief 	:	Callbacks for communication.
 * @author	:	Marco Valdez @mvaldezc
 *
 ***********************************************************************/

#pragma once
#include <cstdint>
#include <unordered_map>
#include <cstring>
#include <memory>
#include <functional>
#include "message_format.hpp"
#include "trajectory.hpp"

namespace Communication {
namespace RobotArm {

    #define RAW(rxId) static_cast<RxMessageId>(rxId)
    using Callback = std::function<void(const uint8_t * msgData, const size_t dataLength)>;

    constexpr enum class RxIds : RxMessageId 
    {
        EMERGENCY_STOP = 0x00,
        CANCEL = 0x01,
        LOAD = 0x02,
        TEACH = 0x03,
        START = 0x04,
        PAUSE = 0x05,
        RESUME = 0x06,
        PROGRAM_DATA = 0x07
    };

    void emergencyStopCallback(const uint8_t * msgData, const size_t dataLength);

    void cancelOperationCallback(const uint8_t * msgData, const size_t dataLength);

    void loadProgramCallback(const uint8_t * msgData, const size_t dataLength);

    void programDataCallback(const uint8_t * msgData, const size_t dataLength);

    void teachProgramCallback(const uint8_t * msgData, const size_t dataLength);

    void startProgramCallback(const uint8_t * msgData, const size_t dataLength);

    void pauseProgramCallback(const uint8_t * msgData, const size_t dataLength);

    void resumeProgramCallback(const uint8_t * msgData, const size_t dataLength);

    const std::unordered_map<RxMessageId, Callback> messageDictionary =
        {
            {RAW(RxIds::EMERGENCY_STOP), emergencyStopCallback},
            {RAW(RxIds::CANCEL), cancelOperationCallback},
            {RAW(RxIds::LOAD), loadProgramCallback},
            {RAW(RxIds::TEACH), teachProgramCallback},
            {RAW(RxIds::START), startProgramCallback},
            {RAW(RxIds::PAUSE), pauseProgramCallback},
            {RAW(RxIds::RESUME), resumeProgramCallback},
            {RAW(RxIds::PROGRAM_DATA), programDataCallback}
        };

    void rxCallback(RxMessageId msgId, size_t dataLength, uint8_t * msgData);

    void txCallback(uint8_t * msgData);

    void installDataContainer(std::shared_ptr<Robotics::Trajectory> via_points);

} // namespace RobotArm
} // namespace Communication
