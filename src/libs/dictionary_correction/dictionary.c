#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 2000
#define LENGTH 45


typedef struct node
{
    char word[LENGTH + 1];
    struct node* next;
}node;
// link to the dico
char* dictionary =  "src/libs/dictionary_correction/dictionary.txt";

node* hashtable[HASH_SIZE];

int number_of_words = 0;


int hash_function(const char* word)
{
    int sum = 0;
    int word_length = strlen(word);

    for (int i = 0; i < word_length; i++)
        sum += word[i];

    int bucket = sum % HASH_SIZE;
    return bucket;
}

char* car_last = "]}),;.:!%?'";
char* car_first = "[{(";

bool is_last_char(char word){
    bool b = false;
    for (size_t i = 0; i <strlen(car_last); i++)
        b = b || car_last[i] == word;
    return b;
}
bool is_first_char(char word){
    bool b = false;
    for (size_t i = 0; i <strlen(car_first); i++)
        b = b || car_first[i] == word;
    return b;
}

bool check_in_dictionary(char* word_to_check)
{
    int word_length = strlen(word_to_check);
    char *word = calloc(word_length + 1, sizeof(char));
    strcpy(word, word_to_check);
    char lower_word[LENGTH+1];
    bool b = true;
    for (int j = 0; j < word_length && b ; j++)
        b = b && isdigit(word[j]);

    if (b)
           return true;

    for (int i = 0; i < word_length; i++)
    {
        if(isupper(word[i]))
            lower_word[i] = tolower(word[i]) ;
        else
            lower_word[i] = word[i];
    }

    if (is_last_char(word[word_length-1])){
        word_length-=1;
        word[word_length] = 0;
    }
    if (is_first_char(word[0])){
        word_length-=1;
        word = word+1;
    }

    lower_word[word_length] = '\0';
    int bucket = hash_function(lower_word);
    node* cursor = hashtable[bucket];
    while (cursor != NULL)
    {
        if (strcmp(lower_word, cursor->word) == 0)
            return true;
        cursor = cursor->next;
    }
    return false;
}


bool load_dictionary()
{
    for(int i = 0; i < HASH_SIZE; i++)
        hashtable[i] = NULL;

    FILE* the_dictionary;
    the_dictionary = fopen(dictionary, "r");
    if (the_dictionary == NULL)
        return false;

    char buffer[LENGTH+1];
    while (fscanf(the_dictionary, "%s", buffer) > 0)
    {
        node* new_node = malloc(sizeof(node));
        new_node->next = NULL;
        strcpy(new_node->word, buffer);
        int bucket = hash_function(new_node->word);
        if (hashtable[bucket] == NULL)
            hashtable[bucket] = new_node;
        else
            new_node->next = hashtable[bucket];
        hashtable[bucket] = new_node;
        number_of_words++;
    }
    fclose(the_dictionary);
    return true;
}




