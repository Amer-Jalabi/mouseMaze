// mouseMaze Project

#include <iostream> // For input and output
#include <stack> // For stack
//#include <utility> // For pair

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
Point findPath(Point currLoc, Point cheese) {
    clog << "Entered findPath " << rec++ << endl;
    int x = currLoc.x;
    int y = currLoc.y;
    int possibleMoves = 0;
    bool moved = false;
    int toMoveTo[2] = { -1, -1 };

    clog << "{x, y} = " << x << ", " << y << endl;

    // If current location is the cheese then return true
    if (cheese.x == x && cheese.y == y) {
        maze[x][y] = 'c';
        return currLoc;
    }

    // !*** Don't forget cannot go back to previous move ***!
    if (maze[x + 1][y] == t) { // Check down: 0
        clog << "Entered down" << endl;
        possibleMoves++; // Add 1
        // Update to move to position
        toMoveTo[0] = x + 1; // X
        toMoveTo[1] = y; // Y
        moved = true;
    }
    if (maze[x][y + 1] == t) { // Check right: 1
        clog << "Entered right" << endl;
        possibleMoves++; // Add 1
        if (!moved) {
            // Update to move to position
            toMoveTo[0] = x; // X
            toMoveTo[1] = y + 1; // Y
            moved = true;
        }
    }
    if (maze[x][y - 1] == t) { // Check left: 2
        clog << "Entered left" << endl;
        possibleMoves++; // Add 1
        if (!moved) {
            // Update to move to position
            toMoveTo[0] = x; // X
            toMoveTo[1] = y - 1; // Y
            moved = true;
        }
    }
    if (maze[x - 1][y] == t) { // Check up: 3
        clog << "Entered up" << endl;
        possibleMoves++; // Add 1
        if (!moved) {
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
        // If cheese is not found at all
        if(multipath.empty()) return Point {-1, -1};
        
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
    return findPath(currLoc, cheese);
}

int main() {
    // STILL HAVENT PUSHED THE PATH TO THE STACK TO RECORD THE PATHWAY. (only pushed the init pos)
    // While loop for popping until i go back to stack pushed position so end pathway is correct one maybe
    // OR we could have the whole path even if a path reached a dead end
    Point cheese { 1, 4 };
    
    Point init = initMaze();
    
    // To check if the cheese has been found or not
    Point foundCheese = findPath(init, cheese); //(preMove) 0 for down, 1 for right, 2 for left, 3 for up. REMOVE PREMOVE
    bool found;
    if(foundCheese.x == -1 && foundCheese.y == -1) found = true;
    else found = false;

    if (found)
        cout << "Mouse found the cheese!\nCheese is at: " << foundCheese.x << ", " << foundCheese.y << endl;
    else
        cout << "Mouse did not find the cheese" << endl;

    printMaze();

    return 0;
}
