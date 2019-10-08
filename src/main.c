#include "../sdk/allocator.h"
#include "../sdk/logger.h"
#include "../sdk/side_module_api.h"

#include "fluence_libvips.c"

const int RESPONSE_SIZE_BYTES = 4;

/**
void log(char *msg) {
    wasm_log(msg, strlen(msg));
}*/

char *invoke(const char *str, int length) {
    Request *request = parse_request(str);
    Response *response = handle_request(request);
    char *json_str = print_json_response(response);

    // prepend size bytes
    char *response_str = malloc(RESPONSE_SIZE_BYTES + strlen(json_str));
    for (int i = 0; i < RESPONSE_SIZE_BYTES; ++i) {
        response_str[i] = (strlen(json_str) >> 8 * i) & 0xFF;
    }
    strcpy(response_str + RESPONSE_SIZE_BYTES, json_str);
    return response_str;
}
