#include "hittable_list.h"

void init_list(hittable_list *l){
    l->size = 0;
    l->max_size = 10;
}

void add_list(hittable_list *l, void *h, fptr_is_hit f, fptr_get_box get){
    if(l->size == l->max_size){
        l->max_size *= 2;
        hittable_node *temp[l->max_size];
        int i;
        for(i = 0; i < l->size; i++){
            temp[i] = l->list[i];
        }
        *(l->list) = *temp;
    }
    hittable_node *currNode = (hittable_node *) malloc(sizeof(hittable_node));
    currNode->hittable = h;
    currNode->is_hit = f;
    if(l->size == 0){
        copy_box(&(l->box), (*get)(h) );
    } else {
        add_boxes(&(l->box), (*get)(h) );
    }
    currNode->get_box = get;
    l->list[l->size] = currNode;
    l->size++;
}

void delete_list(hittable_list *l){
    while(l->size > 0){
        l->size--;
        free(l->list[l->size]);
    }
    l->size = 0;
}

hittable_node *index_list(hittable_list *l, int i){
    if(i >= l->size) return NULL;
    return l->list[i];
}

bool box_compare(aabb a, aabb b, int axis){
    if(axis == 0){
        return a.x[0] < b.x[0];
    } else if(axis == 1){
        return a.y[0] < b.y[0];
    } else {
        return a.z[0] < b.z[0];
    }
}

void swap(void *a, void *b){
    void *temp = a;
    a = b;
    b = temp;
}

void sort_list(hittable_list *l, int start, int end, int axis){
   int i, j, min_idx;

   for(i = start; i < end - 1; i++){
        
       min_idx = i;
       for(j = i + 1; j < end; j++){
            if(box_compare((*l->list[i]->get_box)(l->list[i]->hittable), 
                       (*l->list[j]->get_box)(l->list[j]->hittable), axis)){
                min_idx = j;
            }
       }

       if(min_idx != i){
            swap(l->list[i], l->list[min_idx]);
       }
   }
}

bool hit(hittable_list *l, ray r, double ray_tmin, 
        double ray_tmax, hit_record *rec){
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = ray_tmax;

    int i;
    hittable_node *currNode;
    for(i = 0; i < l->size; i++){
        currNode = l->list[i];
        if((*(currNode->is_hit))(currNode->hittable, r, ray_tmin, closest_so_far,
                    &temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            copy_hit_record(rec, temp_rec);
        }
    }
    return hit_anything;
}
