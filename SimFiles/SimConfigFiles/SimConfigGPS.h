#ifndef SIMPROJECT_SIMCONFIGGPS_H
#define SIMPROJECT_SIMCONFIGGPS_H


//--------------------------------------------------------------------------------------
//-------------------------------------------GPS Propagator-----------------------------
//--------------------------------------------------------------------------------------

#define GPS_USE_PRINTS 0
#define GPS_MIN_TIME_STEP   1   // [sec]

//#define STK_SAT_POS_VEC_CSV_PATH   ("C:\\Users\\97250\\Downloads\\Satellite1_J2000_Position_Velocity.csv") // path of STK file- satellite position over time
#define STK_SAT_POS_VEC_CSV_PATH   ("C:\\Users\\97250\\Downloads\\RssiTestOrbit.csv") // path of STK file- satellite position over time
#define STK_SAT_POS_CSV_FILE_LINE_OFFSET (1)
#define STK_SAT_POS_SAMPLE_TIME (60.0)    // sample time period between STK records [sec]


#define STK_SUN_VEC_PATH    ("C:\\Users\\97250\\Downloads\\Equl_Sun_Vector_J2000.txt")// path of STK file- sun position over time
#define STK_SUN_VEC_CSV_FILE_LINE_OFFSET (5)
#define STK_SUN_VEC_SAMPLE_TIME (10.0)    // sample time period between STK records [sec]

#define STK_EARTH_RADIUS_km (6357.0)
#define STK_EARTH_COORDINATE_CARTESIAN {0,0,0}


#define GPS_CARTESIAN_ANOMALY 1
#define GPS_SPHERICAL_ANOMALY 0
#if(GPS_SPHERICAL_ANOMALY !=0 && GPS_CARTESIAN_ANOMALY !=0)
    #error GPS configuration error. allowd to use only one anomaly system
#endif
#define  GPS_USE_TIME_DILATION  1

#define GPS_TIME_DIST_STD   1
#define GPS_TIME_DIST_MEAN  0

#define GPS_RADIUS_DIST_STD  1
#define GPS_RADIUS_DIST_MEAN 0

#define GPS_TANGENT_DIST_STD  1
#define GPS_TANGENT_DIST_MEAN 0

#define GPS_X_DIST_STD 1
#define GPS_X_DIST_MEAN 0
#define GPS_Y_DIST_STD 1
#define GPS_Y_DIST_MEAN 0
#define GPS_Z_DIST_STD 1
#define GPS_Z_DIST_MEAN 0
#endif //SIMPROJECT_SIMCONFIGGPS_H
