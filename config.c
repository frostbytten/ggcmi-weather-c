#include <stdio.h>
#include <string.h>
#include <jansson.h>
#include "config.h"

static char *InsertConfigString(json_t *obj, const char *key) {
    if (obj == NULL || key == NULL) return NULL;
    json_t *json_v = json_object_get(obj, key);
    if (json_v == NULL) {
        return NULL;
    }
    const char *string_v = json_string_value(json_v);
    if (string_v == NULL) {
        return NULL;
    }
    size_t string_size = json_string_length(json_v) + 1;
    char *v = malloc(string_size);
    if (v == NULL) {
        return NULL;
    }
    v = strncpy(v, string_v, string_size);
    return v;
}

Config *LoadConfig(const char *source) {
    json_t *root;
    json_error_t error;

    root = json_load_file(source, JSON_REJECT_DUPLICATES, &error);

    if (!root) {
        fprintf(stderr, "error: [line %d] %s\n", error.line, error.text);
        return NULL;
    }
    if (!json_is_object(root)) {
        fprintf(stderr, "error: root is not a JSON object\n");
        goto cleanup;
    }
    json_t *start_year, *mappings, *station_id_field;
    start_year = json_object_get(root, "start_year");
    if (!json_is_integer(start_year)) {
        fprintf(stderr, "error: root->start_year is not an integer\n");
        goto cleanup;
    }
    if (json_integer_value(start_year) > 3000) {
        fprintf(stderr, "error: we haven't come up with a better way to handle this by now?\n");
        goto cleanup;
    }
    if (json_integer_value(start_year) < 1700) {
        fprintf(stderr, "error: this probably isn't realistic to run\n");
        goto cleanup;
    }

    station_id_field = json_object_get(root, "station_id_field");
    if (!json_is_string(station_id_field)) {
        fprintf(stderr, "error: root->station_id_field is not a string\n");
        goto cleanup;
    }

    mappings = json_object_get(root, "mapping");
    size_t mappings_size = json_array_size(mappings);
    if (!json_is_array(mappings)) {
        fprintf(stderr, "error: root->mappings is not an array\n");
        goto cleanup;
    }

    /* Start actually loading in the config once everything is checked */
    Config *config = (Config *) malloc(sizeof(Config) + (sizeof(FileConfig) * mappings_size));

    if (config == NULL) goto cleanup;
    config->num_mappings = mappings_size;
    config->start_year = json_integer_value(start_year);
    if ((config->station_id = InsertConfigString(root, "station_id_field")) == NULL) goto cleanup;
    if ((config->station_vector = InsertConfigString(root, "station_vector")) == NULL) goto cleanup;

    size_t index;
    json_t *value;
    json_array_foreach(mappings, index, value) {
        config->mappings[index].file_name = InsertConfigString(value, "file");
        config->mappings[index].dssat_var = InsertConfigString(value, "dssatVar");
        config->mappings[index].netcdf_var = InsertConfigString(value, "netcdfVar");
        config->mappings[index].source_unit = InsertConfigString(value, "sourceUnit");
        config->mappings[index].target_unit = InsertConfigString(value, "targetUnit");
        config->mappings[index].netcdf_id = -1;
        config->mappings[index].is_temp = 0;
        if(config->mappings[index].dssat_var != NULL) {
          if(strlen(config->mappings[index].dssat_var) == 4) {
            if (strncmp("TMIN", config->mappings[index].dssat_var, 4) == 0) {
              config->mappings[index].is_temp = 1;
            } else if (strncmp("TMAX", config->mappings[index].dssat_var, 4) == 0) {
              config->mappings[index].is_temp = 2;
            }
          }
        }
    }
    json_decref(root);
    return config;

    cleanup:
    json_decref(root);
    return NULL;
}

void FreeConfig(Config *config) {
    /* Prototype release of nested configuration array */
    if (config != NULL) {
        for (size_t i = 0; i < config->num_mappings; ++i) {
            free(config->mappings[i].file_name);
            config->mappings[i].file_name = NULL;
            free(config->mappings[i].netcdf_var);
            config->mappings[i].netcdf_var = NULL;
            free(config->mappings[i].dssat_var);
            config->mappings[i].dssat_var = NULL;
            free(config->mappings[i].source_unit);
            config->mappings[i].source_unit = NULL;
            free(config->mappings[i].target_unit);
            config->mappings[i].target_unit = NULL;
        }

        if (config->station_vector != NULL) {
            free(config->station_vector);
            config->station_vector = NULL;
        }

        if (config->station_id != NULL) {
            free(config->station_id);
            config->station_id = NULL;
        }
        free(config);
        config = NULL;
    }
}
