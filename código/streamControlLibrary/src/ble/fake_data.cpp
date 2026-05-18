#include "fake_data.h"
#include "cJSON.h"
#include <stdlib.h>
#include <math.h>

// ============================================
// Internal State
// ============================================
static uint32_t g_sequence_counter = 0;

// ============================================
// Initialize Fake Data Generator
// ============================================
void fake_data_init(void) {
    g_sequence_counter = 0;
}

// ============================================
// Generate a Single Fake Sample
// ============================================
sample_t generate_sample(uint32_t timestamp) {
    sample_t sample;

    sample.t = timestamp;
    sample.temp = 25.0f + (random(-10, 10) / 10.0f);
    sample.angle = 45.0f + (random(-20, 20) / 10.0f);
    sample.pwm = 80.0f + (random(-50, 50) / 10.0f);

    return sample;
}

// ============================================
// Create Batch JSON String (array of samples)
// ============================================
char* create_batch_json(sample_t* samples, uint16_t count) {
    if (samples == nullptr || count == 0) {
        return nullptr;
    }

    cJSON* root = cJSON_CreateObject();
    if (root == nullptr) {
        return nullptr;
    }

    cJSON_AddStringToObject(root, "type", "control_data");
    cJSON_AddNumberToObject(root, "count", count);

    cJSON* samples_array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "samples", samples_array);

    for (uint16_t i = 0; i < count; i++) {
        cJSON* sample_obj = cJSON_CreateObject();
        cJSON_AddItemToArray(samples_array, sample_obj);

        cJSON_AddNumberToObject(sample_obj, "t", samples[i].t);
        cJSON_AddNumberToObject(sample_obj, "temp", samples[i].temp);
        cJSON_AddNumberToObject(sample_obj, "angle", samples[i].angle);
        cJSON_AddNumberToObject(sample_obj, "pwm", samples[i].pwm);
    }

    char* json_string = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    return json_string;
}