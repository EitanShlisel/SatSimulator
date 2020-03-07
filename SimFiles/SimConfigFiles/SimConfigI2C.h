#ifndef SIMPROJECT_SIMCONFIGI2C_H
#define SIMPROJECT_SIMCONFIGI2C_H

//--------------------------------------------------------------------------------------
//-------------------------------------------I2C----------------------------------------
//--------------------------------------------------------------------------------------

#define I2C_USE_PRINTS 0
#define I2C_MAX_SDA_BUFFER_SIZE 1024    // maximum size of data transferred in a single transmission on the I2C

// 100kBit/sec in Standard Mode; 400kBit/sec in Fast-Speed Mode;
// 1MBit/sec in Fast-Mode Plus; 3.4MBit/sec in High-Speed Mode;
#define I2C_DATA_SPEED_KBIT_SEC      (100000)

#endif //SIMPROJECT_SIMCONFIGI2C_H
