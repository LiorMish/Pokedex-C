#include "Pokemon.h"
#include "Defs.h"
#include <stdio.h>
#include <string.h>

/////////////////////////////////////////////////// Function declaration

static int isTypeExistInList(type **, char *, int);

/////////////////////////////////////////////////// create structs functions

type *create_pokemon_type(char *name)
{
    type *init = (type *)malloc(sizeof(type));
    if (init == NULL || name == NULL)
        return NULL;
    init->name = (char *)malloc(sizeof(char) * strlen(name) + 1);
    if (init->name == NULL)
        return NULL;
    strcpy(init->name, name);
    init->effective_against_me = NULL;
    init->effective_against_others = NULL;
    init->numOfPokemons = 0;
    init->effective_against_me_len = 0;
    init->effective_against_others_len = 0;
    return init;
}

bioInfo *create_biological_information(double height, double weight, int attack)
{
    bioInfo *init = (bioInfo *)malloc(sizeof(bioInfo));
    if (init == NULL)
    {
        return NULL;
    }
    init->weight = weight;
    init->height = height;
    init->attack = attack;
    return init;
}

pokemon *create_pokemon(char *name, char *species, bioInfo *bio, type *type)
{
    pokemon *p = (pokemon *)malloc(sizeof(pokemon));
    if (name == NULL || species == NULL || p == NULL || bio == NULL)
    {
        return NULL;
    }
    p->name = (char *)malloc(sizeof(char) * strlen(name) + 1);
    if (p->name == NULL)
        return NULL;
    p->species = (char *)malloc(sizeof(char) * strlen(species) + 1);
    if (p->species == NULL)
        return NULL;

    strcpy(p->name, name);
    strcpy(p->species, species);
    p->bio = bio;
    p->type = type;
    type->numOfPokemons++;
    return p;
}

/////////////////////////////////////////////////// add to lists functions

status add_effective_against_me_pokemon(type *a, type *b)
{
    if (a == NULL || b == NULL)
        return failure;

    int index = isTypeExistInList(a->effective_against_me, b->name, a->effective_against_me_len);
    if (index != -1)
        return exist;

    a->effective_against_me = (type **)realloc(a->effective_against_me, (a->effective_against_me_len + 1)*sizeof(type*));
    a->effective_against_me[a->effective_against_me_len] = b;
    a->effective_against_me_len++;
    return success;
}

status add_effective_against_others_pokemon(type *a, type *b)
{
    if (a == NULL || b == NULL)
        return failure;

    int index = isTypeExistInList(a->effective_against_others, b->name, a->effective_against_others_len);
    if (index != -1)
        return exist;

    a->effective_against_others = (type **)realloc(a->effective_against_others, (a->effective_against_others_len + 1)*sizeof(type*));
    a->effective_against_others[a->effective_against_others_len] = b;
    a->effective_against_others_len++;
    return success;
}

/////////////////////////////////////////////////// delete from lists functions

status delete_effective_against_me_pokemon(type *a, char *b)
{
    if (a == NULL || b == NULL)
        return failure;

    int index = isTypeExistInList(a->effective_against_me, b, a->effective_against_me_len);
    if (index == -1)
        return not_exist;

    int j;
    for (j = index + 1; j < a->effective_against_me_len; j++)
    {
        a->effective_against_me[j - 1] = a->effective_against_me[j];
    }
    a->effective_against_me = (type **)realloc(a->effective_against_me, (a->effective_against_me_len - 1)*sizeof(type*));
    a->effective_against_me_len--;

    return success;
}

status delete_effective_against_others_pokemon(type *a, char *b)
{
    if (a == NULL || b == NULL)
        return failure;

    int index = isTypeExistInList(a->effective_against_others, b, a->effective_against_others_len);
    if (index == -1)
        return not_exist;

    int j;
    for (j = index + 1; j < a->effective_against_others_len; j++)
    {
        a->effective_against_others[j - 1] = a->effective_against_others[j];
    }
    a->effective_against_others = (type **)realloc(a->effective_against_others, (a->effective_against_others_len - 1)*sizeof(type*));
    a->effective_against_others_len--;

    return success;
}

/////////////////////////////////////////////////// print functions

status print_pokemon(pokemon *p)
{
    if (p == NULL)
    {
        return failure;
    }
    printf("%s :\n", p->name);
    printf("%s, %s Type.\n", p->species, p->type->name);
    printf("Height: %.2f m    Weight: %.2f kg    Attack: %d", p->bio->height, p->bio->weight, p->bio->attack);
    printf("\n\n");
    return success;
}

status print_pokemon_type(type *t)
{
    if (t == NULL)
    {
        return failure;
    }
    printf("Type %s -- %d pokemons", t->name, t->numOfPokemons);
    if (t->effective_against_me_len > 0)
    {
        printf("\n");
        printf("\tThese types are super-effective against %s:", t->name);

        int i;
        for (i = 0; i < t->effective_against_me_len - 1; i++)
        {
            printf("%s ,", t->effective_against_me[i]->name);
        }
        printf("%s", t->effective_against_me[i]->name);
    }
    if (t->effective_against_others_len > 0)
    {
        printf("\n");
        printf("\t%s moves are super-effective against:", t->name);
        int i;
        for (i = 0; i < t->effective_against_others_len - 1; i++)
        {
            printf("%s ,", t->effective_against_others[i]->name);
        }
        printf("%s", t->effective_against_others[i]->name);
    }
    printf("\n\n");
    return success;
}

/////////////////////////////////////////////////// static functions

static int isTypeExistInList(type **list, char *name, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        if (strcmp(list[i]->name, name) == 0)
            return i;
    }
    return -1;
}

/////////////////////////////////////////////////// free functions

void freeBio(bioInfo* bio){
    free(bio);
}

void freePokemon(pokemon* p){
    free(p->name);
    free(p->species);
    free(p);
}

void freeType(type* type){
    free(type->effective_against_me);
    free(type->effective_against_others);
    free(type->name);
    free(type);
}