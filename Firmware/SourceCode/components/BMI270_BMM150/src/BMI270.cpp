/**
 * @file BMI270.cpp
 * @author Forairaaaaa
 * @brief Port to IDF
 * @version 0.1
 * @date 2023-05-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "BMI270.h"
#include <esp_timer.h>
#include <esp_log.h>
#include <cstring>


#define NOP() asm volatile("nop")
#define TAG "IMU"
#define bmi2_error_codes_print_result(rslt) print_rslt(rslt)


static bmi2_dev* _bmi2_dev_ptr = nullptr;


namespace BMI270 {

    int BMI270::init(i2c_port_t i2c_port)
    {
        _i2c_port = i2c_port;


        /* BMI270 init */
        bmi2.chip_id = BMI2_I2C_PRIM_ADDR;
        bmi2.read = bmi2_i2c_read;
        bmi2.write = bmi2_i2c_write;
        bmi2.delay_us = bmi2_delay_us;
        bmi2.intf = BMI2_I2C_INTF;
        bmi2.intf_ptr = &accel_gyro_dev_info;
        bmi2.read_write_len = 30;    // Limitation of the Wire library
        bmi2.config_file_ptr = NULL; // Use the default BMI270 config file

        accel_gyro_dev_info.i2c_port = _i2c_port;
        accel_gyro_dev_info.dev_addr = bmi2.chip_id;

        int8_t rslt = bmi270_init(&bmi2);
        print_rslt(rslt);
        rslt = configure_sensor(&bmi2);
        print_rslt(rslt);


        // /* BMM150 init */
        // bmm1.chip_id = BMM150_DEFAULT_I2C_ADDRESS;
        // bmm1.read = bmi2_i2c_read;
        // bmm1.write = bmi2_i2c_write;
        // bmm1.delay_us = bmi2_delay_us;
        // bmm1.intf = BMM150_I2C_INTF;
        // bmm1.intf_ptr = &mag_dev_info;

        // mag_dev_info.i2c_port = _i2c_port;
        // mag_dev_info.dev_addr = bmm1.chip_id;

        // rslt = bmm150_init(&bmm1);
        // print_rslt(rslt);
        // rslt = configure_sensor(&bmm1);
        // print_rslt(rslt);


        // /* BMI270 Aux init */
        // bmi2_sens_config config;
        // config.type = BMI2_AUX;

        // /* Get config */
        // rslt = bmi270_get_sensor_config(&config, 1, &bmi2);
        // bmi2_error_codes_print_result(rslt);

        // /* Rewrite */
        // config.cfg.aux.odr = BMI2_AUX_ODR_50HZ;
        // config.cfg.aux.aux_en = BMI2_ENABLE;
        // config.cfg.aux.i2c_device_addr = BMM150_DEFAULT_I2C_ADDRESS;
        // config.cfg.aux.fcu_write_en = BMI2_ENABLE;
        // config.cfg.aux.man_rd_burst = BMI2_AUX_READ_LEN_3;
        // config.cfg.aux.read_addr = BMM150_REG_DATA_X_LSB;
        // // config.cfg.aux.manual_en = BMI2_ENABLE;
        // config.cfg.aux.manual_en = BMI2_DISABLE;

        // /* Set back */
        // rslt = bmi270_set_sensor_config(&config, 1, &bmi2);
        // bmi2_error_codes_print_result(rslt);


        // /* BMM150 init */
        // bmm1.chip_id = BMM150_DEFAULT_I2C_ADDRESS;
        // bmm1.read = aux_i2c_read;
        // bmm1.write = aux_i2c_write;
        // bmm1.delay_us = bmi2_delay_us;
        // bmm1.intf = BMM150_I2C_INTF;
        // bmm1.intf_ptr = &mag_dev_info;

        // mag_dev_info.i2c_port = _i2c_port;
        // mag_dev_info.dev_addr = bmm1.chip_id;

        // rslt = bmm150_init(&bmm1);
        // print_rslt(rslt);
        // rslt = configure_sensor(&bmm1);
        // print_rslt(rslt);


        // /* Map data ready interrupt to interrupt pin. */
        // rslt = bmi2_map_data_int(BMI2_DRDY_INT, BMI2_INT1, &bmi2);
        // bmi2_error_codes_print_result(rslt);







        _bmi2_dev_ptr = &bmi2;
        _initialized = true;
        return 1;
    }


    void BMI270::setContinuousMode()
    {
        bmi2_set_fifo_config(BMI2_FIFO_GYR_EN | BMI2_FIFO_ACC_EN, 1, &bmi2);
        continuousMode = true;
    }


    void BMI270::oneShotMode()
    {
        bmi2_set_fifo_config(BMI2_FIFO_GYR_EN | BMI2_FIFO_ACC_EN, 0, &bmi2);
        continuousMode = false;
    }


    // default range is +-4G, so conversion factor is (((1 << 15)/4.0f))
    #define INT16_to_G (8192.0f)


    // Accelerometer
    int BMI270::readAcceleration(float &x, float &y, float &z)
    {
        struct bmi2_sens_data sensor_data;
        auto ret = bmi2_get_sensor_data(&sensor_data, &bmi2);

        x = sensor_data.acc.x / INT16_to_G;
        y = sensor_data.acc.y / INT16_to_G;
        z = sensor_data.acc.z / INT16_to_G;
        return (ret == 0);
    }


    int BMI270::accelerationAvailable()
    {
        uint16_t status;
        bmi2_get_int_status(&status, &bmi2);
        int ret = ((status | _int_status) & BMI2_ACC_DRDY_INT_MASK);
        _int_status = status;
        _int_status &= ~BMI2_ACC_DRDY_INT_MASK;
        return ret;
    }


    float BMI270::accelerationSampleRate()
    {
        struct bmi2_sens_config sens_cfg;
        sens_cfg.type = BMI2_ACCEL;
        bmi2_get_sensor_config(&sens_cfg, 1, &bmi2);
        return (1 << sens_cfg.cfg.acc.odr) * 0.39;
    }


    // default range is +-2000dps, so conversion factor is (((1 << 15)/4.0f))
    #define INT16_to_DPS (16.384f)


    // Gyroscope
    int BMI270::readGyroscope(float &x, float &y, float &z)
    {
        struct bmi2_sens_data sensor_data;
        auto ret = bmi2_get_sensor_data(&sensor_data, &bmi2);

        x = sensor_data.gyr.x / INT16_to_DPS;
        y = sensor_data.gyr.y / INT16_to_DPS;
        z = sensor_data.gyr.z / INT16_to_DPS;
        return (ret == 0);
    }


    int BMI270::gyroscopeAvailable()
    {
        uint16_t status;
        bmi2_get_int_status(&status, &bmi2);
        int ret = ((status | _int_status) & BMI2_GYR_DRDY_INT_MASK);
        _int_status = status;
        _int_status &= ~BMI2_GYR_DRDY_INT_MASK;
        return ret;
    }


    float BMI270::gyroscopeSampleRate()
    {
        struct bmi2_sens_config sens_cfg;
        sens_cfg.type = BMI2_GYRO;
        bmi2_get_sensor_config(&sens_cfg, 1, &bmi2);
        return (1 << sens_cfg.cfg.gyr.odr) * 0.39;
    }


    // Magnetometer
    int BMI270::readMagneticField(float &x, float &y, float &z)
    {
        struct bmm150_mag_data mag_data;
        int const rc = bmm150_read_mag_data(&mag_data, &bmm1);
        x = mag_data.x;
        y = mag_data.y;
        z = mag_data.z;

        if (rc == BMM150_OK)
            return 1;
        else
            return 0;
    }


    int BMI270::magneticFieldAvailable()
    {
        bmm150_get_interrupt_status(&bmm1);
        return bmm1.int_status & BMM150_INT_ASSERTED_DRDY;
    }


    float BMI270::magneticFieldSampleRate()
    {
        struct bmm150_settings settings;
        bmm150_get_sensor_settings(&settings, &bmm1);
        switch (settings.data_rate)
        {
        case BMM150_DATA_RATE_10HZ:
            return 10;
        case BMM150_DATA_RATE_02HZ:
            return 2;
        case BMM150_DATA_RATE_06HZ:
            return 6;
        case BMM150_DATA_RATE_08HZ:
            return 8;
        case BMM150_DATA_RATE_15HZ:
            return 15;
        case BMM150_DATA_RATE_20HZ:
            return 20;
        case BMM150_DATA_RATE_25HZ:
            return 25;
        case BMM150_DATA_RATE_30HZ:
            return 30;
        }
        return 0;
    }


    int8_t BMI270::configure_sensor(struct bmi2_dev *dev)
    {
        int8_t rslt;
        uint8_t sens_list[2] = {BMI2_ACCEL, BMI2_GYRO};

        // struct bmi2_int_pin_config int_pin_cfg;
        // int_pin_cfg.pin_type = BMI2_INT1;
        // int_pin_cfg.int_latch = BMI2_INT_NON_LATCH;
        // int_pin_cfg.pin_cfg[0].lvl = BMI2_INT_ACTIVE_HIGH;
        // int_pin_cfg.pin_cfg[0].od = BMI2_INT_PUSH_PULL;
        // int_pin_cfg.pin_cfg[0].output_en = BMI2_INT_OUTPUT_ENABLE;
        // int_pin_cfg.pin_cfg[0].input_en = BMI2_INT_INPUT_DISABLE;

        struct bmi2_sens_config sens_cfg[2];
        sens_cfg[0].type = BMI2_ACCEL;
        sens_cfg[0].cfg.acc.bwp = BMI2_ACC_OSR2_AVG2;
        sens_cfg[0].cfg.acc.odr = BMI2_ACC_ODR_50HZ;
        sens_cfg[0].cfg.acc.filter_perf = BMI2_PERF_OPT_MODE;
        sens_cfg[0].cfg.acc.range = BMI2_ACC_RANGE_4G;
        sens_cfg[1].type = BMI2_GYRO;
        sens_cfg[1].cfg.gyr.filter_perf = BMI2_PERF_OPT_MODE;
        sens_cfg[1].cfg.gyr.bwp = BMI2_GYR_OSR2_MODE;
        sens_cfg[1].cfg.gyr.odr = BMI2_ACC_ODR_50HZ;
        sens_cfg[1].cfg.gyr.range = BMI2_GYR_RANGE_2000;
        sens_cfg[1].cfg.gyr.ois_range = BMI2_GYR_OIS_2000;

        // rslt = bmi2_set_int_pin_config(&int_pin_cfg, dev);
        // if (rslt != BMI2_OK)
        //     return rslt;

        rslt = bmi2_map_data_int(BMI2_DRDY_INT, BMI2_INT1, dev);
        if (rslt != BMI2_OK)
            return rslt;

        rslt = bmi2_set_sensor_config(sens_cfg, 2, dev);
        if (rslt != BMI2_OK)
            return rslt;

        rslt = bmi2_sensor_enable(sens_list, 2, dev);
        if (rslt != BMI2_OK)
            return rslt;

        return rslt;
    }


    int8_t BMI270::configure_sensor(struct bmm150_dev *dev)
    {
        /* Status of api are returned to this variable. */
        int8_t rslt;
        struct bmm150_settings settings;

        /* Set powermode as normal mode */
        settings.pwr_mode = BMM150_POWERMODE_NORMAL;
        rslt = bmm150_set_op_mode(&settings, dev);

        if (rslt == BMM150_OK)
        {
            /* Setting the preset mode as Low power mode
            * i.e. data rate = 10Hz, XY-rep = 1, Z-rep = 2
            */
            settings.preset_mode = BMM150_PRESETMODE_REGULAR;
            rslt = bmm150_set_presetmode(&settings, dev);

            if (rslt == BMM150_OK)
            {
                /* Map the data interrupt pin */
                settings.int_settings.drdy_pin_en = 0x01;
                rslt = bmm150_set_sensor_settings(BMM150_SEL_DRDY_PIN_EN, &settings, dev);
            }
        }
        return rslt;
    }


    void BMI270::setWristWearWakeup()
    {
        /* Variable to define result */
        int8_t rslt;

        /* List the sensors which are required to enable */
        // uint8_t sens_list[2] = { BMI2_ACCEL, BMI2_WRIST_GESTURE };
        uint8_t sens_list[2] = { BMI2_ACCEL, BMI2_WRIST_WEAR_WAKE_UP };

        /* Structure to define the type of the sensor and its configurations */
        struct bmi2_sens_config config;

        /* Interrupt pin configuration */
        // struct bmi2_int_pin_config pin_config = { 0 };
        struct bmi2_int_pin_config pin_config;
        memset((void*)&pin_config, 0, sizeof(bmi2_int_pin_config));

        /* Configure type of feature */
        // config.type = BMI2_WRIST_GESTURE;
        config.type = BMI2_WRIST_WEAR_WAKE_UP;

        /* Enable the selected sensors */
        rslt = bmi270_sensor_enable(sens_list, 2, &bmi2);
        bmi2_error_codes_print_result(rslt);

        rslt = bmi2_get_int_pin_config(&pin_config, &bmi2);
        bmi2_error_codes_print_result(rslt);

        if (rslt == BMI2_OK)
        {
            /* Get default configurations for the type of feature selected */
            rslt = bmi270_get_sensor_config(&config, 1, &bmi2);
            bmi2_error_codes_print_result(rslt);

            if (rslt == BMI2_OK)
            {
                config.cfg.wrist_gest.wearable_arm = BMI2_ARM_RIGHT;

                /* Set the new configuration along with interrupt mapping */
                rslt = bmi270_set_sensor_config(&config, 1, &bmi2);
                bmi2_error_codes_print_result(rslt);

                /* Interrupt pin configuration */
                pin_config.pin_type = BMI2_INT2;
                pin_config.pin_cfg[1].input_en = BMI2_INT_INPUT_DISABLE;
                pin_config.pin_cfg[1].lvl = BMI2_INT_ACTIVE_LOW;
                pin_config.pin_cfg[1].od = BMI2_INT_PUSH_PULL;
                pin_config.pin_cfg[1].output_en = BMI2_INT_OUTPUT_ENABLE;
                pin_config.int_latch = BMI2_INT_NON_LATCH;

                rslt = bmi2_set_int_pin_config(&pin_config, &bmi2);
                bmi2_error_codes_print_result(rslt);
            }
        }

        if (rslt == BMI2_OK)
        {
            /* Map the feature interrupt */
            struct bmi2_sens_int_config sens_int;
            // sens_int.type = BMI2_WRIST_GESTURE;
            sens_int.type = BMI2_WRIST_WEAR_WAKE_UP;
            sens_int.hw_int_pin = BMI2_INT2;
            rslt = bmi270_map_feat_int(&sens_int, 1, &bmi2);
            bmi2_error_codes_print_result(rslt);
        }






        // // /* Initialize status of wrist gesture interrupt */
        // uint16_t int_status = 0;
        // /* The gesture movements are listed in array */
        // const char *gesture_output[6] =
        // { "unknown_gesture", "push_arm_down", "pivot_up", "wrist_shake_jiggle", "flick_in", "flick_out" };
        // /* Sensor data structure */
        // struct bmi2_feat_sensor_data sens_data = { 0 };
        // sens_data.type = BMI2_WRIST_GESTURE;




        // gpio_reset_pin(GPIO_NUM_40);
        // gpio_set_direction(GPIO_NUM_40, GPIO_MODE_INPUT);
        // gpio_set_pull_mode(GPIO_NUM_40, GPIO_FLOATING);

        // gpio_reset_pin(GPIO_NUM_38);
        // gpio_set_direction(GPIO_NUM_38, GPIO_MODE_INPUT);
        // gpio_set_pull_mode(GPIO_NUM_38, GPIO_FLOATING);




        // /* Loop to print the wrist gesture data when interrupt occurs */
        // for (;;)
        // {

        //     /* Get the interrupt status of the wrist gesture */
        //     rslt = bmi2_get_int_status(&int_status, &bmi2);
        //     bmi2_error_codes_print_result(rslt);

        //     // if ((rslt == BMI2_OK) && (int_status & BMI270_WRIST_GEST_STATUS_MASK))
        //     // {
        //     //     printf("Wrist gesture detected\n");

        //     //     /* Get wrist gesture output */
        //     //     rslt = bmi270_get_feature_data(&sens_data, 1, &bmi2);
        //     //     bmi2_error_codes_print_result(rslt);

        //     //     printf("Wrist gesture = %d\r\n", sens_data.sens_data.wrist_gesture_output);

        //     //     printf("Gesture output = %s\n", gesture_output[sens_data.sens_data.wrist_gesture_output]);
                
        //     //     vTaskDelay(1000);
        //     //     continue;
        //     // }

        //     /* To check the interrupt status of the wrist gesture */
        //     if ((rslt == BMI2_OK) && (int_status & BMI270_WRIST_WAKE_UP_STATUS_MASK))
        //     {
        //         printf("Wrist wear wakeup detected\n");
                
        //         vTaskDelay(1000);
        //         continue;
        //     }


        //     printf("%d %d\n", gpio_get_level(GPIO_NUM_38), gpio_get_level(GPIO_NUM_40));
        //     // vTaskDelay(10);

        // }





    }




    int BMI270::getGesture()
    {
        /* Variable to define result */
        int8_t rslt;
        /* Initialize status of wrist gesture interrupt */
        uint16_t int_status = 0;
        /* Sensor data structure */
        // struct bmi2_feat_sensor_data sens_data = {0};
        struct bmi2_feat_sensor_data sens_data;
        memset((void*)&sens_data, 0, sizeof(bmi2_feat_sensor_data));

        sens_data.type = BMI2_WRIST_GESTURE;

        /* Get the interrupt status of the wrist gesture */
        rslt = bmi2_get_int_status(&int_status, &bmi2);
        bmi2_error_codes_print_result(rslt);

        if ((rslt == BMI2_OK) && (int_status & BMI270_WRIST_GEST_STATUS_MASK)) {
            /* Get wrist gesture output */
            rslt = bmi270_get_feature_data(&sens_data, 1, &bmi2);
            bmi2_error_codes_print_result(rslt);

            // printf("Wrist gesture = %d\r\n", sens_data.sens_data.wrist_gesture_output);
            return sens_data.sens_data.wrist_gesture_output;
        }
        return -1;
    }


    std::string BMI270::getGestureName(uint8_t gesture)
    {
        if (gesture > 5) {
            return "error";
        }

        const char *gesture_output[6] =
        { "unknown_gesture", "push_arm_down", "pivot_up", "wrist_shake_jiggle", "flick_in", "flick_out" };

        return gesture_output[gesture];
    }


    void BMI270::enableStepCounter()
    {
        /* Status of api are returned to this variable. */
        int8_t rslt;

        /* Accel sensor and step counter feature are listed in array. */
        uint8_t sensor_sel[2] = { BMI2_ACCEL, BMI2_STEP_COUNTER };

        /* Enable the selected sensor. */
        rslt = bmi270_sensor_enable(sensor_sel, 2, &bmi2);
        bmi2_error_codes_print_result(rslt);

    }


    uint32_t BMI270::getSteps()
    {
        /* Status of api are returned to this variable. */
        int8_t rslt;

        /* Structure to define type of sensor and their respective data. */
        struct bmi2_feat_sensor_data sensor_data;
        memset((void*)&sensor_data, 0, sizeof(bmi2_feat_sensor_data));

        /* Type of sensor to get step counter data. */
        sensor_data.type = BMI2_STEP_COUNTER;

        /* Get step counter output. */
        rslt = bmi270_get_feature_data(&sensor_data, 1, &bmi2);
        bmi2_error_codes_print_result(rslt);

        // /* Print the step counter output. */
        // printf("No of steps counted  = %lu\n",
        //         (long unsigned int)sensor_data.sens_data.step_counter_output);
            
        return sensor_data.sens_data.step_counter_output;
    }






    /* I2C W/R buffer */
    static uint8_t _data_buffer[32];
    static esp_err_t _i2c_ret;
    struct dev_info* _dev_info;


    int8_t BMI270::bmi2_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr)
    {
        if ((reg_data == NULL) || (len == 0) || (len > 32)) {
            return -1;
        }

        _dev_info = (struct dev_info*)intf_ptr;

        _i2c_ret = i2c_master_write_read_device(_dev_info->i2c_port, _dev_info->dev_addr, &reg_addr, 1, reg_data, len, portMAX_DELAY);
        if (_i2c_ret != ESP_OK) {
            ESP_LOGE(TAG, "P:%d R %02X %02X E:%d", _dev_info->i2c_port, _dev_info->dev_addr, reg_addr, _i2c_ret);
            return -1;
        }

        return 0;
    }


    int8_t BMI270::bmi2_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr)
    {
        if ((reg_data == NULL) || (len == 0) || (len > 32)) {
            return -1;
        }

        _dev_info = (struct dev_info *)intf_ptr;

        _data_buffer[0] = reg_addr;
        for (int i = 0; i < len; i++) {
            _data_buffer[i + 1] = reg_data[i];
        }

        _i2c_ret = i2c_master_write_to_device(_dev_info->i2c_port, _dev_info->dev_addr, _data_buffer, (len + 1), portMAX_DELAY);
        if (_i2c_ret != ESP_OK) {
            ESP_LOGE(TAG, "P:%d W %02X %02X E:%d", _dev_info->i2c_port, _dev_info->dev_addr, reg_addr, _i2c_ret);
            return -1;
        }

        return 0;
    }


    void BMI270::bmi2_delay_us(uint32_t period, void *intf_ptr)
    {
        /* Copy from Arduino */
        uint64_t m = (uint64_t)esp_timer_get_time();
        if (period) {
            uint64_t e = (m + period);
            if (m > e) { // overflow
                while ((uint64_t)esp_timer_get_time() > e) {
                    NOP();
                }
            }
            while ((uint64_t)esp_timer_get_time() < e) {
                NOP();
            }
        }
    }





    int8_t BMI270::aux_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr)
    {
        return bmi2_read_aux_man_mode(reg_addr, reg_data, length, _bmi2_dev_ptr);
    }


    int8_t BMI270::aux_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr)
    {
        return bmi2_write_aux_man_mode(reg_addr, reg_data, length, _bmi2_dev_ptr);
    }



    void BMI270::print_rslt(int8_t rslt)
    {
        switch (rslt) {
            case BMI2_OK:
                return; /* Do nothing */
                break;
            case BMI2_E_NULL_PTR:
                ESP_LOGE(TAG, "Error [%d] : Null pointer", rslt);
                break;
            case BMI2_E_COM_FAIL:
                ESP_LOGE(TAG, "Error [%d] : Communication failure", rslt);
                break;
            case BMI2_E_DEV_NOT_FOUND:
                ESP_LOGE(TAG, "Error [%d] : Device not found", rslt);
                break;
            case BMI2_E_OUT_OF_RANGE:
                ESP_LOGE(TAG, "Error [%d] : Out of range", rslt);
                break;
            case BMI2_E_ACC_INVALID_CFG:
                ESP_LOGE(TAG, "Error [%d] : Invalid accel configuration", rslt);
                break;
            case BMI2_E_GYRO_INVALID_CFG:
                ESP_LOGE(TAG, "Error [%d] : Invalid gyro configuration", rslt);
                break;
            case BMI2_E_ACC_GYR_INVALID_CFG:
                ESP_LOGE(TAG, "Error [%d] : Invalid accel/gyro configuration", rslt);
                break;
            case BMI2_E_INVALID_SENSOR:
                ESP_LOGE(TAG, "Error [%d] : Invalid sensor", rslt);
                break;
            case BMI2_E_CONFIG_LOAD:
                ESP_LOGE(TAG, "Error [%d] : Configuration loading error", rslt);
                break;
            case BMI2_E_INVALID_PAGE:
                ESP_LOGE(TAG, "Error [%d] : Invalid page", rslt);
                break;
            case BMI2_E_INVALID_FEAT_BIT:
                ESP_LOGE(TAG, "Error [%d] : Invalid feature bit", rslt);
                break;
            case BMI2_E_INVALID_INT_PIN:
                ESP_LOGE(TAG, "Error [%d] : Invalid interrupt pin", rslt);
                break;
            case BMI2_E_SET_APS_FAIL:
                ESP_LOGE(TAG, "Error [%d] : Setting advanced power mode failed", rslt);
                break;
            case BMI2_E_AUX_INVALID_CFG:
                ESP_LOGE(TAG, "Error [%d] : Invalid auxiliary configuration", rslt);
                break;
            case BMI2_E_AUX_BUSY:
                ESP_LOGE(TAG, "Error [%d] : Auxiliary busy", rslt);
                break;
            case BMI2_E_SELF_TEST_FAIL:
                ESP_LOGE(TAG, "Error [%d] : Self test failed", rslt);
                break;
            case BMI2_E_REMAP_ERROR:
                ESP_LOGE(TAG, "Error [%d] : Remapping error", rslt);
                break;
            case BMI2_E_GYR_USER_GAIN_UPD_FAIL:
                ESP_LOGE(TAG, "Error [%d] : Gyro user gain update failed", rslt);
                break;
            case BMI2_E_SELF_TEST_NOT_DONE:
                ESP_LOGE(TAG, "Error [%d] : Self test not done", rslt);
                break;
            case BMI2_E_INVALID_INPUT:
                ESP_LOGE(TAG, "Error [%d] : Invalid input", rslt);
                break;
            case BMI2_E_INVALID_STATUS:
                ESP_LOGE(TAG, "Error [%d] : Invalid status", rslt);
                break;
            case BMI2_E_CRT_ERROR:
                ESP_LOGE(TAG, "Error [%d] : CRT error", rslt);
                break;
            case BMI2_E_ST_ALREADY_RUNNING:
                ESP_LOGE(TAG, "Error [%d] : Self test already running", rslt);
                break;
            case BMI2_E_CRT_READY_FOR_DL_FAIL_ABORT:
                ESP_LOGE(TAG, "Error [%d] : CRT ready for DL fail abort", rslt);
                break;
            case BMI2_E_DL_ERROR:
                ESP_LOGE(TAG, "Error [%d] : DL error", rslt);
                break;
            case BMI2_E_PRECON_ERROR:
                ESP_LOGE(TAG, "Error [%d] : PRECON error", rslt);
                break;
            case BMI2_E_ABORT_ERROR:
                ESP_LOGE(TAG, "Error [%d] : Abort error", rslt);
                break;
            case BMI2_E_GYRO_SELF_TEST_ERROR:
                ESP_LOGE(TAG, "Error [%d] : Gyro self test error", rslt);
                break;
            case BMI2_E_GYRO_SELF_TEST_TIMEOUT:
                ESP_LOGE(TAG, "Error [%d] : Gyro self test timeout", rslt);
                break;
            case BMI2_E_WRITE_CYCLE_ONGOING:
                ESP_LOGE(TAG, "Error [%d] : Write cycle ongoing", rslt);
                break;
            case BMI2_E_WRITE_CYCLE_TIMEOUT:
                ESP_LOGE(TAG, "Error [%d] : Write cycle timeout", rslt);
                break;
            case BMI2_E_ST_NOT_RUNING:
                ESP_LOGE(TAG, "Error [%d] : Self test not running", rslt);
                break;
            case BMI2_E_DATA_RDY_INT_FAILED:
                ESP_LOGE(TAG, "Error [%d] : Data ready interrupt failed", rslt);
                break;
            case BMI2_E_INVALID_FOC_POSITION:
                ESP_LOGE(TAG, "Error [%d] : Invalid FOC position", rslt);
                break;
            default:
                ESP_LOGE(TAG, "Error [%d] : Unknown error code", rslt);
                break;
        }
    }

}
