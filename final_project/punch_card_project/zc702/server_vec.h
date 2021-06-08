#ifndef SERVER_VEC_H /*  1 */
#define SERVER_VEC_H

typedef struct
{
    int userID;
    int location;
    int status; //0:normal, 1:confirmed, 2:isolation
    int day;
} person_info;

typedef struct _darray
{
    size_t size;
    size_t actual_size;
    person_info *content;
} darray;

void darray_create(darray *);

void darray_append(darray *, person_info);

person_info *darray_data(darray *);

void darray_erase(darray *, int);

void darray_erase_all(darray *);

void darray_destroy(darray *);

size_t darray_size(darray *);

#endif
