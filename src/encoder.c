#include "../inc/encoder.h"

int string_decode(const char *es, char *s) {
    int s_index = 0;
    int es_index = 0;

    // check for opening quote and skip if present
    if (es[es_index] == '\"') es_index++;
    else {
        printf("error: missing opening quote.\n");
        return 1;
    }

    while (es[es_index] != '\0') {
        if (s_index >= 255) {
            printf("error: decoded string exceeds maximum length.\n");
            return 1;
        }

        if (es[es_index] == '\\') {
            es_index++;
            switch (es[es_index]) {
                case 'a': s[s_index++] = 7; break;
                case 'b': s[s_index++] = 8; break;
                case 'e': s[s_index++] = 27; break;
                case 'f': s[s_index++] = 12; break;
                case 'n': s[s_index++] = 10; break;
                case 'r': s[s_index++] = 13; break;
                case 't': s[s_index++] = 9; break;
                case 'v': s[s_index++] = 11; break;
                case '\\': s[s_index++] = 92; break;
                case '\'': s[s_index++] = 39; break;
                case '\"': s[s_index++] = 34; break;
                case '0':
                    if (es[es_index + 1] != 'x') {
                        printf("error: unrecognized escape\n");
                        return 1;
                    }
                    es_index++;
                    // check for incomplete hexadecimal sequence
                    if (!isxdigit((unsigned char)es[es_index + 1]) || !isxdigit((unsigned char)es[es_index + 2])) {
                        printf("error: incomplete hexadecimal sequence.\n");
                        return 1;
                    }
                    // convert the hexadecimal sequence to a byte
                    char hexValue[5] = { '0', 'x', es[es_index + 1], es[es_index + 2], '\0' };
                    s[s_index++] = (char)strtol(hexValue, NULL, 16);
                    es_index += 3; // skip the two hex digits
                    break;
                default:
                    printf("error: unrecognized escape.\n"); 
                    return 1;
            }
            es_index++;
        }
        // check for closing quote
        else if (es[es_index] == '\"') {
            es_index++;
            if (es[es_index] != ' ' && es[es_index] != '\0') {
                printf("error: unexpected character after closing quote.\n");
                return 1;
            }
        }
        // check for invalid characters
        else if (es[es_index] < 32 || es[es_index] > 126) {
            printf("error: invalid character found.\n");
            return 1;
        }
        else s[s_index++] = es[es_index++];
    }

    if (es[es_index-1] != '\"') {
        printf("error: missing closing quote.\n");
        return 1;
    }


    s[s_index] = '\0';

    return 0;
}

int string_encode(const char *s, char *es) {
    int s_index = 0;
    int es_index = 0;

    es[es_index++] = '\"';

    while (s[s_index] != '\0') {
        if (es_index >= 255) {
            printf("error: encoded string exceeds maximum length.\n");
            return 1;
        }

        char c = s[s_index];

        switch (c) {
            case '\"': es[es_index++] = '\\'; es[es_index++] = '\"'; break;
            case '\\': es[es_index++] = '\\'; es[es_index++] = '\\'; break;
            case '\a': es[es_index++] = '\\'; es[es_index++] = 'a'; break;
            case '\b': es[es_index++] = '\\'; es[es_index++] = 'b'; break;
            case '\e': es[es_index++] = '\\'; es[es_index++] = 'e'; break;
            case '\f': es[es_index++] = '\\'; es[es_index++] = 'f'; break;
            case '\n': es[es_index++] = '\\'; es[es_index++] = 'n'; break;
            case '\r': es[es_index++] = '\\'; es[es_index++] = 'r'; break;
            case '\t': es[es_index++] = '\\'; es[es_index++] = 't'; break;
            case '\v': es[es_index++] = '\\'; es[es_index++] = 'v'; break;
            default:
                if (c >= 32 && c <= 126) {
                    es[es_index++] = c;
                } else {
                    es[es_index++] = '\\';
                    es[es_index++] = '0';
                    es[es_index++] = 'x';
                    es[es_index++] = (c >> 4) < 10 ? (c >> 4) + '0' : (c >> 4) + 'a' - 10;
                    es[es_index++] = (c & 0x0F) < 10 ? (c & 0x0F) + '0' : (c & 0x0F) + 'a' - 10;
                }
        }

        s_index++;
    }

    es[es_index++] = '\"';
    es[es_index] = '\0';

    return 0;
}