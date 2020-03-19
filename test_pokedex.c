// This program was written by Aaron Lin on 29/04/2019
// Tests for pokedex.c

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "pokedex.h"

// Sample data on Bulbasaur, the Pokemon with pokemon_id 1.
#define BULBASAUR_ID 1
#define BULBASAUR_NAME "Bulbasaur"
#define BULBASAUR_HEIGHT 0.7
#define BULBASAUR_WEIGHT 6.9
#define BULBASAUR_FIRST_TYPE GRASS_TYPE
#define BULBASAUR_SECOND_TYPE POISON_TYPE

// Sample data on Ivysaur, the Pokemon with pokemon_id 2.
#define IVYSAUR_ID 2
#define IVYSAUR_NAME "Ivysaur"
#define IVYSAUR_HEIGHT 1.0
#define IVYSAUR_WEIGHT 13.0
#define IVYSAUR_FIRST_TYPE GRASS_TYPE
#define IVYSAUR_SECOND_TYPE POISON_TYPE

// Sample data on Venusaur, the Pokemon with pokemon_id 3
#define VENUSAUR_ID 3
#define VENUSAUR_NAME "Venusaur"
#define VENUSAUR_HEIGHT 2.0
#define VENUSAUR_WEIGHT 100.0
#define VENUSAUR_FIRST_TYPE POISON_TYPE
#define VENUSAUR_SECOND_TYPE GRASS_TYPE

// Sample data on Rattata, the Pokemon with pokemon_id 19
#define RATTATA_ID 19
#define RATTATA_NAME "Rattata"
#define RATTATA_HEIGHT 0.3
#define RATTATA_WEIGHT 3.5
#define RATTATA_FIRST_TYPE NORMAL_TYPE
#define RATTATA_SECOND_TYPE NONE_TYPE

// Sample data on Raticate, the Pokemon with pokemon_id 20
#define RATICATE_ID 20
#define RATICATE_NAME "Raticate"
#define RATICATE_HEIGHT 0.7
#define RATICATE_WEIGHT 18.5
#define RATICATE_FIRST_TYPE NORMAL_TYPE
#define RATICATE_SECOND_TYPE NONE_TYPE

// Sample data on Ekans, the Pokemon with pokemon_id 23
#define EKANS_ID 23
#define EKANS_NAME "Ekans"
#define EKANS_HEIGHT 2.0
#define EKANS_WEIGHT 6.9
#define EKANS_FIRST_TYPE POISON_TYPE
#define EKANS_SECOND_TYPE NONE_TYPE

// Sample data on Arbok, the Pokemon with pokemon_id 24
#define ARBOK_ID 24
#define ARBOK_NAME "Arbok"
#define ARBOK_HEIGHT 3.5
#define ARBOK_WEIGHT 65.0
#define ARBOK_FIRST_TYPE POISON_TYPE
#define ARBOK_SECOND_TYPE NONE_TYPE

// Sample data on Koffing, the Pokemon with pokemon_id 109
#define KOFFING_ID 109
#define KOFFING_NAME "Koffing"
#define KOFFING_HEIGHT 0.6
#define KOFFING_WEIGHT 1.0
#define KOFFING_FIRST_TYPE POISON_TYPE
#define KOFFING_SECOND_TYPE NONE_TYPE

// Sample data on Weezing, the Pokemon with pokemon_id 110
#define WEEZING_ID 110
#define WEEZING_NAME "Weezing"
#define WEEZING_HEIGHT 1.2
#define WEEZING_WEIGHT 9.5
#define WEEZING_FIRST_TYPE POISON_TYPE
#define WEEZING_SECOND_TYPE NONE_TYPE

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

static Pokemon create_venusaur(void);
static Pokemon create_rattata(void);
static Pokemon create_raticate(void);
static Pokemon create_ekans(void);
static Pokemon create_arbok(void);
static Pokemon create_koffing(void);
static Pokemon create_weezing(void);

// Tests for Pokedex functions from pokedex.c.

static void test_new_pokedex(void);
static void test_add_pokemon(void);
static void test_next_pokemon(void);
static void test_get_found_pokemon(void);
static void test_prev_pokemon(void);
static void test_change_current_pokemon(void);
static void test_remove_pokemon(void);
static void test_go_exploring(void);
static void test_add_pokemon_evolution(void);
static void test_get_pokemon_of_type(void);
static void test_search_pokemon(void);

// Helper functions for creating/comparing Pokemon.
static Pokemon create_bulbasaur(void);
static Pokemon create_ivysaur(void);
static int is_same_pokemon(Pokemon first, Pokemon second);
static int is_copied_pokemon(Pokemon first, Pokemon second);



int main(int argc, char *argv[]) {
    printf("Welcome to the COMP1511 Pokedex Tests!\n");

    printf("\n==================== Pokedex Tests ====================\n");

    test_new_pokedex();
    test_add_pokemon();
    test_next_pokemon();
    test_change_current_pokemon();
    test_prev_pokemon();
    test_remove_pokemon();
    test_go_exploring();
    test_add_pokemon_evolution();
    test_get_pokemon_of_type();
    test_get_found_pokemon();
    test_search_pokemon();

    printf("\nAll Pokedex tests passed, you are Awesome!\n");
}


////////////////////////////////////////////////////////////////////////
//                     Pokedex Test Functions                         //
////////////////////////////////////////////////////////////////////////

// `test_new_pokedex` checks whether the new_pokedex and destroy_pokedex
// functions work correctly, to the extent that it can.
//
// It does this by creating a new Pokedex, checking that it's not NULL,
// then calling destroy_pokedex.
//
// Note that it isn't possible to check whether destroy_pokedex has
// successfully destroyed/freed the Pokedex, so the best we can do is to
// call the function and make sure that it doesn't crash..
static void test_new_pokedex(void) {
    printf("\n>> Testing new_pokedex\n");

    printf("    ... Creating a new Pokedex\n");
    Pokedex pokedex = new_pokedex();

    printf("       --> Checking that the returned Pokedex is not NULL\n");
    assert(pokedex != NULL);

    printf("    ... Destroying the Pokedex\n");
    destroy_pokedex(pokedex);

    printf(">> Passed new_pokedex tests!\n");
}

// `test_add_pokemon` checks whether the add_pokemon function works
// correctly.
//
// It does this by creating the Pokemon: Bulbasaur, Ivysaur, Venusaur, 
// Rattata, Raticate, Ekans, Arbok, Koffing, and Weezing (using the helper
// functions in this file and the provided code in pokemon.c), and
// calling add_pokemon to add it to the Pokedex.
//
// Some of the ways that you could extend these test would include:
//   - adding additional Pokemon other than just Bulbasaur,
//   - checking whether the currently selected Pokemon is correctly set,
//   - checking that functions such as `count_total_pokemon` return the
//     correct result after more Pokemon are added,
//   - ... and more!
static void test_add_pokemon(void) {
    
    printf("\n>> Testing add_pokemon\n");

    printf("    ... Creating a new Pokedex\n");
    Pokedex pokedex = new_pokedex();

    printf("    ... Creating Bulbasaur\n");
    Pokemon bulbasaur = create_bulbasaur();

    printf("    ... Adding Bulbasaur to the Pokedex\n");
    add_pokemon(pokedex, bulbasaur);
    
    printf("    ... Creating Ivysaur\n");
    Pokemon ivysaur = create_ivysaur();

    printf("    ... Adding Ivysaur to the Pokedex\n");
    add_pokemon(pokedex, ivysaur);

    printf("    ... Creating Venusaur\n");
    Pokemon venusaur = create_venusaur();

    printf("    ... Adding Venusaur to the Pokedex\n");
    add_pokemon(pokedex, venusaur);
    
    printf("    ... Creating Rattata\n");
    Pokemon rattata = create_rattata();

    printf("    ... Adding Rattata to the Pokedex\n");
    add_pokemon(pokedex, rattata);

    printf("    ... Creating Raticate\n");
    Pokemon raticate = create_raticate();

    printf("    ... Adding Raticate to the Pokedex\n");
    add_pokemon(pokedex, raticate);
    
    printf("    ... Creating Ekans\n");
    Pokemon ekans = create_ekans();

    printf("    ... Adding Ekans to the Pokedex\n");
    add_pokemon(pokedex, ekans);
    
    printf("    ... Creating Arbok\n");
    Pokemon arbok = create_arbok();

    printf("    ... Adding Arbok to the Pokedex\n");
    add_pokemon(pokedex, arbok);
    
    printf("    ... Creating Koffing\n");
    Pokemon koffing = create_koffing();

    printf("    ... Adding Koffing to the Pokedex\n");
    add_pokemon(pokedex, koffing);
    
    printf("    ... Creating Weezing\n");
    Pokemon weezing = create_weezing();

    printf("    ... Adding Weezing to the Pokedex\n");
    add_pokemon(pokedex, weezing);
    
    printf("    ... Checking selected Pokemon\n");
    struct pokenode *curr = pokedex->head;
    // Check that only first pokemon added is selected
    while (curr != NULL) {
        if (curr == pokedex->head) {
            assert(curr->selected == 1);
        } else {
            assert(curr->selected == 0);
        }
        curr = curr->next;
    }
    
    printf("    ... Checking all Pokemon are not found\n");
    assert(count_found_pokemon(pokedex) == 0);
    
    printf("    ... Destroying the Pokedex\n");
    destroy_pokedex(pokedex);

    printf(">> Passed add_pokemon tests!\n");
}

// `test_next_pokemon` checks whether the next_pokemon function works
// correctly.
//
// It does this by creating five Pokemon: Bulbasaur, Ivysaur, Venusaur,  
// Rattata, and Raticate (using the helper functions in this file and 
// the provided code in pokemon.c).
//
// It then adds these to the Pokedex, then checks that calling the
// next_pokemon function changes the currently selected Pokemon from
// Bulbasaur to Ivysaur.
//
// Some of the ways that you could extend these tests would include:
//   - adding even more Pokemon to the Pokedex,
//   - calling the next_pokemon function when there is no "next" Pokemon,
//   - calling the next_pokemon function when there are no Pokemon in
//     the Pokedex,
//   - ... and more!
static void test_next_pokemon(void) {
    printf("\n>> Testing next_pokemon\n");

    printf("    ... Creating a new Pokedex\n");
    Pokedex pokedex = new_pokedex();
    
    printf("       --> Checking that nothing happens when the Pokedex is empty\n");
    next_pokemon(pokedex);

    printf("    ... Creating Bulbasaur, Ivysaur, Venusaur, Rattata, and Raticate\n");
    Pokemon bulbasaur = create_bulbasaur();
    Pokemon ivysaur = create_ivysaur();
    Pokemon venusaur = create_venusaur();
    Pokemon rattata = create_rattata();
    Pokemon raticate = create_raticate();

    printf("    ... Adding Bulbasaur, Ivysaur, Venusaur, Rattata, and Raticate to the Pokedex\n");
    add_pokemon(pokedex, bulbasaur);
    add_pokemon(pokedex, ivysaur);
    add_pokemon(pokedex, venusaur);
    add_pokemon(pokedex, rattata);
    add_pokemon(pokedex, raticate);

    printf("       --> Checking that the current Pokemon is Bulbasaur\n");
    assert(is_same_pokemon(get_current_pokemon(pokedex), bulbasaur));

    printf("    ... Moving to the next pokemon\n");
    next_pokemon(pokedex);

    printf("       --> Checking that the current Pokemon is Ivysaur\n");
    assert(is_same_pokemon(get_current_pokemon(pokedex), ivysaur));
    
    printf("    ... Moving to the next pokemon\n");
    next_pokemon(pokedex);

    printf("       --> Checking that the current Pokemon is Venusaur\n");
    assert(is_same_pokemon(get_current_pokemon(pokedex), venusaur));

    printf("    ... Moving to the next pokemon\n");
    next_pokemon(pokedex);

    printf("       --> Checking that the current Pokemon is Rattata\n");
    assert(is_same_pokemon(get_current_pokemon(pokedex), rattata));
    
    printf("    ... Moving to the next pokemon\n");
    next_pokemon(pokedex);

    printf("       --> Checking that the current Pokemon is Raticate\n");
    assert(is_same_pokemon(get_current_pokemon(pokedex), raticate));
    
    printf("    ... At the end of the Pokedex\n");

    printf("       --> Checking that next_pokemon function does nothing at the end of the Pokedex\n");
    next_pokemon(pokedex);
    assert(is_same_pokemon(get_current_pokemon(pokedex), raticate));

    printf("    ... Destroying the Pokedex\n");
    destroy_pokedex(pokedex);

    printf(">> Passed next_pokemon tests!\n");
}

// `test_prev_pokemon` checks whether the prev_pokemon function works
// correctly.
//
// Call the function once to test the previous funciton does nothing
// in an empty Pokedex.
//
// It does this by creating five Pokemon: Bulbasaur, Ivysaur, Venusaur,  
// Rattata, and Raticate (using the helper functions in this file and 
// the provided code in pokemon.c).
//
// It then adds these to the Pokedex, and sets the currently selected
// Pokemon to the last Pokemon (Raticate). It then checks that calling the
// prev_pokemon function changes the currently selected Pokemon from
// Raticate to Rattata.
//
// Repeat until the first Pokemon (Bulbasaur) is reached and call the function
// once more to ensure that the selected Pokemon remains Bulbasaur

static void test_prev_pokemon(void) {
    printf("\n>> Testing prev_pokemon\n");

    printf("    ... Creating a new Pokedex\n");
    Pokedex pokedex = new_pokedex();
    
    printf("       --> Checking that nothing happens when the Pokedex is empty\n");
    prev_pokemon(pokedex);

    printf("    ... Creating Bulbasaur, Ivysaur, Venusaur, Rattata, and Raticate\n");
    Pokemon bulbasaur = create_bulbasaur();
    Pokemon ivysaur = create_ivysaur();
    Pokemon venusaur = create_venusaur();
    Pokemon rattata = create_rattata();
    Pokemon raticate = create_raticate();

    printf("    ... Adding Bulbasaur, Ivysaur, Venusaur, Rattata, and Raticate to the Pokedex\n");
    add_pokemon(pokedex, bulbasaur);
    add_pokemon(pokedex, ivysaur);
    add_pokemon(pokedex, venusaur);
    add_pokemon(pokedex, rattata);
    add_pokemon(pokedex, raticate);

    printf("       --> Checking that the current Pokemon is Bulbasaur\n");
    assert(is_same_pokemon(get_current_pokemon(pokedex), bulbasaur));

    printf("    ... Setting selected Pokemon as the last Pokemon\n");
    change_current_pokemon(pokedex, RATICATE_ID);
    
    printf("       --> Checking that the current Pokemon is Raticate\n");
    assert(is_same_pokemon(get_current_pokemon(pokedex), raticate));
    
    printf("    ... Moving to the previous pokemon\n");
    prev_pokemon(pokedex);

    printf("       --> Checking that the current Pokemon is Rattata\n");
    assert(is_same_pokemon(get_current_pokemon(pokedex), rattata));
    
    printf("    ... Moving to the previous pokemon\n");
    prev_pokemon(pokedex);

    printf("       --> Checking that the current Pokemon is Venusaur\n");
    assert(is_same_pokemon(get_current_pokemon(pokedex), venusaur));

    printf("    ... Moving to the pprevious pokemon\n");
    prev_pokemon(pokedex);

    printf("       --> Checking that the current Pokemon is Ivysaur\n");
    assert(is_same_pokemon(get_current_pokemon(pokedex), ivysaur));
    
    printf("    ... Moving to the previous pokemon\n");
    prev_pokemon(pokedex);

    printf("       --> Checking that the current Pokemon is Bulbasaur\n");
    assert(is_same_pokemon(get_current_pokemon(pokedex), bulbasaur));
    
    printf("    ... At the start of the Pokedex\n");

    printf("       --> Checking that prev_pokemon function does nothing at the start of the Pokedex\n");
    prev_pokemon(pokedex);
    assert(is_same_pokemon(get_current_pokemon(pokedex), bulbasaur));

    printf("    ... Destroying the Pokedex\n");
    destroy_pokedex(pokedex);

    printf(">> Passed prev_pokemon tests!\n");
}

//'test_change_current_pokemon' checks whether change_current_pokemon 
// function works correctly.
//
// It does this by creating three Pokemon: Bulbasaur, Ivysaur, and 
// Venusaur, and checking whether the function can change the selected 
// pokemon successfully from Bulbasaur (first added pokemon) to 
// Venusaur, and also from Venusaur to Ivysaur.
//
// Also test when changing to a ID not in the Pokedex.

static void test_change_current_pokemon(void) {
    printf("\n>> Testing change_current_pokemon\n");

    printf("    ... Creating a new Pokedex\n");
    Pokedex pokedex = new_pokedex();

    printf("    ... Creating Bulbasaur, Ivysaur, and Venusaur\n");
    Pokemon bulbasaur = create_bulbasaur();
    Pokemon ivysaur = create_ivysaur();
    Pokemon venusaur = create_venusaur();

    printf("    ... Adding Bulbasaur, Ivysaur, and Venusaur to the Pokedex\n");
    add_pokemon(pokedex, bulbasaur);
    add_pokemon(pokedex, ivysaur);
    add_pokemon(pokedex, venusaur);
    
    printf("       --> Checking that the current Pokemon is Bulbasaur\n");
    assert(is_same_pokemon(get_current_pokemon(pokedex), bulbasaur));
    
    printf("    ... Changing current Pokemon to Venusaur\n");
    change_current_pokemon(pokedex, VENUSAUR_ID);
    
    printf("       --> Checking that the current Pokemon is Venusaur\n");
    assert(is_same_pokemon(get_current_pokemon(pokedex), venusaur));
    
    printf("    ... Changing current Pokemon to Ivysaur\n");
    change_current_pokemon(pokedex, IVYSAUR_ID);
    
    printf("       --> Checking that the current Pokemon is Ivysaur\n");
    assert(is_same_pokemon(get_current_pokemon(pokedex), ivysaur));
    
    printf("    ... Changing current Pokemon to a Pokemon not in Pokedex\n");
    change_current_pokemon(pokedex, 999);
    
    printf("       --> Checking that the current Pokemon is still Ivysaur\n");
    assert(is_same_pokemon(get_current_pokemon(pokedex), ivysaur));
    
    printf("    ... Destroying the Pokedex\n");
    destroy_pokedex(pokedex);
    
    printf(">> Passed change_current_pokemon tests!\n");
}

// `test_remove_pokemon` checks whether the get_found_pokemon
// function works correctly.
// 
// It does this by creating three Pokemon: Bulbasaur, Ivysaur, and 
// Venusaur, and checking whether the function successfully removes Ivysaur. 
// 
// Also check that currently selected pokemon is now Venusaur.
//
// Also test when removing the last Pokemon in the Pokedex, the first pokemon,
// and when there are no Pokemon in the Pokedex.

static void test_remove_pokemon(void) {
    printf("\n>> Testing remove_pokemon\n");

    printf("    ... Creating a new Pokedex\n");
    Pokedex pokedex = new_pokedex();

    printf("    ... Creating Bulbasaur, Ivysaur, and Venusaur\n");
    Pokemon bulbasaur = create_bulbasaur();
    Pokemon ivysaur = create_ivysaur();
    Pokemon venusaur = create_venusaur();

    printf("    ... Adding Bulbasaur, Ivysaur, and Venusaur to the Pokedex\n");
    add_pokemon(pokedex, bulbasaur);
    add_pokemon(pokedex, ivysaur);
    add_pokemon(pokedex, venusaur);
    
    printf("       --> Checking that the current Pokemon is Bulbasaur\n");
    assert(is_same_pokemon(get_current_pokemon(pokedex), bulbasaur));
    
    printf("    ... Changing current Pokemon to Ivysaur\n");
    change_current_pokemon(pokedex, IVYSAUR_ID);
    
    printf("       --> Checking that the current Pokemon is Ivysaur\n");
    assert(is_same_pokemon(get_current_pokemon(pokedex), ivysaur));
    
    printf("    ... Removing Ivysaur from the Pokedex\n");
    remove_pokemon(pokedex);
    
    printf("       --> Checking that there are only two pokemon\n");
    assert(count_total_pokemon(pokedex) == 2);
    
    printf("       --> Checking that the current Pokemon is Venusaur\n");
    assert(is_same_pokemon(get_current_pokemon(pokedex), venusaur));
    
    printf("    ... Removing Venusaur from the Pokedex\n");
    remove_pokemon(pokedex);
    
    printf("       --> Checking that there is only one pokemon\n");
    assert(count_total_pokemon(pokedex) == 1);
    
    printf("       --> Checking that the current Pokemon is Bulbasaur\n");
    assert(is_same_pokemon(get_current_pokemon(pokedex), bulbasaur));
    
    printf("    ... Removing Bulbasaur from the Pokedex\n");
    remove_pokemon(pokedex);
    
    printf("       --> Checking that there are no Pokemon in the Pokedex\n");
    assert(count_total_pokemon(pokedex) == 0);
    
    printf("    ... Running remove_pokemon on an empty Pokedex\n");
    remove_pokemon(pokedex);
    
    printf("       --> Checking that nothing happens\n");
    assert(count_total_pokemon(pokedex) == 0);
    
    printf("    ... Destroying the Pokedex\n");
    destroy_pokedex(pokedex);
    
    printf(">> Passed remove_pokemon tests!\n");
}

// `test_go_exploring` checks whether the go_exploring function works
// correctly.
//
// It does this by creating the Pokemon: Bulbasaur, Ivysaur, Venusaur, 
// Rattata, Raticate, Ekans, Arbok, Koffing, and Weezing (using the helper
// functions in this file and the provided code in pokemon.c), and
// calling go_exploring to make sure it finds a certain amount of Pokemon.
//
// It also checks that the range is indeed 0 to 'factor-1' by checking that
// only Bulbasaur is set to found when the factor is set to 2 and also that
// only Ivysaur is set to found when factor is set to 3.

static void test_go_exploring(void) {
    
    printf("\n>> Testing go_exploring\n");

    printf("    ... Creating a new Pokedex\n");
    Pokedex pokedex = new_pokedex();

    printf("    ... Creating Bulbasaur, Ivysaur, Venusaur, Rattata, Raticate,\n");
    printf("    ... Ekans, Arbok, Koffing, and Weezing to the Pokedex\n");
    Pokemon bulbasaur = create_bulbasaur();
    Pokemon ivysaur = create_ivysaur();
    Pokemon venusaur = create_venusaur();
    Pokemon rattata = create_rattata();
    Pokemon raticate = create_raticate();
    Pokemon ekans = create_ekans();
    Pokemon arbok = create_arbok();
    Pokemon koffing = create_koffing();
    Pokemon weezing = create_weezing();
    
    printf("    ... Adding Bulbasaur, Ivysaur, Venusaur, Rattata, Raticate,\n");
    printf("    ... Ekans, Arbok, Koffing, and Weezing to the Pokedex\n");
    add_pokemon(pokedex, bulbasaur);
    add_pokemon(pokedex, ivysaur);
    add_pokemon(pokedex, venusaur);
    add_pokemon(pokedex, rattata);
    add_pokemon(pokedex, raticate);
    add_pokemon(pokedex, ekans);
    add_pokemon(pokedex, arbok);
    add_pokemon(pokedex, koffing);
    add_pokemon(pokedex, weezing);
    
    
    printf("    ... Going exploring for one Pokemon using factor '2' and a seed '1'\n");
    // Bulbasaur must be found if the program is running correctly since
    // it is the only pokemon with ID less than 1 (factor - 1)
    go_exploring(pokedex, 1, 2, 1);
    
    printf("       --> Checking that there is only one Pokemon found\n");
    assert(count_found_pokemon(pokedex) == 1);
    
    printf("       --> Checking that Bulbasaur #001 is the only found Pokemon\n");
    assert(is_same_pokemon(pokedex->head->pokemon, bulbasaur));
    assert(pokedex->head->found == 1);
    
    printf("    ... Going exploring for one Pokemon using factor '3' and a seed '3'\n");
    // Ivysaur must be found if the program is running correctly since
    // it is the only pokemon with ID less than 2 (factor - 1) that is
    // not already found
    go_exploring(pokedex, 3, 3, 1);
    
    printf("       --> Checking that there are now two Pokemon found\n");
    assert(count_found_pokemon(pokedex) == 2);
    
    printf("       --> Checking that Ivysaur #002 is now also found\n");
    assert(is_same_pokemon(pokedex->head->next->pokemon, ivysaur));
    assert(pokedex->head->next->found == 1);
    
    printf("    ... Going exploring for four Pokemon using factor '999'\n");
    printf("    ... and a seed '2'\n");
    go_exploring(pokedex, 2, 999, 4);
    
    printf("       --> Checking that there are now six Pokemon found\n");
    assert(count_found_pokemon(pokedex) == 6);
    
    printf("    ... Destroying the Pokedex\n");
    destroy_pokedex(pokedex);

    printf(">> Passed go_exploring tests!\n");
}

// `test_add_pokemon_evolution` checks whether the add_pokemon_evolution 
// function works correctly.
//
// It does this by creating three Pokemon: Bulbasaur, Ivysaur and 
// Venusaur (using the helper functions in this file and 
// the provided code in pokemon.c) and adding them to the Pokedex
//
// It then sets the evolution of Bulbasaur to be Ivysaur, then sets
// the evolution of Bulbasaur to be Venusaur to check whether it changes
// the end evolution of Bulbasaur.
//
// This will also test the get_next_evolution function by calling it to
// check if the evolution of Bulbasaur is correct.
static void test_add_pokemon_evolution(void) {
    printf("\n>> Testing add_pokemon_evolution\n");

    printf("    ... Creating a new Pokedex\n");
    Pokedex pokedex = new_pokedex();

    printf("    ... Creating Bulbasaur, Ivysaur, and Venusaur\n");
    Pokemon bulbasaur = create_bulbasaur();
    Pokemon ivysaur = create_ivysaur();
    Pokemon venusaur = create_venusaur();

    printf("    ... Adding Bulbasaur, Ivysaur, and Venusaur to the Pokedex\n");
    add_pokemon(pokedex, bulbasaur);
    add_pokemon(pokedex, ivysaur);
    add_pokemon(pokedex, venusaur);

    printf("       --> Checking that the current Pokemon is Bulbasaur\n");
    assert(is_same_pokemon(get_current_pokemon(pokedex), bulbasaur));

    printf("    ... Setting evolution of Bulbasaur to be Ivysaur\n");
    add_pokemon_evolution(pokedex, BULBASAUR_ID, IVYSAUR_ID);

    printf("       --> Checking that Bulbasaur's evolution is Ivysaur\n");
    assert(get_next_evolution(pokedex) == IVYSAUR_ID);
    
    printf("    ... Setting evolution of Bulbasaur to be Venusaur\n");
    add_pokemon_evolution(pokedex, BULBASAUR_ID, VENUSAUR_ID);

    printf("       --> Checking that Bulbasaur's evolution is changed to Venusaur\n");
    assert(get_next_evolution(pokedex) == VENUSAUR_ID);

    printf("    ... Destroying the Pokedex\n");
    destroy_pokedex(pokedex);

    printf(">> Passed add_pokemon_evolution tests!\n");
}

// `test_get_pokemon_of_type` checks whether the get_pokemon_of_type
// function works correctly.
//
// It first does this by calling the function on an empty Pokedex.
//
// It then creates five Pokemon: Bulbasaur, Ivysaur, Venusaur,
// Rattata, and Raticate (using the helper functions in this file and
// the provided code in pokemon.c).
//
// It then adds these to the Pokedex, sets Bulbasaur, Ivysaur, Rattata,
// and Raticate to be found, then calls the get_pokemon_of_type with type
// grass, which would return a pokedex with Bulbasaur and Ivysaur.
static void test_get_pokemon_of_type(void) {
    printf("\n>> Testing get_pokemon_of_type\n");

    printf("    ... Creating a new Pokedex\n");
    Pokedex pokedex = new_pokedex();
    
    printf("       --> Checking if an empty Pokedex is returned when called on an empty Pokedex\n");
    Pokedex empty_type_pokedex = get_pokemon_of_type(pokedex, GRASS_TYPE);
    assert(count_total_pokemon(empty_type_pokedex) == 0);
    
    printf("    ... Destroying empty Pokedex\n"); 
    destroy_pokedex(empty_type_pokedex);

    printf("    ... Creating Bulbasaur, Ivysaur, Venusaur, Rattata, and Raticate\n");
    Pokemon bulbasaur = create_bulbasaur();
    Pokemon ivysaur = create_ivysaur();
    Pokemon venusaur = create_venusaur();
    Pokemon rattata = create_rattata();
    Pokemon raticate = create_raticate();

    printf("    ... Adding Bulbasaur, Venusaur, Rattata, Ivysaur and Raticate to the Pokedex in that order\n");
    add_pokemon(pokedex, bulbasaur);
    add_pokemon(pokedex, venusaur);
    add_pokemon(pokedex, rattata);
    add_pokemon(pokedex, ivysaur);
    add_pokemon(pokedex, raticate);
    
    printf("       --> Checking if an empty Pokedex is returned when no Pokemon have been found\n");
    Pokedex no_found_pokedex = get_pokemon_of_type(pokedex, GRASS_TYPE);
    assert(count_total_pokemon(no_found_pokedex) == 0);
    
    printf("    ... Destroying empty Pokedex\n"); 
    destroy_pokedex(no_found_pokedex);
    
    printf("       --> Checking that the current Pokemon is Bulbasaur\n");
    assert(get_current_pokemon(pokedex) == bulbasaur);
    
    printf("    ... Setting Bulbasaur to be found\n");
    find_current_pokemon(pokedex);
    
    printf("    ... Setting Bulbasaur's evolution to be Ivysaur\n");
    add_pokemon_evolution(pokedex, BULBASAUR_ID, IVYSAUR_ID);
    
    printf("    ... Skipping over Venusaur\n");
    next_pokemon(pokedex);
    next_pokemon(pokedex);
    
    printf("    ... Setting Rattata to be found\n");
    find_current_pokemon(pokedex);
    
    printf("    ... Moving to the next pokemon\n");
    next_pokemon(pokedex);
    
    printf("    ... Setting Ivysaur to be found\n");
    find_current_pokemon(pokedex);
    
    printf("    ... Moving to the next pokemon\n");
    next_pokemon(pokedex);
    
    printf("    ... Setting Raticate to be found\n");
    find_current_pokemon(pokedex);

    printf("    ... Getting all found Grass type Pokemon into a new Pokedex\n");
    Pokedex type_pokedex = get_pokemon_of_type(pokedex, GRASS_TYPE);

    printf("       --> Checking correct amount of Pokemon were copied and were set to be found\n");
    assert(count_total_pokemon(type_pokedex) == 2);
    assert(count_found_pokemon(type_pokedex) == 2);
    
    printf("       --> Checking that the current Pokemon is Bulbasaur\n");
    assert(pokemon_id(get_current_pokemon(type_pokedex)) == BULBASAUR_ID);
    
    printf("       --> Checking that copied Pokemon have no evolutions\n");
    assert(pokemon_id(type_pokedex->head->pokemon) == BULBASAUR_ID);
    assert(type_pokedex->head->evolution == NULL);
    assert(type_pokedex->head->next->evolution == NULL);
    
    printf("       --> Checking whether Pokemon have been successfully 'copied'\n");
    // Make sure that the two pokemon added are copies
    // Order should be Bulbasaur [Grass][Poison] -> Ivysaur [Grass][Poison]
    assert(is_copied_pokemon(get_current_pokemon(type_pokedex), bulbasaur));
    next_pokemon(type_pokedex);
    assert(is_copied_pokemon(get_current_pokemon(type_pokedex), ivysaur));

    printf("    ... Destroying both Pokedexes\n");
    destroy_pokedex(pokedex);
    destroy_pokedex(type_pokedex);

    printf(">> Passed get_pokemon_of_type tests!\n");
}

// `test_get_found_pokemon` checks whether the get_found_pokemon
// function works correctly.
//
// It does this by creating two Pokemon: Bulbasaur and Ivysaur (using
// the helper functions in this file and the provided code in pokemon.c).
//
// It then adds these to the Pokedex, sets Bulbasaur to be found, and
// then calls the get_found_pokemon function to get all of the Pokemon
// which have been found (which should be just the one, Bulbasaur).
//
// Some of the ways that you could extend these tests would include:
//   - calling the get_found_pokemon function on an empty Pokedex,
//   - calling the get_found_pokemon function on a Pokedex where none of
//     the Pokemon have been found,
//   - checking that the Pokemon in the new Pokedex are in ascending
//     order of pokemon_id (regardless of the order that they appeared
//     in the original Pokedex),
//   - checking that the currently selected Pokemon in the returned
//     Pokedex has been set correctly,
//   - checking that the original Pokedex has not been modified,
//   - ... and more!
static void test_get_found_pokemon(void) {
    printf("\n>> Testing get_found_pokemon\n");

    printf("    ... Creating a new Pokedex\n");
    Pokedex pokedex = new_pokedex();
    
    printf("       --> Checking if an empty Pokedex is returned when called on an empty Pokedex\n");
    Pokedex empty_found_pokedex = get_found_pokemon(pokedex);
    assert(count_total_pokemon(empty_found_pokedex) == 0);
    
    printf("    ... Destroying empty Pokedex\n"); 
    destroy_pokedex(empty_found_pokedex);

    printf("    ... Creating Bulbasaur, Ivysaur, Raticate, and Ekans\n");
    Pokemon raticate = create_raticate();
    Pokemon bulbasaur = create_bulbasaur();
    Pokemon ekans = create_ekans();
    Pokemon ivysaur = create_ivysaur();

    printf("    ... Adding Raticate, Bulbasaur, Ekans, and Ivysaur to the Pokedex in that order\n");
    add_pokemon(pokedex, raticate);
    add_pokemon(pokedex, bulbasaur);
    add_pokemon(pokedex, ekans);
    add_pokemon(pokedex, ivysaur);

    printf("       --> Checking if an empty Pokedex is returned when no Pokemon have been found\n");
    Pokedex no_found_pokedex = get_found_pokemon(pokedex);
    assert(count_total_pokemon(no_found_pokedex) == 0);
    
    printf("    ... Destroying empty Pokedex\n"); 
    destroy_pokedex(no_found_pokedex);
    
    printf("       --> Checking that the current Pokemon is Raticate\n");
    assert(get_current_pokemon(pokedex) == raticate);
    
    printf("    ... Setting Raticate to be found\n");
    find_current_pokemon(pokedex);
    
    printf("    ... Moving to the next pokemon\n");
    next_pokemon(pokedex);
    
    printf("    ... Setting Bulbasaur to be found\n");
    find_current_pokemon(pokedex);
    
    printf("    ... Setting Bulbasaur's evolution to be Ivysaur\n");
    add_pokemon_evolution(pokedex, BULBASAUR_ID, IVYSAUR_ID);
    
    printf("    ... Moving to the next pokemon\n");
    next_pokemon(pokedex);
    
    printf("    ... Setting Ekans to be found\n");
    find_current_pokemon(pokedex);

    printf("    ... Getting all found Pokemon into a new Pokedex\n");
    Pokedex found_pokedex = get_found_pokemon(pokedex);

    printf("       --> Checking correct amount of Pokemon were copied and were set to be found\n");
    assert(count_total_pokemon(found_pokedex) == 3);
    assert(count_found_pokemon(found_pokedex) == 3);
    
    printf("       --> Checking that the current Pokemon is Bulbasaur\n");
    assert(pokemon_id(get_current_pokemon(found_pokedex)) == BULBASAUR_ID);
    
    printf("       --> Checking that copied Pokemon have no evolutions\n");
    assert(pokemon_id(found_pokedex->head->pokemon) == BULBASAUR_ID);
    assert(found_pokedex->head->evolution == NULL);
    assert(found_pokedex->head->next->evolution == NULL);
    assert(found_pokedex->head->next->next->evolution == NULL);
    
    printf("       --> Checking whether Pokemon have been successfully 'copied'\n");
    // Make sure that the three pokemon added are copies
    // Order should be Bulbasaur #001 -> Raticate #020 -> Ekans #023
    assert(is_copied_pokemon(get_current_pokemon(found_pokedex), bulbasaur));
    next_pokemon(found_pokedex);
    assert(is_copied_pokemon(get_current_pokemon(found_pokedex), raticate));
    next_pokemon(found_pokedex);
    assert(is_copied_pokemon(get_current_pokemon(found_pokedex), ekans));
    
    printf("       --> Checking new Pokedex is in order of ascending Pokemon ID\n");
    struct pokenode *curr = found_pokedex->head;
    while (curr->next != NULL) {
        assert(pokemon_id(curr->pokemon) < pokemon_id(curr->next->pokemon));
        curr = curr->next;
    }

    printf("    ... Destroying both Pokedexes\n");
    destroy_pokedex(pokedex);
    destroy_pokedex(found_pokedex);

    printf(">> Passed get_found_pokemon tests!\n");
}

// `test_search_pokemon` checks whether the search_pokemon
// function works correctly.
//
// It first does this by calling the function on an empty Pokedex.
//
// It then creates five Pokemon: Bulbasaur, Ivysaur, Venusaur,
// Rattata, and Raticate (using the helper functions in this file and
// the provided code in pokemon.c).
//
// It then adds these to the Pokedex, sets Bulbasaur, Ivysaur, Rattata,
// and Raticate to be found.
//
// It then calls the search_pokemon with 'ra' to create a Pokedex with
// Rattata and Raticate (To check whether it works for when the Pokemon name
// is capitalised but the text is not).
//
// It then calls the function with 'AUr' to see it if works at the end of the
// name, and to check if it adds Venusaur (which is not found)
//
// It will also check if an empty pokedex is returned when there are no matching
// pokemon.
static void test_search_pokemon(void) {
    printf("\n>> Testing search_pokemon\n");

    printf("    ... Creating a new Pokedex\n");
    Pokedex pokedex = new_pokedex();
    
    printf("       --> Checking if an empty Pokedex is returned when called on an empty Pokedex\n");
    Pokedex empty_search_pokedex = search_pokemon(pokedex, "a");
    assert(count_total_pokemon(empty_search_pokedex) == 0);
    
    printf("    ... Destroying empty Pokedex\n"); 
    destroy_pokedex(empty_search_pokedex);

    printf("    ... Creating Bulbasaur, Ivysaur, Venusaur, Rattata, and Raticate\n");
    Pokemon bulbasaur = create_bulbasaur();
    Pokemon ivysaur = create_ivysaur();
    Pokemon venusaur = create_venusaur();
    Pokemon rattata = create_rattata();
    Pokemon raticate = create_raticate();

    printf("    ... Adding Bulbasaur, Venusaur, Rattata, Ivysaur and Raticate to the Pokedex in that order\n");
    add_pokemon(pokedex, bulbasaur);
    add_pokemon(pokedex, venusaur);
    add_pokemon(pokedex, rattata);
    add_pokemon(pokedex, ivysaur);
    add_pokemon(pokedex, raticate);
    
    printf("       --> Checking if an empty Pokedex is returned when no Pokemon has been found\n");
    Pokedex no_found_pokedex = search_pokemon(pokedex, "a");
    assert(count_total_pokemon(no_found_pokedex) == 0);
    
    printf("    ... Destroying empty Pokedex\n"); 
    destroy_pokedex(no_found_pokedex);
    
    printf("       --> Checking that the current Pokemon is Bulbasaur\n");
    assert(get_current_pokemon(pokedex) == bulbasaur);
    
    printf("    ... Setting Bulbasaur to be found\n");
    find_current_pokemon(pokedex);
    
    printf("    ... Setting Bulbasaur's evolution to be Ivysaur\n");
    add_pokemon_evolution(pokedex, BULBASAUR_ID, IVYSAUR_ID);
    
    printf("    ... Skipping over Venusaur\n");
    next_pokemon(pokedex);
    next_pokemon(pokedex);
    
    printf("    ... Setting Rattata to be found\n");
    find_current_pokemon(pokedex);
    
    printf("    ... Setting Rattata's evolution to be Raticate\n");
    add_pokemon_evolution(pokedex, RATTATA_ID, RATICATE_ID);
    
    printf("    ... Moving to the next pokemon\n");
    next_pokemon(pokedex);
    
    printf("    ... Setting Ivysaur to be found\n");
    find_current_pokemon(pokedex);
    
    printf("    ... Moving to the next pokemon\n");
    next_pokemon(pokedex);
    
    printf("    ... Setting Raticate to be found\n");
    find_current_pokemon(pokedex);

    // First search_pokedex using 'ra'
    printf("    ... Getting all Pokemon with 'ra' in the name into a new Pokedex\n");
    Pokedex search_pokedex = search_pokemon(pokedex, "ra");

    printf("       --> Checking correct amount of Pokemon were copied and were set to be found\n");
    assert(count_total_pokemon(search_pokedex) == 2);
    assert(count_found_pokemon(search_pokedex) == 2);
    
    printf("       --> Checking that the current Pokemon is Rattata\n");
    assert(pokemon_id(get_current_pokemon(search_pokedex)) == RATTATA_ID);
    
    printf("       --> Checking that copied Pokemon have no evolutions\n");
    assert(pokemon_id(search_pokedex->head->pokemon) == RATTATA_ID);
    assert(search_pokedex->head->evolution == NULL);
    assert(search_pokedex->head->next->evolution == NULL);
    
    printf("       --> Checking whether Pokemon have been successfully 'copied'\n");
    // Make sure that the two pokemon added are copies
    // Order should be Rattata -> Raticate
    assert(is_copied_pokemon(get_current_pokemon(search_pokedex), rattata));
    next_pokemon(search_pokedex);
    assert(is_copied_pokemon(get_current_pokemon(search_pokedex), raticate));
    
    printf("    ... Destroying search_pokedex\n");
    destroy_pokedex(search_pokedex);
    
    // Second search_pokedex with 'AUr'
    printf("    ... Getting all Pokemon with 'AUr' in the name into a new Pokedex\n");
    search_pokedex = search_pokemon(pokedex, "AUr");

    printf("       --> Checking correct amount of Pokemon were copied and were set to be found\n");
    assert(count_total_pokemon(search_pokedex) == 2);
    assert(count_found_pokemon(search_pokedex) == 2);
    
    printf("       --> Checking that the current Pokemon is Bulbasaur\n");
    assert(pokemon_id(get_current_pokemon(search_pokedex)) == BULBASAUR_ID);
    
    printf("       --> Checking that copied Pokemon have no evolutions\n");
    assert(pokemon_id(search_pokedex->head->pokemon) == BULBASAUR_ID);
    assert(search_pokedex->head->evolution == NULL);
    assert(search_pokedex->head->next->evolution == NULL);
    
    printf("       --> Checking whether Pokemon have been successfully 'copied'\n");
    // Make sure that the two pokemon added are copies
    // Order should be Bulbasaur -> Ivysaur
    assert(is_copied_pokemon(get_current_pokemon(search_pokedex), bulbasaur));
    next_pokemon(search_pokedex);
    assert(is_copied_pokemon(get_current_pokemon(search_pokedex), ivysaur));
    
    printf("    ... Destroying search_pokedex\n");
    destroy_pokedex(search_pokedex);
    
    // Third search_pokedex with 'abcd' - should return an empty pokedex
    printf("    ... Getting all Pokemon with 'abcd' in the name into a new Pokedex\n");
    search_pokedex = search_pokemon(pokedex, "abcd");

    printf("       --> Checking that an empty Pokedex is returned\n");
    assert(count_total_pokemon(search_pokedex) == 0);
    
    printf("    ... Destroying both Pokedexes\n");
    destroy_pokedex(pokedex);
    destroy_pokedex(search_pokedex);

    printf(">> Passed search_pokemon tests!\n");
}

////////////////////////////////////////////////////////////////////////
//                     Helper Functions                               //
////////////////////////////////////////////////////////////////////////

// Helper function to create Bulbasaur for testing purposes.
static Pokemon create_bulbasaur(void) {
    Pokemon pokemon = new_pokemon(
            BULBASAUR_ID, BULBASAUR_NAME,
            BULBASAUR_HEIGHT, BULBASAUR_WEIGHT,
            BULBASAUR_FIRST_TYPE,
            BULBASAUR_SECOND_TYPE
    );
    return pokemon;
}

// Helper function to create Ivysaur for testing purposes.
static Pokemon create_ivysaur(void) {
    Pokemon pokemon = new_pokemon(
            IVYSAUR_ID, IVYSAUR_NAME,
            IVYSAUR_HEIGHT, IVYSAUR_WEIGHT,
            IVYSAUR_FIRST_TYPE,
            IVYSAUR_SECOND_TYPE
    );
    return pokemon;
}

// Helper function to create Venusaur for testing purposes.
static Pokemon create_venusaur(void) {
	Pokemon pokemon = new_pokemon(
        VENUSAUR_ID, VENUSAUR_NAME,
	    VENUSAUR_HEIGHT, VENUSAUR_WEIGHT,
	    VENUSAUR_FIRST_TYPE,
	    VENUSAUR_SECOND_TYPE
	);
    return pokemon;
}

// Helper function to create Rattata for testing purposes.
static Pokemon create_rattata(void) {
	Pokemon pokemon = new_pokemon(
	    RATTATA_ID, RATTATA_NAME,
	    RATTATA_HEIGHT, RATTATA_WEIGHT,
	    RATTATA_FIRST_TYPE,
	    RATTATA_SECOND_TYPE
    );
    return pokemon;
}

// Helper function to create Raticate for testing purposes.
static Pokemon create_raticate(void) {
	Pokemon pokemon = new_pokemon(
	    RATICATE_ID, RATICATE_NAME,
	    RATICATE_HEIGHT, RATICATE_WEIGHT,
	    RATICATE_FIRST_TYPE,
	    RATICATE_SECOND_TYPE
    );
    return pokemon;
}


// Helper function to create Ekans for testing purposes.
static Pokemon create_ekans(void) {
	Pokemon pokemon = new_pokemon(
	    EKANS_ID, EKANS_NAME,
	    EKANS_HEIGHT, EKANS_WEIGHT,
	    EKANS_FIRST_TYPE,
	    EKANS_SECOND_TYPE
	);
    return pokemon;
}

// Helper function to create Arbok for testing purposes.
static Pokemon create_arbok(void) {
	Pokemon pokemon = new_pokemon(
	    ARBOK_ID, ARBOK_NAME,
	    ARBOK_HEIGHT, ARBOK_WEIGHT,
	    ARBOK_FIRST_TYPE,
	    ARBOK_SECOND_TYPE
    );
    return pokemon;
}

// Helper function to create Koffing for testing purposes.
static Pokemon create_koffing(void) {
	Pokemon pokemon = new_pokemon(
	    KOFFING_ID, KOFFING_NAME,
	    KOFFING_HEIGHT, KOFFING_WEIGHT,
	    KOFFING_FIRST_TYPE,
	    KOFFING_SECOND_TYPE
    );
    return pokemon;
}

// Helper function to create Weezing for testing purposes.
static Pokemon create_weezing(void) {
	Pokemon pokemon = new_pokemon(
	    WEEZING_ID, WEEZING_NAME,
	    WEEZING_HEIGHT, WEEZING_WEIGHT,
	    WEEZING_FIRST_TYPE,
	    WEEZING_SECOND_TYPE
    );
    return pokemon;
}


// Helper function to compare whether two Pokemon are the same.
// This checks that the two pointers contain the same address, i.e.
// they are both pointing to the same pokemon struct in memory.
//
// Pokemon ivysaur = new_pokemon(0, 'ivysaur', 1.0, 13.0, GRASS_TYPE, POISON_TYPE)
// Pokemon also_ivysaur = ivysaur
// is_same_pokemon(ivysaur, also_ivysaur) == TRUE
static int is_same_pokemon(Pokemon first, Pokemon second) {
    return first == second;
}

// Helper function to compare whether one Pokemon is a *copy* of
// another, based on whether their attributes match (e.g. pokemon_id,
// height, weight, etc).
// 
// It also checks that the pointers do *not* match -- i.e. that the
// pointers aren't both pointing to the same pokemon struct in memory.
// If the pointers both contain the same address, then the second
// Pokemon is not a *copy* of the first Pokemon.
static int is_copied_pokemon(Pokemon first, Pokemon second) {
    return (pokemon_id(first) == pokemon_id(second))
    &&  (first != second)
    &&  (pokemon_height(first) == pokemon_height(second))
    &&  (pokemon_weight(first) == pokemon_weight(second))
    &&  (pokemon_first_type(first) == pokemon_first_type(second))
    &&  (pokemon_second_type(first) == pokemon_second_type(second));
};