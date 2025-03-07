#ifndef CF_HEADER
#define CF_HEADER

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define CF_LIB_VERSION "0.1.0"

#define cf_default_encode_tax_code(T)\
  cf_encode_tax_code(T, 0)

#define cf_default_tax_code_is_valid(P, T) \
    cf_tax_code_is_valid(P, T, 0)

#define DEFAULT_VALUE 'X'

#define OMOCODE_LEVEL_MIN 0
#define OMOCODE_LEVEL_MAX 7

#define SURNAME_PART_LENGTH 3
#define NAME_PART_LENGTH 3

#define DAY_OF_BIRTH_LENGTH 2
#define MONTH_OF_BIRTH_LENGTH 1
#define YEAR_OF_BIRTH_LENGTH 2

#define DATE_OF_BIRTH_AND_SEX_PART_LENGTH \
    DAY_OF_BIRTH_LENGTH + MONTH_OF_BIRTH_LENGTH + YEAR_OF_BIRTH_LENGTH

#define BIRTH_COUNTRY_PART_LENGTH 4
#define CONTROL_CHARACTER_PART_LENGTH 1

#define TAX_CODE_LENGTH \
    SURNAME_PART_LENGTH + NAME_PART_LENGTH + \
    DATE_OF_BIRTH_AND_SEX_PART_LENGTH + BIRTH_COUNTRY_PART_LENGTH + \
    CONTROL_CHARACTER_PART_LENGTH

const char cf_consonants [] = {
    'B', 'C', 'D', 'F', 'G', 'H', 'L',
    'M', 'N', 'P', 'Q', 'R', 'S', 'T',
    'V', 'Z', 'J', 'K', 'W', 'Y', 'X',
    '\0'
};

const char cf_vowels [] = {
    'A', 'E', 'I', 'O', 'U', '\0'
};

const char cf_months [] = {
    'A', 'B', 'C', 'D', 'E', 'H',
    'L', 'M', 'P', 'R', 'S', 'T',
    '\0'
};

const int cf_control_characters_odd_values [36][2] =  {
    { '0', 1 }, { '1', 0 }, { '2', 5 }, { '3', 7 },
    { '4', 9 }, { '5', 13 }, { '6', 15 }, { '7', 17 },
    { '8', 19 }, { '9', 21 }, { 'A', 1 }, { 'B', 0 },
    { 'C', 5 }, { 'D', 7 }, { 'E', 9 }, { 'F', 13 },
    { 'G', 15 }, { 'H', 17 }, { 'I', 19 }, { 'J', 21 },
    { 'K', 2 }, { 'L', 4 }, { 'M', 18 }, { 'N', 20 },
    { 'O', 11 }, { 'P', 3 }, { 'Q', 6 }, { 'R', 8 },
    { 'S', 12 }, { 'T', 14 }, { 'U', 16 }, { 'V', 10 },
    { 'W', 22 }, { 'X', 25 }, { 'Y', 24 }, { 'Z', 23 }
};

const int cf_control_characters_even_values [36][2] =  {
    { '0', 0 }, { '1', 1 }, { '2', 2 }, { '3', 3 },
    { '4', 4 }, { '5', 5 }, { '6', 6 }, { '7', 7 },
    { '8', 8 }, { '9', 9 }, { 'A', 0 }, { 'B', 1 },
    { 'C', 2 }, { 'D', 3 }, { 'E', 4 }, { 'F', 5 },
    { 'G', 6 }, { 'H', 7 }, { 'I', 8 }, { 'J', 9 },
    { 'K', 10 }, { 'L', 11 }, { 'M', 12 }, { 'N', 13 },
    { 'O', 14 }, { 'P', 15 }, { 'Q', 16 }, { 'R', 17 },
    { 'S', 18 }, { 'T', 19 }, { 'U', 20 }, { 'V', 21 },
    { 'W', 22 }, { 'X', 23 }, { 'Y', 24 }, { 'Z', 25 }
};

const char cf_control_characters [] = {
    'A', 'B', 'C', 'D', 'E', 'F',
    'G', 'H', 'I', 'J', 'K', 'L',
    'M', 'N', 'O', 'P', 'Q', 'R',
    'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', '\0'
};


const size_t omocode_positions[] = { 6, 7, 9, 10, 12, 13, 14 };

const char cf_omocodes [] = {
    'L', 'M', 'N', 'P', 'Q', 'R',
    'S', 'T', 'P', 'U', 'V', '\0'
};

typedef enum {
    MALE = 1,
    FEMALE = 40
} gender;

typedef struct {
    char* surname;
    char* name;
    gender sex;
    struct tm* birth_day;
    char* birth_country;
} person;

typedef struct {
    char* surname;
    char* name;
    char* date_of_birth_and_sex;
    char* birth_country;
    char control_character;
    size_t omocode_level;
} tax_code;

static bool cf_find_in_array(const char*, const char);

static char* cf_encode_surname(person*);

static char* cf_encode_name(person*);

static char* cf_encode_date_of_birth_and_sex(person*, size_t);

static char* cf_encode_birth_country(person*, size_t);

static char cf_encode_control_character(person*, size_t);

tax_code* cf_encode_tax_code(person*, size_t);

bool cf_tax_code_is_valid(person*, char*, size_t);

char* cf_encode(tax_code*);

bool cf_tax_code_have_omocode(const char*);

void cf_person_free(person**);

void cf_tax_code_free(tax_code**);

#endif // CF_HEADER
