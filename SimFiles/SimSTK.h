#ifndef SIMPROJECT_SIMSTK_H
#define SIMPROJECT_SIMSTK_H

#include "GPS_types.h"

int SimSTK_initStkRecords();

// returns the most updated record from the STK
int SimSTK_GetCurrentStkDataRecord(gps_record_t *record);

// returns a range from the current time
int SimSTK_GetStkDataRecordRange(gps_record_t *log, unsigned int num_of_records);

// -------------------------------------------------- TESTS
int SimSTK_Test(unsigned int num_of_lines_in_test_file);

#endif //SIMPROJECT_SIMSTK_H
