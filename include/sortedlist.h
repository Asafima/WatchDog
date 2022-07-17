/*
    team: OL125-126    
    version: 1.0

*/
#ifndef __SORTLIST_H__
#define __SORTLIST_H__

#include <stddef.h> /* size_t */
#include "dlist.h"

typedef struct sortedlist sorted_list_t;

typedef struct sorted_list_iter
{
    dlist_iter_t internal_iter;
    #ifndef NDEBUG
    sorted_list_t *list;
    #endif
}sorted_list_iter_t;

/* DESCRIPTION:
 * Function pointer to a function which compares sorted list elements.
 * The function compares data1 to data2. 
 *
 * PARAMS:
 * data1 - the first data to check
 * data2 - the second data to compare with first data
 *         
 * RETURN:
 * returns an integer which is the subtraction of data1 - data2
 *
 * COMPLEXITY:
 * time: O(1)
 * space: O(1)
 */
typedef int (*sorted_list_cmp_t)(const void *data1, const void *data2);


typedef int (*sorted_list_action_t)(void *data, void *param);

/* DESCRIPTION:
 * Function pointer to a function which compares sorted list elements.
 * The function compares data to param. 
 *
 * PARAMS:
 * data - the data to check from list
 * param - the data to compare with data from the list
 *         
 * RETURN:
 * returns an integer 0 if the data matches, and an integer otherwise.
 *
 * COMPLEXITY:
 * time: O(1)
 * space: O(1)
 */
typedef int (*sorted_list_is_match_t)(const void *data, const void *param);




/* DESCRIPTION:
 * Function creates an empty sorted list
 *
 * PARAMS:
 * compare function
 *         
 * RETURN:
 * Returns a pointer to the created sorted list
 *
 * COMPLEXITY:
 * time: best - O(1), worst - indeterminable
 * space: O(1)
 */
sorted_list_t *SortedListCreate(sorted_list_cmp_t func);

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
void SortedListDestroy(sorted_list_t *list);

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
int SortedListIsEmpty(const sorted_list_t *list);

/* DESCRIPTION:
 * Function inserts the given data to the list in the correct position.
 * passing an invalid iterator would result in undefined behaviour.
 *
 * PARAMS:
 * list - pointer to the list to insert data into
 * data - the data to insert
 *      
 * RETURN:															
 * On success, an iterator to the data that has been inserted. On fail, an iterator to the end of the list.
 *
 * COMPLEXITY:
 * time: O(n) 
 * space: O(1)
 */
sorted_list_iter_t SortedListInsert(sorted_list_t *list, void *data);

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
sorted_list_iter_t SortedListRemove(sorted_list_iter_t where);

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
void *SortedListPopBack(sorted_list_t *list);

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
void *SortedListPopFront(sorted_list_t *list);

/* DESCRIPTION:
 * Function finds first element in the list based on the return value of compare function.
 * passing an invalid list would result in undefined behaviour. ( to excluded)/////
 *
 * PARAMS:
 * list          - pointer to the list to find elements in
 * param         - paramter to be passed to is_match func
 *         
 * RETURN:
 * iterator to the found data. if not found, it will return to.
 *
 * COMPLEXITY:
 * time: O(n)
 * space: O(1)
 */
sorted_list_iter_t SortedListFind(const sorted_list_iter_t from,const sorted_list_iter_t to, const sorted_list_t *list,const void *param);

/* DESCRIPTION:
 * Function finds elements base on the return value of compare function ,in a given range.
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
sorted_list_iter_t SortedListFindIf(const sorted_list_iter_t from, const sorted_list_iter_t to, sorted_list_is_match_t is_match, const void *param);

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
size_t SortedListSize(const sorted_list_t *list);

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
void *SortedListGetData(const sorted_list_iter_t where);

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
 * 0 on succes or return status defined by the user; /// add documentation action_func.
 * time: O(n)
 * space: O(1)
 */
int SortedListForEach(sorted_list_iter_t from, sorted_list_iter_t to, sorted_list_action_t action_func, void *param); 

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
sorted_list_iter_t SortedListIterNext(sorted_list_iter_t curr);

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
sorted_list_iter_t SortedListIterPrev(sorted_list_iter_t curr);

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
sorted_list_iter_t SortedListBegin(const sorted_list_t *list);

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
sorted_list_iter_t SortedListEnd(const sorted_list_t *list);

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
int SortedListIsSameIter(sorted_list_iter_t iter_one, sorted_list_iter_t iter_two);

/* DESCRIPTION:
 * Function merges 2 lists based on destination list compare function, puts
 * new merged list in dest_list and empties src_list.
 * passing invalid list pointers would result in undefined behaviour.
 *
 * PARAMS:
 * dest_list     - pointer to a destination list 
 * src_list      - pointer to a source list
 *      
 * RETURN:
 * pointer to the destination list.
 * time: O(n)
 * space: O(1)
 */
void SortedListMerge(sorted_list_t *dest_list, sorted_list_t *src_list); 



#endif /* __sortlist_H__ */

