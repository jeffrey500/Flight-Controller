// Implement sending unidirectional dshot to the motors https://betaflight.com/docs/development/API/Dshot
// dshot packets are as follows: 11 bit throttle request, 1 bit telemetry request, 4 bit CRC

#include "main.h"
#include "dshot.h"
#include "mixer.h"
#include "stm32h7xx_hal_tim.h"
#include <stdint.h>

extern TIM_HandleTypeDef htim2;

#define DSHOT_BUFFER_LENGTH 18

// Create dshot bit value buffers for DMA to read from
static uint32_t bufferA[DSHOT_BUFFER_LENGTH];
static uint32_t bufferB[DSHOT_BUFFER_LENGTH];
static uint32_t bufferC[DSHOT_BUFFER_LENGTH];
static uint32_t bufferD[DSHOT_BUFFER_LENGTH];

// Create the 16 bit packet
static uint16_t create_dshot_packet(uint16_t motor_value){
    uint16_t packet = (motor_value << 1) | 0;
    uint16_t crc = (packet ^ (packet >> 4) ^ (packet >> 8)) & 0x0F;
    uint16_t output = packet << 4 | crc;
    return output;
}

// Translate the 16 bit packet into the DMA buffer
// D-shot 300	300kbit/s	2.50	1.25	3.33	53.28 => 1:75% 0:38%
static void packet_to_dma(uint16_t motor_value, uint32_t *buffer){

    // Find max value that the timer counts up to
    uint32_t timer_period = htim2.Instance->ARR;

    // Add 
    for (int i = 0; i < 16; i++){
        if ((motor_value >> (15 - i)) & 1){
            buffer[i] = (timer_period * 3) / 4;
        } else {
            buffer[i] = (timer_period * 3) / 8;
        }
    }

    buffer[16] = 0;
    buffer[17] = 0;
}

void DSHOT_Update(void){
    // Create dshot packaets from motor values
    uint16_t dshotA = create_dshot_packet(motor_outputs[0]);
    uint16_t dshotB = create_dshot_packet(motor_outputs[1]);
    uint16_t dshotC = create_dshot_packet(motor_outputs[2]);
    uint16_t dshotD = create_dshot_packet(motor_outputs[3]);

    // Update the DMA using the dshot packets
    packet_to_dma(dshotA, bufferA);
    packet_to_dma(dshotB, bufferB);
    packet_to_dma(dshotC, bufferC);
    packet_to_dma(dshotD, bufferD);

    // Send the DMA
    HAL_TIM_PWM_Start_DMA(&htim2,TIM_CHANNEL_1, bufferA, DSHOT_BUFFER_LENGTH);
    HAL_TIM_PWM_Start_DMA(&htim2,TIM_CHANNEL_2, bufferB, DSHOT_BUFFER_LENGTH);
    HAL_TIM_PWM_Start_DMA(&htim2,TIM_CHANNEL_3, bufferC, DSHOT_BUFFER_LENGTH);
    HAL_TIM_PWM_Start_DMA(&htim2,TIM_CHANNEL_4, bufferD, DSHOT_BUFFER_LENGTH);
}