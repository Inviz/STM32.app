#include "blank.h"


static ODR_t blank_property_write(OD_stream_t *stream, const void *buf, OD_size_t count,
                                            OD_size_t *countWritten) {
    device_blank_t *blank = stream->object;
    (void)blank;
    ODR_t result = OD_writeOriginal(stream, buf, count, countWritten);
    return result;
}

static app_signal_t blank_validate(device_blank_properties_t *properties) {
    return properties->phase != DEVICE_ENABLED;
}

static app_signal_t blank_phase_constructing(device_blank_t *blank) {
    return 0;
}

static app_signal_t blank_phase_starting(device_blank_t *blank) {
    (void)blank;
    return 0;
}

static app_signal_t blank_phase_stoping(device_blank_t *blank) {
    (void)blank;
    return 0;
}

static app_signal_t blank_phase_pausing(device_blank_t *blank) {
    (void)blank;
    return 0;
}

static app_signal_t blank_phase_resuming(device_blank_t *blank) {
    (void)blank;
    return 0;
}
static app_signal_t blank_phase_linking(device_blank_t *blank) {
    (void)blank;
    return 0;
}

static app_signal_t blank_phase(device_blank_t *blank, device_phase_t phase) {
    (void)blank;
    (void)phase;
    return 0;
}

device_methods_t device_blank_methods = {.validate = (app_method_t) blank_validate,
                                             .phase_constructing = (app_method_t)blank_phase_constructing,
                                             .phase_linking = (app_method_t) blank_phase_linking,
                                             .phase_starting = (app_method_t) blank_phase_starting,
                                             .phase_stoping = (app_method_t) blank_phase_stoping,
                                             .phase_pausing = (app_method_t) blank_phase_pausing,
                                             .phase_resuming = (app_method_t) blank_phase_resuming,
                                             //.accept = (int (*)(void *, device_t *device, void *channel))device_blank_accept,
                                             .callback_phase = (app_signal_t (*)(void *, device_phase_t phase))blank_phase,
                                             .property_write = blank_property_write};
