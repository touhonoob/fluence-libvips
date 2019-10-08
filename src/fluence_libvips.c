#include "../cJSON/cJSON.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

enum ACTION {
    UNKNOWN,
    HELLO
};

static const char *ACTION_STRING[] = {
        "UNKNOWN",
        "HELLO"
};

typedef struct Request {
    enum ACTION action;
    const char *parse_error;
    void *parameters;
} Request;

typedef struct Response {
    const char *error;
    char *response;
} Response;

typedef struct HelloParameters {
    char *name;
} HelloParameters;

enum ACTION parse_action(char *action_str) {
    for (size_t i = 1; i < sizeof(ACTION_STRING) / sizeof(const char*); i++) {
        if (strcmp(action_str, ACTION_STRING[i]) == 0) {
            return i;
        }
    }
    return UNKNOWN;
}

void *parse_parameters(enum ACTION action, cJSON *parameters) {
    switch (action) {
        case HELLO: {
            cJSON *name = cJSON_GetObjectItemCaseSensitive(parameters, "name");
            if (name == NULL || !cJSON_IsString(name) || name->valuestring == NULL) {
                return NULL;
            }

            HelloParameters* hello_parameters = (HelloParameters*) malloc(sizeof(HelloParameters));
            hello_parameters->name = (char*) malloc(strlen(name->valuestring));
            strcpy(hello_parameters->name, name->valuestring);
            return hello_parameters;
        }

        default:
            assert(0);
    }
}

Request *parse_request(const char *str) {
    Request *request = (Request*) malloc(sizeof(Request));
    request->action = UNKNOWN;
    request->parse_error = NULL;
    request->parameters = NULL;

    cJSON *request_json = cJSON_Parse(str);
    if (request_json == NULL) {
        request->parse_error = "JSON parse error";
        return request;
    }

    cJSON *action_json = cJSON_GetObjectItemCaseSensitive(request_json, "action");
    if (action_json == NULL || !cJSON_IsString(action_json) || (action_json->valuestring == NULL)) {
        request->parse_error = "Invalid action";
        return request;
    } else {
        request->action = parse_action(action_json->valuestring);
        if (request->action == UNKNOWN) {
            request->parse_error = "Unknown action";
            return request;
        }
    }

    cJSON *parameters_json = cJSON_GetObjectItemCaseSensitive(request_json, "parameters");
    if (parameters_json == NULL || !cJSON_IsObject(parameters_json)) {
        request->parse_error = "Invalid parameters";
        return request;
    } else {
        request->parameters = parse_parameters(request->action, parameters_json);
        if (request->parameters == NULL) {
            request->parse_error = "Invalid parameters";
            return request;
        }
    }

    cJSON_Delete(request_json);
    return request;
}

Response *handle_request(Request *request) {
    Response *response = (Response*) malloc(sizeof(Response));
    response->error = NULL;

    if (request->parse_error != NULL) {
        response->error = request->parse_error;
        return response;
    }

    switch (request->action) {
        case HELLO: {
            const char *const greeting = "Hello world! From ";
            HelloParameters *parameters = (HelloParameters*) request->parameters;
            response->response = (char*) malloc(strlen(greeting) + strlen(parameters->name));
            strcpy(response->response, greeting);
            strcat(response->response, parameters->name);
            return response;
        }

        default: {
            response->error = "Unknown action";
            return response;
        }
    }
}

char *print_json_response(Response *response) {
    cJSON *response_json = cJSON_CreateObject();

   if (response->error != NULL) {
        cJSON_AddItemToObject(response_json, "error", cJSON_CreateString(response->error));
        cJSON_AddItemToObject(response_json, "response", cJSON_CreateNull());
    } else {
        cJSON_AddItemToObject(response_json, "error", cJSON_CreateNull());
        cJSON_AddItemToObject(response_json, "response", cJSON_CreateString(response->response));
    }

    char *json_str = cJSON_PrintUnformatted(response_json);
    cJSON_Delete(response_json);
    return json_str;
}