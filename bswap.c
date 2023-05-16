#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROGRAM "bswap"
#define AUTHOR  "rarafael <https://rarafael.net>"
#define VERSION "2.0.0"

typedef signed long long int ntype;
typedef unsigned long long int untype;

static const char *symbols = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void usage(void)
{
    fprintf(stderr, "usage: bswap [output base] [number]\n"
                    "     --version: provides program version\n");
}

ntype parse_number(const char *str)
{
    if(!strlen(str) || !str) {
        fprintf(stderr, "failed parsing input string, empty or none given\n");
        exit(EXIT_FAILURE);
    }

    if(strlen(str) > 2 && str[0] == '0') {
        switch(str[1]) {
            case 'x': return strtoll(str + 2, NULL, 16);
            case '0':
            case 'o': return strtoll(str + 2, NULL, 8);
            case 'b': return strtoll(str + 2, NULL, 2);
        }
    }
        
    return strtoll(str, NULL, 10);

    return 0;
}

char *bswap(ntype base, ntype num)
{
    static char ret[256];
    size_t i, len;
    untype unsign;
    char *ptr = &ret[0];

    if(!num || !base)
        return "0";
    if(base < 2 || base > 35) {
        fprintf(stderr, "invalid base \'%lld\', try a different one [range 2-35]\n", base);
        exit(EXIT_FAILURE);
    }

    if(num < 0 && base != 2) {
        unsign = (~num) + 1;
        *ptr++ = '-';
    } else
        unsign = (untype)num;

    i = 0;
    while(unsign) {
        ptr[i++] = symbols[unsign % base];
        unsign /= base;
    }

    len = strlen(ptr) - 1;
    for(i = 0; i < (len / 2) + 1; i++) {
        char tmp = ptr[i];
        ptr[i] = ptr[len - i];
        ptr[len - i] = tmp;
    }

    return ret;
}

int main(int argc, char **argv)
{
    static ntype base, num;

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

    base = parse_number(argv[1]);
    if(base == LLONG_MAX || base == LLONG_MIN) {
        fprintf(stderr, "failed reading base from string \'%s\', underflow or overflow\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    num = parse_number(argv[2]);
    if(num == LLONG_MAX || num == LLONG_MIN) {
        fprintf(stderr, "failed reading number from string \'%s\', underflow or overflow\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    puts(bswap(base, num));

    return EXIT_SUCCESS;
}
