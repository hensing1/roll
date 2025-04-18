#include <bits/time.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int num_sides;
    int value;
} roll_t;

typedef struct {
    roll_t* rolls;
    size_t num_rolls;
    size_t capacity;
} rolls_t;

void init_rng();
void insert(rolls_t* rolls, roll_t roll);
regex_t make_regex();
void exec_command(char* command, regex_t* regex, rolls_t* rolls);
void print_rolls(rolls_t rolls);
roll_t make_roll(int max);
int substr2int(char* string, size_t start, size_t end);


static char rngstate[32];

int main(int argc, char* argv[]) {
    init_rng();

    rolls_t rolls;
    rolls.capacity = 4;
    rolls.rolls = malloc(rolls.capacity * sizeof(roll_t));
    rolls.num_rolls = 0;

    if (argc == 1) {
        insert(&rolls, make_roll(6));
        print_rolls(rolls);
        free(rolls.rolls);
        return 0;
    }

    regex_t regex = make_regex();

    for (int i = 1; i < argc; i++) {
        exec_command(argv[i], &regex, &rolls);
    }

    print_rolls(rolls);

    if (rolls.num_rolls > 1) {
        long sum = 0;
        for (int i = 0; i < rolls.num_rolls; i++) {
            sum += rolls.rolls[i].value;
        }
        printf("Sum: %ld\n", sum);
    }
    regfree(&regex);
    free(rolls.rolls);
    return 0;
}


void init_rng() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    unsigned int seed = ts.tv_nsec;
    initstate(seed, rngstate, sizeof(rngstate));
    setstate(rngstate);
}


void insert(rolls_t* rolls, roll_t roll) {
    if (rolls->num_rolls == rolls->capacity) {
        rolls->capacity *= 2;
        rolls->rolls = realloc(rolls->rolls, rolls->capacity * sizeof(roll_t));
    }

    rolls->rolls[rolls->num_rolls] = roll;
    rolls->num_rolls++;
}


regex_t make_regex() {
    regex_t regex;
    int ret = regcomp(&regex, "(([0-9]*)d([0-9]+))(\\+([0-9]*)d([0-9]+))*", REG_EXTENDED);
    if (ret) {
        printf("shit fuck\n");
        exit(1);
    }
    return regex;
}


void exec_command(char* command, regex_t* regex, rolls_t* rolls) {

    if (strcmp(command, "ini") == 0 || strcmp(command, "initiative") == 0) {
        insert(rolls, make_roll(20));
        return;
    }

    regmatch_t matches[256];
    int ret = regexec(regex, command, 256, matches, 0);
    if (ret == 0) {
        // first match: entire match
        // then: three matches per capture group
        // 1. entire capture group (e.g. 2d20)
        // 2. num of dice (may be empty)
        // 3. sides of dice

        int i = 1;
        while (matches[i].rm_so != -1) {
            regmatch_t match_num_dice = matches[i + 1];
            regmatch_t match_sides_dice = matches[i + 2];

            int num_dice = (match_num_dice.rm_so == match_num_dice.rm_eo) ?
                1 :
                substr2int(command, match_num_dice.rm_so, match_num_dice.rm_eo);

            int sides_dice = substr2int(command, match_sides_dice.rm_so, match_sides_dice.rm_eo);

            for (int i = 0; i < num_dice; i++) {
                insert(rolls, make_roll(sides_dice));
            }

            i += 3;
        }
    }
    else {
        printf("arg %s not recognized, rolling a d6 instead\n", command);
        insert(rolls, make_roll(6));
    }
}


void print_rolls(rolls_t rolls) {
    for (size_t i = 0; i < rolls.num_rolls; i++) {
        printf("%d\t(d%d)\n", rolls.rolls[i].value, rolls.rolls[i].num_sides);
    }
}


roll_t make_roll(int max) {
    return (roll_t) {
        .num_sides = max,
        .value = random() % max + 1
    };
}


int substr2int(char* string, size_t start, size_t end) {
    int num = 0;
    while (start != end) {
        num *= 10;
        num += string[start] - '0';
        start++;
    }
    return num;
}
