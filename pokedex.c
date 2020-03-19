// This program was written by Aaron Lin on 18/04/19
// Tests for pokedex.c

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "pokedex.h"

struct pokedex {
    struct pokenode *head;
};

struct pokenode {
    struct pokenode *next;
    struct pokenode *evolution;

    Pokemon         pokemon;
    int selected;
    int found;
};

static void print_asterisks(char *name);
static void add_pokemon_order(Pokedex pokedex, Pokemon pokemon, int id);
static char char_to_lower(char character) ;
static int text_in_name(char *name, char *text);

Pokedex new_pokedex(void) {
    Pokedex new_pokedex = malloc(sizeof (struct pokedex));
    assert(new_pokedex != NULL);
    new_pokedex->head = NULL;
    return new_pokedex;
}

////////////////////////////////////////////////////////////////////////
//                         Stage 1 Functions                          //
////////////////////////////////////////////////////////////////////////

// Adds Pokemon to the end of the Pokedex
void add_pokemon(Pokedex pokedex, Pokemon pokemon) {
    // Allocate memory to pokenode n
    struct pokenode *n = malloc(sizeof(struct pokenode));
    assert (n != NULL); // exits if no memory has been allocated
    
    // Sets the starting conditions of the pokenode
    n->pokemon = pokemon;
    n->selected = 0;
    n->found = 0;
    n->next = NULL;
    n->evolution = NULL;
    
    // If head is NULL, the Pokedex is currently empty
    if (pokedex->head == NULL) {
        n->selected = 1;
        pokedex->head = n;
    } else { // Pokedex is not empty, add Pokemon to end of the Pokedex
        struct pokenode *current_node = pokedex->head;
        struct pokenode *previous = NULL;
        while (current_node != NULL) {
            // if pokemon_id are the same, it is already in the Pokedex
            if (pokemon_id(current_node->pokemon) == pokemon_id(n->pokemon)){
                fprintf(stderr, "Pokemon already in Pokedex!\n");
                exit(1);
            }
            previous = current_node;
            current_node = current_node->next;
        }
        // Adds on pokemon to end of the Pokedex
        previous->next = n;
        current_node = n;
        
        // Sets the head of the pokedex to be selected
        if (current_node == pokedex->head) {
            current_node->selected = 1;
        }
    }
}

// Prints out all the details of the currently selected pokemon
void detail_pokemon(Pokedex pokedex) {
    struct pokenode *current_node = pokedex->head;
    // Finds the currently selected Pokemon
    while (current_node->selected != 1) {
        current_node = current_node->next;
    }
    printf("Id: %03d\n", pokemon_id(current_node->pokemon));
    
    // Prints out all the information if the Pokemon is found
    if (current_node->found == 1) {
        printf("Name: %s\n", pokemon_name(current_node->pokemon));
        printf("Height: %.1lfm\n", pokemon_height(current_node->pokemon));
        printf("Weight: %.1lfkg\n", pokemon_weight(current_node->pokemon));
        if (pokemon_first_type(current_node->pokemon) == NONE_TYPE) {
            fprintf(stderr, "Type 1 cannot be none type.\n");
            exit(1);
        } else if (pokemon_first_type(current_node->pokemon) == pokemon_second_type(current_node->pokemon)) {
            fprintf(stderr, "Type 1 is the same as type 2.\n");
            exit(1);
        } else if (pokemon_second_type(current_node->pokemon) == NONE_TYPE) {
            printf("Type: %s\n", pokemon_type_to_string(pokemon_first_type(current_node->pokemon)));
        } else {
            printf("Type: %s %s\n", pokemon_type_to_string(pokemon_first_type(current_node->pokemon)), pokemon_type_to_string(pokemon_second_type(current_node->pokemon)));
        }
    } else { // Prints out all the information if the Pokemon has not yet been found
        char *name = pokemon_name(current_node->pokemon);
        printf("Name: ");
        print_asterisks(name);
        printf("\n");
        printf("Height: --\n");
        printf("Weight: --\n");
        printf("Type: --\n");
    }
}

// Returns the pokemon struct of the currently selected Pokemon
Pokemon get_current_pokemon(Pokedex pokedex) {
    // If the Pokedex is not empty
    if (pokedex->head != NULL) {
        struct pokenode *current_node = pokedex->head;
        while (current_node->selected != 1) {
            current_node = current_node->next;
        }
        // Current node is currently selected Pokemon
        return current_node->pokemon;
    } else {
        fprintf(stderr, "Pokedex is currently empty!\n");
        exit(1);
    }
    
}

// Sets currently selected Pokemon to be 'found'
void find_current_pokemon(Pokedex pokedex) {
    if (pokedex->head != NULL) {
        struct pokenode *current_node = pokedex->head;
        while (current_node->selected != 1) {
            current_node = current_node->next;
        }
        current_node->found = 1;
    }
}

// Prints out each Pokemon in the Pokedex in order of when they were added
void print_pokemon(Pokedex pokedex) {
    struct pokenode *current_node = pokedex->head;
    while (current_node != NULL) {
        // Prints an arrow for the selected Pokemon
        if (current_node->selected == 1) {
            printf("--> ");
        } else {
            printf("    ");
        }
        printf("#%03d: ", pokemon_id(current_node->pokemon));
        // Prints the name if found
        if (current_node->found == 1) {
            printf("%s", pokemon_name(current_node->pokemon));
        } else {
            // Prints asterisks equivalent to name length if not found
            print_asterisks(pokemon_name(current_node->pokemon));
        }
        printf("\n");
        current_node = current_node->next;
    }
}

////////////////////////////////////////////////////////////////////////
//                         Stage 2 Functions                          //
////////////////////////////////////////////////////////////////////////

// Moves currently selected Pokemon to the next Pokemon in the Pokedex
void next_pokemon(Pokedex pokedex) {
    if (pokedex->head != NULL) {
        struct pokenode *current_node = pokedex->head;
        while (current_node->selected != 1) {
            current_node = current_node->next;
        }
        current_node->selected = 0;
        // Selected Pokemon remains the same if the function is called at
        // the end of the Pokedex
        if (current_node->next == NULL) {
            current_node->selected = 1;
        } else {
            current_node->next->selected = 1;
        }
    }
}

// Moves currently selected Pokemon to the previous Pokemon in the Pokedex
void prev_pokemon(Pokedex pokedex) {
    if (pokedex->head != NULL) {
        struct pokenode *current_node = pokedex->head;
        struct pokenode *previous = NULL;
        while (current_node->selected != 1) {
            previous = current_node;
            current_node = current_node->next;
        }
        current_node->selected = 0;
        // Selected Pokemon remains the same if the function is called at
        // the end of the Pokedex
        if (previous == NULL) {
            current_node->selected = 1;
        } else {
            previous->selected = 1;
        }
    }
}

// Changes currently selected Pokemon to that with pokemon_id = id
void change_current_pokemon(Pokedex pokedex, int id) {
    struct pokenode *current = pokedex->head;
    struct pokenode *prev_selected = NULL;
    int changed_selected = 0;
    // Finds currently selected Pokemon
    while (current != NULL) {
        if (current->selected == 1) {
            current->selected = 0;
            prev_selected = current;
        }
        if (pokemon_id(current->pokemon) == id) {
            current->selected = 1;
            changed_selected = 1;
        }
        current = current->next;
    }
    // If there is no cange required (i.e. id same as currently selected)
    if (changed_selected == 0) {
        prev_selected->selected = 1;
    }
}

// Removes currently selected Pokemon from Pokedex
void remove_pokemon(Pokedex pokedex) {
    // If Pokedex is not empty
    if (pokedex->head != NULL) {
        struct pokenode *current_node = pokedex->head;
        struct pokenode *previous = NULL;
        // Finds currently selected Pokemon
        while (current_node->selected != 1) {
            previous = current_node;
            current_node = current_node->next;
        }
        // Currently selected is the head of the Pokedex
        if (current_node == pokedex->head) {
            // Sets new head as the next pokenode before freeing the current head
            pokedex->head = current_node->next;
            free(pokemon_name(current_node->pokemon));
            free(current_node->pokemon);
            free(current_node);
            // Sets new head as selected Pokemon if it exists
            if (pokedex->head != NULL) {
                pokedex->head->selected = 1;
            }
        // Currently selected is not the head of the Pokedex
        } else {
            // Skips over the currently selected pokenode
            previous->next = current_node->next;
            if (previous->next != NULL) {
                previous->next->selected = 1;
            } else {
                previous->selected = 1;
            }
            free(pokemon_name(current_node->pokemon));
            free(current_node->pokemon);
            free(current_node);
        } 
    }
}

// Destroys the pokedex and frees everything inside of it
void destroy_pokedex(Pokedex pokedex) {
    if (pokedex->head != NULL) {
        struct pokenode *current_node = pokedex->head;
        struct pokenode *previous = NULL;
        while (current_node->next != NULL) {
            previous = current_node;
            current_node = current_node->next;
            if (previous != NULL) {
                free(pokemon_name(previous->pokemon));
                free(previous->pokemon);
                free(previous);
            }
        }
        if (current_node != NULL) {
            free(pokemon_name(current_node->pokemon));
            free(current_node->pokemon);
            free(current_node);
        }
        free(pokedex);
    } else {
        free(pokedex);
    }
}

////////////////////////////////////////////////////////////////////////
//                         Stage 3 Functions                          //
////////////////////////////////////////////////////////////////////////

// Sets a certain number of random Pokemon to be found
void go_exploring(Pokedex pokedex, int seed, int factor, int how_many) {
    if (pokedex->head != NULL) {
        struct pokenode *current_node = pokedex->head;
        int in_pokedex = 0; // Tells us whether there are enough Pokemon in the
                            // Pokedex in the ID range given
        while (current_node != NULL) {
            if (pokemon_id(current_node->pokemon) >= 0 && 
                pokemon_id(current_node->pokemon) <= (factor - 1) &&
                current_node->found != 1){
                
                in_pokedex += 1;
            }
            current_node = current_node->next;
        }
        if (in_pokedex < how_many) {
            fprintf(stderr, "No Pokemon with ID in that range.\n");
            exit(1);
        }
        // Initialises the random number generator to the provided seed
        srand(seed);
        int i = 0;
        while (i < how_many) {
            int search_id = rand() % (factor); // Random search_id in range of
                                               // 0 to factor - 1
            current_node = pokedex->head;
            while (current_node != NULL) {
                if (search_id == pokemon_id(current_node->pokemon)
                    && current_node->found != 1) {
                    
                    current_node->found = 1;
                    i += 1;
                }
                current_node = current_node->next;
            }
        }
    } else {
        fprintf(stderr, "No Pokemon in Pokedex.\n");
        exit(1);
    }
}

// Returns the number of 'found' Pokemon in the Pokedex
int count_found_pokemon(Pokedex pokedex) {
    int num_found = 0;
    struct pokenode *current_node = pokedex->head;
    while (current_node != NULL) {
        if (current_node->found == 1) {
            num_found += 1;
        }
        current_node = current_node->next;
    }
    
    return num_found;
}

// Returns the total number of Pokemon in the Pokedex
int count_total_pokemon(Pokedex pokedex) {
    int total = 0;
    struct pokenode *current_node = pokedex->head;
    while (current_node != NULL) {
        total += 1;
        current_node = current_node->next;
    }
    
    return total;
}

////////////////////////////////////////////////////////////////////////
//                         Stage 4 Functions                          //
////////////////////////////////////////////////////////////////////////

// Adds a next evolution to the Pokemon with from_id
void add_pokemon_evolution(Pokedex pokedex, int from_id, int to_id) {
    if (from_id == to_id) {
        fprintf(stderr, "Same ID inputted.\n");
        exit(1);
    } else {
        //Finding pokemon with ID equals to from_id and to_id
        struct pokenode *current_node = pokedex->head;
        struct pokenode *evolving_pokemon = NULL;
        struct pokenode *evolution_pokemon = NULL;
        while (current_node != NULL) {
            if (pokemon_id(current_node->pokemon) == from_id) {
                evolving_pokemon = current_node;
            } else if (pokemon_id(current_node->pokemon) == to_id) {
                evolution_pokemon = current_node;
            }
            current_node = current_node->next;
        }
        // If we cannot find either Pokemon, prints an error
        if (evolving_pokemon == NULL || evolution_pokemon == NULL) {
            fprintf(stderr, "Cannot find Pokemon in Pokedex.\n");
            exit(1);
        } else {
            // Sets the evolution of Pokemon with from_id to the Pokemon with to_id
            evolving_pokemon->evolution = evolution_pokemon;
        }
    }
}

// Shows the evolution chain of the currently selected Pokemon
void show_evolutions(Pokedex pokedex) {
    if (pokedex->head != NULL) {
        struct pokenode *current_node = pokedex->head;
        while (current_node->selected != 1) {// Finding selected pokemon
            current_node = current_node->next;
        }
        if (current_node->found == 1) {
            printf("#%03d ", pokemon_id(current_node->pokemon));
            printf("%s ", pokemon_name(current_node->pokemon));
            printf("[%s", pokemon_type_to_string(pokemon_first_type(current_node->pokemon)));
            if (pokemon_second_type(current_node->pokemon) != NONE_TYPE) { // Check there is a second type
                printf(", %s", pokemon_type_to_string(pokemon_first_type(current_node->pokemon)));
            }
            printf("] ");
        } else {
            printf("#%03d ???? [????] ", pokemon_id(current_node->pokemon));
        }
        while (current_node->evolution != NULL) { // Loop through until there is no next evolution
            current_node = current_node->evolution;
            if (current_node->found == 1) {
                printf("--> #%03d ", pokemon_id(current_node->pokemon));
                printf("%s ", pokemon_name(current_node->pokemon));
                printf("[%s", pokemon_type_to_string(pokemon_first_type(current_node->pokemon)));
                if (pokemon_second_type(current_node->pokemon) != NONE_TYPE) { // Check there is a second type
                    printf(", %s", pokemon_type_to_string(pokemon_first_type(current_node->pokemon)));
                }
                printf("] ");
            } else {
                printf("--> #%03d ???? [????] ", pokemon_id(current_node->pokemon));
            }
        }
        printf("\n");
    }
}

// Returns the Pokemon_id of the next evolution of the currently selected Pokemon
int get_next_evolution(Pokedex pokedex) {
    if (pokedex->head != NULL) { // Check if pokedex is not empty
        struct pokenode *current_node = pokedex->head;
        //Goes through pokedex until the selected pokemon is reached
        while (current_node->selected != 1) {
            current_node = current_node->next;
        }
        if (current_node->evolution == NULL) { // No evolution
            return DOES_NOT_EVOLVE;
        } else {
            // Returns pokemon ID of evolution
            return pokemon_id(current_node->evolution->pokemon);
        }
    } else { // Pokedex is empty
        fprintf(stderr, "Pokedex is empty.\n");
        exit(1);
    }
}

////////////////////////////////////////////////////////////////////////
//                         Stage 5 Functions                          //
////////////////////////////////////////////////////////////////////////

// Makes a new Pokedex with Pokemon of the given type
Pokedex get_pokemon_of_type(Pokedex pokedex, pokemon_type type) {
    struct pokedex *new_type_pokedex = new_pokedex();
    if (pokedex->head == NULL) {
        // Returns an empty Pokedex
        return new_type_pokedex;
    } else if (type == NONE_TYPE || type == INVALID_TYPE || type == MAX_TYPE) {
        fprintf(stderr, "Incorrect type name.");
        exit(1);
    // There are Pokemon in the Pokedex and the Type is valid
    } else {
        struct pokenode *current_node = pokedex->head;
        while (current_node != NULL) {
            // Adds a clone of the Pokemon if it is of the same type and is found
            if ((pokemon_first_type(current_node->pokemon) == type ||
                pokemon_second_type(current_node->pokemon) == type) &&
                current_node->found == 1) {
                
                struct pokemon *clone = clone_pokemon(current_node->pokemon);
                add_pokemon(new_type_pokedex, clone);
            }
            current_node = current_node->next;
        }
        current_node = new_type_pokedex->head;
        // Sets all pokemon in new pokedex to be found
        while (current_node != NULL) {
            current_node->found = 1;
            current_node = current_node->next;
        }
        return new_type_pokedex;
    }
}

// Makes a new Pokedex including all the 'found' Pokemon
Pokedex get_found_pokemon(Pokedex pokedex) {
    struct pokedex *new_found_pokedex = new_pokedex();
    if (pokedex->head == NULL) {
        return new_found_pokedex; // Returns an empty pokedex
    } else {
        struct pokenode *current_node = pokedex->head;
        while (current_node != NULL) {
            if (current_node->found == 1) { // If current pokemon is found
                struct pokemon *clone = clone_pokemon(current_node->pokemon);
                add_pokemon_order(new_found_pokedex, clone, pokemon_id(clone));
            }
            current_node = current_node->next;
        }
        return new_found_pokedex;
    }
}

// Makes a new Pokedex with Pokemon that have "text" in their name
Pokedex search_pokemon(Pokedex pokedex, char *text) {
    struct pokedex *new_name_pokedex = new_pokedex();
    if (pokedex->head == NULL) {
        // Returns an empty pokedex
        return new_name_pokedex;
    } else {
        struct pokenode *current_node = pokedex->head;
        while (current_node != NULL) {
            char *name = pokemon_name(current_node->pokemon);
            // If current pokemon is found and the text is in their name
            if (text_in_name(name, text) == 1 && current_node->found == 1) {
                struct pokemon *clone = clone_pokemon(current_node->pokemon);
                add_pokemon(new_name_pokedex, clone);
                
            }
            current_node = current_node->next;
        }
        current_node = new_name_pokedex->head;
        // Sets all pokemon in new pokedex to be found
        while (current_node != NULL) {
            current_node->found = 1;
            current_node = current_node->next;
        }
        return new_name_pokedex;
    }
}

// [EXTRA FUNCTIONS] //

// Prints asterisks to replace the Pokemon name
static void print_asterisks(char *name) {
    int i = 0;
    while (name[i] != '\0') {
        printf("*");
        i += 1;
    }
}

// Adds Pokemon in order of Pokemon ID
static void add_pokemon_order(Pokedex pokedex, Pokemon pokemon, int id) {
    struct pokenode *n = malloc(sizeof(struct pokenode));
    assert (n != NULL);
    n->pokemon = pokemon;
    n->selected = 0;
    n->found = 1;
    n->evolution = NULL;
    if (pokedex->head == NULL) {
        n->selected = 1;
        n->next = NULL;
        pokedex->head = n;
    } else {
        struct pokenode *current_node = pokedex->head;
        struct pokenode *previous = NULL;
        // Loops until there is an ID reached that is greater than the given ID
        // or until the end of the Pokedex is reached
        while (id > pokemon_id(current_node->pokemon) && current_node->next != NULL) {
            if (pokemon_id(current_node->pokemon) == id){
                fprintf(stderr, "Pokemon already in Pokedex!\n");
                exit(1);
            }
            previous = current_node;
            current_node = current_node->next;
        }
        // While loop stopped due to being at end of linked list
        if (id > pokemon_id(current_node->pokemon)) {
            n->next = current_node->next;
            current_node->next = n;
        // If Pokemon is to be placed at start of new pokedex
        } else if (current_node == pokedex->head) {
            n->next = current_node;
            current_node->selected = 0;
            pokedex->head = n;
            n->selected = 1;
        // Somewhere in the middle of the Pokedex
        } else {
            previous->next = n;
            n->next = current_node;
        }
    }
}

// Changes the character given to a lower case
static char char_to_lower(char character) {
    char new_char = character;
    if (new_char >= 'A' && new_char <= 'Z') { 
        new_char = new_char - 'A' + 'a';
    }
    return new_char;
}

// Checks whether the text occurs in the name
static int text_in_name(char *name, char *text) {
    int i = 0;
    int is_in_name = 0;
    int stop = 0;
    while (name[i] != '\0' && stop == 0) {
        int j = 0;
        int still_same = 1;
        // Sets the name and text characters to be lowercase
        char name_char = char_to_lower(name[i]);
        char text_char = char_to_lower(text[j]);
        // If the first letter of the text matches a letter in the name
        if (name_char == text_char) {
            // Loops till the end of text or until they are not the same
            while (text_char != '\0' && still_same == 1) {
                if (name_char == text_char) {
                    is_in_name = 1;
                } else {
                    is_in_name = 0;
                    still_same = 0;
                }
                j += 1;
                // Sets the name and text characters to be the next one in their
                // respective strings
                name_char = char_to_lower(name[i+j]);
                text_char = char_to_lower(text[j]);
                if (name_char == '\0' && text_char != '\0') {
                    stop = 1;
                    still_same = 0;
                }
            }
            // If while loop ended due to reaching the end of text (i.e. the
            // text is in the name!)
            if (is_in_name == 1 && stop == 0) {
                stop = 1;
            // While loop stopped due to text not being in the name
            } else {
                is_in_name = 0;
            }
        }
        i += 1;
    }
    // Integer representing whether the text is in the name
    return is_in_name;
}