#include <application.h>
#include <bcl.h>

static struct
{
    struct { bool valid; float value; } temperature;
    struct { bool valid; float value; } humidity;
    struct { bool valid; float value; } luminosity;
    struct { bool valid; float value; } pressure;

} i2c_sensors;

static bc_led_t led;

static bc_module_sigfox_t sigfox_module;

void climate_event_event_handler(bc_module_climate_event_t event, void *event_param)
{
    (void) event_param;

    switch (event)
    {
    	case BC_MODULE_CLIMATE_EVENT_UPDATE_THERMOMETER:
    		i2c_sensors.temperature.valid = bc_module_climate_get_temperature_celsius(&i2c_sensors.temperature.value);
    		break;
    	case BC_MODULE_CLIMATE_EVENT_UPDATE_HYGROMETER:
    		i2c_sensors.humidity.valid = bc_module_climate_get_humidity_percentage(&i2c_sensors.humidity.value);
    		break;
    	case BC_MODULE_CLIMATE_EVENT_UPDATE_LUX_METER:
    		i2c_sensors.luminosity.valid = bc_module_climate_get_luminosity_lux(&i2c_sensors.luminosity.value);
    		break;
    	case BC_MODULE_CLIMATE_EVENT_UPDATE_BAROMETER:
    		i2c_sensors.pressure.valid = bc_module_climate_get_pressure_pascal(&i2c_sensors.pressure.value);
    		break;
        case BC_MODULE_CLIMATE_EVENT_ERROR_THERMOMETER:
        case BC_MODULE_CLIMATE_EVENT_ERROR_HYGROMETER:
        case BC_MODULE_CLIMATE_EVENT_ERROR_LUX_METER:
        case BC_MODULE_CLIMATE_EVENT_ERROR_BAROMETER:
    	default:
    		break;
    }
}

void timer_task(void *param)
{

    (void) param;

	struct {
		uint8_t code;
		uint8_t humidity;
		int16_t temperature;
		uint16_t luminosity;
		uint16_t pressure;

	} data;

	data.code = 0x00;

	if (i2c_sensors.humidity.valid)
	{
		data.code |= 0x01;
		data.humidity = (i2c_sensors.humidity.value * 2);
	}

	if (i2c_sensors.temperature.valid)
	{
		data.code |= 0x02;
		data.temperature = (i2c_sensors.temperature.value * 2);
	}

	if (i2c_sensors.luminosity.valid)
	{
		data.code |= 0x04;
		data.luminosity = i2c_sensors.luminosity.value;
	}

	if (i2c_sensors.pressure.valid)
	{
		data.code |= 0x08;
		data.pressure = i2c_sensors.pressure.value / 100; //conver to hPa
	}

	//bc_uart_write(BC_UART_UART1, &data, sizeof(data), 100);

	if (bc_module_sigfox_send_rf_frame(&sigfox_module, &data, sizeof(data)))
	{
        bc_scheduler_disable_sleep();
		bc_scheduler_plan_current_relative(1000 * 60 * 15); //15 min
	}
	else
	{
		bc_scheduler_plan_current_relative(1000);
	}
}

void sigfox_module_event_handler(bc_module_sigfox_t *self, bc_module_sigfox_event_t event, void *event_param)
{
    (void) self;
    (void) event_param;

    // If event error
    if (event == BC_MODULE_SIGFOX_EVENT_ERROR)
    {
        bc_led_set_mode(&led, BC_LED_MODE_BLINK);
    }
    // If event is start of transmission...
    if (event == BC_MODULE_SIGFOX_EVENT_SEND_RF_FRAME_START)
    {
        bc_led_set_mode(&led, BC_LED_MODE_ON);
    }
    // If event is end of transmission...
    else if (event == BC_MODULE_SIGFOX_EVENT_SEND_RF_FRAME_DONE)
    {
        bc_scheduler_enable_sleep();

        bc_led_set_mode(&led, BC_LED_MODE_OFF);
    }
}

void application_init(void)
{
	bc_rtc_init();

    bc_led_init(&led, BC_GPIO_LED, false, false);
    bc_led_set_mode(&led, BC_LED_MODE_ON);

    bc_module_climate_init();
    bc_module_climate_set_update_interval_thermometer(60000); // 1 min
    bc_module_climate_set_update_interval_hygrometer(60000);
	bc_module_climate_set_update_interval_lux_meter(60000);
	bc_module_climate_set_update_interval_barometer(60000);
    bc_module_climate_set_event_handler(climate_event_event_handler, NULL);

    // Initialize SigFox Module
	bc_module_sigfox_init(&sigfox_module, BC_MODULE_SIGFOX_REVISION_R1);
	bc_module_sigfox_set_event_handler(&sigfox_module, sigfox_module_event_handler, NULL);

	bc_scheduler_register(timer_task, NULL, 10000);

    bc_led_set_mode(&led, BC_LED_MODE_OFF);
}
