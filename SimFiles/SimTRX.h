#ifndef SIMPROJECT_SIMTRX_H
#define SIMPROJECT_SIMTRX_H
#include <stdbool.h>

typedef enum{
 BUFFER_DOWNLINK,
 BUFFER_UPLINK
}TrxPacketBuffer;

// returns the amount of packets in the TRX tele-command / telemetry buffers
unsigned int SimTRX_GetNumberOfPacketsInBuffer(TrxPacketBuffer buff);

// tells the TRX that a packet need to be sent to the ground
int SimTRX_AddPacketToDownlinkBuffer(unsigned char *data, unsigned int length);

//tells the TRX that a packet has been received and adds it to the buffer
int SimTRX_AddPacketToUplinkBuffer(unsigned char *data, unsigned int length);

// transmits a packet from the downlink buffer
int SimTRX_TransmitFromBuffer();

// reads a packet from the uplink buffer
int SimTRX_ReadFromBuffer(unsigned char *data, unsigned int *length);

// ------------------------------- CHANNEL PROPERTIES
//delay between sending packet from the ground sation to the satellite and vice versa
double SimTRX_CalcTransmissionDelay(unsigned int gs_index);
// time it takes to transmit a packet
double SimTRX_CalcTransmissionTime(unsigned int baud_bit_sec,unsigned int num_of_bytes_in_packet);

// ---------------------------- LINK BUDGET
double SimTRX_CalcAtmosphereAttenuation_dB(unsigned int gs_index);
double SimTRX_CalcFSPL_dB(unsigned int gs_index, double dist_km);
double SimTRX_CalcSystemLosses_dB();
double SimTRX_CalcAntennaGain_dB(unsigned int gs_index);
double SimTRX_CalcNoiseFloorLevel();
double SimTRX_CalcRSSI(unsigned int gs_index, double gs_transmit_power_dBm);

// check if there is a line of sight(LOS) between the satellite and the GS
bool SimTRX_CheckLOS(unsigned int ground_station_index);
// checks if a link whith the satellites is possible
bool SimTRX_CheckIfLinkExists(unsigned int ground_station_index, double gs_transmit_power_dBm);

#endif //SIMPROJECT_SIMTRX_H
