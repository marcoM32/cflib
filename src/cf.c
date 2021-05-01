#include "cf.h"

static bool cf_find_in_array(const char* c, const char value) {
    for(size_t i = 0; c[i]; i++) {
        if(c[i] == value) return true;
    }
    return false;
}

static char* cf_encode_surname(person* param) {
    size_t c = 0;
    char* result = calloc(SURNAME_PART_LENGTH + 1, sizeof(char));
    if(!result) abort();
    for(size_t i = 0; i < strlen(param->surname) && c < SURNAME_PART_LENGTH; i++) {
        if(cf_find_in_array(cf_consonants, param->surname[i])) {
            result[c++] = param->surname[i];
        }
    }
    if(c != SURNAME_PART_LENGTH) {
        for(size_t i = 0; i < strlen(param->surname) && c < SURNAME_PART_LENGTH; i++) {
            if(cf_find_in_array(cf_vowels, param->surname[i])) {
                result[c++] = param->surname[i];
            }
        }
    }
    if(c != SURNAME_PART_LENGTH) {
        while(c < SURNAME_PART_LENGTH) {
            result[c++] = DEFAULT_VALUE;
        }
    }
    return result;
}

static char* cf_encode_name(person* param) {
    size_t c = 0;
    char* result = calloc(NAME_PART_LENGTH + 1, sizeof(char));
    if(!result) abort();

    size_t consonant_occurrences = 0;
    for(size_t i = 0; i < strlen(param->name); i++) {
        if(cf_find_in_array(cf_consonants, param->name[i])) {
            consonant_occurrences++;
        }
    }

    for(size_t i = 0, w = 0; i < strlen(param->name) && c < NAME_PART_LENGTH; i++) {
        if(cf_find_in_array(cf_consonants, param->name[i])) {
            if(consonant_occurrences >= 4) {
                // 1a, la 3a e la 4a
                switch(w) {
                case 0:
                case 2:
                case 3:
                    result[c++] = param->name[i];
                    break;
                }
            } else {
                result[c++] = param->name[i];
            }
            w++;
        }
    }

    if(c != NAME_PART_LENGTH) {
        for(size_t i = 0; i < strlen(param->name) && c < NAME_PART_LENGTH; i++) {
            if(cf_find_in_array(cf_vowels, param->name[i])) {
                result[c++] = param->name[i];
            }
        }
    }
    if(c != NAME_PART_LENGTH) {
        while(c < NAME_PART_LENGTH) {
            result[c++] = DEFAULT_VALUE;
        }
    }
    return result;
}

static char* cf_encode_date_of_birth_and_sex(person* param, size_t omocode_level) {
    char* result = calloc(DATE_OF_BIRTH_AND_SEX_PART_LENGTH + 1, sizeof(char));
    if(!result) abort();
    snprintf(result, DATE_OF_BIRTH_AND_SEX_PART_LENGTH + 1, "%.*d%c%.*d", //
            DAY_OF_BIRTH_LENGTH,
            param->birth_day->tm_year, //
            cf_months[param->birth_day->tm_mon], //
            YEAR_OF_BIRTH_LENGTH,
            param->birth_day->tm_mday + ((param->sex == FEMALE) ? FEMALE : 0)
           );

    if(omocode_level > 3) {
        size_t to_remove = (omocode_level < OMOCODE_LEVEL_MAX) ? omocode_level - 3 : 4;
        for(int i = strlen(result) - 1; i >= 0 && to_remove > 0; i--) {
            if(isdigit(result[i])) {
                result[i] = cf_omocodes[atoi(result + i)];
                to_remove--;
            }
        }
    }

    return result;
}

static char* cf_encode_birth_country(person* param, size_t omocode_level) {
    char* result = calloc(BIRTH_COUNTRY_PART_LENGTH + 1, sizeof(char));
    if(!result) abort();
    snprintf(result, BIRTH_COUNTRY_PART_LENGTH + 1, "%.*s", //
            BIRTH_COUNTRY_PART_LENGTH, //
            param->birth_country //
           );

    if(omocode_level > OMOCODE_LEVEL_MIN) {
        size_t to_remove = (omocode_level > 3) ? 3 : omocode_level;
        for(int i = strlen(result) - 1; i >= 0 && to_remove > 0; i--) {
            if(isdigit(result[i])) {
                result[i] = cf_omocodes[atoi(result + i)];
                to_remove--;
            }
        }
    }

    return result;
}

static char cf_encode_control_character(person* param, size_t omocode_level) {
    char result = '\0';

    size_t string_size = SURNAME_PART_LENGTH +
                   NAME_PART_LENGTH +
                   DATE_OF_BIRTH_AND_SEX_PART_LENGTH +
                   BIRTH_COUNTRY_PART_LENGTH + 1;

    char* tc = calloc(string_size //
                   , //
                   sizeof(char) //
               );
    if(!tc) abort();

    snprintf(tc, string_size, "%.*s%.*s%.*s%.*s", //
            SURNAME_PART_LENGTH, //
            cf_encode_surname(param), //
            NAME_PART_LENGTH, //
            cf_encode_name(param), //
            DATE_OF_BIRTH_AND_SEX_PART_LENGTH, //
            cf_encode_date_of_birth_and_sex(param, omocode_level), //
            BIRTH_COUNTRY_PART_LENGTH, //
            cf_encode_birth_country(param, omocode_level) //
           );

    size_t even_value = 0;
    size_t odd_value = 0;
    for(size_t i = 0, w = 1; i < strlen(tc); i++, w++) {
        if(w % 2 != 0) {
            for(size_t o = 0; o < 36; o++) {
                if(cf_control_characters_odd_values[o][0] == tc[i]) {
                    odd_value += cf_control_characters_odd_values[o][1];
                    break;
                }
            }
        } else {
            for(size_t e = 0; e < 36; e++) {
                if(cf_control_characters_even_values[e][0] == tc[i]) {
                    even_value += cf_control_characters_even_values[e][1];
                    break;
                }
            }
        }
    }
    free(tc);
    tc = NULL;

    result = cf_control_characters[(even_value + odd_value) % 26];
    return result;
}

tax_code* cf_encode_tax_code(person* param, size_t omocode_level) {
    tax_code* tc = calloc(1, sizeof(tax_code));
    if(!tc) abort();
    tc->surname = cf_encode_surname(param);
    tc->name = cf_encode_name(param);
    tc->date_of_birth_and_sex = cf_encode_date_of_birth_and_sex(param, omocode_level);
    tc->birth_country = cf_encode_birth_country(param, omocode_level);
    tc->control_character = cf_encode_control_character(param, omocode_level);
    tc->omocode_level = omocode_level;
    return tc;
}

bool cf_tax_code_is_valid(person* param, char* expected, size_t omocode_level) {
    bool result = false;
    if(expected) {
        tax_code* tc = cf_encode_tax_code(param, omocode_level);
        if(tc) {
           char* generated = cf_encode(tc);
            if(generated) {
                result = strcmp(expected, generated) == 0;
                free(generated);
                generated = NULL;
            }
            cf_tax_code_free(tc);
        }
    }
    return result;
}

char* cf_encode(tax_code* param) {
    char* result = calloc(TAX_CODE_LENGTH + 1, sizeof(char));
    if(!result) abort();

    snprintf(result, TAX_CODE_LENGTH + 1, "%.*s%.*s%.*s%.*s%c", //
            SURNAME_PART_LENGTH, //
            param->surname, //
            NAME_PART_LENGTH, //
            param->name, //
            DATE_OF_BIRTH_AND_SEX_PART_LENGTH, //
            param->date_of_birth_and_sex, //
            BIRTH_COUNTRY_PART_LENGTH, //
            param->birth_country, //
            param->control_character
           );

    return result;
}

bool cf_tax_code_have_omocode(const char* param) {
    bool result = false;
    if(param) {
        const size_t length = strlen(param);
        for(size_t i = 0; i < sizeof(omocode_positions) / sizeof(omocode_positions[0]); i++) {
            if(omocode_positions[i] <= length) {
               if(param[omocode_positions[i]] != '\0' && //
                  !isdigit(param[omocode_positions[i]])) {
                    result = true;
                    break;
               }
            }
        }
    }
    return result;
}

void cf_person_free(person* param) {
    if(param) {
        if(param->surname) free(param->surname), param->surname = NULL;
        if(param->name) free(param->name), param->name = NULL;
        if(param->birth_day) free(param->birth_day), param->birth_day = NULL;
        if(param->birth_country) free(param->birth_country), param->birth_country = NULL;
        free(param);
        param = NULL;
    }
}

void cf_tax_code_free(tax_code* param) {
    if(param) {
        if(param->surname) free(param->surname), param->surname = NULL;
        if(param->name) free(param->name), param->name = NULL;
        if(param->date_of_birth_and_sex) free(param->date_of_birth_and_sex), param->date_of_birth_and_sex = NULL;
        if(param->birth_country) free(param->birth_country), param->birth_country = NULL;
        free(param);
        param = NULL;
    }
}
