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

    // Cast enum class to generic RxMessageId type
    #define RAW(rxId) static_cast<RxMessageId>(rxId)

    /**
     * @brief Enum class for received message ids.
     */
    enum class RxIds : RxMessageId 
    {
        EMERGENCY_STOP = 0x00,
        CANCEL = 0x01,
        LOAD = 0x02,
        TEACH = 0x03,
        START = 0x04,
        PAUSE = 0x05,
        RESUME = 0x06,
        PROGRAM_DATA = 0x07,
        MCU_RESET = 0x08
    };

    // Define callback function signature
    using Callback = std::function<void(const uint8_t * msgData, const size_t dataLength)>;

    void emergencyStopCallback(const uint8_t * msgData, const size_t dataLength);
    void cancelOperationCallback(const uint8_t * msgData, const size_t dataLength);
    void loadProgramCallback(const uint8_t * msgData, const size_t dataLength);
    void programDataCallback(const uint8_t * msgData, const size_t dataLength);
    void teachProgramCallback(const uint8_t * msgData, const size_t dataLength);
    void startProgramCallback(const uint8_t * msgData, const size_t dataLength);
    void pauseProgramCallback(const uint8_t * msgData, const size_t dataLength);
    void resumeProgramCallback(const uint8_t * msgData, const size_t dataLength);
    void resetCallback(const uint8_t * msgData, const size_t dataLength);

    /**
     * @brief Dictionary to map received message ids to their respective callbacks.
     */
    const std::unordered_map<RxMessageId, Callback> messageDictionary =
        {
            {RAW(RxIds::EMERGENCY_STOP), emergencyStopCallback},
            {RAW(RxIds::CANCEL), cancelOperationCallback},
            {RAW(RxIds::LOAD), loadProgramCallback},
            {RAW(RxIds::TEACH), teachProgramCallback},
            {RAW(RxIds::START), startProgramCallback},
            {RAW(RxIds::PAUSE), pauseProgramCallback},
            {RAW(RxIds::RESUME), resumeProgramCallback},
            {RAW(RxIds::PROGRAM_DATA), programDataCallback},
            {RAW(RxIds::MCU_RESET), resetCallback}
        };

    /**
     * @brief Callback function to be called when a message is received.
     * @param[in] msgId Received message id.
     * @param[in] dataLength Length of the received message.
     * @param[in] msgData Pointer to the received message.
     */
    void rxCallback(RxMessageId msgId, size_t dataLength, uint8_t * msgData);

    /**
     * @brief Callback function to be called when a message is transmitted.
     * @param[out] msgData Pointer to the transmitted message.
     */
    void txCallback(uint8_t * msgData);

    /**
     * @brief Install the program data container.
     * @param[in] via_points Pointer to the trajectory object.
     */
    void installDataContainer(std::shared_ptr<Robotics::Trajectory> via_points);

    /**
     * @brief Uninstall the program data container.
     */
    void uninstallDataContainer();

    /**
     * @brief Get the program data container.
     * @return Pointer to the trajectory object.
     */
    std::shared_ptr<Robotics::Trajectory> getContainer();

} // namespace RobotArm
} // namespace Communication
