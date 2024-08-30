#include "hittable_list.h"

void init_list(hittable_list *l, void *h, fptr_is_hit f){
    l->hittable = h;
    l->is_hit = f;
    l->next = NULL;
}

void add_list(hittable_list *l, void *h, fptr_is_hit f){
    hittable_list *currNode = l;
    while(currNode->next != NULL){
       currNode = currNode->next; 
    }
    currNode->next = (hittable_list*) malloc(sizeof(hittable_list));
    currNode = currNode->next;
    currNode->hittable = h;
    currNode->is_hit = f;
    currNode->next = NULL;
}

void delete_list(hittable_list *l){
    hittable_list *currNode;
    while(l->next != NULL){
        currNode = l->next;
        l->next = currNode->next;
        free(currNode);
    }
}


bool hit(hittable_list *l, ray r, double ray_tmin, 
        double ray_tmax, hit_record *rec){
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = ray_tmax;

    hittable_list *currNode = l;
    while(currNode){
        if((*(currNode->is_hit))(currNode->hittable, r, ray_tmin, closest_so_far,
                    &temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            copy_hit_record(rec, temp_rec);
        }
        currNode = currNode->next;
    }
    return hit_anything;
}
