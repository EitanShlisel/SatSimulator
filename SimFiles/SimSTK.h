#ifndef SIMPROJECT_SIMSTK_H
#define SIMPROJECT_SIMSTK_H

#include "../SubsystemModules/GPS_types.h"

int SimSTK_initStkRecords();

// returns an stk data record at index
int SimSTK_GetStkDataRecordAtIndex(gps_record_t *record, unsigned long index);

// returns the most updated record from the STK
int SimSTK_GetCurrentStkSatPosition(gps_record_t *record);

// returns a range from the current time
int SimSTK_GetStkSatPositionRange(gps_record_t *records, unsigned int num_of_records);

int SimSTK_GetStkSatPositionRange(gps_record_t *records, unsigned int num_of_records);

int SimSTK_GetStkSunPosition(sun_vec_t *records, unsigned int num_of_records);

int SimStk_GetCurrentStkSunVec(sun_vec_t *record);

// -------------------------------------------------- TESTS
int SimSTK_Test(unsigned int num_of_lines_in_test_file);


#endif //SIMPROJECT_SIMSTK_H
