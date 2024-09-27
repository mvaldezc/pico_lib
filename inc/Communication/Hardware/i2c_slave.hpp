/***********************************************************************
 * @file	:	i2c_slave.hpp
 * @brief 	:	I2C Slave Library
 * 				Library that enables i2c slave functionality in rp2350.
 * @author	:	Marco Valdez @mvaldezc
 *
 ***********************************************************************/

#pragma once

#include <cstdint>
#include <cstdio>
#include "message_format.hpp"

#include "hardware/i2c.h"
#include "hardware/dma.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "dma_wrapper.hpp"

// Declare the bits in the registers we use
#define FIRST_DATA_BYTE (1 << 11)
#define DAT 0xFF
#define RX_FULL (1 << 2)
#define RD_REQ (1 << 5)
#define TX_ABRT (1 << 6)
#define START_DET (1 << 8)
#define STOP_DET (1 << 9)
#define RX_IRQ RX_FULL
#define TX_IRQ RD_REQ
#define DMA_TX 0x2
#define DMA_RX 0x1

namespace Communication {
namespace Hardware {

    // Declare config constants
    constexpr uint DEFAULT_I2C_BAUD_RATE = 400000;      // I2C baud rate b/s
    constexpr uint8_t DEFAULT_I2C_SLAVE_ADDRESS = 0x55; // I2C device address

    /**
     * @class I2CSlave
     * @brief Static class that configures I2C hardware as a slave and 
     * installs callbacks for read and write ISRs.
     */
    class I2CSlave
    {
        public:
            
            /**
             * @brief Static method that initializes I2C hardware as slave.
             * @param[in] rxHandlerPtr Pointer to function that will be called
             *            once a message is received.
             * @param[in] txHandlerPtr Pointer to function that will be called
             *            once a message request is received.
             * @param[in] address I2C slave address.
             * @param[in] dmaEnabled Enable or disable DMA transfers.
             */
            static void init(RxHandler rxHandlerPtr, TxHandler txHandlerPtr, uint8_t address, uint baudRate, bool enableDMA = true)
            {
                rxHandler = rxHandlerPtr;
                txHandler = txHandlerPtr;
                dmaEnabled = enableDMA;

                // Use I2C0 on the default SDA and SCL pins (GP4, GP5 on a Pico)
                gpio_set_dir(PICO_DEFAULT_I2C_SDA_PIN, GPIO_IN);
                gpio_set_dir(PICO_DEFAULT_I2C_SCL_PIN, GPIO_IN);
                gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
                gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
                gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
                gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
                i2c_init(i2c0, baudRate);
                i2c_set_slave_mode(i2c0, true, address);

                // Set interrupt mask of i2c0 hardware (activate slave write and slave read)
                i2c0_hw->intr_mask = RD_REQ | TX_ABRT | START_DET | STOP_DET;

                // Install handler for the interruption
                irq_set_exclusive_handler(I2C0_IRQ, i2c0_irq_handler);

                // Enable I2C0 interrupt
                irq_set_enabled(I2C0_IRQ, true);

                // Enable dma transfers
                if (dmaEnabled)
                {
                    i2c0_hw->dma_cr = DMA_RX;
                    channel_config_set_dreq(&dma.config, DREQ_I2C0_RX);
                    dma_channel_configure(dma.channel, &dma.config, &rxBuffer.data_, &i2c0_hw->data_cmd, 1, true);
                }
                else // enable irq otherwise
                {
                    i2c0_hw->intr_mask |= RX_FULL;
                }
            }

        private:
            I2CSlave()= default; // Private constructor to avoid instance creation
            static RxHandler rxHandler;
            static TxHandler txHandler;
            static Queue<uint8_t> rxBuffer;
            static bool txInProgress;
            static bool rxInProgress;
            static RxDMA dma;
            static bool dmaEnabled;
            static uint8_t * bufferFront;

            static void i2c0_irq_handler()
            {
                // Get interrupt status
                uint32_t status = i2c0_hw->intr_stat;
                
                // Check if the transfer was aborted, completed, or started
                if (status & (TX_ABRT | STOP_DET | START_DET)) {
                    if (status & TX_ABRT) {
                        i2c0_hw->clr_tx_abrt;
                    }
                    if (status & STOP_DET) {
                        stop_irq();
                    }
                    if (status & START_DET) {
                        start_irq();
                    }
                    txInProgress = false;
                    rxInProgress = false;
                }

                // Check if we have received data from the I2C master
                if (status & RX_IRQ) {
                    rx_irq();
                }

                // Check to see if the I2C master is requesting data from us
                if (status & TX_IRQ) {
                    tx_irq();
                }
            }

            inline static void rx_irq()
            {
                // Read the data from i2c buffer (this will clear the interrupt)
                uint32_t value = i2c0_hw->data_cmd;
                uint8_t data = value & DAT;

                rxBuffer.push(data);
                rxInProgress = true;
            }

            inline static void tx_irq()
            {
                uint8_t data = 0;
                if (txHandler != nullptr)
                {
                    // Load the data to variable
                    txHandler(&data);
                }
                // Write the data to buffer
                i2c0_hw->data_cmd = (uint32_t)(data & DAT);
                // Clear the interruption
                i2c0_hw->clr_rd_req;
            }

            inline static void stop_irq()
            {
                // Clear the interruption
                i2c0_hw->clr_stop_det;
                // If the transfer finished, call the handler
                if (rxInProgress && rxHandler != nullptr)
                {
                    rxHandler(*bufferFront, rxBuffer.capacity, (bufferFront+1));
                    rxBuffer.clear();
                }
            }

            inline static void start_irq()
            {
                // Clear the interruption
                i2c0_hw->clr_start_det;
                // Get buffer front
                if (dmaEnabled)
                {
                    bufferFront = dma.getWriteAddress();
                }
                else
                {
                    bufferFront = rxBuffer.front();
                }
                // assert bufferFront is within the addresses of rxBuffer
                assert(bufferFront >= rxBuffer.data_ && bufferFront <= rxBuffer.data_ + BUFFER_SIZE);
            }

    };

} // namespace Hardware
} // namespace Communication
