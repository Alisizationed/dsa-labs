#include <stdio.h>
#include "stack.h"
#include "simple_queue.h"
#include "circular_queue.h"
#include "priority_queue.h"
#include "deque.h"

int main() {
    // Display users' choices
    printf("Queue versions:\n");
    printf("1 - stack\n");
    printf("2 - simple queue\n");
    printf("3 - double ended queue\n");
    printf("4 - circular queue\n");
    printf("5 - priority queue\n");
    int choice;

    // Read the choice
    printf("Choice: ");
    scanf("%d", &choice);

    //Run the corresponding main version
    switch (choice) {
        case 1:
            stack_menu();
            break;
        case 2:
            simple_queue_menu();
            break;
        case 3:
            circular_queue_menu();
            break;
        case 4:
            priority_queue_menu();
            break;
        case 5:
            deque_menu();
            break;
        default:
            printf("Invalid choice\n");
    }
    return 0;
}
