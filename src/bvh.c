#include "bvh.h"

void bvh_setup(bvh_node *b, hittable_list *l, int start, int end);

aabb get_bvh_box(void *b){
    return ((bvh_node*)b)->bbox;
}

void init_bvh(bvh_node *b, hittable_list *list){
    bvh_setup(b, list, 0, list->size); 
}

bool hit_bvh(void *b, ray r, double ray_tmin, double ray_tmax, hit_record *rec){
    if(!hit_box(&(((bvh_node*)b)->bbox), r, ray_tmin, ray_tmax))
        return false;

    bool h_left = (*((bvh_node*)b)->hit_left)(((bvh_node*)b)->left, r, ray_tmin, ray_tmax, rec); 
    bool h_right = (*((bvh_node*)b)->hit_right)(((bvh_node*)b)->right, r, ray_tmin, h_left ? rec->t : ray_tmax, rec);

    return h_left || h_right;
}

void bvh_setup(bvh_node *b, hittable_list *l, int start, int end){
    int axis = longest_axis(b->bbox);

    int span = end - start;
    if(span == 1) {
        hittable_node *node = index_list(l, start);
        b->left = b->right = node->hittable;
        b->hit_left = b->hit_right = node->is_hit;
        b->box_left = b->box_right = node->get_box;
        b->is_leaf = true;
    } else if (span == 2){
        hittable_node *node = index_list(l, start);
        b->left = node->hittable;
        b->hit_left = node->is_hit;
        b->box_left = node->get_box;
        
        node = index_list(l, start + 1);
        b->right = node->hittable;
        b->hit_right = node->is_hit;
        b->box_right = node->get_box;
        b->is_leaf = true;
    } else {
        sort_list(l, start, end, axis);
        
        int mid = start + (span / 2);

        b->left = malloc(sizeof(bvh_node));
        b->hit_left = &hit_bvh;
        b->box_left = &get_bvh_box;

        b->right = malloc(sizeof(bvh_node));
        b->hit_right = &hit_bvh;
        b->box_right = &get_bvh_box;

        bvh_setup((bvh_node *)b->left, l, start, mid);
        bvh_setup((bvh_node *)b->right, l, mid, end);
        b->is_leaf = false;
    }

    copy_box(&(b->bbox), (*((bvh_node*)b)->box_left)(b->left));
    add_boxes(&(b->bbox), (*((bvh_node*)b)->box_right)(b->right));
}

void delete_bvh(bvh_node *b){
    if(b->is_leaf) return;    
    bvh_node *l;
    bvh_node *r;
    l = (bvh_node*)b->left;
    r = (bvh_node*)b->right;

    delete_bvh(l);
    delete_bvh(r);

    free(l);
    free(r);
}

