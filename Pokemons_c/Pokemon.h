#ifndef _POKEMON_H_
#define _POKEMON_H_
#include "Defs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

////////////////////////////////////// Structs
typedef struct Pokemon_Type
{
   char *name;
   int numOfPokemons;
   struct Pokemon_Type **effective_against_me;
   int effective_against_me_len;
   struct Pokemon_Type **effective_against_others;
   int effective_against_others_len;
}type;

typedef struct Biological_Information
{
   double height;
   double weight;
   int attack;
}bioInfo;

typedef struct Pokemon
{
   char *name;
   char *species;
   struct Pokemon_Type *type;
   struct Biological_Information *bio;
}pokemon;

////////////////////////////////////// Functions Declaration
pokemon* create_pokemon(char *, char *, bioInfo *, type *);
type* create_pokemon_type(char *);
bioInfo* create_biological_information(double, double, int);
status add_effective_against_me_pokemon(type *, type *);
status add_effective_against_others_pokemon(type *, type *);
status delete_effective_against_me_pokemon(type *, char *);
status delete_effective_against_others_pokemon(type *, char *);
status print_pokemon(pokemon *);
status print_pokemon_type(type *);
void freeBio(bioInfo*);
void freePokemon(pokemon*);
void freeType(type*);

#endif // _POKEMON_H_