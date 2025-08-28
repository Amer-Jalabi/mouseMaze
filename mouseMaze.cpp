// mouseMaze Project

#include <iostream> // For input and output
#include <stack> // For stack
//#include <utility> // For pair

# define f 'f' // Barriers
# define t 't' // Allowed spaces
# define SIZE_X 7
# define SIZE_Y 8

using namespace std;

// Original Maze
//char maze[6][6] = { f, f, f, f, f, f,\
//                    f, t, f, t, t, f,\
//                    f, t, f, t, f, f,\
//                    f, t, t, t, t, f,\
//                    f, t, t, f, t, f,\
//                    f, f, f, f, f, f };

char maze[SIZE_X][SIZE_Y] = { f, f, f, f, f, f, f, f,\
                              f, t, t, t, f, t, t, f,\
                              f, f, f, t, f, t, f, f,\
                              f, t, t, t, f, t, t, f,\
                              f, t, t, t, t, f, t, f,\
                              f, f, f, f, t, t, t, f,\
                              f, f, f, f, f, f, f, f };

struct Point {
    int x, y;
};

//stack<int[2]> multipathes; // I do not think this works...
//stack<pair<int, int>> multipath;
//stack<pair<int, int>> currentPathway;

stack<Point> multipath;
stack<Point> currentPathway;

void printMaze() {
    cout << "\n\n  ";
    for(int i = 0; i < SIZE_Y; i++) cout << " " << i; // To print the top numbers
    cout << "\n";
    for (int i = 0; i < SIZE_X; i++) {
        cout << " " << i; // To print the side numbers
        for (int j = 0; j < SIZE_Y; j++) {
            cout << ' ' << maze[i][j];
        }
        if(i+1 == SIZE_X/2) cout << "\t\tAny point on the grid is { top, side }";
        cout << endl;
    }
    cout << endl;
}

int rec = 0;

// !*** This function finds the path to the cheese (checks in this order: down, right, left, up) ***!
Point findPath(Point currLoc, Point cheese) {
    clog << "Entered findPath " << rec++ << endl;
    int x = currLoc.x, y = currLoc.y, possibleMoves = 0;
    bool moved = false;
    int toMoveTo[2] = { -1, -1 };

    clog << "{x, y} = " << x << ", " << y << endl;

    // If current location is the cheese then return true
    if (cheese.x == x && cheese.y == y) {
        // currentPathway.push({ x, y }); // Uncomment if you do not want to show the symbol c when priting pathway
        maze[x][y] = 'c';
        return currLoc;
    }

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
    
    maze[x][y] = '*'; // Make current location *
    currentPathway.push({ x, y });
    clog << "Pushed: " << x << ", " << y << " to currentPath" << endl;

    // If there is a multipath
    if (possibleMoves >= 2) {
        
        // Add current location to second stack
        multipath.push({ x, y });
        clog << "Pushed: " << x << ", " << y << " to multipath" << endl;
        
        // Move to toMoveTo
        currLoc.x = toMoveTo[0];
        currLoc.y = toMoveTo[1];
    }
    else if (possibleMoves == 0) { // Reached a dead end
        
        // If cheese is not found at all
        if(multipath.empty()) return Point { -1, -1 };
        
        Point popped = multipath.top();

        
        // Go back to the last multipath
        currLoc.x = popped.x;
        currLoc.y = popped.y;
        clog << "Popped multipath" << " " << x << ", " << y << endl;
        
        // Pop currentPathway back to the multipath
        do{
            popped = currentPathway.top();
            cout << "Popped: " << popped.x << ", " << popped.y << endl;
            currentPathway.pop();
        } while(popped.x != currLoc.x || popped.y != currLoc.y);

        // Pop the last multipath (if there was more than 2 pathes it will be pushed back in
        // (IG I can maybe add possible pathes/moves to each portion with the Point struct)
        multipath.pop();
    }
    else {
        // Move to toMoveTo
        currLoc.x = toMoveTo[0];
        currLoc.y = toMoveTo[1];
    }
   
    printMaze();
    clog << "Moved to " << currLoc.x << ", " << currLoc.y << endl;

    // Recurse to the next location <-- idk if thats how you write it ;)
    return findPath(currLoc, cheese);
}

 void findOptimalPath() {
    
}

void printPathToCheese() {
    cout << "Actual pathway to the cheese (o) :" << endl;
    while(!currentPathway.empty()) {
        Point popped = currentPathway.top();
        cout << popped.x << ", " << popped.y << "} {"; // To print each location of path
        maze[popped.x][popped.y] = 'o';
        currentPathway.pop();
    }
    printMaze();
}

int main() {
    
    // While loop for popping until I go back to stack pushed position so end pathway is correct one maybe
    // OR we could have the whole path even if a path reached a dead end
    
    // Implement find (maybe with a seperate function) the most efficient path to the cheese
    
    // Add the ability to add any maze (add a checker that checks if the maze is legit (has 4 walls)
    
    Point cheese { 3, 2 };
    
    Point init { 1, 1 };
    
    // To check if the cheese has been found or not
    Point foundCheese = findPath(init, cheese);
    bool found;
    if(foundCheese.x == -1 && foundCheese.y == -1) found = false;
    else found = true;

    if (found){
        cout << "Mouse found the cheese!\nCheese is at: " << foundCheese.x << ", " << foundCheese.y << endl;
        
        // Print maze with pathway the mouse took to find the cheese
        cout << "Pathway taken by the mouse to find the cheese:" << endl;
        printMaze();
        
        // Print maze with the pathway to the cheese (not necessarily the most optimal)
        printPathToCheese();
    }
    else{
        cout << "Mouse did not find the cheese" << endl;
        
        // Print maze with pathway the mouse took to try to find the cheese
        cout << "Pathway taken by the mouse to try to find the cheese:" << endl;
        printMaze();
    }
    
    return 0;
}
