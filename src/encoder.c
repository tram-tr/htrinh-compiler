#include "../inc/encoder.h"

int string_decode(const char *es, char *s) {
    int s_index = 0;
    int es_index = 0;
    int closing_quote = 0;

    // check for opening quote and skip if present
    if (es[es_index] == '\"') es_index++;
    else {
        printf("encode error: missing opening quote.\n");
        return 1;
    }

    while (es[es_index] != '\0') {
        if (s_index > 255) {
            printf("encode error: decoded string exceeds maximum length.\n");
            return 1;
        }

        if (closing_quote == 1) {
            printf("encode error: unexpected character after closing quote.\n");
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
                        printf("encode error: unexpected end of string.\n");
                        s[s_index++] = '\0';
                        return 1;
                    }
                    es_index++;
                    // check for incomplete hexadecimal sequence
                    if (!isxdigit((unsigned char)es[es_index + 1]) || !isxdigit((unsigned char)es[es_index + 2])) {
                        printf("encode error: incomplete hexadecimal sequence.\n");
                        return 1;
                    }
                    // convert the hexadecimal sequence to a byte
                    char hex_seq[5] = { '0', 'x', es[es_index + 1], es[es_index + 2], '\0' };
                    int hex_value = (int)strtol(hex_seq, NULL, 16);
                    
                    if (hex_value >= 0x20 && hex_value <= 0x7E) s[s_index++] = (char)hex_value; // ascii range 32 - 126
                    else if ((hex_value >= 0x07 && hex_value <= 0x0D) || (hex_value == 0x1B) || (hex_value == 0x22) || (hex_value == 0x27)) s[s_index++] = (char)hex_value; // outside of range, but in the backlash code
                    else if (hex_value > 0) {
                        s[s_index++] = '\\';
                        s[s_index++] = '0';
                        s[s_index++] = 'x';
                        s[s_index++] = (hex_value >> 4) < 10 ? (hex_value >> 4) + '0' : (hex_value >> 4) + 'a' - 10;
                        s[s_index++] = (hex_value & 0x0F) < 10 ? (hex_value & 0x0F) + '0' : (hex_value & 0x0F) + 'a' - 10;
                    } 
                    else {
                        printf("encode error: unexpected end of string.\n");
                        return 1;
                    }
                    es_index += 2; // skip the two hex digits
                    break;
                default:
                    printf("encode error: unrecognized escape.\n"); 
                    return 1;
            }
            es_index++;
        }
        // check for closing quote
        else if (es[es_index] == '\"') {
            es_index++;
            /*if (es[es_index] != ' ' && es[es_index] != '\0') {
                printf("error: unexpected character after closing quote.\n");
                return 1;
            }*/
            closing_quote = 1;
        }
        // check for invalid characters
        else if (es[es_index] < 32 || es[es_index] > 126) {
            printf("encode error: invalid character found.\n");
            return 1;
        }
        else s[s_index++] = es[es_index++];
    }

    if (!closing_quote) {
        printf("encode error: missing closing quote.\n");
        return 1;
    }

    s[s_index] = '\0';

    return 0;
}

int string_encode(const char *s, char *es) {
    int s_index = 0;
    int es_index = 0;

    //es[es_index++] = '\"';

    while (s[s_index] != '\0') {
        if (es_index > 255) {
            printf("encode error: encoded string exceeds maximum length.\n");
            return 1;
        }

        char c = s[s_index];

        switch (c) {
            case '\"': es[es_index++] = '\\'; es[es_index++] = '\"'; break;
            case '\\':
                if (s[s_index + 1] != '0') 
                    es[es_index++] = '\\'; es[es_index++] = '\\'; break;
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
                } 
                else {
                    es[es_index++] = '\\';
                    es[es_index++] = '0';
                    es[es_index++] = 'x';
                    es[es_index++] = (c >> 4) < 10 ? (c >> 4) + '0' : (c >> 4) + 'a' - 10;
                    es[es_index++] = (c & 0x0F) < 10 ? (c & 0x0F) + '0' : (c & 0x0F) + 'a' - 10;
                }
                break;
        }

        s_index++;
    }

    //es[es_index++] = '\"';
    es[es_index] = '\0';

    return 0;
}

int char_decode(const char *es, char *c) {
    int es_index = 0;
    int closing_quote = 0;
    
    es_index++;
    if (es[es_index] == '\\') {
        es_index++;
        if (es[es_index] == '\'') {
            printf("encode error: invalid character found.\n");
            return 1;
        }
        switch (es[es_index]) {
            case 'a': (*c) = 7; break;
            case 'b': (*c) = 8; break;
            case 'e': (*c) = 27; break;
            case 'f': (*c) = 12; break;
            case 'n': (*c) = 10; break;
            case 'r': (*c) = 13; break;
            case 't': (*c) = 9; break;
            case 'v': (*c) = 11; break;
            case '\\': (*c) = 92; break;
            case '\'': (*c) = 39; break;
            case '\"': (*c) = 34; break;
            case '0':
                if (es[es_index + 1] != 'x') {
                    (*c) = 0;
                    break;
                }
                es_index++;
                // check for incomplete hexadecimal sequence
                if (!isxdigit((unsigned char)es[es_index + 1]) || !isxdigit((unsigned char)es[es_index + 2])) {
                    printf("encode error: incomplete hexadecimal sequence.\n");
                    return 1;
                }
                // convert the hexadecimal sequence to a byte
                char hex_seq[5] = { '0', 'x', es[es_index + 1], es[es_index + 2], '\0' };
                int hex_value = (int)strtol(hex_seq, NULL, 16);
                
                if (hex_value >= 0x20 && hex_value <= 0x7E && hex_value != 0x5C || hex_value == 0) (*c) = (char)hex_value;
                else {
                    printf("encode error: invalid character.\n");
                    return 1;
                }
                es_index += 2;
                break;
            default:
                printf("encode error: unrecognized escape.\n"); 
                return 1;
        }
    }
    else if (es[es_index] < 32 || es[es_index] > 126 || es[es_index] == 92 || es[es_index] == 39) {
        printf("encode error: invalid character.\n");
        return 1;
    }
    else (*c) = es[es_index];

    return 0;
}
