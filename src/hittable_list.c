#include "hittable_list.h"

void init_list(hittable_list *l){
    l->head = NULL;
    l->size = 0;
}

void add_list(hittable_list *l, void *h, fptr_is_hit f, fptr_get_box get){
    hittable_node *currNode = l->head;

    if(!currNode){
        currNode = (hittable_node*) malloc(sizeof(hittable_node));
        currNode->hittable = h;
        currNode->is_hit = f;
        currNode->next = NULL;
        copy_box(&(l->box), (*get)(h) );
        currNode->get_box = get;
        l->size++;
        l->head = currNode;
        return;
    }

    while(currNode->next != NULL){
       currNode = currNode->next; 
    }
    currNode->next = (hittable_node*) malloc(sizeof(hittable_node));
    currNode = currNode->next;
    currNode->hittable = h;
    currNode->is_hit = f;
    currNode->next = NULL;
    add_boxes(&(l->box), (*get)(h) );
    currNode->get_box = get;
    l->size++;
}

void delete_list(hittable_list *l){
    hittable_node *currNode;
    while(l->head != NULL){
        currNode = l->head;
        l->head = currNode->next;
        free(currNode);
    }
    l->size = 0;
}

hittable_node *index_list(hittable_list *l, int i){
    int n = 0;
    hittable_node *curr = l->head;
    while(n < i){
        if(!curr) return NULL;
        curr = curr->next;
        n++;
    }
    return curr;
}

void sort_list(hittable_list *l, int start, int end, int axis){
    
}

bool hit(hittable_list *l, ray r, double ray_tmin, 
        double ray_tmax, hit_record *rec){
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = ray_tmax;

    hittable_node *currNode = l->head;
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
