#ifndef _KD_TREE_H_
#define _KD_TREE_H_

//��ƽ��Ľṹ��  ����һ�����Ե�ά����ÿά�����������Сֵ���ɵ�����   
typedef struct
{
	int dim;
	float *min, *max;              /* minimum/maximum coords */
}kd_hyper_rect_s;

//�ڵ�Ľṹ�壬Ҳ���������Ľṹ��   
typedef struct kd_node
{
	float *pos;
	int dir;
	void *data;
	struct kd_node *left, *right;    /* negative/positive side */
}kd_node_s;

//���ؽ���ڵ㣬 �������Ľڵ�,����ֵ, ��һ�����������ʽ   
typedef struct result_node 
{
	kd_node_s *item;
	float dist_sq;
	struct result_node *next;
}result_node_s;

//���м������ԣ�һ��ά����һ�������ڵ㣬һ�ǳ�ƽ�棬һ������data�ĺ���   
typedef struct 
{
	int dim;
	kd_node_s *root;
	kd_hyper_rect_s *rect;
	void(*destr)(void*);
}kd_tree_s;

//kdtree�ķ��ؽ��������kdtree������һ��˫�������ʽ   
typedef struct 
{
	kd_tree_s *tree;
	result_node_s *rlist, *riter;  //˫����?  
	int size;
}kd_result_s;

/* create a kd-tree for "k"-dimensional data */
kd_tree_s *kd_create(int k);

/* free the struct kdtree */
void kd_free(kd_tree_s *tree);

/* remove all the elements from the tree */
void kd_clear(kd_tree_s *tree);

/* if called with non-null 2nd argument, the function provided
* will be called on data pointers (see kd_insert) when nodes
* are to be removed from the tree.
*/
void kd_data_destructor(kd_tree_s *tree, void(*destr)(void*));

/* insert a node, specifying its position, and optional data */
int kd_insert(kd_tree_s *tree, const float *pos, void *data);
int kd_insertf(kd_tree_s *tree, const float *pos, void *data);
int kd_insert3(kd_tree_s *tree, float x, float y, float z, void *data);
int kd_insert3f(kd_tree_s *tree, float x, float y, float z, void *data);

/* Find the nearest node from a given point.
*
* This function returns a pointer to a result set with at most one element.
*/
kd_result_s *kd_nearest(kd_tree_s *tree, const float *pos);
kd_result_s *kd_nearestf(kd_tree_s *tree, const float *pos);
//kd_result_s *kd_nearest3(kd_tree_s *tree, float x, float y, float z);
kd_result_s *kd_nearest3f(kd_tree_s *tree, float x, float y, float z);

/* Find the N nearest nodes from a given point.
*
* This function returns a pointer to a result set, with at most N elements,
* which can be manipulated with the kd_res_* functions.
* The returned pointer can be null as an indication of an error. Otherwise
* a valid result set is always returned which may contain 0 or more elements.
* The result set must be deallocated with kd_res_free after use.
*/
/*
struct kdres *kd_nearest_n(struct kdtree *tree, const float *pos, int num);
struct kdres *kd_nearest_nf(struct kdtree *tree, const float *pos, int num);
struct kdres *kd_nearest_n3(struct kdtree *tree, float x, float y, float z);
struct kdres *kd_nearest_n3f(struct kdtree *tree, float x, float y, float z);
*/

/* Find any nearest nodes from a given point within a range.
*
* This function returns a pointer to a result set, which can be manipulated
* by the kd_res_* functions.
* The returned pointer can be null as an indication of an error. Otherwise
* a valid result set is always returned which may contain 0 or more elements.
* The result set must be deallocated with kd_res_free after use.
*/
kd_result_s *kd_nearest_range(kd_tree_s *tree, const float *pos, float range);
kd_result_s *kd_nearest_rangef(kd_tree_s *tree, const float *pos, float range);
kd_result_s *kd_nearest_range3(kd_tree_s *tree, float x, float y, float z, float range);
kd_result_s *kd_nearest_range3f(kd_tree_s *tree, float x, float y, float z, float range);

/* frees a result set returned by kd_nearest_range() */
void kd_res_free(kd_result_s *set);

/* returns the size of the result set (in elements) */
int kd_res_size(kd_result_s *set);

/* rewinds the result set iterator */
void kd_res_rewind(kd_result_s *set);

/* returns non-zero if the set iterator reached the end after the last element */
int kd_res_end(kd_result_s *set);

/* advances the result set iterator, returns non-zero on success, zero if
* there are no more elements in the result set.
*/
int kd_res_next(kd_result_s *set);

/* returns the data pointer (can be null) of the current result set item
* and optionally sets its position to the pointers(s) if not null.
*/
void *kd_res_item(kd_result_s *set, float *pos);
void *kd_res_itemf(kd_result_s *set, float *pos);
void *kd_res_item3(kd_result_s *set, float *x, float *y, float *z);
void *kd_res_item3f(kd_result_s *set, float *x, float *y, float *z);

/* equivalent to kd_res_item(set, 0) */
void *kd_res_item_data(kd_result_s *set);


void test_kd_tree();





#endif