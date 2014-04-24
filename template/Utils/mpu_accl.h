#include "utils/I2C_Stellaris_API_r02/I2C_Stellaris_API.h"

    #define MPU6050_EXT_SYNC_TEMP_OUT_L   MPU6050_EXT_SYNC_SET_1
    #define MPU6050_EXT_SYNC_GYRO_XOUT_L  MPU6050_EXT_SYNC_SET_2
    #define MPU6050_EXT_SYNC_GYRO_YOUT_L  MPU6050_EXT_SYNC_SET_3
    #define MPU6050_EXT_SYNC_GYRO_ZOUT_L  MPU6050_EXT_SYNC_SET_4
    #define MPU6050_EXT_SYNC_ACCEL_XOUT_L MPU6050_EXT_SYNC_SET_5
    #define MPU6050_EXT_SYNC_ACCEL_YOUT_L MPU6050_EXT_SYNC_SET_6
    #define MPU6050_EXT_SYNC_ACCEL_ZOUT_L MPU6050_EXT_SYNC_SET_7
     
    // Combined definitions for the DLPF_CFG values
    #define MPU6050_DLPF_CFG_0 (0)
    #define MPU6050_DLPF_CFG_1 (bit(MPU6050_DLPF_CFG0))
    #define MPU6050_DLPF_CFG_2 (bit(MPU6050_DLPF_CFG1))
    #define MPU6050_DLPF_CFG_3 (bit(MPU6050_DLPF_CFG1)|bit(MPU6050_DLPF_CFG0))
    #define MPU6050_DLPF_CFG_4 (bit(MPU6050_DLPF_CFG2))
    #define MPU6050_DLPF_CFG_5 (bit(MPU6050_DLPF_CFG2)|bit(MPU6050_DLPF_CFG0))
    #define MPU6050_DLPF_CFG_6 (bit(MPU6050_DLPF_CFG2)|bit(MPU6050_DLPF_CFG1))
    #define MPU6050_DLPF_CFG_7 (bit(MPU6050_DLPF_CFG2)|bit(MPU6050_DLPF_CFG1)|bit(MPU6050_DLPF_CFG0))
     
    // Alternative names for the combined definitions
    // This name uses the bandwidth (Hz) for the accelometer,
    // for the gyro the bandwidth is almost the same.
    #define MPU6050_DLPF_260HZ    MPU6050_DLPF_CFG_0
    #define MPU6050_DLPF_184HZ    MPU6050_DLPF_CFG_1
    #define MPU6050_DLPF_94HZ     MPU6050_DLPF_CFG_2
    #define MPU6050_DLPF_44HZ     MPU6050_DLPF_CFG_3
    #define MPU6050_DLPF_21HZ     MPU6050_DLPF_CFG_4
    #define MPU6050_DLPF_10HZ     MPU6050_DLPF_CFG_5
    #define MPU6050_DLPF_5HZ      MPU6050_DLPF_CFG_6
    #define MPU6050_DLPF_RESERVED MPU6050_DLPF_CFG_7
     
    // GYRO_CONFIG Register
    // The XG_ST, YG_ST, ZG_ST are bits for selftest.
    // The FS_SEL sets the range for the gyro.
    // These are the names for the bits.
    // Use these only with the bit() macro.
    #define MPU6050_FS_SEL0 MPU6050_D3
    #define MPU6050_FS_SEL1 MPU6050_D4
    #define MPU6050_ZG_ST   MPU6050_D5
    #define MPU6050_YG_ST   MPU6050_D6
    #define MPU6050_XG_ST   MPU6050_D7
     
    // Combined definitions for the FS_SEL values
    #define MPU6050_FS_SEL_0 (0)
    #define MPU6050_FS_SEL_1 (bit(MPU6050_FS_SEL0))
    #define MPU6050_FS_SEL_2 (bit(MPU6050_FS_SEL1))
    #define MPU6050_FS_SEL_3 (bit(MPU6050_FS_SEL1)|bit(MPU6050_FS_SEL0))
     
    // Alternative names for the combined definitions
    // The name uses the range in degrees per second.
    #define MPU6050_FS_SEL_250  MPU6050_FS_SEL_0
    #define MPU6050_FS_SEL_500  MPU6050_FS_SEL_1
    #define MPU6050_FS_SEL_1000 MPU6050_FS_SEL_2
    #define MPU6050_FS_SEL_2000 MPU6050_FS_SEL_3
     
    // ACCEL_CONFIG Register
    // The XA_ST, YA_ST, ZA_ST are bits for selftest.
    // The AFS_SEL sets the range for the accelerometer.
    // These are the names for the bits.
    // Use these only with the bit() macro.
    #define MPU6050_ACCEL_HPF0 MPU6050_D0
    #define MPU6050_ACCEL_HPF1 MPU6050_D1
    #define MPU6050_ACCEL_HPF2 MPU6050_D2
    #define MPU6050_AFS_SEL0   MPU6050_D3
    #define MPU6050_AFS_SEL1   MPU6050_D4
    #define MPU6050_ZA_ST      MPU6050_D5
    #define MPU6050_YA_ST      MPU6050_D6
    #define MPU6050_XA_ST      MPU6050_D7
     
    // Combined definitions for the ACCEL_HPF values
    #define MPU6050_ACCEL_HPF_0 (0)
    #define MPU6050_ACCEL_HPF_1 (bit(MPU6050_ACCEL_HPF0))
    #define MPU6050_ACCEL_HPF_2 (bit(MPU6050_ACCEL_HPF1))
    #define MPU6050_ACCEL_HPF_3 (bit(MPU6050_ACCEL_HPF1)|bit(MPU6050_ACCEL_HPF0))
    #define MPU6050_ACCEL_HPF_4 (bit(MPU6050_ACCEL_HPF2))
    #define MPU6050_ACCEL_HPF_7 (bit(MPU6050_ACCEL_HPF2)|bit(MPU6050_ACCEL_HPF1)|bit(MPU6050_ACCEL_HPF0))
     
    // Alternative names for the combined definitions
    // The name uses the Cut-off frequency.
    #define MPU6050_ACCEL_HPF_RESET  MPU6050_ACCEL_HPF_0
    #define MPU6050_ACCEL_HPF_5HZ    MPU6050_ACCEL_HPF_1
    #define MPU6050_ACCEL_HPF_2_5HZ  MPU6050_ACCEL_HPF_2
    #define MPU6050_ACCEL_HPF_1_25HZ MPU6050_ACCEL_HPF_3
    #define MPU6050_ACCEL_HPF_0_63HZ MPU6050_ACCEL_HPF_4
    #define MPU6050_ACCEL_HPF_HOLD   MPU6050_ACCEL_HPF_7
     
    // Combined definitions for the AFS_SEL values
    #define MPU6050_AFS_SEL_0 (0)
    #define MPU6050_AFS_SEL_1 (bit(MPU6050_AFS_SEL0))
    #define MPU6050_AFS_SEL_2 (bit(MPU6050_AFS_SEL1))
    #define MPU6050_AFS_SEL_3 (bit(MPU6050_AFS_SEL1)|bit(MPU6050_AFS_SEL0))
     
    // Alternative names for the combined definitions
    // The name uses the full scale range for the accelerometer.
    #define MPU6050_AFS_SEL_2G  MPU6050_AFS_SEL_0
    #define MPU6050_AFS_SEL_4G  MPU6050_AFS_SEL_1
    #define MPU6050_AFS_SEL_8G  MPU6050_AFS_SEL_2
    #define MPU6050_AFS_SEL_16G MPU6050_AFS_SEL_3
     
    // FIFO_EN Register
    // These are the names for the bits.
    // Use these only with the bit() macro.
    #define MPU6050_SLV0_FIFO_EN  MPU6050_D0
    #define MPU6050_SLV1_FIFO_EN  MPU6050_D1
    #define MPU6050_SLV2_FIFO_EN  MPU6050_D2
    #define MPU6050_ACCEL_FIFO_EN MPU6050_D3
    #define MPU6050_ZG_FIFO_EN    MPU6050_D4
    #define MPU6050_YG_FIFO_EN    MPU6050_D5
    #define MPU6050_XG_FIFO_EN    MPU6050_D6
    #define MPU6050_TEMP_FIFO_EN  MPU6050_D7
     
    // I2C_MST_CTRL Register
    // These are the names for the bits.
    // Use these only with the bit() macro.
    #define MPU6050_I2C_MST_CLK0  MPU6050_D0
    #define MPU6050_I2C_MST_CLK1  MPU6050_D1
    #define MPU6050_I2C_MST_CLK2  MPU6050_D2
    #define MPU6050_I2C_MST_CLK3  MPU6050_D3
    #define MPU6050_I2C_MST_P_NSR MPU6050_D4
    #define MPU6050_SLV_3_FIFO_EN MPU6050_D5
    #define MPU6050_WAIT_FOR_ES   MPU6050_D6
    #define MPU6050_MULT_MST_EN   MPU6050_D7
     
    // Combined definitions for the I2C_MST_CLK
    #define MPU6050_I2C_MST_CLK_0 (0)
    #define MPU6050_I2C_MST_CLK_1  (bit(MPU6050_I2C_MST_CLK0))
    #define MPU6050_I2C_MST_CLK_2  (bit(MPU6050_I2C_MST_CLK1))
    #define MPU6050_I2C_MST_CLK_3  (bit(MPU6050_I2C_MST_CLK1)|bit(MPU6050_I2C_MST_CLK0))
    #define MPU6050_I2C_MST_CLK_4  (bit(MPU6050_I2C_MST_CLK2))
    #define MPU6050_I2C_MST_CLK_5  (bit(MPU6050_I2C_MST_CLK2)|bit(MPU6050_I2C_MST_CLK0))
    #define MPU6050_I2C_MST_CLK_6  (bit(MPU6050_I2C_MST_CLK2)|bit(MPU6050_I2C_MST_CLK1))
    #define MPU6050_I2C_MST_CLK_7  (bit(MPU6050_I2C_MST_CLK2)|bit(MPU6050_I2C_MST_CLK1)|bit(MPU6050_I2C_MST_CLK0))
    #define MPU6050_I2C_MST_CLK_8  (bit(MPU6050_I2C_MST_CLK3))
    #define MPU6050_I2C_MST_CLK_9  (bit(MPU6050_I2C_MST_CLK3)|bit(MPU6050_I2C_MST_CLK0))
    #define MPU6050_I2C_MST_CLK_10 (bit(MPU6050_I2C_MST_CLK3)|bit(MPU6050_I2C_MST_CLK1))
    #define MPU6050_I2C_MST_CLK_11 (bit(MPU6050_I2C_MST_CLK3)|bit(MPU6050_I2C_MST_CLK1)|bit(MPU6050_I2C_MST_CLK0))
    #define MPU6050_I2C_MST_CLK_12 (bit(MPU6050_I2C_MST_CLK3)|bit(MPU6050_I2C_MST_CLK2))
    #define MPU6050_I2C_MST_CLK_13 (bit(MPU6050_I2C_MST_CLK3)|bit(MPU6050_I2C_MST_CLK2)|bit(MPU6050_I2C_MST_CLK0))
    #define MPU6050_I2C_MST_CLK_14 (bit(MPU6050_I2C_MST_CLK3)|bit(MPU6050_I2C_MST_CLK2)|bit(MPU6050_I2C_MST_CLK1))
    #define MPU6050_I2C_MST_CLK_15 (bit(MPU6050_I2C_MST_CLK3)|bit(MPU6050_I2C_MST_CLK2)|bit(MPU6050_I2C_MST_CLK1)|bit(MPU6050_I2C_MST_CLK0))
     
    // Alternative names for the combined definitions
    // The names uses I2C Master Clock Speed in kHz.
    #define MPU6050_I2C_MST_CLK_348KHZ MPU6050_I2C_MST_CLK_0
    #define MPU6050_I2C_MST_CLK_333KHZ MPU6050_I2C_MST_CLK_1
    #define MPU6050_I2C_MST_CLK_320KHZ MPU6050_I2C_MST_CLK_2
    #define MPU6050_I2C_MST_CLK_308KHZ MPU6050_I2C_MST_CLK_3
    #define MPU6050_I2C_MST_CLK_296KHZ MPU6050_I2C_MST_CLK_4
    #define MPU6050_I2C_MST_CLK_286KHZ MPU6050_I2C_MST_CLK_5
    #define MPU6050_I2C_MST_CLK_276KHZ MPU6050_I2C_MST_CLK_6
    #define MPU6050_I2C_MST_CLK_267KHZ MPU6050_I2C_MST_CLK_7
    #define MPU6050_I2C_MST_CLK_258KHZ MPU6050_I2C_MST_CLK_8
    #define MPU6050_I2C_MST_CLK_500KHZ MPU6050_I2C_MST_CLK_9
    #define MPU6050_I2C_MST_CLK_471KHZ MPU6050_I2C_MST_CLK_10
    #define MPU6050_I2C_MST_CLK_444KHZ MPU6050_I2C_MST_CLK_11
    #define MPU6050_I2C_MST_CLK_421KHZ MPU6050_I2C_MST_CLK_12
    #define MPU6050_I2C_MST_CLK_400KHZ MPU6050_I2C_MST_CLK_13
    #define MPU6050_I2C_MST_CLK_381KHZ MPU6050_I2C_MST_CLK_14
    #define MPU6050_I2C_MST_CLK_364KHZ MPU6050_I2C_MST_CLK_15
     
    // I2C_SLV0_ADDR Register
    // These are the names for the bits.
    // Use these only with the bit() macro.
    #define MPU6050_I2C_SLV0_RW MPU6050_D7
     
    // I2C_SLV0_CTRL Register
    // These are the names for the bits.
    // Use these only with the bit() macro.
    #define MPU6050_I2C_SLV0_LEN0    MPU6050_D0
    #define MPU6050_I2C_SLV0_LEN1    MPU6050_D1
    #define MPU6050_I2C_SLV0_LEN2    MPU6050_D2
    #define MPU6050_I2C_SLV0_LEN3    MPU6050_D3
    #define MPU6050_I2C_SLV0_GRP     MPU6050_D4
    #define MPU6050_I2C_SLV0_REG_DIS MPU6050_D5
    #define MPU6050_I2C_SLV0_BYTE_SW MPU6050_D6
    #define MPU6050_I2C_SLV0_EN      MPU6050_D7
     
    // A mask for the length
    #define MPU6050_I2C_SLV0_LEN_MASK 0x0F
     
    // I2C_SLV1_ADDR Register
    // These are the names for the bits.
    // Use these only with the bit() macro.
    #define MPU6050_I2C_SLV1_RW MPU6050_D7
     
    // I2C_SLV1_CTRL Register
    // These are the names for the bits.
    // Use these only with the bit() macro.
    #define MPU6050_I2C_SLV1_LEN0    MPU6050_D0
    #define MPU6050_I2C_SLV1_LEN1    MPU6050_D1
    #define MPU6050_I2C_SLV1_LEN2    MPU6050_D2
    #define MPU6050_I2C_SLV1_LEN3    MPU6050_D3
    #define MPU6050_I2C_SLV1_GRP     MPU6050_D4
    #define MPU6050_I2C_SLV1_REG_DIS MPU6050_D5
    #define MPU6050_I2C_SLV1_BYTE_SW MPU6050_D6
    #define MPU6050_I2C_SLV1_EN      MPU6050_D7
     
    // A mask for the length
    #define MPU6050_I2C_SLV1_LEN_MASK 0x0F
     
    // I2C_SLV2_ADDR Register
    // These are the names for the bits.
    // Use these only with the bit() macro.
    #define MPU6050_I2C_SLV2_RW MPU6050_D7
     
    // I2C_SLV2_CTRL Register
    // These are the names for the bits.
    // Use these only with the bit() macro.
    #define MPU6050_I2C_SLV2_LEN0    MPU6050_D0
    #define MPU6050_I2C_SLV2_LEN1    MPU6050_D1
    #define MPU6050_I2C_SLV2_LEN2    MPU6050_D2
    #define MPU6050_I2C_SLV2_LEN3    MPU6050_D3
    #define MPU6050_I2C_SLV2_GRP     MPU6050_D4
    #define MPU6050_I2C_SLV2_REG_DIS MPU6050_D5
    #define MPU6050_I2C_SLV2_BYTE_SW MPU6050_D6
    #define MPU6050_I2C_SLV2_EN      MPU6050_D7
     
    // A mask for the length
    #define MPU6050_I2C_SLV2_LEN_MASK 0x0F
     
    // I2C_SLV3_ADDR Register
    // These are the names for the bits.
    // Use these only with the bit() macro.
    #define MPU6050_I2C_SLV3_RW MPU6050_D7
     
    // I2C_SLV3_CTRL Register
    // These are the names for the bits.
    // Use these only with the bit() macro.
    #define MPU6050_I2C_SLV3_LEN0    MPU6050_D0
    #define MPU6050_I2C_SLV3_LEN1    MPU6050_D1
    #define MPU6050_I2C_SLV3_LEN2    MPU6050_D2
    #define MPU6050_I2C_SLV3_LEN3    MPU6050_D3
    #define MPU6050_I2C_SLV3_GRP     MPU6050_D4
    #define MPU6050_I2C_SLV3_REG_DIS MPU6050_D5
    #define MPU6050_I2C_SLV3_BYTE_SW MPU6050_D6
    #define MPU6050_I2C_SLV3_EN      MPU6050_D7
     
    // A mask for the length
    #define MPU6050_I2C_SLV3_LEN_MASK 0x0F
     
    // I2C_SLV4_ADDR Register
    // These are the names for the bits.
    // Use these only with the bit() macro.
    #define MPU6050_I2C_SLV4_RW MPU6050_D7
     
    // I2C_SLV4_CTRL Register
    // These are the names for the bits.
    // Use these only with the bit() macro.
    #define MPU6050_I2C_MST_DLY0     MPU6050_D0
    #define MPU6050_I2C_MST_DLY1     MPU6050_D1
    #define MPU6050_I2C_MST_DLY2     MPU6050_D2
    #define MPU6050_I2C_MST_DLY3     MPU6050_D3
    #define MPU6050_I2C_MST_DLY4     MPU6050_D4
    #define MPU6050_I2C_SLV4_REG_DIS MPU6050_D5
    #define MPU6050_I2C_SLV4_INT_EN  MPU6050_D6
    #define MPU6050_I2C_SLV4_EN      MPU6050_D7
     
    // A mask for the delay
    #define MPU6050_I2C_MST_DLY_MASK 0x1F
     
    // I2C_MST_STATUS Register
    // These are the names for the bits.
    // Use these only with the bit() macro.
    #define MPU6050_I2C_SLV0_NACK MPU6050_D0
    #define MPU6050_I2C_SLV1_NACK MPU6050_D1
    #define MPU6050_I2C_SLV2_NACK MPU6050_D2
    #define MPU6050_I2C_SLV3_NACK MPU6050_D3
    #define MPU6050_I2C_SLV4_NACK MPU6050_D4
    #define MPU6050_I2C_LOST_ARB  MPU6050_D5
    #define MPU6050_I2C_SLV4_DONE MPU6050_D6
    #define MPU6050_PASS_THROUGH  MPU6050_D7
     
    // I2C_PIN_CFG Register
    // These are the names for the bits.
    // Use these only with the bit() macro.
    #define MPU6050_CLKOUT_EN       MPU6050_D0
    #define MPU6050_I2C_BYPASS_EN   MPU6050_D1
    #define MPU6050_FSYNC_INT_EN    MPU6050_D2
    #define MPU6050_FSYNC_INT_LEVEL MPU6050_D3
    #define MPU6050_INT_RD_CLEAR    MPU6050_D4
    #define MPU6050_LATCH_INT_EN    MPU6050_D5
    #define MPU6050_INT_OPEN        MPU6050_D6
    #define MPU6050_INT_LEVEL       MPU6050_D7
     
    // INT_ENABLE Register
    // These are the names for the bits.
    // Use these only with the bit() macro.
    #define MPU6050_DATA_RDY_EN    MPU6050_D0
    #define MPU6050_I2C_MST_INT_EN MPU6050_D3
    #define MPU6050_FIFO_OFLOW_EN  MPU6050_D4
    #define MPU6050_ZMOT_EN        MPU6050_D5
    #define MPU6050_MOT_EN         MPU6050_D6
    #define MPU6050_FF_EN          MPU6050_D7
     
    // INT_STATUS Register
    // These are the names for the bits.
    // Use these only with the bit() macro.
    #define MPU6050_DATA_RDY_INT   MPU6050_D0
    #define MPU6050_I2C_MST_INT    MPU6050_D3
    #define MPU6050_FIFO_OFLOW_INT MPU6050_D4
    #define MPU6050_ZMOT_INT       MPU6050_D5
    #define MPU6050_MOT_INT        MPU6050_D6
    #define MPU6050_FF_INT         MPU6050_D7
     
    // MOT_DETECT_STATUS Register
    // These are the names for the bits.
    // Use these only with the bit() macro.
    #define MPU6050_MOT_ZRMOT MPU6050_D0
    #define MPU6050_MOT_ZPOS  MPU6050_D2
    #define MPU6050_MOT_ZNEG  MPU6050_D3
    #define MPU6050_MOT_YPOS  MPU6050_D4
    #define MPU6050_MOT_YNEG  MPU6050_D5
    #define MPU6050_MOT_XPOS  MPU6050_D6
    #define MPU6050_MOT_XNEG  MPU6050_D7
     
    // IC2_MST_DELAY_CTRL Register
    // These are the names for the bits.
    // Use these only with the bit() macro.
    #define MPU6050_I2C_SLV0_DLY_EN MPU6050_D0
    #define MPU6050_I2C_SLV1_DLY_EN MPU6050_D1
    #define MPU6050_I2C_SLV2_DLY_EN MPU6050_D2
    #define MPU6050_I2C_SLV3_DLY_EN MPU6050_D3
    #define MPU6050_I2C_SLV4_DLY_EN MPU6050_D4
    #define MPU6050_DELAY_ES_SHADOW MPU6050_D7
     
    // SIGNAL_PATH_RESET Register
    // These are the names for the bits.
    // Use these only with the bit() macro.
    #define MPU6050_TEMP_RESET  MPU6050_D0
    #define MPU6050_ACCEL_RESET MPU6050_D1
    #define MPU6050_GYRO_RESET  MPU6050_D2
     
    // MOT_DETECT_CTRL Register
    // These are the names for the bits.
    // Use these only with the bit() macro.
    #define MPU6050_MOT_COUNT0      MPU6050_D0
    #define MPU6050_MOT_COUNT1      MPU6050_D1
    #define MPU6050_FF_COUNT0       MPU6050_D2
    #define MPU6050_FF_COUNT1       MPU6050_D3
    #define MPU6050_ACCEL_ON_DELAY0 MPU6050_D4
    #define MPU6050_ACCEL_ON_DELAY1 MPU6050_D5
     
    // Combined definitions for the MOT_COUNT
    #define MPU6050_MOT_COUNT_0 (0)
    #define MPU6050_MOT_COUNT_1 (bit(MPU6050_MOT_COUNT0))
    #define MPU6050_MOT_COUNT_2 (bit(MPU6050_MOT_COUNT1))
    #define MPU6050_MOT_COUNT_3 (bit(MPU6050_MOT_COUNT1)|bit(MPU6050_MOT_COUNT0))
     
    // Alternative names for the combined definitions
    #define MPU6050_MOT_COUNT_RESET MPU6050_MOT_COUNT_0
     
    // Combined definitions for the FF_COUNT
    #define MPU6050_FF_COUNT_0 (0)
    #define MPU6050_FF_COUNT_1 (bit(MPU6050_FF_COUNT0))
    #define MPU6050_FF_COUNT_2 (bit(MPU6050_FF_COUNT1))
    #define MPU6050_FF_COUNT_3 (bit(MPU6050_FF_COUNT1)|bit(MPU6050_FF_COUNT0))
     
    // Alternative names for the combined definitions
    #define MPU6050_FF_COUNT_RESET MPU6050_FF_COUNT_0
     
    // Combined definitions for the ACCEL_ON_DELAY
    #define MPU6050_ACCEL_ON_DELAY_0 (0)
    #define MPU6050_ACCEL_ON_DELAY_1 (bit(MPU6050_ACCEL_ON_DELAY0))
    #define MPU6050_ACCEL_ON_DELAY_2 (bit(MPU6050_ACCEL_ON_DELAY1))
    #define MPU6050_ACCEL_ON_DELAY_3 (bit(MPU6050_ACCEL_ON_DELAY1)|bit(MPU6050_ACCEL_ON_DELAY0))
     
    // Alternative names for the ACCEL_ON_DELAY
    #define MPU6050_ACCEL_ON_DELAY_0MS MPU6050_ACCEL_ON_DELAY_0
    #define MPU6050_ACCEL_ON_DELAY_1MS MPU6050_ACCEL_ON_DELAY_1
    #define MPU6050_ACCEL_ON_DELAY_2MS MPU6050_ACCEL_ON_DELAY_2
    #define MPU6050_ACCEL_ON_DELAY_3MS MPU6050_ACCEL_ON_DELAY_3
     
    // USER_CTRL Register
    // These are the names for the bits.
    // Use these only with the bit() macro.
    #define MPU6050_SIG_COND_RESET MPU6050_D0
    #define MPU6050_I2C_MST_RESET  MPU6050_D1
    #define MPU6050_FIFO_RESET     MPU6050_D2
    #define MPU6050_I2C_IF_DIS     MPU6050_D4   // must be 0 for MPU-6050
    #define MPU6050_I2C_MST_EN     MPU6050_D5
    #define MPU6050_FIFO_EN        MPU6050_D6
     
    // PWR_MGMT_1 Register
    // These are the names for the bits.
    // Use these only with the bit() macro.
    #define MPU6050_CLKSEL0      MPU6050_D0
    #define MPU6050_CLKSEL1      MPU6050_D1
    #define MPU6050_CLKSEL2      MPU6050_D2
    #define MPU6050_TEMP_DIS     MPU6050_D3    // 1: disable temperature sensor
    #define MPU6050_CYCLE        MPU6050_D5    // 1: sample and sleep
    #define MPU6050_SLEEP        MPU6050_D6    // 1: sleep mode
    #define MPU6050_DEVICE_RESET MPU6050_D7    // 1: reset to default values
     
    // Combined definitions for the CLKSEL
    #define MPU6050_CLKSEL_0 (0)
    #define MPU6050_CLKSEL_1 (bit(MPU6050_CLKSEL0))
    #define MPU6050_CLKSEL_2 (bit(MPU6050_CLKSEL1))
    #define MPU6050_CLKSEL_3 (bit(MPU6050_CLKSEL1)|bit(MPU6050_CLKSEL0))
    #define MPU6050_CLKSEL_4 (bit(MPU6050_CLKSEL2))
    #define MPU6050_CLKSEL_5 (bit(MPU6050_CLKSEL2)|bit(MPU6050_CLKSEL0))
    #define MPU6050_CLKSEL_6 (bit(MPU6050_CLKSEL2)|bit(MPU6050_CLKSEL1))
    #define MPU6050_CLKSEL_7 (bit(MPU6050_CLKSEL2)|bit(MPU6050_CLKSEL1)|bit(MPU6050_CLKSEL0))
     
    // Alternative names for the combined definitions
    #define MPU6050_CLKSEL_INTERNAL    MPU6050_CLKSEL_0
    #define MPU6050_CLKSEL_X           MPU6050_CLKSEL_1
    #define MPU6050_CLKSEL_Y           MPU6050_CLKSEL_2
    #define MPU6050_CLKSEL_Z           MPU6050_CLKSEL_3
    #define MPU6050_CLKSEL_EXT_32KHZ   MPU6050_CLKSEL_4
    #define MPU6050_CLKSEL_EXT_19_2MHZ MPU6050_CLKSEL_5
    #define MPU6050_CLKSEL_RESERVED    MPU6050_CLKSEL_6
    #define MPU6050_CLKSEL_STOP        MPU6050_CLKSEL_7
     
    // PWR_MGMT_2 Register
    // These are the names for the bits.
    // Use these only with the bit() macro.
    #define MPU6050_STBY_ZG       MPU6050_D0
    #define MPU6050_STBY_YG       MPU6050_D1
    #define MPU6050_STBY_XG       MPU6050_D2
    #define MPU6050_STBY_ZA       MPU6050_D3
    #define MPU6050_STBY_YA       MPU6050_D4
    #define MPU6050_STBY_XA       MPU6050_D5
    #define MPU6050_LP_WAKE_CTRL0 MPU6050_D6
    #define MPU6050_LP_WAKE_CTRL1 MPU6050_D7
     
    // Combined definitions for the LP_WAKE_CTRL
    #define MPU6050_LP_WAKE_CTRL_0 (0)
    #define MPU6050_LP_WAKE_CTRL_1 (bit(MPU6050_LP_WAKE_CTRL0))
    #define MPU6050_LP_WAKE_CTRL_2 (bit(MPU6050_LP_WAKE_CTRL1))
    #define MPU6050_LP_WAKE_CTRL_3 (bit(MPU6050_LP_WAKE_CTRL1)|bit(MPU6050_LP_WAKE_CTRL0))
     
    // Alternative names for the combined definitions
    // The names uses the Wake-up Frequency.
    #define MPU6050_LP_WAKE_1_25HZ MPU6050_LP_WAKE_CTRL_0
    #define MPU6050_LP_WAKE_2_5HZ  MPU6050_LP_WAKE_CTRL_1
    #define MPU6050_LP_WAKE_5HZ    MPU6050_LP_WAKE_CTRL_2
    #define MPU6050_LP_WAKE_10HZ   MPU6050_LP_WAKE_CTRL_3
     

    // Default I2C address for the MPU-6050 is 0x68.
    // But only if the AD0 pin is low.
    // Some sensor boards have AD0 high, and the
    // I2C address thus becomes 0x69.
    #define MPU6050_I2C_ADDRESS 0x68
     

    // --------------------------------------------------------
    // MPU6050_read
    //
    // This is a common function to read multiple bytes
    // from an I2C device.
    //
    // It uses the boolean parameter for Wire.endTransMission()
    // to be able to hold or release the I2C-bus.
    // This is implemented in Arduino 1.0.1.
    //
    // Only this function is used to read.
    // There is no function for a single byte.
    //

    int MPU6050_read(unsigned long ulI2CBase,
								unsigned char ucSlaveAdress,
								unsigned char ucReg,
								char* cReadData,
								unsigned int uiSize)
    {

      int i, n, error;
error=    I2CReadData(ulI2CBase,ucSlaveAdress,ucReg,cReadData,uiSize);


    return (error);  // return : no error
    }


    // --------------------------------------------------------
    // MPU6050_write
    //
    // This is a common function to write multiple bytes to an I2C device.
    //
    // If only a single register is written,
    // use the function MPU_6050_write_reg().
    //
    // Parameters:
    //   start : Start address, use a define for the register
    //   pData : A pointer to the data to write.
    //   size  : The number of bytes to write.
    //
    // If only a single register is written, a pointer
    // to the data has to be used, and the size is
    // a single byte:
    //   int data = 0;        // the data to write
    //   MPU6050_write (MPU6050_PWR_MGMT_1, &c, 1);
    //
    int MPU6050_write(unsigned long ulI2CBase,unsigned char ucReg, char* pData, unsigned int size)
    {

     return I2CRegWrite(ulI2CBase,MPU6050_I2C_ADDRESS,ucReg,pData,size);

    }

    // --------------------------------------------------------
    // MPU6050_write_reg
    //
    // An extra function to write a single register.
    // It is just a wrapper around the MPU_6050_write()
    // function, and it is only a convenient function
    // to make it easier to write a single register.
    //
    int MPU6050_write_reg(unsigned long ulI2CBase, unsigned char ucSlaveAdress,
		 unsigned char ucReg, unsigned char ucValue)
    {
      int error;

      error = I2CRegWrite(ulI2CBase,ucSlaveAdress, ucReg,ucValue);

      return (error);
    }

