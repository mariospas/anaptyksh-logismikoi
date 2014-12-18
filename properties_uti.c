#include "properties_uti.h"
#include <string.h>

//proth testmain


Properties createProperties(int number)
{
    Properties prop = malloc(sizeof(struct properties));
    prop->num_of_prop = number;
    prop->age = -1;
    prop->name = NULL;
    prop->surname = NULL;
    prop->type =  NULL;
    prop->weight = -1;
    return prop;
}

void setStringProperty(char* property, int index, Properties p)
{
	int len = strlen(property) + 1;
    if(p->num_of_prop == 3)
    {
    	if(index == 0)
		{
    		if(p->name == NULL)
    		{
    			p->name = (char*) malloc(len * sizeof(char));
    		}
    		else
    		{
    			free(p->name);
    			p->name = (char*) malloc(len * sizeof(char));
    		}
    		strcpy((p->name),property);
		}
    	else if(index == 1)
    	{
    		if(p->surname == NULL)
			{
				p->surname = (char*) malloc(len * sizeof(char));
			}
    		else
			{
				free(p->surname);
				p->surname = (char*) malloc(len * sizeof(char));
			}
    		strcpy(p->surname,property);
    	}
    }
    else if(p->num_of_prop == 2)
	{
		if(index == 0)
		{
			if(p->type == NULL)
			{
				p->type = (char*) malloc(len * sizeof(char));
			}
			else
			{
				free(p->type);
				p->type = (char*) malloc(len * sizeof(char));
			}
			strcpy(p->type,property);
		}
	}
}


void setIntegerProperty(int property, int index, Properties p)
{
	if(p->num_of_prop == 3)
	{
		if(index == 2) p->age = property;
	}
	else if(p->num_of_prop == 2)
	{
		if(index == 1) p->weight;
	}
}


///telos proth test main



ptr_date create_date(size_t year,size_t month,size_t day,size_t hour,size_t minutes,size_t second)
{
	ptr_date date = malloc(sizeof(struct date));

	date->day = day;
	date->hour = hour;
	date->minutes = minutes;
	date->month = month;
	date->second = second;
	date->year = year;

	return date;
}


ptr_person_properties create_person_properties( char firstname[FIRSTNAME_SIZE],
												char lastname[LASTNAME_SIZE],
                                                int gender,
												ptr_date birthday,
												ptr_date creationDate,
												char location[IP_SIZE],
												char browserUsed[BROWSER_SIZE])
{
	ptr_person_properties person_prop = malloc(sizeof(struct person_properties));

	strcpy(person_prop->firstname,firstname);
	strcpy(person_prop->lastname,lastname);
	strcpy(person_prop->location,location);
	strcpy(person_prop->browserUsed,browserUsed);
	person_prop->gender = gender;
	person_prop->birthday = birthday;
	person_prop->creationDate = creationDate;

	return person_prop;

}
