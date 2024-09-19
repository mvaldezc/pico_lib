/***********************************************************************
 * @file	:	communication_handler.hpp
 * @brief 	:	Callbacks for communication.
 * @author	:	Marco Valdez @marcovc41
 *
 ***********************************************************************/

#pragma once
#include <cstdint>
#include <unordered_map>
#include <cstring>
#include "message_format.hpp"

namespace Communication {

    typedef void (*Callback)(const volatile uint8_t * msgData);

    constexpr enum class RxIds : RxMessageId 
    {
        CHANGE_MODE = 0x00,
        DOWNLOAD_PROGRAM = 0x01,
        TRAJECTORY_DATA = 0x02,
        CANCEL_OPERATION = 0x03,
        START_OPERATION = 0x04,
        STOP_OPERATION = 0x05,
        EMERGENCY_STOP = 0x06
    };

    typedef struct 
    {
        uint8_t length;
        Callback rxMsgCallback;
    } RxMessageDataTemplate;

    void changeModeCallback(const volatile uint8_t *msgData);

    void downloadProgramCallback(const volatile uint8_t * msgData);

    void receiveTrajectoryDataCallback(const volatile uint8_t * msgData);

    void cancelOperationCallback(const volatile uint8_t * msgData);

    void startOperationCallback(const volatile uint8_t * msgData);

    void stopOperationCallback(const volatile uint8_t * msgData);

    void emergencyStopCallback(const volatile uint8_t * msgData);

    const std::unordered_map<RxIds, RxMessageDataTemplate> messageDictionary =
        {
            {RxIds::CHANGE_MODE, {1, &changeModeCallback}},
            {RxIds::DOWNLOAD_PROGRAM, {1, &downloadProgramCallback}},
            {RxIds::TRAJECTORY_DATA, {10, &receiveTrajectoryDataCallback}},
            {RxIds::CANCEL_OPERATION, {0, &cancelOperationCallback}},
            {RxIds::START_OPERATION, {0, &startOperationCallback}},
            {RxIds::STOP_OPERATION, {0, &stopOperationCallback}},
            {RxIds::EMERGENCY_STOP, {0, &emergencyStopCallback}}};

    void rxCallback(RxMessageId msgId, uint8_t dataLength, volatile uint8_t * msgData);

    void txCallback(uint8_t * msgData);

} // namespace Communication
