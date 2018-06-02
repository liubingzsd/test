#include <stdio.h>   
#include <stdlib.h>   
#include <string.h>  
#include <malloc.h>
#include <math.h>   
#include "kd_tree.h"   

//����ƽ���ĺ궨��,�൱�ں���   
#define SQ(x)           ((x) * (x))   

static void clear_rec(kd_node_s *node, void(*destr)(void*));
static int insert_rec(kd_node_s **node, const float *pos, void *data, int dir, int dim);
static int rlist_insert(result_node_s *list, kd_node_s *item, float dist_sq);
static void clear_results(kd_result_s *set);

static kd_hyper_rect_s *hyperrect_create(int dim, const float *min, const float *max);
static void hyperrect_free(kd_hyper_rect_s *rect);
static kd_hyper_rect_s *hyperrect_duplicate(const kd_hyper_rect_s *rect);
static void hyperrect_extend(kd_hyper_rect_s *rect, const float *pos);
static float hyperrect_dist_sq(kd_hyper_rect_s *rect, const float *pos);

 
#define alloc_resnode()     malloc(sizeof(result_node_s))  
#define free_resnode(n)     free(n)   
 


//create kd_tree   
kd_tree_s *kd_create(int k)
{
	kd_tree_s *tree;
	tree = (kd_tree_s *)malloc(sizeof(*tree));
	if (!tree)
	{
		return 0;
	}
	tree->dim = k;
	tree->root = 0;
	tree->destr = 0;
	tree->rect = 0;
	return tree;
}

//free kdtree   
void kd_free(kd_tree_s *tree)
{
	if (tree) 
	{
		kd_clear(tree);
		free(tree);
	}
}

//�������ƽ��,�ǰ��ڵ�ݹ�ؽ��е�   
static void clear_rec(kd_node_s *node, void(*destr)(void*))
{
	if (!node)
	{
		return;   //һ���ڵ��Ӧһ����ƽ�� 
	} 

	 //�ݹ麯�����ݹ����������������֧�ĳ�ƽ��Ͷ������ҷ�֧�ĳ�ƽ��   
	clear_rec(node->left, destr);
	clear_rec(node->right, destr);

	//���data���������Ϊ��,���ͷŵ�data   
	if (destr)
	{
		destr(node->data);
	}
	//�ͷŽڵ����������   
	free(node->pos);
	//�ͷŽڵ�   
	free(node);
}

//kdtree���   
void kd_clear(kd_tree_s *tree)
{
	//�������ÿ���ڵ�ĳ�ƽ��,�ͷ����еĸ����ڵ�   
	clear_rec(tree->root, tree->destr);
	tree->root = 0;

	//������ĳ�ƽ��ָ�벻Ϊ��,��������ͷ�   
	if (tree->rect)
	{
		hyperrect_free(tree->rect);
		tree->rect = 0;
	}
}

//�������٣���һ�������ĺ���������data������   
void kd_data_destructor(kd_tree_s *tree, void(*destr)(void*))
{
	//�������ĺ�����ִ��kdtree�����ٺ���   
	tree->destr = destr;
}


//��һ�����ڵ�λ�ô����볬����   
static int insert_rec(kd_node_s **nptr, const float *pos, void *data, int dir, int dim)
{
	int new_dir;
	kd_node_s *node;

	//�������ڵ��ǲ����ڵ�   
	if (!*nptr)
	{
		//����һ�����   
		node = (kd_node_s *)malloc(sizeof(*node));
		if (!node)
		{
			return -1;
		}
		if (!(node->pos = (float*)malloc(dim * sizeof *node->pos))) {
			free(node);
			return -1;
		}
		memcpy(node->pos, pos, dim * sizeof *node->pos);
		node->data = data;
		node->dir = dir;
		node->left = node->right = 0;
		*nptr = node;
		return 0;
	}

	node = *nptr;
	new_dir = (node->dir + 1) % dim;
	if (pos[node->dir] < node->pos[node->dir])
	{
		return insert_rec(&(*nptr)->left, pos, data, new_dir, dim);
	}
	return insert_rec(&(*nptr)->right, pos, data, new_dir, dim);
}

//�ڵ�������   
//����Ϊ:Ҫ���в��������kdtree,Ҫ����Ľڵ�����,Ҫ����Ľڵ������   
int kd_insert(kd_tree_s *tree, const float *pos, void *data)
{
	//���볬����   
	if (insert_rec(&tree->root, pos, data, 0, tree->dim))
	{
		return -1;
	}
	//�������û�г�����,�ʹ���һ��������   
	//����Ѿ����˳�����,����չԭ�еĳ�����   
	if (tree->rect == 0)
	{
		tree->rect = hyperrect_create(tree->dim, pos, pos);
	}
	else
	{
		hyperrect_extend(tree->rect, pos);
	}

	return 0;
}

//����float������Ľڵ�   
//����Ϊ:Ҫ���в��������kdtree,Ҫ����Ľڵ�����,Ҫ����Ľڵ������   
//��float�͵����긳ֵ��float�͵Ļ�����,�����������ת������в���   
//��������һ������ת��   
int kd_insertf(kd_tree_s *tree, const float *pos, void *data)
{
	static float sbuf[16];
	float *bptr, *buf = 0;
	int res, dim = tree->dim;

	//���kdtree��ά������16, ����dimάfloat���͵�����   
	if (dim > 16)
	{
#ifndef NO_ALLOCA   
		if (dim <= 256)
			bptr = buf = (float*)alloca(dim * sizeof *bptr);
		else
#endif   
			if (!(bptr = buf = (float*)malloc(dim * sizeof *bptr)))
			{
				return -1;
			}
	}
	//���kdtree��ά��С��16, ֱ�ӽ�ָ��ָ���ѷ�����ڴ�   
	else
	{
		bptr = buf = sbuf;
	}

	//��Ҫ������λ�����긳ֵ�����������   
	while (dim-- > 0)
	{
		*bptr++ = *pos++;
	}

	//���ýڵ���뺯��kd_insert   
	res = kd_insert(tree, buf, data);
#ifndef NO_ALLOCA   
	if (tree->dim > 256)
#else   
	if (tree->dim > 16)
#endif   
		//�ͷŻ���   
		free(buf);
	return res;
}

//������ά����ֵ����άkdtree����   
int kd_insert3(kd_tree_s *tree, float x, float y, float z, void *data)
{
	float buf[3];
	buf[0] = x;
	buf[1] = y;
	buf[2] = z;
	return kd_insert(tree, buf, data);
}

//������άfloat������ֵ����άkdtree����   
int kd_insert3f(kd_tree_s *tree, float x, float y, float z, void *data)
{
	float buf[3];
	buf[0] = x;
	buf[1] = y;
	buf[2] = z;
	return kd_insert(tree, buf, data);
}

//�ҵ�����ڵĵ�   
//����Ϊ:���ڵ�ָ��, λ������, ��ֵ, ���ؽ���Ľڵ�, bool������,ά��   
static int find_nearest(kd_node_s *node, const float *pos, float range, result_node_s *list, int ordered, int dim)
{
	float dist_sq, dx;
	int i, ret, added_res = 0;

	if (!node) return 0;  //ע������ط�,���ڵ�Ϊ�յ�ʱ��,�����Ѿ����ҵ����յ�Ҷ�ӽ��,����ֵΪ��  

	dist_sq = 0;
	//���������ڵ���ƽ����   
	for (i = 0; i<dim; i++)
	{
		dist_sq += SQ(node->pos[i] - pos[i]);
	}
	//�����������ֵ��Χ��,�ͽ�����뵽���ؽ��������   
	if (dist_sq <= SQ(range))
	{
		if (rlist_insert(list, node, ordered ? dist_sq : -1.0) == -1)
		{
			return -1;
		}
		added_res = 1;
	}

	//������ڵ�Ļ��ַ�����,������֮��Ĳ�ֵ   
	dx = pos[node->dir] - node->pos[node->dir];

	//���������ֵ�ķ���, ѡ����еݹ���ҵķ�֧����   
	ret = find_nearest(dx <= 0.0 ? node->left : node->right, pos, range, list, ordered, dim);
	//������ص�ֵ���ڵ�����,�����������֧�������������Ľڵ�,�򷵻ؽ���ĸ��������ۼ�,���ڽڵ����һ��������в�������Ľڵ�  
	if (ret >= 0 && fabs(dx) < range)
	{
		added_res += ret;
		ret = find_nearest(dx <= 0.0 ? node->right : node->left, pos, range, list, ordered, dim);
	}
	if (ret == -1)
	{
		return -1;
	}
	added_res += ret;

	return added_res;
}


//�ҵ�����ڵ�n���ڵ�   
#if 0   
static int find_nearest_n(struct kdnode *node, const float *pos, float range, int num, struct rheap *heap, int dim)
{
	float dist_sq, dx;
	int i, ret, added_res = 0;

	if (!node) return 0;

	/* if the photon is close enough, add it to the result heap */
	//����㹻���ͽ�����뵽�������   
	dist_sq = 0;
	//�������߼��ŷʽ����   
	for (i = 0; i<dim; i++)
	{
		dist_sq += SQ(node->pos[i] - pos[i]);
	}
	//����������þ���С����ֵ   
	if (dist_sq <= range_sq) {
		//����ѵĴ�С����num,Ҳ���Ǵ����ܵ�Ҫ�ҵĽڵ���   
		if (heap->size >= num)
		{
			/* get furthest element */
			//�õ���Զ�Ľڵ�   
			struct res_node *maxelem = rheap_get_max(heap);

			/* and check if the new one is closer than that */
			//��������ڵ��ǲ��Ǳ���Զ�Ľڵ�Ҫ��   
			if (maxelem->dist_sq > dist_sq)
			{
				//����ǵĻ�,���Ƴ���Զ�Ľڵ�   
				rheap_remove_max(heap);
				//�����˽ڵ�������   
				if (rheap_insert(heap, node, dist_sq) == -1)
				{
					return -1;
				}
				added_res = 1;

				range_sq = dist_sq;
			}
		}
		//����ѵĴ�СС��num,ֱ�ӽ��˽ڵ�������   
		else
		{
			if (rheap_insert(heap, node, dist_sq) == -1)
			{
				return = 1;
			}
			added_res = 1;
		}
	}


	/* find signed distance from the splitting plane */
	dx = pos[node->dir] - node->pos[node->dir];

	ret = find_nearest_n(dx <= 0.0 ? node->left : node->right, pos, range, num, heap, dim);
	if (ret >= 0 && fabs(dx) < range) {
		added_res += ret;
		ret = find_nearest_n(dx <= 0.0 ? node->right : node->left, pos, range, num, heap, dim);
	}
}
#endif   


static void kd_nearest_i(kd_node_s *node, const float *pos, kd_node_s **result, float *result_dist_sq, kd_hyper_rect_s *rect)
{
	int dir = node->dir;
	int i;
	float dummy, dist_sq;
	kd_node_s *nearer_subtree, *farther_subtree;
	float *nearer_hyperrect_coord, *farther_hyperrect_coord;

	/* Decide whether to go left or right in the tree */
	dummy = pos[dir] - node->pos[dir];
	if (dummy <= 0)
	{
		nearer_subtree = node->left;
		farther_subtree = node->right;
		nearer_hyperrect_coord = rect->max + dir;
		farther_hyperrect_coord = rect->min + dir;
	}
	else
	{
		nearer_subtree = node->right;
		farther_subtree = node->left;
		nearer_hyperrect_coord = rect->min + dir;
		farther_hyperrect_coord = rect->max + dir;
	}

	if (nearer_subtree)
	{
		/* Slice the hyperrect to get the hyperrect of the nearer subtree */
		dummy = *nearer_hyperrect_coord;
		*nearer_hyperrect_coord = node->pos[dir];
		/* Recurse down into nearer subtree */
		kd_nearest_i(nearer_subtree, pos, result, result_dist_sq, rect);
		/* Undo the slice */
		*nearer_hyperrect_coord = dummy;
	}

	/* Check the distance of the point at the current node, compare it
	* with our best so far */
	dist_sq = 0;
	for (i = 0; i < rect->dim; i++)
	{
		dist_sq += SQ(node->pos[i] - pos[i]);
	}
	if (dist_sq < *result_dist_sq)
	{
		*result = node;
		*result_dist_sq = dist_sq;
	}

	if (farther_subtree) {
		/* Get the hyperrect of the farther subtree */
		dummy = *farther_hyperrect_coord;
		*farther_hyperrect_coord = node->pos[dir];
		/* Check if we have to recurse down by calculating the closest
		* point of the hyperrect and see if it's closer than our
		* minimum distance in result_dist_sq. */
		if (hyperrect_dist_sq(rect, pos) < *result_dist_sq) {
			/* Recurse down into farther subtree */
			kd_nearest_i(farther_subtree, pos, result, result_dist_sq, rect);
		}
		/* Undo the slice on the hyperrect */
		*farther_hyperrect_coord = dummy;
	}
}

//��kdtree�����pos����ڵ�ֵ   
kd_result_s *kd_nearest(kd_tree_s *kd, const float *pos)
{
	kd_hyper_rect_s *rect;
	kd_node_s *result;
	kd_result_s *rset;
	float dist_sq;
	int i;

	//���kd������,�����䳬ƽ�治���ڵĻ�,��Ͳ����н��   
	if (!kd) return 0;
	if (!kd->rect) return 0;

	/* Allocate result set */
	//Ϊ���ؽ�����Ϸ���ռ�   
	if (!(rset = (kd_result_s *)malloc(sizeof *rset)))
	{
		return 0;
	}
	if (!(rset->rlist = (result_node_s *)alloc_resnode())) {
		free(rset);
		return 0;
	}
	rset->rlist->next = 0;
	rset->tree = kd;

	/* Duplicate the bounding hyperrectangle, we will work on the copy */
	//���Ʊ߽糬ƽ��   
	if (!(rect = hyperrect_duplicate(kd->rect)))
	{
		kd_res_free(rset);
		return 0;
	}

	/* Our first guesstimate is the root node */
	result = kd->root;
	dist_sq = 0;
	for (i = 0; i < kd->dim; i++)
		dist_sq += SQ(result->pos[i] - pos[i]);

	/* Search for the nearest neighbour recursively */
	//�ݹ�ز�������ڵ��ھ�   
	kd_nearest_i(kd->root, pos, &result, &dist_sq, rect);

	/* Free the copy of the hyperrect */
	//�ͷų�����   
	hyperrect_free(rect);

	/* Store the result */
	//�洢���   
	if (result)
	{
		if (rlist_insert(rset->rlist, result, -1.0) == -1)
		{
			kd_res_free(rset);
			return 0;
		}
		rset->size = 1;
		kd_res_rewind(rset);
		return rset;
	}
	else
	{
		kd_res_free(rset);
		return 0;
	}
}

//kd_nearest��float����   
kd_result_s *kd_nearestf(kd_tree_s *tree, const float *pos)
{
	static float sbuf[16];
	float *bptr, *buf = 0;
	int dim = tree->dim;
	kd_result_s *res;

	if (dim > 16) {
#ifndef NO_ALLOCA   
		if (dim <= 256)
			bptr = buf = (float*)alloca(dim * sizeof *bptr);
		else
#endif   
			if (!(bptr = buf = (float*)malloc(dim * sizeof *bptr))) {
				return 0;
			}
	}
	else {
		bptr = buf = sbuf;
	}

	while (dim-- > 0) {
		*bptr++ = *pos++;
	}

	res = kd_nearest(tree, buf);
#ifndef NO_ALLOCA   
	if (tree->dim > 256)
#else   
	if (tree->dim > 16)
#endif   
		free(buf);
	return res;
}

//kd_nearest������������   
kd_result_s *kd_nearest3(kd_tree_s *tree, float x, float y, float z)
{
	float pos[3];
	pos[0] = x;
	pos[1] = y;
	pos[2] = z;
	return kd_nearest(tree, pos);
}

//kd_nearest��������float����   
kd_result_s *kd_nearest3f(kd_tree_s *tree, float x, float y, float z)
{
	float pos[3];
	pos[0] = x;
	pos[1] = y;
	pos[2] = z;
	return kd_nearest(tree, pos);
}

/* ---- nearest N search ---- */
/*
static kdres *kd_nearest_n(struct kdtree *kd, const float *pos, int num)
{
int ret;
struct kdres *rset;

if(!(rset = malloc(sizeof *rset))) {
return 0;
}
if(!(rset->rlist = alloc_resnode())) {
free(rset);
return 0;
}
rset->rlist->next = 0;
rset->tree = kd;

if((ret = find_nearest_n(kd->root, pos, range, num, rset->rlist, kd->dim)) == -1) {
kd_res_free(rset);
return 0;
}
rset->size = ret;
kd_res_rewind(rset);
return rset;
}*/

//�ҵ��������С��rangeֵ�Ľڵ�   
kd_result_s *kd_nearest_range(kd_tree_s *kd, const float *pos, float range)
{
	int ret;
	kd_result_s *rset;

	if (!(rset = (kd_result_s *)malloc(sizeof *rset))) {
		return 0;
	}
	if (!(rset->rlist = (result_node_s *)alloc_resnode())) {
		free(rset);
		return 0;
	}
	rset->rlist->next = 0;
	rset->tree = kd;

	if ((ret = find_nearest(kd->root, pos, range, rset->rlist, 0, kd->dim)) == -1) {
		kd_res_free(rset);
		return 0;
	}
	rset->size = ret;
	kd_res_rewind(rset);
	return rset;
}

//kd_nearest_range��float����   
kd_result_s *kd_nearest_rangef(kd_tree_s *kd, const float *pos, float range)
{
	static float sbuf[16];
	float *bptr, *buf = 0;
	int dim = kd->dim;
	kd_result_s *res;

	if (dim > 16) {
#ifndef NO_ALLOCA   
		if (dim <= 256)
			bptr = buf = (float*)alloca(dim * sizeof *bptr);
		else
#endif   
			if (!(bptr = buf = (float*)malloc(dim * sizeof *bptr))) {
				return 0;
			}
	}
	else {
		bptr = buf = sbuf;
	}

	while (dim-- > 0) {
		*bptr++ = *pos++;
	}

	res = kd_nearest_range(kd, buf, range);
#ifndef NO_ALLOCA   
	if (kd->dim > 256)
#else   
	if (kd->dim > 16)
#endif   
		free(buf);
	return res;
}

//kd_nearest_range������������   
kd_result_s *kd_nearest_range3(kd_tree_s *tree, float x, float y, float z, float range)
{
	float buf[3];
	buf[0] = x;
	buf[1] = y;
	buf[2] = z;
	return kd_nearest_range(tree, buf, range);
}

//kd_nearest_range��������float����   
kd_result_s *kd_nearest_range3f(kd_tree_s *tree, float x, float y, float z, float range)
{
	float buf[3];
	buf[0] = x;
	buf[1] = y;
	buf[2] = z;
	return kd_nearest_range(tree, buf, range);
}

//���ؽ�����ͷ�   
void kd_res_free(kd_result_s *rset)
{
	clear_results(rset);
	free_resnode(rset->rlist);
	free(rset);
}

//��ȡ���ؽ�����ϵĴ�С   
int kd_res_size(kd_result_s *set)
{
	return (set->size);
}

//�ٴλص�����ڵ㱾���λ��   
void kd_res_rewind(kd_result_s *rset)
{
	rset->riter = rset->rlist->next;
}

//�ҵ����ؽ���е����սڵ�   
int kd_res_end(kd_result_s *rset)
{
	return rset->riter == 0;
}

//���ؽ���б��е���һ���ڵ�   
int kd_res_next(kd_result_s *rset)
{
	rset->riter = rset->riter->next;
	return rset->riter != 0;
}

//�����ؽ���Ľڵ�������data��ȡ����   
void *kd_res_item(kd_result_s *rset, float *pos)
{
	if (rset->riter) {
		if (pos) {
			memcpy(pos, rset->riter->item->pos, rset->tree->dim * sizeof *pos);
		}
		return rset->riter->item->data;
	}
	return 0;
}

//�����ؽ���Ľڵ�������data��ȡ����,����Ϊfloat�͵�ֵ   
void *kd_res_itemf(kd_result_s *rset, float *pos)
{
	if (rset->riter)
	{
		if (pos)
		{
			int i;
			for (i = 0; i<rset->tree->dim; i++) 
			{
				pos[i] = rset->riter->item->pos[i];
			}
		}
		return rset->riter->item->data;
	}
	return 0;
}

//�����ؽ���Ľڵ�������data��ȡ����,���������ʽ����   
void *kd_res_item3(kd_result_s *rset, float *x, float *y, float *z)
{
	if (rset->riter) {
		if (*x)
		{
			*x = rset->riter->item->pos[0];
		}
		if (*y)
		{
			*y = rset->riter->item->pos[1];
		}
		if (*z)
		{
			*z = rset->riter->item->pos[2];
		}
	}
	return 0;
}

//�����ؽ���Ľڵ�������data��ȡ����,����Ϊfloat�͵�ֵ,���������ʽ����   
void *kd_res_item3f(kd_result_s *rset, float *x, float *y, float *z)
{
	if (rset->riter)
	{
		if (*x)
		{
			*x = rset->riter->item->pos[0];
		}
		if (*y)
		{
			*y = rset->riter->item->pos[1];
		}
		if (*z)
		{
			*z = rset->riter->item->pos[2];
		}
	}
	return 0;
}

//��ȡdata����   
void *kd_res_item_data(kd_result_s *set)
{
	return kd_res_item(set, 0);
}

/* ---- hyperrectangle helpers ---- */
//������ƽ��,������������:ά��,ÿά����Сֵ�����ֵ����   
static kd_hyper_rect_s *hyperrect_create(int dim, const float *min, const float *max)
{
	size_t size = dim * sizeof(float);
	kd_hyper_rect_s *rect = 0;

	if (!(rect = (kd_hyper_rect_s *)malloc(sizeof(kd_hyper_rect_s))))       //check check check check  !!!!!!!!!!!!!!!!!!!!!!
	{
		return 0;
	}

	rect->dim = dim;
	if (!(rect->min = (float*)malloc(size))) {
		free(rect);
		return 0;
	}
	if (!(rect->max = (float*)malloc(size))) {
		free(rect->min);
		free(rect);
		return 0;
	}
	memcpy(rect->min, min, size);
	memcpy(rect->max, max, size);

	return rect;
}

//�ͷų�ƽ��ṹ��   
static void hyperrect_free(kd_hyper_rect_s *rect)
{
	free(rect->min);
	free(rect->max);
	free(rect);
}

//��ֵ��ƽ��ṹ��   
static kd_hyper_rect_s *hyperrect_duplicate(const kd_hyper_rect_s *rect)
{
	return hyperrect_create(rect->dim, rect->min, rect->max);
}

//���³�ƽ��ṹ�����\��Сֵ����   
static void hyperrect_extend(kd_hyper_rect_s *rect, const float *pos)
{
	int i;

	for (i = 0; i < rect->dim; i++)
	{
		if (pos[i] < rect->min[i]) 
		{
			rect->min[i] = pos[i];
		}
		if (pos[i] > rect->max[i]) 
		{
			rect->max[i] = pos[i];
		}
	}
}

//����̶�������볬ƽ��֮��ľ���   
static float hyperrect_dist_sq(kd_hyper_rect_s *rect, const float *pos)
{
	int i;
	float result = 0;

	for (i = 0; i < rect->dim; i++)
	{
		if (pos[i] < rect->min[i])
		{
			result += SQ(rect->min[i] - pos[i]);
		}
		else if (pos[i] > rect->max[i])
		{
			result += SQ(rect->max[i] - pos[i]);
		}
	}
	return result;
}


/* inserts the item. if dist_sq is >= 0, then do an ordered insert */
/* TODO make the ordering code use heapsort */
//��������: ���ؽ���ڵ�ָ��,���ڵ�ָ��,���뺯��   
//��һ������ڵ���뵽���ؽ�����б���   
static int rlist_insert(result_node_s *list, kd_node_s *item, float dist_sq)
{
	result_node_s *rnode;
	//����һ�����ؽ���Ľڵ�   
	if (!(rnode = (result_node_s *)alloc_resnode()))
	{
		return -1;
	}
	rnode->item = item;           //��Ӧ�����ڵ�   
	rnode->dist_sq = dist_sq;     //��Ӧ�ľ���ֵ   

								  //������������ʱ��   
	if (dist_sq >= 0.0)
	{
		while (list->next && list->next->dist_sq < dist_sq)
		{
			list = list->next;
		}
	}
	rnode->next = list->next;
	list->next = rnode;
	return 0;
}

//������ؽ���ļ���   
//�������Ǹ�˫�����е����������   
static void clear_results(kd_result_s *rset)
{
	result_node_s *tmp, *node = rset->rlist->next;

	while (node)
	{
		tmp = node;
		node = node->next;
		free_resnode(tmp);
	}

	rset->rlist->next = 0;
}

void test_kd_tree()
{
	kd_tree_s tree;
	kd_tree_s *p = &tree;
	p = kd_create(10);
}