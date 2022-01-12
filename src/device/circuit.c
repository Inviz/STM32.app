#include "circuit.h"

/* Start of autogenerated OD accessors */
OD_ACCESSORS(device, circuit, values, duty_cycle, SUBIDX_CIRCUIT_DUTY_CYCLE, int16_t, i16) /* 0x58XX01: values */
OD_ACCESSORS(device, circuit, values, current, SUBIDX_CIRCUIT_CURRENT, uint16_t, u16) /* 0x58XX02: values */
OD_ACCESSORS(device, circuit, values, voltage, SUBIDX_CIRCUIT_VOLTAGE, uint16_t, u16) /* 0x58XX03: values */
OD_ACCESSORS(device, circuit, values, consumers, SUBIDX_CIRCUIT_CONSUMERS, uint8_t, u8) /* 0x58XX04: values */
/* End of autogenerated OD accessors */

static ODR_t OD_write_device_circuit_property(OD_stream_t *stream, const void *buf, OD_size_t count, OD_size_t *countWritten) {
    device_circuit_t *circuit = stream->object;
    bool was_on = device_circuit_get_state(circuit);
    ODR_t result = OD_writeOriginal(stream, buf, count, countWritten);
    bool is_on = device_circuit_get_state(circuit);
    if (is_on != was_on) {
        device_circuit_set_state(circuit, is_on);
    }
    switch (stream->subIndex) {
    case SUBIDX_CIRCUIT_DUTY_CYCLE:
        log_printf("OD - Circuit [%X] duty cycle:  %i\n", circuit->device->seq, circuit->values->duty_cycle);
        break;
    case SUBIDX_CIRCUIT_CONSUMERS: log_printf("OD - Circuit [%X] consumers: %i\n", circuit->device->seq, circuit->values->consumers); break;
    }
    return result;
}

/* Circuit needs its relay GPIO configured */
static app_signal_t circuit_validate(OD_entry_t *config_entry) {
    device_circuit_config_t *config = (device_circuit_config_t *)OD_getPtr(config_entry, 0x01, 0, NULL);
    return config->port == 0 || config->pin == 0;
}

static app_signal_t circuit_phase_constructing(device_circuit_t *circuit, device_t *device) {
    circuit->config = (device_circuit_config_t *)OD_getPtr(device->config, 0x01, 0, NULL);
    circuit->values = (device_circuit_values_t *)OD_getPtr(device->values, 0x01, 0, NULL);
    return 0;
}

static app_signal_t circuit_phase_linking(device_circuit_t *circuit) {
    return device_phase_linking(circuit->device, (void **)&circuit->current_sensor, circuit->config->sensor_index, NULL) +
           device_phase_linking(circuit->device, (void **)&circuit->psu, circuit->config->psu_index, NULL);
}

// receive value from current sensor
static app_signal_t circuit_receive(device_circuit_t *circuit, device_t *device, void *value, void *argument) {
    (void)argument;
    if (circuit->current_sensor->device == device) {
        device_circuit_set_current(circuit, (uint16_t)((uint32_t)value));
    }
    return 1;
}

static app_signal_t circuit_phase_starting(device_circuit_t *circuit) {
    device_gpio_configure_output_with_value("Relay", circuit->config->port, circuit->config->pin, device_circuit_get_state(circuit));

    return 0;
}

static app_signal_t circuit_phase_stoping(device_circuit_t *circuit) {
    (void)circuit;
    return 0;
}

bool_t device_circuit_get_state(device_circuit_t *circuit) {
    return circuit->values->duty_cycle > 0 || circuit->values->consumers > 0;
}

void device_circuit_set_state(device_circuit_t *circuit, bool state) {
    gpio_set_state(circuit->config->port, circuit->config->pin, state);
    if (circuit->psu) {
        device_circuit_set_consumers(circuit->psu, circuit->psu->values->consumers + (state ? 1 : -1));
    }
}

device_methods_t device_circuit_methods = {
    .validate = circuit_validate,
    .phase_constructing = (app_signal_t (*)(void *, device_t *))circuit_phase_constructing,
    .phase_starting = (app_method_t) circuit_phase_starting,
    .phase_stoping = (app_method_t) circuit_phase_stoping,
    .phase_linking = (app_method_t) circuit_phase_linking,
    .callback_value = (app_signal_t (*)(void *, device_t *device, void *value, void *argument))circuit_receive,
    .write_values = OD_write_device_circuit_property,
};
