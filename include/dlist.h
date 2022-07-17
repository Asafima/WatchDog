/*
    team: OL125-126    
    version: 1.0

*/
#ifndef __LIST_H__
#define __LIST_H__

#include <stddef.h> /* size_t */

typedef struct dlist dlist_t;
typedef struct dlist_node dlist_node_t;

typedef dlist_node_t *dlist_iter_t;
typedef int (*is_match_t)(const void *data, const void *param);
typedef int (*dlist_action_t)(void *data, void *param);

/* DESCRIPTION:
 * Function creates an empty list
 *
 * PARAMS:
 * none
 *         
 * RETURN:
 * Returns a pointer to the created list
 *
 * COMPLEXITY:
 * time: best - O(1), worst - indeterminable
 * space: O(1)
 */
dlist_t *DoublyListCreate(void);

/* DESCRIPTION:
 * Function destroys and performs cleanup on the given list.
 * passing an invalid list pointer would result in undefined behaviour
 * This function should be used when finshing using the list.
 *
 * PARAMS:
 * list - pointer to the list to be destroyed
 *         
 * RETURN:
 * void
 *
 * COMPLEXITY:
 * time: O(n)
 * space: O(1)
 */
void DoublyListDestroy(dlist_t *list);

/* DESCRIPTION:
 * Function checks whether the list is empty
 *
 * PARAMS:
 * list - pointer to the list to check if empty
 *         
 * RETURN:
 * 1 if the list is empty or 0 otherwise
 *
 * COMPLEXITY:
 * time: O(1) 
 * space: O(1)
 */
int DoublyListIsEmpty(const dlist_t *list);

/* DESCRIPTION:
 * Function returns the data of the given element after inserting it before the given iterator.
 * passing an invalid iterator would result in undefined behaviour.
 *
 * PARAMS:
 * where - iterator position in the list to enter before
 * data - the data to insert
 *      
 * RETURN:															
 * an iterator to the data that has been inserted, if not returns an iterator to the end of the list.
 *
 * COMPLEXITY:
 * time: O(1) 
 * space: O(1)
 */
dlist_iter_t DoublyListInsertBefore(dlist_t *list, dlist_iter_t where, const void *data);

/* DESCRIPTION:
 * Function removes the selected element from the list.
 * passing an invalid iterator would result in undefined behaviour.
 *
 * PARAMS:
 * where - selected element to remove.
        
 * RETURN:
 * iterator to the next element after the removed  
 *
 * COMPLEXITY: 
 * time: O(1)
 * space: O(1)
 */
dlist_iter_t DoublyListRemove(dlist_iter_t where);

/* DESCRIPTION:
 * Function removes the last element of the list and returns it
 * trying to pop an empty list will result in undefined behavior
 *
 * PARAMS:
 * list - pointer to the list to pop from
 *         
 * RETURN:
 * pointer to the element.
 *
 * COMPLEXITY:
 * time: O(1) 
 * space: O(1)
 */
void *DoublyListPopBack(dlist_t *list);

/* DESCRIPTION:
 * Function removes the first element of the list and returns it
 * trying to pop an empty list will result in undefined behavior
 *
 * PARAMS:
 * list - pointer to the list to pop from
 *         
 * RETURN:
 * pointer to the element.
 *
 * COMPLEXITY:
 * time: O(1) 
 * space: O(1)
 */
void *DoublyListPopFront(dlist_t *list);

/* DESCRIPTION:
 * Function returns the data of the given element after inserting it to the end of the list.
 * passing an invalid list would result in undefined behaviour.
 *
 * PARAMS:
 * list 	     - pointer to list to push into
 * element_to_push  - pointer to the element to push
 *      
 * RETURN:															
 * On success- an iterator to the data that has been pushed. On failure- returns an iterator to the end of the list.
 *
 * COMPLEXITY:
 * time: O(1) 
 * space: O(1)
 */
dlist_iter_t DoublyListPushBack(dlist_t *list, void *data);

/* DESCRIPTION:
 * Function returns the data of the given element after inserting it to the start of the list.
 * passing an invalid list would result in undefined behaviour.
 * passing an invalid data would result in undefined behaviour.
 *
 * PARAMS:
 * list 	     - pointer to list to push into
 * element_to_push  - pointer to the element to push
 *      
 * RETURN:															
 * On success- an iterator to the data that has been pushed. On failure- returns an iterator to the end of the list.
 *
 * COMPLEXITY:
 * time: O(1) 
 * space: O(1)
 */
dlist_iter_t DoublyListPushFront(dlist_t *list, void *element_to_push);

/* DESCRIPTION:
 * Function finds elements base on the return value of is_match function ,in a given range.
 * passing an invalid iterator would result in undefined behaviour.
 *
 * PARAMS:
 * from 	 - iterator to the part of the list to start from 
 * to           - iterator to the end of the iteration
 * func         - function to check if the values match
 * param         - paramter to be passed to is_match func
 *         
 * RETURN:
 * iterator to the found data. if not found, it will return the iterator "to".
 *
 * COMPLEXITY:
 * time: O(n)
 * space: O(1)
 */
dlist_iter_t DoublyListFind(const dlist_iter_t from, const dlist_iter_t to, is_match_t func, const void *param);

/* DESCRIPTION:
 * Function finds nodes in a given range based on is_match function, and copies them
 * to a destination list.
 * passing an invalid iterator would result in undefined behaviour.
 * passing an invalid destination list would result in undefined behaviour.
 *
 * PARAMS:
 * from 	 - iterator to the part of the list to start from 
 * to           - iterator to the end of the iteration
 * func         - function to check if the values match
 * param         - element to find
 * dest_list    - pointer to a list to copy elements into
 *         
 * RETURN:
 * 1 on sucess, 0 on failure
 *
 * COMPLEXITY:
 * time: O(n)
 * space: O(n)
 */
int DoublyListMultiFind(const dlist_iter_t from, const dlist_iter_t to, 
		         is_match_t func, const void *param, dlist_t *dest_list);

/* DESCRIPTION:
 * Function returns the number of elements in the list.
 * passing an invalid list would result in undefined behaviour.
 *
 * PARAMS:
 * list - pointer to a list  
 *      
 * RETURN:
 * number of elements
 * COMPLEXITY:
 * time: O(n) 
 * space: O(1)
 */
size_t DoublyListSize(const dlist_t *list);

/* DESCRIPTION:
 * Function gets the data in the iterator's place
 * passing an invalid iterator would result in undefined behaviour.
 * passing an iterator to the end of the list would result in undefined behaviour.
 *
 * PARAMS:
 * where - iterator to the list to access from
 *         
 * RETURN:
 * Returns a pointer to the data that has been accessed.
 *
 * COMPLEXITY:
 * time: O(1)
 * space: O(1)
 */
void *DoublyListGetData(const dlist_iter_t where);

/* DESCRIPTION:
 * Function set the data of the given element.
 * passing an invalid iterator would result in undefined behaviour.
 * passing an iterator to the end of the list would result in undefined behaviour.
 *
 * PARAMS:
 * where - iterator to the list to set
 * data - new data to set
 *      
 * RETURN:
 * void.
 * COMPLEXITY:
 * time: O(1)
 * space: O(1)
 */
void DoublyListSetData(dlist_iter_t where, void *data);

/* DESCRIPTION:
 * Function performs an action on each element in a given range.
 * passing an invalid iterator would result in undefined behaviour.
 *
 * PARAMS:
 * from         - iterator to the start of the range of elements (included) 
 * to           - iterator to the end of the range of elements (excluded) 
 * action_func  - function pointer to perform an action on an element
 * param        - element for action function
 *      
 * RETURN:
 * 0 if succes 1 else.
 * time: O(n)
 * space: O(1)
 */
int DoublyListForEach(dlist_iter_t from, dlist_iter_t to, dlist_action_t action_func, void *param); 

/* DESCRIPTION:
 * Function returns an iterator to the next element in the list.
 * passing an invalid iterator would result in undefined behaviour.
 * passing iterator to the end of the list would result in undefined behaviour.
 *
 * PARAMS:
 * where         - iterator of the part of the list to get the next from.
 *      
 * RETURN:
 * iterator to the next element.
 * time: O(1)
 * space: O(1)
 */
dlist_iter_t DoublyListIterNext(const dlist_iter_t where);

/* DESCRIPTION:
 * Function returns an iterator to the previous element in the list.
 * passing an invalid iterator would result in undefined behaviour.
 * passing iterator to the first element of the list would result in undefined behaviour.
 *
 * PARAMS:
 * where         - iterator of the part of the list to get the previous of.
 *      
 * RETURN:
 * iterator to the previous element.
 * time: O(1)
 * space: O(1)
 */
dlist_iter_t DoublyListIterPrev(const dlist_iter_t where);

/* DESCRIPTION:
 * Function returns an iterator to the beginning of the list.
 * passing an empty list would result in undefined behaviour.
 *
 * PARAMS:
 * list         - pointer to a list 
 *      
 * RETURN:
 * iterator to the first element of the list.
 * 
 * time: O(1)
 * space: O(1)
 */
dlist_iter_t DoublyListBegin(const dlist_t *list);

/* DESCRIPTION:
 * Function returns an iterator to the end of the list.
 * passing an empty list would result in undefined behaviour.
 *
 * PARAMS:
 * list         - pointer to a list 
 *      
 * RETURN:
 * iterator to the end of the list.
 * 
 * time: O(1)
 * space: O(1)
 */
dlist_iter_t DoublyListEnd(const dlist_t *list);

/* DESCRIPTION:
 * Function compares between two iterators.
 * passing an invalid iterator would result in undefined behaviour.
 * 
 * PARAMS:
 * iter_one         - first iterator to compare 
 * iter_two         - second iterator to compare
 * 
 * RETURN:
 * 1 when iterators are identical, 0 otherwise.
 * 
 * time: O(1)
 * space: O(1)
 */
int DoublyListIsSameIter(const dlist_iter_t iter_one, const dlist_iter_t iter_two);

/* DESCRIPTION:
 * Function moves all elements in range from (included) -> to (excluded) to before where.
 * passing invalid iterators would result in undefined behaviour.
 *
 * PARAMS:
 * from         - iterator to the start of the range to move from 
 * to           - iterator to the end of the range to move from
 * where        - iterator to the destination to move elements to
 *      
 * RETURN:
 * iterator to the last element moved
 * time: O(1)
 * space: O(1)
 */
dlist_iter_t DoublyListSplice(dlist_iter_t from, dlist_iter_t to, dlist_iter_t where); 

#endif /* __list_H__ */

