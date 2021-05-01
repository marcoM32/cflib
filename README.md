CFLib
=====
This library was developed in C language and does not contain external dependencies other than the stdlib.

The functions of the library allow you to easily work on the generation / validation of Italian codice fiscale, a simple example: 

#include "cf.h"

int main(void) {

	struct tm str_time = { .tm_year = 89, .tm_mon = 7, .tm_mday = 28 };

	person param;
	param.surname = "Rossi";
	param.name = "Mario";
	param.sex = MALE;
	param.birth_day = &str_time;
	param.birth_country = "F205";
	
	tax_code* cf = cf_default_encode_tax_code(&param);
	char* result = cf_encode(cf);
	printf("CF: %s", result);
	free(result);
	cf_tax_code_free(cf);
}

by default the library works using a homocody level = 0 but this level can be increased up to 7
as per the coding specifications of the Italian tax code you can then transform the example above in this way.

#include "cf.h"

int main(void) {

	struct tm str_time = { .tm_year = 89, .tm_mon = 7, .tm_mday = 28 };

	person param;
	param.surname = "Rossi";
	param.name = "Mario";
	param.sex = MALE;
	param.birth_day = &str_time;
	param.birth_country = "F205";
	
	size_t omocode_level = OMOCODE_LEVEL_MAX;
	
	tax_code* cf = cf_encode_tax_code(&param, omocode_level);
	char* result = cf_encode(cf);
	printf("CF: %s", result);
	free(result);
	cf_tax_code_free(cf);
}

However, it is possible to access the individual fields that make up the tax code in this way 

    printf("Surname --> %s\n", cf->surname);
    printf("Name --> %s\n", cf->name);
    printf("Date of birth --> %s\n", cf->date_of_birth_and_sex);
    printf("Birth country --> %s\n", cf->birth_country);
    printf("Control character --> %c\n", cf->control_character);

You can find the binaries for Windows here: 



This library and its source code are released under the terms of this (license)[LICENSE].