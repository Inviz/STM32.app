#ifndef INC_DEV_I2C
#define INC_DEV_I2C

#ifdef __cplusplus
extern "C" {
#endif

#include "core/device.h"
#include <libopencm3/stm32/i2c.h>

/* Start of autogenerated OD types */
/* 0x6260: Transport I2C 1
   Serial protocol */
typedef struct transport_i2c_properties {
    uint8_t parameter_count;
    uint8_t dma_rx_unit;
    uint8_t dma_rx_stream;
    uint8_t dma_rx_channel;
    uint8_t dma_rx_buffer_size;
    uint8_t dma_tx_unit;
    uint8_t dma_tx_stream;
    uint8_t dma_tx_channel;
    uint32_t baudrate;
    uint8_t databits;
    uint8_t phase;
} transport_i2c_properties_t;
/* End of autogenerated OD types */

struct transport_i2c{
    device_t *device;
    transport_i2c_properties_t *properties;
    uint32_t clock;
    uint32_t address;
    uint32_t dma_tx_address;
    uint32_t dma_rx_address;
    device_t *target_device;
    void *target_argument;

    uint8_t *dma_rx_buffer;
} ;


extern device_methods_t transport_i2c_methods;

/* Start of autogenerated OD accessors */
#define SUBIDX_I2C_DMA_RX_UNIT 0x1
#define SUBIDX_I2C_DMA_RX_STREAM 0x2
#define SUBIDX_I2C_DMA_RX_CHANNEL 0x3
#define SUBIDX_I2C_DMA_RX_BUFFER_SIZE 0x4
#define SUBIDX_I2C_DMA_TX_UNIT 0x5
#define SUBIDX_I2C_DMA_TX_STREAM 0x6
#define SUBIDX_I2C_DMA_TX_CHANNEL 0x7
#define SUBIDX_I2C_BAUDRATE 0x8
#define SUBIDX_I2C_DATABITS 0x9
#define SUBIDX_I2C_PHASE 0x10

OD_ACCESSORS(transport, i2c, properties, phase, SUBIDX_I2C_PHASE, uint8_t, u8) /* 0x62XX0a: {} */
/* End of autogenerated OD accessors */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif