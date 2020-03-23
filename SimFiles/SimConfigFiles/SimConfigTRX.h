#ifndef SIMPROJECT_SIMCONFIGTRX_H
#define SIMPROJECT_SIMCONFIGTRX_H

#define TRX_TRANSMIT_TO_PRINT      1    // all downlink packets will be printed to console

#define TRX_TRANSMIT_TO_TCP        0      // all downlink packets will be sent through TCP port
#define TRX_DOWNLINK_TCP_PORT   7890

#define TRX_TRANSMIT_TO_FILE       0     // all downlink packets will be added to a file
#define TRX_DOWNLINK_FILE_PATH  ("\\Users\\97250\\Desktop\\downlink_transmissions")


#define TRX_UPLINK_CENTER_FREQUANCY_Hz      500000
#define TRX_RX_ANTENNA_BANDWIDTH_Hz         50000
#define TRX_MIN_SAT_LINK_POWER_dBm          -100.0

#define TRX_NUMBER_OF_GROUND_STATIONS   2
#define TRX_GROUND_STATION_LOCATION_ECEF    {\
                                            {4436.224, 3086.573,3375.916},  \
                                            {4400.155,3086.241,3371.081}    \
                                            };    // in km
#endif //SIMPROJECT_SIMCONFIGTRX_H
