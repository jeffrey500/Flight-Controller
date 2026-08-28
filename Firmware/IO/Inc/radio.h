#ifndef RADIO_H
#define RADIO_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint16_t roll;
    uint16_t pitch;
    uint16_t yaw;
    uint16_t throttle;
    uint16_t arm;
} RC_Channels;

void Radio_ProcessByte(uint8_t data);
bool Radio_GetChannels(RC_Channels *data);

#endif