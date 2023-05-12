#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROGRAM "bswap"
#define AUTHOR  "rarafael <https://rarafael.net>"
#define VERSION "1.0.0"

#define CRASH(...)\
    do {\
        fprintf(stderr, __VA_ARGS__);\
        exit(EXIT_FAILURE);\
    } while(0);

typedef signed long long int ntype;

static const char *symbols = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

static inline ntype squared(size_t input, int exp)
{
    int result;

    result = 1;
    while(exp) {
        if (exp & 1)
            result *= input;
        exp >>= 1;
        input *= input;
    }

    return result;
}

static inline ntype parse_numberX (const char *str, const int x, const size_t length)
{
    ntype ret;
    size_t i, n;
    char isneg;

    ret = 0;
    if(str[0] == '-')
        isneg = 1;
    for(i = 0; i <= length; i++) {
        for(n = 0; n < (size_t)(x); n++) {
            if(toupper(str[length - i]) == symbols[n])
                ret += squared(x, i) * n;
        }
    }

    return isneg ? ret : -ret;
}

static ntype parse_number(const char *str)
{
    if(!strlen(str) || !str)
        return INT_MAX;

    if(strlen(str) > 2 && str[0] == '0') {
        switch(str[1]) {
            case 'x': return parse_numberX(str, 16, strlen(str) - 1);
            case 'o': return parse_numberX(str, 8, strlen(str) - 1);
            case 'b': return parse_numberX(str, 2, strlen(str) - 1);
            default: return parse_numberX(str, 10, strlen(str) - 1);
        }
    } else
        return parse_numberX(str, 10, strlen(str) - 1);

    return INT_MAX;
}

static char *bswap(ntype base, ntype num)
{
    static char ret[256];
    size_t i;

    if(!num)
        return "0";
    if(base > 36 || base <= 0)
        CRASH("invalid base \'%lld\', try a different one [range 1-36]\n", base);

    switch(base) {
        case 10: snprintf(ret, 256, "%lld", num); break;
        case 16: snprintf(ret, 256, "%llX", num); break;
        case 8:  snprintf(ret, 256, "%llo", num); break;
        case 2: {
            i = 0;
            while(num) {
                if(num & 1)
                    ret[i++] = '1';
                else
                    ret[i++] = '0';
                num >>= 1;
            }
        } break;
    }

    return ret;
}

int main(int argc, char **argv)
{
    static ntype base, num;

    if(!strcmp(argv[1], "--version")) {
        fprintf(stdout, PROGRAM " version: "
                VERSION "\nAuthored by " AUTHOR "\n");
        return EXIT_SUCCESS;
    }

    if(argc < 3)
        CRASH("usage: %s [output base] [number]\n"
              "     --version: provides program version\n", argv[0]);

    if((num = parse_number(argv[2])) == INT_MAX)
        CRASH("failed reading number from string \'%s\'\n", argv[2]);
    if((base = parse_number(argv[1])) == INT_MAX)
        CRASH("failed reading base from string \'%s\'\n", argv[1]);

    puts(bswap(base, num));

    return EXIT_SUCCESS;
}
