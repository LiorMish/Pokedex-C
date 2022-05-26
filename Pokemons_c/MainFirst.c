#include "Pokemon.h"
#include "Defs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define BUFSIZE 300 // max size of user input.

/////////////////////////////////////////////////// Function declaration

static int findType(char *);
static int findPokemon(char *);
static void printPokemons();
static void printPokemonTypes();
static void freePokedex(int, int);
static void readFile(char *);
static void printScreen();

/////////////////////////////////////////////////// Global veriables

pokemon **pokemons = NULL;     // pokemons list.
type **pokemonTypes = NULL;    // types list.
bioInfo **bios = NULL;         // bio informaion list.
int numOfTypes, numOfPokemons; // number of types and pokemons sent in the file.

/////////////////////////////////////////////////// Static functions

static int findType(char *typeName)
{
    if (typeName == NULL)
        return -1;
    int i;
    for (i = 0; i < numOfTypes; i++)
    {
        if (strcmp(pokemonTypes[i]->name, typeName) == 0)
            return i;
    }
    return -1;
}

static int findPokemon(char *typeName)
{
    if (typeName == NULL)
        return -1;
    int i;
    for (i = 0; i < numOfPokemons; i++)
    {
        if (strcmp(pokemons[i]->name, typeName) == 0)
            return i;
    }
    return -1;
}

static void printPokemons()
{
    int i;
    for (i = 0; i < numOfPokemons; i++)
        print_pokemon(pokemons[i]);
}

static void printPokemonTypes()
{
    int i;
    for (i = 0; i < numOfTypes; i++)
        print_pokemon_type(pokemonTypes[i]);
}

/////////////////////////////////////////////////// free function

static void freePokedex(int pokemonsCount, int typesCount)
{
    // free all the types created.
    int i;
    for (i = 0; i < typesCount; i++)
    {
        freeType(pokemonTypes[i]);
    }
    // free all the pokemons and their biology information created.
    for (i = 0; i < pokemonsCount; i++)
    {
        freeBio(bios[i]);
        freePokemon(pokemons[i]);
    }

    // free main lists created.
    if (pokemonsCount > 0)
    {
        free(bios);
        free(pokemons);
    }
    if (typesCount > 0)
        free(pokemonTypes);
}

/////////////////////////////////////////////////// read file function

static void readFile(char *file)
{
    FILE *cFile = fopen(file, "r");
    char line[BUFSIZE];
    int lineCount = 0;
    char *tempWord1, *tempWord2, *tempWord3, *tempWord4, *tempWord5, *tempWord6;
    char *me = "effective-against-me";
    char *other = "effective-against-other";
    int index1, index2;

    //////////////////////////////////////////////////////////////////////////////// create types
    pokemonTypes = (type **)malloc(sizeof(type *) * numOfTypes);
    if (pokemonTypes == NULL) // Memory allocation glitch
        printf("Memory Problem");

    while (fscanf(cFile, "%s", line) == 1)
    {
        if (lineCount == 1)
        {
            tempWord1 = strtok(line, ",");
            int i;
            for (i = 0; i < numOfTypes; i++)
            {
                pokemonTypes[i] = create_pokemon_type(tempWord1);
                if (pokemonTypes[i] == NULL) // Memory allocation glitch
                {
                    freePokedex(0, i); // free all the types that created until now.
                    printf("Memory Problem");
                }
                tempWord1 = strtok(NULL, ",");
            }
            lineCount++;
            continue;
        }
        //////////////////////////////////////////////////////////////////////////////////// add types to lists
        if (lineCount == 2)
        {
            while (strcmp(line, "Pokemons") != 0)
            {
                index1 = findType(line);
                fscanf(cFile, "%s", line);
                tempWord1 = strtok(line, ":");

                if (strcmp(tempWord1, me) == 0)
                {
                    tempWord2 = strtok(NULL, ",");
                    while (tempWord2 != NULL)
                    {

                        index2 = findType(tempWord2);
                        add_effective_against_me_pokemon(pokemonTypes[index1], pokemonTypes[index2]);
                        tempWord2 = strtok(NULL, ",");
                    }
                }
                else
                {
                    tempWord2 = strtok(NULL, ",");
                    while (tempWord2 != NULL)
                    {

                        index2 = findType(tempWord2);
                        add_effective_against_others_pokemon(pokemonTypes[index1], pokemonTypes[index2]);
                        tempWord2 = strtok(NULL, ",");
                    }
                }

                fscanf(cFile, "%s", line);
            }
            lineCount++;
            continue;
        }
        lineCount++;
        ///////////////////////////////////////////////////////////////////// create pokemons

        if (lineCount > 2)
        {

            bios = (bioInfo **)malloc(sizeof(bioInfo *) * numOfPokemons);
            if (bios == NULL) // Memory allocation glitch
            {
                freePokedex(0, numOfTypes); // free all the types that created until now.
                printf("Memory Problem");
            }
            pokemons = (pokemon **)malloc(sizeof(pokemon *) * numOfPokemons);
            if (pokemons == NULL) // Memory allocation glitch
            {
                freePokedex(0, numOfTypes); // free all the types that created until now.
                printf("Memory Problem");
            }
            int i;
            for (i = 0; i < numOfPokemons; i++)
            {
                tempWord1 = strtok(line, ",");
                tempWord2 = strtok(NULL, ",");
                tempWord3 = strtok(NULL, ",");
                tempWord4 = strtok(NULL, ",");
                tempWord5 = strtok(NULL, ",");
                tempWord6 = strtok(NULL, ",");

                bios[i] = create_biological_information(atof(tempWord3), atof(tempWord4), atoi(tempWord5));
                if (bios[i] == NULL) // Memory allocation glitch
                {
                    freePokedex(i, numOfTypes); // free all the pokemons and types that created until now.
                    printf("Memory Problem");
                }

                int index = findType(tempWord6);
                if (index != -1)
                {
                    pokemons[i] = create_pokemon(tempWord1, tempWord2, bios[i], pokemonTypes[index]);
                    if (pokemons[i] == NULL) // Memory allocation glitch
                    {
                        freePokedex(i, numOfTypes); // free all the pokemons and types that created until now.
                        printf("Memory Problem");
                    }
                }

                fscanf(cFile, "%s", line);
            }
        }
    }

    fclose(cFile); /* close the file */
}

static void printScreen()
{
    printf("Please choose one of the following numbers:\n");
    printf("1 : Print all Pokemons\n");
    printf("2 : Print all Pokemons types\n");
    printf("3 : Add type to effective against me list\n");
    printf("4 : Add type to effective against others list\n");
    printf("5 : Remove type from effective against me list\n");
    printf("6 : Remove type from effective against others list\n");
    printf("7 : Print Pokemon by name\n");
    printf("8 : Print Pokemons by type\n");
    printf("9 : Exit\n");
}

/////////////////////////////////////////////////// Main

int main(int argc, char *argv[])
{
    if (argc != 4)
        printf("Not enough arguments");

    numOfTypes = atoi(argv[1]);
    numOfPokemons = atoi(argv[2]);

    ////////////////////////////////////////////////////////////////////////////////// read the file
    readFile(argv[3]);

    status s;
    int index1, index2;
    char input;
    char typeName1[BUFSIZE], typeName2[BUFSIZE];

    do
    {
        ////////////////////////////////////////////////////////////////////////////////// cases
        printScreen();

        scanf(" %c", &input); // to "eat up" the trailing '\n'

        if ((input > '9') || (input < '1')) // check valid input
            printf("Please choose a valid number.\n");


        switch (input)
        {
            /////////////////////////////////////////////////////////////////////////////// case 1

        case '1': /* Print all Pokemons */

            printPokemons();
            break;

            /////////////////////////////////////////////////////////////////////////////// case 2

        case '2': /* Print all Pokemons types */

            printPokemonTypes();
            break;

            /////////////////////////////////////////////////////////////////////////////// case 3

        case '3': /* Add type to effective against me list */
            printf("Please enter type name:\n");
            scanf("%s", typeName1);

            index1 = findType(typeName1);
            if (index1 == -1)
            {
                printf("Type name doesn't exist.\n");
                break;
            }
            printf("Please enter type name to add to %s effective against me list:\n", typeName1);
            scanf("%s", typeName2);

            index2 = findType(typeName2);
            if (index2 == -1)
            {
                printf("Type name doesn't exist.\n");
                break;
            }

            s = add_effective_against_me_pokemon(pokemonTypes[index1], pokemonTypes[index2]);
            if (s == exist) // check if this type is already exist in the list.
            {
                printf("This type already exist in the list.\n");
                break;
            }
            print_pokemon_type(pokemonTypes[index1]);
            break;

            /////////////////////////////////////////////////////////////////////////////// case 4

        case '4': /* Add type to effective against others list */
            printf("Please enter type name:\n");
            scanf("%s", typeName1);

            index1 = findType(typeName1);
            if (index1 == -1)
            {
                printf("Type name doesn't exist.\n");
                break;
            }
            printf("Please enter type name to add to %s effective against others list:\n", typeName1);
            scanf("%s", typeName2);

            index2 = findType(typeName2);
            if (index2 == -1)
            {
                printf("Type name doesn't exist.\n");
                break;
            }

            s = add_effective_against_others_pokemon(pokemonTypes[index1], pokemonTypes[index2]);
            if (s == exist) // check if this type is already exist in the list.
            {
                printf("This type already exist in the list.\n");
                break;
            }
            print_pokemon_type(pokemonTypes[index1]);
            break;

            ///////////////////////////////////////////////////////////////////////////////  case 5

        case '5': /* Remove type from effective against me list */
            printf("Please enter type name:\n");
            scanf("%s", typeName1);

            index1 = findType(typeName1);
            if (index1 == -1)
            {
                printf("Type name doesn't exist.\n");
                break;
            }
            printf("Please enter type name to remove from %s effective against me list:\n", typeName1);
            scanf("%s", typeName2);

            index2 = findType(typeName2);
            if (index2 == -1)
            {
                printf("Type name doesn't exist in the list.\n");
                break;
            }

            s = delete_effective_against_me_pokemon(pokemonTypes[index1], pokemonTypes[index2]->name);
            if (s == not_exist)
            {
                printf("Type name doesn't exist in the list.\n");
                break;
            }
            print_pokemon_type(pokemonTypes[index1]);
            break;

            ///////////////////////////////////////////////////////////////////////////////  case 6

        case '6': /* Remove type from effective against others list */
            printf("Please enter type name:\n");
            scanf("%s", typeName1);

            index1 = findType(typeName1);
            if (index1 == -1)
            {
                printf("Type name doesn't exist.\n");
                break;
            }
            printf("Please enter type name to remove from %s effective against others list:\n", typeName1);
            scanf("%s", typeName2);

            index2 = findType(typeName2);
            if (index2 == -1)
            {
                printf("Type name doesn't exist in the list.\n");
                break;
            }

            s = delete_effective_against_others_pokemon(pokemonTypes[index1], pokemonTypes[index2]->name);
            if (s == not_exist)
            {
                printf("Type name doesn't exist in the list.\n");
                break;
            }
            print_pokemon_type(pokemonTypes[index1]);
            break;

            ///////////////////////////////////////////////////////////////////////////////  case 7

        case '7': /* Print Pokemon by name */
            printf("Please enter Pokemon name:\n");
            scanf("%s", typeName1);
            index1 = findPokemon(typeName1);
            if (index1 == -1)
            {
                printf("The Pokemon doesn't exist.\n");
                break;
            }

            print_pokemon(pokemons[index1]);
            break;

            ///////////////////////////////////////////////////////////////////////////////  case 8

        case '8': /* Print Pokemons by type */
            printf("Please enter type name:\n");
            scanf("%s", typeName1);
            index1 = findType(typeName1);
            if (index1 == -1)
            {
                printf("Type name doesn't exist.\n");
                break;
            }
            if (pokemonTypes[index1]->numOfPokemons == 0)
            {
                printf("There are no Pokemons with this type.\n");
                break;
            }
            printf("There are %d Pokemons with this type:\n", pokemonTypes[index1]->numOfPokemons);
            int i;
            for (i = 0; i < numOfPokemons; i++)
            {
                if (strcmp(pokemons[i]->type->name, typeName1) == 0)
                    print_pokemon(pokemons[i]);
            }
            break;

            ///////////////////////////////////////////////////////////////////////////////  case 9

        case '9':                                   /* Exit */
            freePokedex(numOfPokemons, numOfTypes); // free all the pokemons and types that created.
            printf("All the memory cleaned and the program is safely closed.\n");
            break;
        }
    } while (input != '9');

    return 0;
}
