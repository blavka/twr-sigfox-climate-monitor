#include <application.h>

#define SIGFOX_FIRST_REPORT_SECONDS 10
#define SIGFOX_REPORT_SECONDS (30 * 60)
#define SENSOR_DATA_STREAM_SAMPLES 8
#define SENSOR_UPDATE_INTERVAL_SECONDS 60

BC_DATA_STREAM_FLOAT_BUFFER(stream_buffer_thermometer, SENSOR_DATA_STREAM_SAMPLES)
bc_data_stream_t stream_thermometer;

BC_DATA_STREAM_FLOAT_BUFFER(stream_buffer_hygrometer, SENSOR_DATA_STREAM_SAMPLES)
bc_data_stream_t stream_hygrometer;

BC_DATA_STREAM_FLOAT_BUFFER(stream_buffer_lux_meter, SENSOR_DATA_STREAM_SAMPLES)
bc_data_stream_t stream_lux_meter;

BC_DATA_STREAM_FLOAT_BUFFER(stream_buffer_barometer, SENSOR_DATA_STREAM_SAMPLES)
bc_data_stream_t stream_barometer;

bc_led_t led;

bc_button_t button;

bc_module_sigfox_t sigfox_module;

void climate_module_event_handler(bc_module_climate_event_t event, void *event_param)
{
    (void) event_param;

    if (event == BC_MODULE_CLIMATE_EVENT_UPDATE_THERMOMETER)
    {
        float temperature;

        if (bc_module_climate_get_temperature_celsius(&temperature))
        {
            bc_data_stream_feed(&stream_thermometer, &temperature);
        }
        else
        {
            bc_data_stream_reset(&stream_thermometer);
        }
    }
    else if (event == BC_MODULE_CLIMATE_EVENT_UPDATE_HYGROMETER)
    {
        float humidity;

        if (bc_module_climate_get_humidity_percentage(&humidity))
        {
            bc_data_stream_feed(&stream_hygrometer, &humidity);
        }
        else
        {
            bc_data_stream_reset(&stream_hygrometer);
        }
    }
    else if (event == BC_MODULE_CLIMATE_EVENT_UPDATE_LUX_METER)
    {
        float illuminance;

        if (bc_module_climate_get_illuminance_lux(&illuminance))
        {
            bc_data_stream_feed(&stream_lux_meter, &illuminance);
        }
        else
        {
            bc_data_stream_reset(&stream_lux_meter);
        }
    }
    else if (event == BC_MODULE_CLIMATE_EVENT_UPDATE_BAROMETER)
    {
        float pressure;

        if (bc_module_climate_get_pressure_pascal(&pressure))
        {
            bc_data_stream_feed(&stream_barometer, &pressure);
        }
        else
        {
            bc_data_stream_reset(&stream_barometer);
        }
    }
}

void sigfox_module_event_handler(bc_module_sigfox_t *self, bc_module_sigfox_event_t event, void *event_param)
{
    (void) self;
    (void) event_param;

    if (event == BC_MODULE_SIGFOX_EVENT_SEND_RF_FRAME_START)
    {
        bc_led_set_mode(&led, BC_LED_MODE_ON);
    }
    else if (event == BC_MODULE_SIGFOX_EVENT_SEND_RF_FRAME_DONE)
    {
        bc_led_set_mode(&led, BC_LED_MODE_OFF);
    }
    else if (event == BC_MODULE_SIGFOX_EVENT_ERROR)
    {
        bc_led_set_mode(&led, BC_LED_MODE_BLINK);
    }
}

void button_event_handler(bc_button_t *self, bc_button_event_t event, void *event_param)
{
    (void) self;
    (void) event_param;

    if (event == BC_BUTTON_EVENT_CLICK)
    {
        bc_scheduler_plan_now(0);
    }
}

void application_init(void)
{
    bc_data_stream_init(&stream_thermometer, BC_DATA_STREAM_TYPE_FLOAT, stream_buffer_thermometer, sizeof(stream_buffer_thermometer));
    bc_data_stream_init(&stream_hygrometer, BC_DATA_STREAM_TYPE_FLOAT, stream_buffer_hygrometer, sizeof(stream_buffer_hygrometer));
    bc_data_stream_init(&stream_lux_meter, BC_DATA_STREAM_TYPE_FLOAT, stream_buffer_lux_meter, sizeof(stream_buffer_lux_meter));
    bc_data_stream_init(&stream_barometer, BC_DATA_STREAM_TYPE_FLOAT, stream_buffer_barometer, sizeof(stream_buffer_barometer));

    bc_led_init(&led, BC_GPIO_LED, false, false);
    bc_led_set_mode(&led, BC_LED_MODE_FLASH);

    bc_button_init(&button, BC_GPIO_BUTTON, BC_GPIO_PULL_DOWN, false);
    bc_button_set_event_handler(&button, button_event_handler, NULL);

    bc_module_climate_init();
    bc_module_climate_set_update_interval_all_sensors(SENSOR_UPDATE_INTERVAL_SECONDS * 1000);
    bc_module_climate_set_event_handler(climate_module_event_handler, NULL);
    bc_module_climate_measure_all_sensors();

    bc_module_sigfox_init(&sigfox_module, BC_MODULE_SIGFOX_REVISION_R2);
    bc_module_sigfox_set_event_handler(&sigfox_module, sigfox_module_event_handler, NULL);

    bc_scheduler_plan_absolute(0, SIGFOX_FIRST_REPORT_SECONDS * 1000);
}

void application_task(void *param)
{
    (void) param;

    static bool reset = true;

    uint8_t header = reset ? 0x80 : 0;

    float average;

    int16_t temperature = 0;

    if (bc_data_stream_get_average(&stream_thermometer, &average))
    {
        header |= 0x01; temperature = average * 2;
    }

    uint8_t humidity = 0;

    if (bc_data_stream_get_average(&stream_hygrometer, &average))
    {
        header |= 0x02; humidity = average * 2;
    }

    uint16_t illuminance = 0;

    if (bc_data_stream_get_average(&stream_lux_meter, &average))
    {
        header |= 0x04; illuminance = average / 2;
    }

    uint16_t pressure = 0;

    if (bc_data_stream_get_average(&stream_barometer, &average))
    {
        header |= 0x08; pressure = average / 2;
    }

    uint8_t buffer[8];

    buffer[0] = header;
    buffer[1] = temperature;
    buffer[2] = temperature >> 8;
    buffer[3] = humidity;
    buffer[4] = illuminance;
    buffer[5] = illuminance >> 8;
    buffer[6] = pressure;
    buffer[7] = pressure >> 8;

    if (bc_module_sigfox_send_rf_frame(&sigfox_module, buffer, sizeof(buffer)))
    {
        reset = false;

        bc_scheduler_plan_current_relative(SIGFOX_REPORT_SECONDS * 1000);
    }
    else
    {
        bc_scheduler_plan_current_relative(1000);
    }
}
