#include <stdio.h>
#include <stdlib.h>
#include "server_vec.h"

// typedef struct _darray
// {
//     size_t size;
//     size_t actual_size;
//     int *content;
// } darray;


void darray_create(darray *d)
{

    d->actual_size = d->size = 0;
    d->content = NULL;
}

void darray_append(darray *d, person_info v)
{
    if (d->size+1 > d->actual_size)
    {
        size_t new_size;
        if (!d->actual_size) 
        { 
            new_size = 1;
        }
        else
        {
            new_size = d->actual_size * 2;
        }

        person_info *temp = realloc(d->content, sizeof(person_info) * new_size);
        if (!temp)
        {
            fprintf(stderr, "Failed to extend array (new_size=%zu)\n", new_size);
            exit(EXIT_FAILURE);
        }

        d->actual_size = new_size;
        d->content = temp;
    }
    d->content[d->size] = v;
    d->size++;
}

person_info* darray_data(darray *d)
{
    return d->content;
}

void darray_erase(darray* d, int v){

    if(d->size==0 || v > d->size-1){
        return;
    }

    int i;
    for(i=v; i<d->size-1; i++){
        d->content[i]=d->content[i+1];
    }

    person_info empty;
    d->content[d->size-1]=empty;
    d->size--;
}

void darray_erase_all(darray* d){
    if(d->size==0){
        return;
    }

    int i;
    for(i=0; i<d->size; i++){
        person_info empty;
        d->content[i]=empty;
    }

    d->size=0;
}

void darray_destroy(darray *d)
{
    free(d->content);
    d->content = NULL;
    d->size = d->actual_size = 0;
}


size_t darray_size(darray *d)
{
    return d->size;
}

