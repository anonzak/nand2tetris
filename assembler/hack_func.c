/*
 * This file contains the definitions for the functions prototyped in
 * "hack_assem.h" and utilized for the HACK assembler program.
 * This file also contains the Symbol Table (sym_tbl) and C-instruction
 * Translation Table (c_tbl) linked list root nodes, declared as global
 * variables.
 */

#include "hack_assem.h"

/*
 * Global variable definitions
 * "sym_head" is the root node of the Symbol Table linked list
 * "c_head" is the root node of the C-instruction Translation Table linked list
 */
sym_node_t *sym_head = NULL;			//CHECK IF THIS IS THE CORRECT WAY TO CREATE A ROOT NODE
c_node_t *c_head = NULL;


/*
 * Recursive function that prints the binary representation of the provided
 * decimal number, which is assumed to be positive.
 * CAVEAT: Will not return any value and is currently not useful/used in the
 * HACK assembler. The function has not been removed, as it may be capable of
 * providing a cleaner version of "get_bin()" with some modification.
 */
void print_binary(unsigned int decimal) {
	if (decimal > 1) {
		print_binary(decimal / 2);
	}
	printf("%d ", decimal % 2);
}


/*
 * This function converts a provided decimal, which is assumed to be positive,
 * to a 15-bit binary representation of the decimal, allowing the assembler to
 * prepend the required op code and output to the machine code file without
 * checking and padding the output.
 * INPUT: (assumed positive) decimal, 15-bit array to store results
 * CAVEAT: This process uses a decrementing counter due to the forward process
 * returning the binary representation in reverse. Using a decrementing counter
 * reverses the order digits in the array, outputing the correct (non-reversed)
 * binary representation. RECURSIVE FUNCTION IS A PREFERRED ALTERNATIVE, IF
 * POSSIBLE.
 */
void get_bin(unsigned int decimal, int bin_arr[15]) {	//THIS MAY BE IMPROVED BY JUST PASSING A POINTER
	for (int i = 14; i >= 0; i--) {						//TO BIN_ARR INSTEAD OF PASSING THE ARRAY
		if (decimal < 1) {
			bin_arr[i] = 0;
		} else {
			bin_arr[i] = decimal % 2;
			decimal /= 2;
		}
	}
}

//Symbol Table functions

/*
 * Function to create a new Symbol Table node, returning a pointer to the
 * new node.
 * MAY REQUIRE MORE WORK -- UNTESTED FUNCTION
 */
void add_sym(char *name, int len, unsigned int value) {
	sym_node_t *n = calloc(1, sizeof(sym_node_t));
	if (n == NULL) {
		mem_error();
	}
	n->sym_name = calloc(len, sizeof(char));
	strncpy(n->sym_name, name, len);

	n->val = value;

	n->next = sym_head;
	sym_head = n;
}


void print_symbol_table(void) {
	sym_node_t *trav = sym_head;
	while (trav != NULL) {
		printf("\t%s = %u\n", trav->sym_name, trav->val);
		trav = trav->next;
	}
}

//C-instruction Translation table functions



/*
 * Free the memory allocated for the Symbol Table and C-instruction Translation
 * table by iterating through both linked lists. This function uses the global
 * variables for the root node of each linked list and does not require the
 * pointers to be passed.
 * MAY REQUIRE MORE WORK -- UNTESTED FUNCTION
 */
bool free_mem(void) {
	sym_node_t *sym_trav = sym_head;		//traversal pointer for Symbol Table linked list
	while (sym_head != NULL) {
		sym_trav = sym_head;
		sym_head = sym_head->next;
		if (sym_trav->sym_name != NULL) {		//if symbol name is set, free that, too
			free(sym_trav->sym_name);
			sym_trav->sym_name = NULL;
		}
		sym_trav->next = NULL;
		free(sym_trav);
	}
	printf("\tAll Symbol Nodes freed.\n");

	c_node_t *c_trav = c_head;			//traversal pointer for C-instruction linked list
	while (c_head != NULL) {
		c_trav = c_head;
		c_head = c_head->next;
		if (c_trav->inst_name != NULL) {
			free(c_trav->inst_name);
			c_trav->inst_name = NULL;
		}
		c_trav->next = NULL;
		free(c_trav);
	}
	printf("\tAll C-instruction Nodes freed.\n");

	return true;
}


/*
 * Function to be called when NULL checking pointers after allocating memory.
 * This function prints an error message indicating that the error rose from
 * issues when allocating memory, then calls the "free_mem()" function in an
 * attempt to free any memory that was successfully allocated earlier in the
 * execution of the program. After the memory has been freed, this fuction
 * exits with an exit code of 1.
 * MAY REQUIRE MORE WORK -- SHOULD EXIT CODE BE REPLACED BY MACROS?
 * (EXIT_SUCCESS or EXIT_FAILURE)
 */
void mem_error(void) {
	printf("Error allocating memory\n");
	free_mem();
	exit(1);
}