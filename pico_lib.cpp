#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "fsm_state_manager.hpp"

int64_t alarm_callback(alarm_id_t id, void *user_data) {
    // Put your timeout handler code in here
    return 0;
}



int main()
{
    auto stateManager = StateMachine::RobotArm::FSMStateManager::getInstance();
    stateManager->handleEvent(StateMachine::RobotArm::Event::Done);

    stdio_init_all();

    // Timer example code - This example fires off the callback after 2000ms
    add_alarm_in_ms(2000, alarm_callback, NULL, false);
    // For more examples of timer use see https://github.com/raspberrypi/pico-examples/tree/master/timer

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
