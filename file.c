#include "cf.h"
#include "cf.c"

int main(void) {

    // 28-08-89
    struct tm str_time = { .tm_year = 89, .tm_mon = 7, .tm_mday = 28 };

    person param;
    param.surname = "rossi";
    param.name = "mario";
    param.sex = MALE;
    param.birth_day = &str_time;
    param.birth_country = "F205";

    tax_code* cf = cf_default_encode_tax_code(&param);
    char* result = cf_encode(cf);

    printf("Surname --> %s\n", cf->surname);
    printf("Name --> %s\n", cf->name);
    printf("Date of birth --> %s\n", cf->date_of_birth_and_sex);
    printf("Birth country --> %s\n", cf->birth_country);
    printf("Control character --> %c\n", cf->control_character);

    printf("CF: %s\n", result);

    if (strcmp("RSSMRA89M28F205S", result) == 0) {
        printf("Codice fiscale corretto.\n");
    } else {
        fprintf(stderr, "Errore codice fiscale errato");
    }

    free(result);
    cf_tax_code_free(&cf);
}