#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// gcc -E trappedMouse trappedMouse.c to see after replace of t, f, and c

#define f 'f'
#define t 't'
#define c 'c'
char maze[6][6] = {f, f, f, f, f, f,\
                   f, t, f, t, t, f,\
                   f, t, f, t, f, f,\
                   f, t, t, t, t, f,\
                   f, t, t, f, c, f,\
                   f, f, f, f, f, f};


struct Mouse {
    int currLoc[2];
    struct Mouse *prev;
};

struct Multipath {
    int loc[2];
    struct Multipath *prev;
};

struct Mouse *top = NULL;

void initMouse(){ // Initialize mouse location [1][1]
    struct Mouse *mouse = (struct Mouse*)malloc(sizeof(struct Mouse));
    mouse->currLoc[0] = 1;
    mouse->currLoc[1] = 1;
    mouse->prev = NULL;
    top = mouse;
}

void pushMov(int currLoc[2]) { // Push into stack of mouse moves // Add printf to what it moved to // Add printf if it finds a dead end
    struct Mouse *el = (struct Mouse*)malloc(sizeof(struct Mouse));
    if(malloc != NULL){
        el->currLoc[0] = currLoc[0];
        el->currLoc[1] = currLoc[1];
        struct Mouse *temp = top;
        if(!temp->prev)
            el->prev = NULL;
        else{
            temp = temp->prev;
            el->prev = temp;
        }
        top = el;
    }
    else
        return;
}

int popMov() { // Pop from stack of mouse moves
    struct Mouse *tmp = top;
    int currLoc[2];
    currLoc[0] = tmp->currLoc[0];
    currLoc[1] = tmp->currLoc[1];
    free(tmp);
    top = top->prev;
    return currLoc[1], currLoc[2]; // But i am not freeing the memory hmm... check my doubly linked list implementation... 
}

void pushPos(int loc[2]){ // Push into stack recording multi-pathed location
    struct Multipath *el = (struct Multipath*)malloc(sizeof(struct Multipath));
    if(malloc != NULL){
        el->loc[0] = loc[0];
        el->loc[1] = loc[1];
        struct Mouse *temp = top;
        if(!temp->prev)
            el->prev = NULL;
        else{
            temp = temp->prev;
            el->prev = temp;
        }
        top = el;
    }
    else
        return;
}

int popPos(){ // Pop from stack recording multi-pathed location
    struct Multipath *tmp = top;
    int loc[2];
    loc[0] = tmp->loc[0];
    loc[1] = tmp->loc[1];
    free(tmp);
    top = top->prev;
    return loc[1], loc[2];
}

int main(){
    initMouse();
    // Need another stack to add
    int x = rn->currLoc[0];
    int y = rn->currLoc[1];
    bool won = false;
    int prevMove = 0; // 0 for down, 1 for right, 2 for left, 3 for up.
    while(!won){ // Can also have infinite loop
        struct Mouse *rn = top; // Current Location (rn = right now)
        int possibleMoves = 0;
        bool addedToStack = false;
        x = rn->currLoc[0];
        y = rn->currLoc[1];
        if(x == 3 && y == 3) // I guess can use it as a check if in each if it didnt break from while loop after reaching c
            won = true;
        else{
            // !*** Don't forget cannot go back to previous move ***!
            if(maze[x + 1][y] == t && prevMove != 3/*|| maze[x][y + 1] == c*/){ // Check down 0
                // add 1 
                possibleMoves++;
                if(maze[x + 1][y] == c){
                    // Move mouse to that and declare won!
                    break;
                }
                prevMove = 0;
                // Move
            }
            if(maze[x][y + 1] == t && prevMove != 2/*|| maze[x][y + 1] == c*/){ // Check right 1
                // add 1
                possibleMoves++;
                if(maze[x][y + 1] == c){
                    // Move mouse to that and declare won!
                    break;
                }
                if(possibleMoves >= 2 && addedToStack == false){
                    // add current location to second stack
                    continue;
                }
                prevMove = 1;
                // Move
            }
            if(maze[x - 1][y] == t && prevMove != 1/*|| maze[x][y + 1] == c*/){ // Check left 2
                // add 1
                possibleMoves++;
                if(maze[x - 1][y] == c){
                    // Move mouse to that and declare won!
                    break;
                }
                if(possibleMoves >= 2 && addedToStack == false){
                    addedToStack = true;
                    // add current location to second stack
                    continue;
                }
                prevMove = 2;
                // Move
            }
            if(maze[x][y - 1] == t && prevMove != 0/*|| maze[x][y + 1] == c*/){ // Check up 3
                // add 1
                possibleMoves++;
                if(maze[x][y - 1] == c){
                    // Move mouse to that and declare won!
                    break;
                }
                if(possibleMoves >= 2 && addedToStack == false){
                    // add current location to second stack
                    continue;
                }
                prevMoves = 3;
                // Move
            }
        }
    }
    return 0;
}