#include "../src/fluence_libvips.c"

#include <string.h>
#include <stdio.h>
#include <assert.h>

char *invoke(const char *str, int length) {
    Request *request = parse_request(str);
    Response *response = handle_request(request);
    return print_json_response(response);
}

char *read_file(char* file) {
    char *buffer = NULL;
    size_t len;
    FILE *fp = fopen(file, "r");
    ssize_t bytes_read = getdelim(&buffer, &len, '\0', fp);
    return buffer;
}

int main(int argc, char *argv[]) {
    char* input = read_file(argv[1]);
    char* output = read_file(argv[2]);
    char* response = invoke(input, strlen(input));

    if (strcmp(response, output) != 0) {
        printf("Fail %s\n", argv[1]);
        puts("==========expected==========");
        puts(output);
        puts("==========actual==========");
        puts(response);
        exit(1);
    } else {
        printf("Pass %s\n", argv[1]);
    }
}