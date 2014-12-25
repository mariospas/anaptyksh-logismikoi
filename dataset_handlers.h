#ifndef _DATASET_HANDLERS_
#define _DATASET_HANDLERS_

#include <stdlib.h>

#define FIRSTNAME_SIZE 50
#define LASTNAME_SIZE 50
#define IP_SIZE 20
#define BROWSER_SIZE 20

//gia thn proth testmain

struct properties {
	int id_entry;
    int num_of_prop;
    char* name;
    char* surname;
    int age;
    char* type;
    int weight;
};
typedef struct properties *Properties;

Properties createProperties(int number);

void setStringProperty(char* property, int index, Properties p);

void setIntegerProperty(int property, int index, Properties p);

//telos proths testmain

struct date
{
	size_t year;
	size_t month;
	size_t day;
	size_t hour;
	size_t minutes;
	size_t second;

};

typedef struct date *ptr_date;

ptr_date create_date(size_t year,size_t month,size_t day,size_t hour,size_t minutes,size_t second);

struct person_properties
{
	char firstname[FIRSTNAME_SIZE];
	char lastname[LASTNAME_SIZE];
	int gender;                          //0 antras 1 gunaika
	ptr_date birthday;
	ptr_date creationDate;
	char location[IP_SIZE];
	char browserUsed[BROWSER_SIZE];

};

typedef struct person_properties *ptr_person_properties;

ptr_person_properties create_person_properties( char firstname[FIRSTNAME_SIZE],
												char lastname[LASTNAME_SIZE],
                                                int gender,
												ptr_date birthday,
												ptr_date creationDate,
												char location[IP_SIZE],
												char browserUsed[BROWSER_SIZE]);




#endif /* _DATASET_HANDLERS_ */
