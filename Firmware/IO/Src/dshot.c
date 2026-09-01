// Implement sending unidirectional dshot to the motors https://betaflight.com/docs/development/API/Dshot
// dshot packets are as follows: 11 bit throttle request, 1 bit telemetry request, 4 bit CRC

#include "main.h"
#include "dshot.h"
#include "mixer.h"
#include "stm32h7xx_hal_tim.h"
#include <stdint.h>

extern TIM_HandleTypeDef htim2;   // motors C (CH3), D (CH4)
extern TIM_HandleTypeDef htim4;   // motors A (CH1), B (CH2)

#define DSHOT_BUFFER_LENGTH 18

// Create dshot bit value buffers for DMA to read from.
// These must live in .ram_d2 (SRAM1 @ 0x30000000) rather than the default
// .bss, which the linker script places in DTCM - unreachable by DMA1/DMA2.
#define DSHOT_DMA_RAM __attribute__((section(".ram_d2"), aligned(32)))

static DSHOT_DMA_RAM uint32_t bufferA[DSHOT_BUFFER_LENGTH];
static DSHOT_DMA_RAM uint32_t bufferB[DSHOT_BUFFER_LENGTH];
static DSHOT_DMA_RAM uint32_t bufferC[DSHOT_BUFFER_LENGTH];
static DSHOT_DMA_RAM uint32_t bufferD[DSHOT_BUFFER_LENGTH];

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

    // Re-arm each timer from a known state: stop it, zero the counter, clear any
    // compare/update flags left set while it free-ran between frames. Done per
    // timer since the four motors are split across TIM4 (A/B) and TIM2 (C/D).
    // No Stop_DMA / pulse-finished callback: the streams are DMA_NORMAL and
    // self-terminate, and HAL_TIM_PWM_Stop_DMA would kill the whole timer.

    // TIM4 : motor A on CH1, motor B on CH2
    __HAL_TIM_DISABLE(&htim4);
    __HAL_TIM_SET_COUNTER(&htim4, 0);
    __HAL_TIM_CLEAR_FLAG(&htim4, TIM_FLAG_UPDATE | TIM_FLAG_CC1 | TIM_FLAG_CC2);
    HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_1, bufferA, DSHOT_BUFFER_LENGTH);
    HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_2, bufferB, DSHOT_BUFFER_LENGTH);

    // TIM2 : motor C on CH3, motor D on CH4
    __HAL_TIM_DISABLE(&htim2);
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE | TIM_FLAG_CC3 | TIM_FLAG_CC4);
    HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_3, bufferC, DSHOT_BUFFER_LENGTH);
    HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_4, bufferD, DSHOT_BUFFER_LENGTH);
}