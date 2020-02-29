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

int SimSTK_GetStkSunPosition(sun_vec_t *records, unsigned int num_of_records);

int SimSTK_GetCurrentStkSunVec(sun_vec_t *record);

atomic_time_t SimSTK_GetOrbitTime();

// -------------------------------------------------- TESTS
int SimSTK_Test(unsigned int num_of_lines_in_test_file);

int SimSTK_PrintSunVec(sun_vec_t *vec);
int SimSTK_PrintfSatPos(gps_record_t *vec);
#endif //SIMPROJECT_SIMSTK_H
