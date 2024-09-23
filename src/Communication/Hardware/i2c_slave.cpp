#include "i2c_slave.hpp"
using namespace Communication::Hardware;
using namespace Communication;

// Static member initialization
RxHandler I2CSlave::rxHandler = nullptr;
TxHandler I2CSlave::txHandler = nullptr;
Queue I2CSlave::rxBuffer;
bool I2CSlave::txInProgress = false;
bool I2CSlave::rxInProgress = false;
RxDMA I2CSlave::dma;
bool I2CSlave::dmaEnabled = false;
uint8_t * I2CSlave::bufferFront = nullptr;
