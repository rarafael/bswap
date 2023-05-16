#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROGRAM "bswap"
#define AUTHOR  "rarafael <https://rarafael.net>"
#define VERSION "1.0.0"
#define USAGE   "usage: %s [output base] [number]\n"\
                "     --version: provides program version\n"

typedef signed long long int ntype;
typedef unsigned long long int untype;

static const char *symbols = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void usage(void)
{
    fprintf(stdout, "usage: bswap [output base] [number]\n"
                    "     --version: provides program version\n");
}

ntype squared(ntype input, size_t exp)
{
    ntype result;

    result = 1;
    while(exp) {
        if (exp & 1)
            result *= input;
        exp >>= 1;
        input *= input;
    }

    return result;
}

ntype parse_numberX(char *str, int x, size_t length)
{
    ntype ret;
    size_t i, n;
    char isneg;

    ret = 0;
    isneg = 0;
    if(str[0] == '-')
        isneg = 1;
    for(i = 0; i <= length; i++) {
        for(n = 0; n < (size_t)(x); n++) {
            if(toupper(str[length - i]) == symbols[n])
                ret += squared(x, i) * n;
        }
    }

    return isneg ? -ret : ret;
}

int parse_number(char *str, ntype *out)
{
    if(!strlen(str) || !str)
        return 1;

    if(strlen(str) > 2 && str[0] == '0') {
        switch(str[1]) {
            case 'x': *out = parse_numberX(str, 16, strlen(str) - 1); return 0;
            case 'o': *out = parse_numberX(str, 8, strlen(str) - 1); return 0;
            case 'b': *out = parse_numberX(str, 2, strlen(str) - 1); return 0;
            default: *out = parse_numberX(str, 10, strlen(str) - 1); return 0;
        }
    } else
        *out = parse_numberX(str, 10, strlen(str) - 1); 

    return 0;
}

char *bswap(ntype base, ntype num)
{
    static char ret[256];
    untype n;
    size_t i;
    char *tmp;

    if(!num)
        return "0";
    if(base != 10 && base != 16 && base != 2 && base != 8) {
        fprintf(stderr, "invalid base \'%lld\', try a different one [10/16/2/8]\n", base);
        exit(EXIT_FAILURE);
    }

    switch(base) {
        case 10: snprintf(ret, 256, "%lld", num); break;
        case 16: snprintf(ret, 256, "%llX", num); break;
        case 8:  snprintf(ret, 256, "%llo", num); break;
        case 2: {
            n = (untype)num;
            for(i = 0; i <= sizeof(untype) * 8; i++) {
                if(n & 1)
                    ret[(sizeof(untype) * 8) - i] = '1';
                else
                    ret[(sizeof(untype) * 8) - i] = '0';
                n >>= 1;
            }
            tmp = &ret[0];
            while(*tmp == '0')
                tmp++;
            return tmp;
        } break;
    }

    return ret;
}

int main(int argc, char **argv)
{
    static ntype base, num;
    int result;

    if(argc < 2) {
        usage();
        exit(EXIT_FAILURE);
    }

    if(!strcmp(argv[1], "--version")) {
        fprintf(stdout, PROGRAM " version: "
                VERSION "\nAuthored by " AUTHOR "\n");
        return EXIT_SUCCESS;
    }

    if(argc < 3) {
        usage();
        exit(EXIT_FAILURE);
    }

    result = parse_number(argv[2], &num);
    if(result) {
        fprintf(stderr, "failed reading number from string \'%s\'\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    result = parse_number(argv[1], &base);
    if(result) {
        fprintf(stderr, "failed reading base from string \'%s\'\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    puts(bswap(base, num));

    return EXIT_SUCCESS;
}
