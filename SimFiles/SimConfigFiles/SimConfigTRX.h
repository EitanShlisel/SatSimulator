#ifndef SIMPROJECT_SIMCONFIGTRX_H
#define SIMPROJECT_SIMCONFIGTRX_H

#define TRX_USE_PRINTS             0

#define TRX_TRANSMIT_TO_PRINT      1    // all downlink packets will be printed to console

#define TRX_TRANSMIT_TO_TCP        0      // all downlink packets will be sent through TCP port
#define TRX_DOWNLINK_TCP_PORT   7890

#define TRX_TRANSMIT_TO_FILE       0     // all downlink packets will be added to a file
#define TRX_DOWNLINK_FILE_PATH  ("\\Users\\97250\\Desktop\\downlink_transmissions")


#define TRX_UPLINK_CENTER_FREQUENCY_Hz      (400000000)
#define TRX_RX_ANTENNA_BANDWIDTH_Hz         (10000000)
#define TRX_MIN_SAT_LINK_POWER_dBm          (-100.0)

//{4436.49115, 3086.20630, 3375.89996} = HSL
//904431.10, -5536764.94, 3024132.14
//{9232.8415, -5535.13082, 3021.42020} = Cape Canaveral
//{0.00, 0.00, 6356.75231} = North Pole
#define TRX_GROUND_STATION_LOCATION_ECEF    {\
                                            {4436.224, 3086.573,3375.916},  \
                                            {9232.8415, -5535.13082, 3021.42020},    \
                                            { 0, 0, 6378.1000}};    // in km
#endif //SIMPROJECT_SIMCONFIGTRX_H
