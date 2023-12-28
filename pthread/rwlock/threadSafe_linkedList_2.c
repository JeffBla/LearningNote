#include <pthread.h>
#include <stdio.h>

struct list_node_s {
    int data;
    struct list_node_s* next;
    pthread_mutex_t mutex;
};

int Member(int value, struct list_node_s* head_p);
int Insert(int value, struct list_node_s** head_pp);
int Delete(int value, struct list_node_s** head_pp);

int main() {
    return 0;
}

int Member(int value, struct list_node_s* head_p) {
    struct list_node_s* curr_p = head_p;

    pthread_mutex_lock(&head_p->mutex);
    while (curr_p != NULL && curr_p->data < value) {
        if (curr_p->next != NULL)
            pthread_mutex_lock(&curr_p->next->mutex);
        pthread_mutex_unlock(&curr_p->mutex);
        curr_p = curr_p->next;
    }

    pthread_mutex_unlock(&curr_p->mutex);
    if (curr_p == NULL || curr_p->data > value) {
        return 0;
    } else {
        return 1;
    }
}

int Insert(int value, struct list_node_s** head_pp) {
    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* pred_p = NULL;
    struct list_node_s* temp_p;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value) {
        temp_p = malloc(sizeof(struct list_node_s));
        temp_p->data = value;
        temp_p->next = curr_p;
        if (pred_p == NULL)
            *head_pp = temp_p;
        else
            pred_p->next = temp_p;
        return 1;
    } else {
        return 0;
    }
}

int Delete(int value, struct list_node_s** head_pp) {
    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* pred_p = NULL;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p != NULL && curr_p->data == value) {
        if (pred_p == NULL) {
            *head_pp = curr_p->next;
            free(curr_p);
        } else {
            pred_p->next = curr_p->next;
            free(curr_p);
        }
        return 1;
    } else {
        return 0;
    }
}
