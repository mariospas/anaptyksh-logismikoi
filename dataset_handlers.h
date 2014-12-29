#ifndef _DATASET_HANDLERS_
#define _DATASET_HANDLERS_

#include <stdlib.h>

/************************ Date ****************************/

struct date {
    size_t year;
    size_t month;
    size_t day;
    size_t hour;
    size_t minute;
    size_t second;
};

struct date date_create( size_t year,
                         size_t month,
                         size_t day,
                         size_t hour,
                         size_t minute,
                         size_t second );

/************************ Person **************************/

typedef enum {
    MALE,
    FEMALE,
} gender_t;

struct person_info {
	int id;
    char *first_name;
    char *surname;
    gender_t gender;
    struct date creation_date;
    char *location_ip;
    char *browser_used;
};

struct person_info *person_create( int id,
                                   char *first_name,
                                   char *surname,
                                   gender_t gender,
                                   struct date creation_date,
                                   char *location_ip,
                                   char *browser_used );
                                   
void person_delete( void *obj );

/************************ Post ****************************/

struct post_info {
    int id;
    char *image_file;
    struct date creation_date;
    char *location_ip;
    char *browser_used;
    char *language;
    char *content;
};


struct post_info *post_create( int id,
                               char *image_file,
                               struct date creation_date,
                               char *location_ip,
                               char *browser_used,
                               char *language,
                               char *content );

void post_delete( void *obj );


/* ********************* Forum *****************************/

struct forum_info {
    int id;
    char *title;
    struct date creation_date;
};

struct forum_info *forum_create( int id,
                                 char *title,
                                 struct date creation_date );

void forum_delete( void *obj );

/********************** Comment ****************************/

struct comment_info {
    int id;
    struct date creation_date;
    char *location_ip;
    char *browser_used;
};

struct comment_info *comment_create( int id,
                                     struct date creation_date,
                                     char *location_ip,
                                     char *browser_used );

void comment_delete( void *obj );

/******************* Organisation **************************/

typedef enum {
    UNIVERSITY,
    COMPANY
} organisation_t;

struct organisation_info {
    int id;
    organisation_t type;
    char *name;
    char *url;
};

struct organisation_info *organisation_create( int id,
                                               organisation_t type,
                                               char *name,
                                               char *url );

void organisation_delete( void *obj );

/************************** Place ***************************/

typedef enum {
    CONTINENT,
    COUNTRY,
    CITY
} place_t;

struct place_info {
    int id;
    char *name;
    char *url;
    place_t type;
};


struct place_info *place_create( int id,
                                 char *name,
                                 char *url,
                                 place_t type );

void place_delete( void *obj );

/*************************** Tag ****************************/

struct tag_info {
    int id;
    char *name;
    char *url;
};


struct tag_info *tag_create( int id,
                             char *name,
                             char *url );

void tag_delete( void *obj );

/************************ Tagclass **************************/

struct tagclass_info {
    int id;
    char *name;
    char *url;
};


struct tagclass_info *tagclass_create( int id,
                                       char *name,
                                       char *url );

void tagclass_delete( void *obj );

#endif /* _DATASET_HANDLERS_ */
