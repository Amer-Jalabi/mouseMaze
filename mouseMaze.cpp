// mouseMaze Project

#include <iostream> // For input and output
#include <stack> // For stack
#include <utility> // For pair

#define f 'f' // Barriers
#define t 't' // Allowed spaces

using namespace std;

char maze[6][6] = { f, f, f, f, f, f,\
                    f, t, f, t, t, f,\
                    f, t, f, t, f, f,\
                    f, t, t, t, t, f,\
                    f, t, t, f, t, f,\
                    f, f, f, f, f, f };


struct Point {
    int x, y;
};

//stack<int[2]> multipathes; // I do not think this works...
//stack<pair<int, int>> multipath;
//stack<pair<int, int>> currentLocation;

stack<Point> multipath;
stack<Point> currentLocation;

void printMaze() {
    cout << "\n  --MAZE--\n" << endl;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            cout << ' ' << maze[i][j];
        }
        cout << endl;
    }
    cout << endl;
}

Point initMaze() {
    Point init { 1, 1 };
    currentLocation.push({init.x, init.y}); // Push first initial position of the mouse
    return init;
}

int rec = 0;

// !*** This function finds the path to the cheese (checks in this order: down, right, left, up) ***!
bool findPath(Point currLoc, Point cheese, int prevMove) {
    clog << "Entered findPath " << rec++ << endl;
    int x = currLoc.x;
    int y = currLoc.y;
    int possibleMoves = 0;
    bool moved = false;
    int toMoveTo[2] = { -1, -1 };

    clog << "{x, y} = " << x << ", " << y << "\nprevMove: " << prevMove << endl;

    // If current location is the cheese then return true
    if (cheese.x == x && cheese.y == y) {
        maze[x][y] = 'c';
        return true;
    }

    // !*** Don't forget cannot go back to previous move ***!
    if (maze[x + 1][y] == t && prevMove != 3) { // Check down: 0
        clog << "Entered down" << endl;
        possibleMoves++; // Add 1
        prevMove = 0;
        // Update to move to position
        toMoveTo[0] = x + 1; // X
        toMoveTo[1] = y; // Y
        moved = true;
    }
    if (maze[x][y + 1] == t && prevMove != 2) { // Check right: 1
        clog << "Entered right" << endl;
        possibleMoves++; // Add 1
        if (!moved) {
            prevMove = 1;
            // Update to move to position
            toMoveTo[0] = x; // X
            toMoveTo[1] = y + 1; // Y
            moved = true;
        }
    }
    if (maze[x][y - 1] == t && prevMove != 1) { // Check left: 2
        clog << "Entered left" << endl;
        possibleMoves++; // Add 1
        if (!moved) {
            prevMove = 2;
            // Update to move to position
            toMoveTo[0] = x; // X
            toMoveTo[1] = y - 1; // Y
            moved = true;
        }
    }
    if (maze[x - 1][y] == t && prevMove != 0) { // Check up: 3
        clog << "Entered up" << endl;
        possibleMoves++; // Add 1
        if (!moved) {
            prevMove = 3;
            // Update to move to position
            toMoveTo[0] = x - 1; // X
            toMoveTo[1] = y; // Y
            moved = true;
        }
    }

    // If there is a multipath
    if (possibleMoves >= 2) {
        // Add current location to second stack
        multipath.push({ x, y });
        clog << "Pushed: " << x << ", " << y << " to multipath" << endl;
    }

    if (possibleMoves == 0) { // Reached a dead end
        Point popped = multipath.top();

        // Go back to the last multipath
        currLoc.x = popped.x;
        currLoc.y = popped.y;
        clog << "Popped multipath" << " " << x << ", " << y << endl;

        // Pop the last multipath (if there was more than 2 pathes it will be pushed back in
        // (IG I can maybe add possible pathes/moves to each portion with the Point struct)
        multipath.pop();
    }
    else {
        // Move to toMoveTo
        currLoc.x = toMoveTo[0];
        currLoc.y = toMoveTo[1];
    }
    maze[x][y] = f; // Make current location f (a barrier) (anything wrong with making a multipath f?)
   
    printMaze();
    clog << "Moved to " << currLoc.x << ", " << currLoc.y << endl;

    // Recurse to the next location <-- idk if thats how you write it ;)
    return findPath(currLoc, cheese, prevMove);
}

int main() {
    // STILL HAVENT PUSHED THE PATH TO THE STACK TO RECORD THE PATHWAY. (only pushed the init pos)
    int prevMove = 0; // 0 for down, 1 for right, 2 for left, 3 for up.
    Point cheese { 1, 4 };

    Point init = initMaze();

    if (findPath(init, cheese, 0))
        cout << "Mouse found the cheese!" << endl;
    else
        cout << "Mouse did not find the cheese" << endl;

    printMaze();

    return 0;
}
