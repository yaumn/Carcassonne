#ifndef LIST_H
#define LIST_H


/**
 * \file list.h
 */


#include <stddef.h>


#define LIST_INIT_CAPACITY 2

// TODO: Handle differents error cases (i.e. add other return values)
#define SUCCESS 0


/**
 * \brief This structure represents a dynamic array containing generic elements
 *
 * This is not a linked list, it is a standard array that can dynamically change in size. Every memory (de)allocation is handled within the structure.
 */
struct list {
    void **d; /*!< The array containing the elements */
    size_t size; /*!< The number of elements in the list */
    size_t capacity; /*!< The maximum number of elements the list can contain without dynamically change its size */
    void *(*copy_element)(void *); /*!< A function pointer to copy a generic element */
    void (*free_element)(void *); /*!< A function pointer to free a generic element */
};


/**
 * \brief Initializes the given list
 * \param list : a pointer to the list to initialize
 * \param copy_element : a function pointer for copying an element
 * \param free_element : a function pointer for freeing an element
 */
void list__init(struct list *list, void *(*copy_element)(void *), void (*free_element)(void *));


/**
 * \brief Inserts an element e into the list at the given index
 * \param list : a pointer to the list
 * \param e : the element to add
 * \param index : the index at which the element is to be added
 * \return SUCCESS if the element was successfully added, !SUCCESS otherwise
 */
int list__insert(struct list *list, void *e, const size_t index);


/**
 * \brief Removes the element located at the given index from the list
 * \param list : a pointer to the list
 * \param index : the index of the element to remove
 * \return SUCCESS if the element was successfully removed, !SUCCESS otherwise
 */
int list__remove(struct list *list, const size_t index);


/**
 * \brief Adds an element at the end of the list
 * \note This is just a convenience function for list__insert(list, e, list->size)
 * \param list : a pointer to the list
 * \param e : the element to add
 * \return SUCCESS if the element was successfully added, !SUCCESS otherwise
 */
int list__push_back(struct list *list, void *e);


/**
 * \brief Removes the last element of the list
 * \note This is just a convenience function for list__remove(list, list->size - 1)
 * \param list : a pointer to the list
 * \return SUCCESS if the element was successfully removed, !SUCCESS otherwise
 */
int list__pop_back(struct list *list);


/**
 * \brief Returns the number of elements in the list
 * \param list : a pointer to the list
 * \return The number of elements in the list
 */
size_t list__size(const struct list *list);


/**
 * \brief Frees the list and all its elements
 * \param list : a pointer to the list to free
 */
void list__free(struct list *list);


/**
 * \brief Returns the data pointer from the ith element
 * \param list : a pointer to the list involved
 * \param i : the element position in the list
 * \retrun the element pointer or NULL if not defined
 */
void * list__get(struct list *list, const size_t i);

#endif // LIST_H
