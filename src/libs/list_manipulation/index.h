#ifndef JUSTREADIT_LIST_INDEX_H
#define JUSTREADIT_LIST_INDEX_H

// ================================
// Type definition
// ================================

/* Bool
 * Definition of a boolean type
 * **/
typedef enum {
    false,
    true
} Bool;

typedef enum {
    ParagraphType,
    LineType,
    WordType,
    LetterType,
    NeuronType,
    LinkType,
    LayerType
} ListElementType;

/* Node
 * Definition of a node of the list
 * */
typedef struct Node {
    void *value;
    struct Node *previous;
    struct Node *next;
} Node;

/* List
 * Definition of a double chained list
 * */
typedef struct List {
    ListElementType elementType;
    unsigned long length;
    struct Node *first;
    struct Node *last;
} *List;

// ================================
// Functions prototypes
// ================================


/* create_list
 * Return:
 *      - List => the new list that has been created
 *
 * Description:
 *      - Return a new list
 * */
List create_list(void);


/* is_empty
 * Input:
 *      - List => the list to check
 * Return:
 *      - Bool => if the list is empty or not
 *
 * Description:
 *      - Check if a list is empty or not
 * */
Bool is_empty(List list);


/* get_length
 * Input:
 *      - List => the list to check
 * Return:
 *      - unsigned long => the length of the list
 *
 * Description:
 *      - Return the length of the list
 * */
unsigned long get_length(List list);


/* get_first_element
 * Input:
 *      - List => the list to check
 * Return:
 *      - Node => the first element of the list (can be null)
 *
 * Description:
 *      - Return the first element of the list
 * */
Node *get_first_element(List list);


/* get_last_element
 * Input:
 *      - List => the list to check
 * Return:
 *      - Node => the last element of the list
 *
 * Description:
 *      - Return the last element of the list
 * */
Node *get_last_element(List list);


/* push_back_list
 * Input:
 *      - list =>  List (the list to use)
 *      - x    => int (the value to add)
 *      - elementType => ListElementType (the type of element)
 * Return:
 *      - List => the list
 *
 * Description:
 *      - Add an element at the back of the list
 * */
List push_back_list(List list, void *x, ListElementType elementType);


/* push_front_list
 * Input:
 *      - list =>  List (the list to use)
 *      - x    => int (the value to add)
 *      - elementType => ListElementType (the type of element)
 * Return:
 *      - List => the list
 *
 * Description:
 *      - Add an element at the front of the list
 * */
List push_front_list(List list, void *x, ListElementType elementType);


/* pop_back_list
 * Input:
 *      - list =>  List (the list to use)
 * Return:
 *      - List => the list
 *
 * Description:
 *      - Remove the element at the end of the list
 * */
List pop_back_list(List list);


/* pop_front_list
 * Input:
 *      - list =>  List (the list to use)
 * Return:
 *      - List => the list
 *
 * Description:
 *      - Remove the element at the start of the list
 * */
List pop_front_list(List list);


/* get_element_by_index
 * Input:
 *      - list =>  List (the list to use)
 *      - i    => int(the index of the wanted value)
 * Return:
 *      - Node => the node at the i position
 *
 * Description:
 *      - return an element by its id
 * */
Node get_element_by_index(List list, int i);

/* print_list
 * Input:
 *      - list =>  List (the list to use)
 *
 * Description:
 *      - print all the key of the list
 * */
void print_list(List list);


/* clear_list
 * Input:
 *      - list =>  List (the list to use)
 *
 * Description:
 *      - Clear the list
 * */
List clear_list(List list);

#endif //JUSTREADIT_INDEX_H
