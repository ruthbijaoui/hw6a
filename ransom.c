#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *readline();
char **split_string(char*);

typedef struct words_node words_node;
typedef struct words_list words_list;

struct words_node {
        char *word;
        words_node *next;
    };

struct words_list {
    words_node *head;
    words_node *tail;
    };

words_node* create_words_node(char *word){
    words_node *node_t = malloc(sizeof(words_node));
    if (node_t==NULL) {
        return(NULL);
    }
    node_t->word = word;
    node_t->next = NULL;
    return node_t;
}

void insert_node_into_linked_list(words_list **list_t, char *word){
    words_node *node_t = create_words_node(word);
    if (!((*list_t)->head)){
        (*list_t)->head = node_t;
    }
    else{
        (*list_t)->tail->next = node_t;
    }
    (*list_t)->tail = node_t;
    return;
}

void free_linked_list(words_node *node_t){
    words_node *cursor_t;
    words_node *cursor_copy_t;
    cursor_t = node_t;
    while (cursor_t) {
        cursor_copy_t= cursor_t;
        cursor_t = cursor_t->next;
        free(cursor_copy_t);
    }
    return;
}

/*This function will create a linked list so that every word from the
*magazine or note will be represented by an element in the list.*/
void make_linked_list(words_list **list, char **text, int count_words) {
    for(int i=0; i<count_words; i++) {
        insert_node_into_linked_list(list,text[i]);
    }
    return;
}

/*This function will delete an element from the linked list of the
*magazine or note when we identify a word that appears in both lists.
*This means that this word can appear on the kidnappers' note.*/
void delete_node(words_list *list_t, char *key) {
    words_node *tmp_t = list_t->head , *prev_t;
    if ((tmp_t != NULL) && (tmp_t->word == key)) {
        list_t->head = tmp_t->next;
        free(tmp_t);
        return;
    }
    while ((tmp_t != NULL) && (tmp_t->word != key)){
        prev_t = tmp_t;
        tmp_t = tmp_t->next;
    }
    if (tmp_t == NULL) {
        return;
    }
    prev_t->next = tmp_t->next;
    free(tmp_t);
}

/*This function will check if the kidnappers can write the note using the
*words from the newspaper, and will print output depending on the result.*/
void checkMagazine(int magazine_count, char **magazine, int note_count,
                   char **note) {
    if (magazine_count < note_count) {
        printf("No");
        return;
    } else {
        /*Create linked lists for the words from the magazine and the note*/
        struct words_list *list_magazine_t;
        struct words_list *list_note_t;

        list_magazine_t = (struct words_list*)malloc(sizeof(struct words_list));
        list_note_t = (struct words_list*)malloc(sizeof(struct words_list));

        make_linked_list(&list_magazine_t, magazine, magazine_count);
        make_linked_list(&list_note_t, note, note_count);

        while (list_note_t->head) {
            words_node *tmp_magazine = list_magazine_t->head;
            words_node *tmp_note = list_note_t->head;
            while((tmp_magazine->next) &&
                  (strcmp(tmp_note->word, tmp_magazine->word))) {
                tmp_magazine = tmp_magazine->next;
            }
            /*When a word appears in both a magazine and a note,
            *it means the kidnappers can use it,
            *so we delete it from the two linked lists.*/
            if (!(strcmp(tmp_note->word, tmp_magazine->word))) {
                delete_node(list_magazine_t, tmp_magazine->word);
                list_note_t->head = list_note_t->head->next;
                free(tmp_note);
                }
            /*We scanned all the words in the magazine but could not
            *complete the contents of the note.*/
            else{
                printf("No");
                free_linked_list(list_magazine_t->head);
                free_linked_list(list_note_t->head);
                free(list_note_t);
                free(list_magazine_t);
                return;
            }
        }
        /*All the elements of the note's linked list have been deleted, which
        *means that the contents of the note have been successfully completed.*/
        free_linked_list(list_magazine_t->head);
        free_linked_list(list_note_t->head);
        free(list_note_t);
        free(list_magazine_t);
        printf("Yes");
        return;
    }
}

int main()
{
    char** mn = split_string(readline());

    char* m_endptr;
    char* m_str = mn[0];
    int m = strtol(m_str, &m_endptr, 10);

    if (m_endptr == m_str || *m_endptr != '\0') { exit(EXIT_FAILURE); }

    char* n_endptr;
    char* n_str = mn[1];
    int n = strtol(n_str, &n_endptr, 10);

    if (n_endptr == n_str || *n_endptr != '\0') { exit(EXIT_FAILURE); }

    char** magazine_temp = split_string(readline());

    char** magazine = malloc(m * sizeof(char*));

    for (int i = 0; i < m; i++) {
        char* magazine_item = *(magazine_temp + i);

        *(magazine + i) = magazine_item;
    }

    int magazine_count = m;

    char** note_temp = split_string(readline());

    char** note = malloc(n * sizeof(char*));

    for (int i = 0; i < n; i++) {
        char* note_item = *(note_temp + i);

        *(note + i) = note_item;
    }

    int note_count = n;

    checkMagazine(magazine_count, magazine, note_count, note);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!line) {
            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);
    } else {
        data = realloc(data, data_length + 1);

        data[data_length] = '\0';
    }

    return data;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);

        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }

    return splits;
}
