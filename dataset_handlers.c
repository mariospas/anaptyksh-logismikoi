#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "dataset_handlers.h"

/********************** Date *****************************/

ptr_date date_create( size_t year,
                         size_t month,
                         size_t day,
                         size_t hour,
                         size_t minute,
                         size_t second )
{
	ptr_date retval = malloc(sizeof(struct date));
    retval->year = year;
    retval->month = month;
    retval->day = day;
    retval->hour = hour;
    retval->minute = minute;
    retval->second = second;

    return retval;
}

void get_date(ptr_date date,size_t* year,size_t* month,size_t* day,size_t* hour,size_t* minute,size_t* second)
{
	*year = date->year;
	*month = date->month;
	*day = date->day;
	*hour = date->hour;
	*minute = date->minute;
	*second = date->second;
}

/************************ Person **************************/

struct person_info *person_create( int id,
                                   char *first_name,
                                   char *surname,
                                   gender_t gender,
                                   ptr_date birthday,
                                   ptr_date creation_date,
                                   char *location_ip,
                                   char *browser_used )
{
    struct person_info *retval = malloc( sizeof(struct person_info) );
    retval->id = id;
    retval->first_name = strdup( first_name );
    retval->surname = strdup( surname );
    retval->gender = gender;
    retval->birthday = birthday;
    retval->creation_date = creation_date;
    retval->location_ip = strdup( location_ip );
    retval->browser_used = strdup( browser_used );

    return retval;
}

void person_delete( void *obj )
{
    struct person_info *obj1 = obj;
    assert( obj1 != NULL );

    free( obj1->first_name );
    free( obj1->surname );
    free( obj1->location_ip );
    free( obj1->browser_used );
    free( obj1->creation_date );
    free( obj1->birthday );
    free( obj1 );
}

int get_gender(ptr_person_info person)
{
	return person->gender;
}


/************************* Post **************************/

struct post_info *post_create( int id,
                               char *image_file,
                               ptr_date creation_date,
                               char *location_ip,
                               char *browser_used,
                               char *language,
                               char *content )
{
    struct post_info *retval = malloc( sizeof(struct post_info) );
    retval->id = id;
 //   printf("insert in post_create\n");

    if(image_file != NULL)
	{
    //	printf("image_file in post_create\n");
    	retval->image_file = strdup( image_file );
	}

    retval->creation_date = creation_date;
  //  printf("creation_date in post_create\n");

    retval->location_ip = strdup( location_ip );
  //  printf("location_ip in post_create\n");

    retval->browser_used = strdup( browser_used );
   // printf("browser_used in post_create\n");

    if(language != NULL)
	{
    	retval->language = strdup( language );
    	//printf("language in post_create\n");
	}
    if(content != NULL)
	{
    	retval->content = strdup( content );
    	//printf("content in post_create\n");
	}

    //printf("exiting in post_create\n");
    return retval;
}

void post_delete( void *obj )
{
    struct post_info *obj1 = obj;
    assert( obj != NULL );

    free( obj1->image_file );
    free( obj1->location_ip );
    free( obj1->browser_used );
    free( obj1->language );
    free( obj1->content );
    free( obj1->creation_date);
    free( obj1 );
}

/************************** Forum *************************/

struct forum_info *forum_create( int id,
                                 char *title,
                                 ptr_date creation_date )
{
    struct forum_info *retval = malloc( sizeof(struct forum_info) );
    retval->id = id;
    retval->title = strdup( title );
    retval->creation_date = creation_date;

    return retval;
}

void forum_delete( void *obj )
{
    struct forum_info *obj1 = obj;
    assert( obj != NULL );

    free( obj1->title );
    free( obj1->creation_date );
    free( obj1 );
}

/************************* Comment **************************/

struct comment_info *comment_create( int id,
                                     ptr_date creation_date,
                                     char *location_ip,
                                     char *browser_used )
{
    struct comment_info *retval = malloc( sizeof(struct comment_info) );
    retval->id = id;
    retval->creation_date = creation_date;
    retval->location_ip = strdup( location_ip );
    retval->browser_used = strdup( browser_used );

    return retval;
}

void comment_delete( void *obj )
{
    struct comment_info *obj1 = obj;
    assert( obj1 != NULL );

    free( obj1->location_ip );
    free( obj1->browser_used );
    free( obj1->creation_date );
    free( obj1 );
}

/********************* Organisation *************************/

struct organisation_info *organisation_create( int id,
                                               organisation_t type,
                                               char *name,
                                               char *url )
{
    struct organisation_info *retval = malloc( sizeof(struct organisation_info) );
    retval->id = id;
    retval->type = type;
    retval->name = strdup( name );
    retval->url = strdup( url );

    return retval;
}

void organisation_delete( void *obj )
{
    struct organisation_info *obj1 = obj;
    assert( obj1 != NULL );

    free( obj1->name );
    free( obj1->url );
    free( obj1 );
}

/************************* Place *****************************/

struct place_info *place_create( int id,
                                 char *name,
                                 char *url,
                                 place_t type )
{
    struct place_info *retval = malloc( sizeof(struct place_info) );
    retval->id = id;
    retval->name = strdup( name );
    retval->url = strdup ( url );
    retval->type = type;

    return retval;
}

void place_delete( void *obj )
{
    struct place_info *obj1 = obj;
    assert( obj1 != NULL );

    free( obj1->name );
    free( obj1->url );
    free( obj1 );
}

/************************** Tag ******************************/

struct tag_info *tag_create( int id,
                             char *name,
                             char *url )
{
    struct tag_info *retval = malloc( sizeof(struct tag_info) );
    retval->id = id;
    retval->name = strdup( name );
    retval->url = strdup( url );

    return retval;
}

void tag_delete( void *obj )
{
    struct tag_info *obj1 = obj;
    assert( obj1 != NULL );

    free( obj1->name );
    free( obj1->url );
    free( obj1 );
}


char* get_tag_name (ptr_tag_info tag)
{
	return tag->name;
}

/************************ Tagclass **************************/

struct tagclass_info *tagclass_create( int id,
                             char *name,
                             char *url )
{
    struct tagclass_info *retval = malloc( sizeof(struct tagclass_info) );
    retval->id = id;
    retval->name = strdup( name );
    retval->url = strdup( url );

    return retval;
}

void tagclass_delete( void *obj )
{
    struct tagclass_info *obj1 = obj;
    assert( obj1 != NULL );

    free( obj1->name );
    free( obj1->url );
    free( obj1 );
}
