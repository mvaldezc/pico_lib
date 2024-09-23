/***********************************************************************
 * @file	:	dma_wrapper.hpp
 * @brief 	:	DMA raii class
 * 				RAII class to manage DMA transfers of rp2350.
 * @author	:	Marco Valdez @mvaldezc
 *
 ***********************************************************************/

#include "hardware/dma.h"
#include "message_format.hpp"

#define COUNT_MODE_SELF (1<<28)

// Macro to get the number of bits in the ring buffer at compile time
constexpr uint8_t RING_BITS = [](){
    constexpr size_t n = Communication::BUFFER_SIZE;
    static_assert((n & (n - 1)) == 0, "n must be a power of 2");
    size_t m = Communication::BUFFER_SIZE;
    uint8_t log = 0;
    while (m > 1)
    {
        m >>= 1;
        ++log;
    }
    return log;
}();

namespace Communication {
namespace Hardware {

    class RxDMA
    {
        public:
            RxDMA()
            {
                // Allocate a dma channel
                channel = dma_claim_unused_channel(true);

                // Configure the dma channel
                config = dma_channel_get_default_config(channel);
                channel_config_set_transfer_data_size(&config, DMA_SIZE_8); // 8 bits transfer
                channel_config_set_read_increment(&config, false);  // static read address
                channel_config_set_write_increment(&config, true);  // write address increments
                channel_config_set_ring(&config, true, RING_BITS);  // circular buffer
                dma_channel_hw_addr(channel)->transfer_count |= COUNT_MODE_SELF; // self trigger
            }
            
            ~RxDMA()
            {
                // Free the dma channel
                dma_channel_unclaim(channel);
            }

            uint8_t * getWriteAddress()
            {
                return reinterpret_cast<uint8_t*> (dma_channel_hw_addr(channel)->write_addr);
            }

            uint channel;
            dma_channel_config config;
    };
    
} // namespace Hardware
} // namespace Communication
