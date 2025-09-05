// mouseMaze Project

#include <iostream> // For input and output
#include <stack> // For stack
#include <limits> // For infinity
#include <queue> // For queues
#include <unordered_set> // For unordered_set

# define f 'f' // Barriers
# define t 't' // Allowed spaces
# define SIZE_X 7 // Vertical size
# define SIZE_Y 9 // Horizontal space

using namespace std;

// Original Maze
//char maze[6][6] = { f, f, f, f, f, f,\
//                    f, t, f, t, t, f,\
//                    f, t, f, t, f, f,\
//                    f, t, t, t, t, f,\
//                    f, t, t, f, t, f,\
//                    f, f, f, f, f, f };
// Cheese at 3, 2 init at 1, 1
//char maze[SIZE_X][SIZE_Y] = { f, f, f, f, f, f, f, f,\
//                              f, t, t, t, f, t, t, f,\
//                              f, f, f, t, f, t, f, f,\
//                              f, t, t, t, f, t, t, f,\
//                              f, t, t, t, t, f, t, f,\
//                              f, f, f, f, t, t, t, f,\
//                              f, f, f, f, f, f, f, f };

char maze[SIZE_X][SIZE_Y] = { f, f, f, f, f, f, f, f, f,\
                              f, t, t, t, t, t, t, t, f,\
                              f, t, f, f, f, t, f, t, f,\
                              f, t, f, t, f, t, t, t, f,\
                              f, t, t, t, f, t, f, t, f,\
                              f, f, f, t, t, t, f, t, f,\
                              f, f, f, f, f, f, f, f, f };

struct Point {
    int x, y;
};

struct Node {
    struct Point location;
    float shortestDist = numeric_limits<float>::infinity();
    struct Node* prevNode = nullptr; // Previous node in shortest path from original node
    bool isVisited = false;
    unordered_set<Node*> neighbours;
};

stack<Point> multipath;
stack<Point> currentPathway;

void printMaze(bool withNumbers) {
    cout << "\n\n   ";
    if(withNumbers) for(int i = 0; i < SIZE_Y; i++) cout << " " << i; // To print the top numbers
    cout << "\n";
    for (int i = 0; i < SIZE_X; i++) {
        cout << " ";
        if(withNumbers) cout << " " << i; // To print the side numbers
        for (int j = 0; j < SIZE_Y; j++) {
            cout << ' ' << maze[i][j];
        }
        if(i+1 == SIZE_X/2) cout << "\t\tAny point on the grid is { side, top }";
        cout << endl;
    }
    cout << endl;
}

int rec = 0;

// !*** This function finds the path to the cheese (checks in this order: down, right, left, up) ***!
Point findPath(Point currLoc, Point cheese) {
    //clog << "Entered findPath " << rec++ << endl;
    int x = currLoc.x, y = currLoc.y, possibleMoves = 0;
    bool moved = false;
    int toMoveTo[2] = { -1, -1 };

    //clog << "{x, y} = " << x << ", " << y << endl;

    // If current location is the cheese then return true
    if (cheese.x == x && cheese.y == y) {
        // currentPathway.push({ x, y }); // Uncomment if you do not want to show the symbol c when priting pathway
        maze[x][y] = 'c';
        return currLoc;
    }

    if (maze[x + 1][y] == t) { // Check down: 0
        //clog << "Entered down" << endl;
        possibleMoves++; // Add 1
        // Update to move to position
        toMoveTo[0] = x + 1; // X
        toMoveTo[1] = y; // Y
        moved = true;
    }
    if (maze[x][y + 1] == t) { // Check right: 1
        //clog << "Entered right" << endl;
        possibleMoves++; // Add 1
        if (!moved) {
            // Update to move to position
            toMoveTo[0] = x; // X
            toMoveTo[1] = y + 1; // Y
            moved = true;
        }
    }
    if (maze[x][y - 1] == t) { // Check left: 2
        //clog << "Entered left" << endl;
        possibleMoves++; // Add 1
        if (!moved) {
            // Update to move to position
            toMoveTo[0] = x; // X
            toMoveTo[1] = y - 1; // Y
            moved = true;
        }
    }
    if (maze[x - 1][y] == t) { // Check up: 3
        //clog << "Entered up" << endl;
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
    //clog << "Pushed: " << x << ", " << y << " to currentPath" << endl;

    // If there is a multipath
    if (possibleMoves >= 2) {
        
        // Add current location to second stack
        multipath.push({ x, y });
        //clog << "Pushed: " << x << ", " << y << " to multipath" << endl;
        
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
        //clog << "Popped multipath" << " " << x << ", " << y << endl;
        
        // Pop currentPathway back to the multipath
        do{
            popped = currentPathway.top();
            //clog << "Popped: " << popped.x << ", " << popped.y << endl;
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
   
    //printMaze(true);
    //clog << "Moved to " << currLoc.x << ", " << currLoc.y << endl;

    // Recurse to the next location <-- idk if thats how you write it ;)
    return findPath(currLoc, cheese);
}

// Function that initializes the findOptimalPath function by creating a vector of all possible spaces and making neighbouring nodes neighbours
vector<Node*> initFindOptimalPath() {
    
    // Create a vector for the whole maze's possible spaces
    vector<Node*> allNodes; // idk whats going on anymore (i need to first create the vector for all t spaces then see neighbours)
    
    // Add all possible spaces mouse can go to to the allNodes vector
    for(int i = 0; i < SIZE_X; i++) {
        for(int j = 0; j < SIZE_Y; j++) {
            if(maze[i][j] == 't') {
//                //clog << "posSpaces i: " << i << " j: " << j << endl;
                Node* n = new Node;
                n->location.x = i;
                n->location.y = j;
                allNodes.push_back(n);
            }
        }
    }
    
    //clog << "Size of stack of t's: " << allNodes.size() << endl;
    
    // Two loops to go through each node and check if any of the other nodes are its neighbours
    for(Node* n1 : allNodes) {
        for(Node* n2 : allNodes) {
            
            // If n2 is a neighbour of n1, add each as a neighbour of the other (if not already added)
            // A'B'(C^D) + C'D'(A^B) (true iff the total distance of n2 from n1 is exactly 1 (either in x or y direction))
            int dx = abs(n1->location.x - n2->location.x);
            int dy = abs(n1->location.y - n2->location.y);
            
            if(dx + dy == 1) {
                
                // Check
//                bool connected = false;
//                for(Node* nn : n1.neighbours) {
//                    if(nn->location.x == n2.location.x && nn->location.y == n2.location.y) {
//                        connected = true;
//                        break;
//                    }
//                }
//                if(!connected) {
                // Need to push the address of the copies of Nodes, because it will be undefined after the loops
                // Its neighbours not n1 or n2, neighbours is a vector of type Node*
                auto result = n1->neighbours.insert(n2);
                auto result2 = n2->neighbours.insert(n1);
                if(result.second && result2.second) {
                    //clog << "NOT CONNECTED neighbours\nn1: " << n1->location.x << ", " << n1->location.y << endl;
                    //clog << "n2: " << n2->location.x << ", " << n2->location.y << endl;
                    //clog << "size of n1 nei: " << n1->neighbours.size() << endl;
                    //clog << "size of n2 nei: " << n2->neighbours.size() << endl;
                    //for(Node* nei : n1->neighbours) clog << "Neighbours of n1: " << nei->location.x << ", " << nei->location.y << endl;
                    //for(Node* nei : n2->neighbours) clog << "Neighbours of n2: " << nei->location.x << ", " << nei->location.y << "\n\n";
                }
//                }
                
            }
        }
    }
    
    return allNodes;
}

// Function that finds the most optimal path to the cheese by using BFS
Node* findOptimalPath(vector<Node*> allNodes, Point init, Point cheese) {
     // Dijkstra's Algorithm
     // Breadth-First Search
     
     // Visited & Unvisited nodes
     // Node | Shortest Distance | Previous Node
     // (1) Mark current node as visited, (2) for all its neighbours if the distance there is less than current distance, update the distance and prevNode to current node, (3) Go to least distance neighbouring node (in weighted graph) (4) When printing path GL ig i need to traverse it like a linked list backwards wait its easy just traverse from max distance node
     
//     // Variable for making sure that if there are still spaces left to visit, the loop continues.
//     bool notAllVisited = false;
     
     // There are many loops I am aware, but time complexity does not matter when adding up complexities (n + n + ... = n)
     Node* currNode = nullptr;
     for(Node* n : allNodes)
         if(n->location.x == init.x && n->location.y == init.y) {
             currNode = n;
             currNode->shortestDist = 0;
             break;
         }
     
     // Queue for BFS
     queue<Node*> bfsQueue;
     bfsQueue.push(currNode);
     bool exit = false;
     
     // Loop that keeps on going until the queue is empty, or it finds the cheese
     while(!bfsQueue.empty() && !exit) {
         
         // Assign front of the queue to currNode and pop from queue
         currNode = bfsQueue.front();
         bfsQueue.pop();
         
         //clog << "fop:\ncurrNode.x: " << currNode->location.x << endl;
         //clog << "currNode.y: " << currNode->location.y << endl;
//         //clog << "currNode.neighbours: " << currNode.neighbours[0] << endl;
         
//         for(Node n : allNodes) {
         // Loop that goes through the neighbours of currNode and adds to queue the unvisited ones
         for(Node* nn : currNode->neighbours) {
             //clog << "Entered neighbours loop\n";
             if(!nn->isVisited) {
                 bfsQueue.push(nn);
             }
             
             // If the distance to that neighbouring node is less that its current shortestDist then update shortestDist and prevNode
             if(currNode->shortestDist + 1 < nn->shortestDist) {
                 //clog << "the dist is shorter so we update the prevNode & dist nn.x: " << nn->location.x << ", " << nn->location.y << endl;
                 nn->shortestDist = currNode->shortestDist + 1;
                 nn->prevNode = currNode;
             }
             
             // If cheese is found break from loop
             if(nn->location.x == cheese.x && nn->location.y == cheese.y) {
                 // Maybe break from other loop as well
//                 currNode = *nn;
                 exit = true;
                 break;
             }
             
         }
//         }
         
     }
     
     return currNode; // Return the node before the cheese
}


//void printPathToCheese() {
//    
//    // Create a copy of the stack using copy constructor
//    stack<Point> copy(currentPathway);
//    
//    cout << "Actual pathway to the cheese (o) :" << endl;
//    while(!copy.empty()) {
//        Point popped = copy.top();
//        cout << popped.x << ", " << popped.y << "} {"; // To print each location of path
//        maze[popped.x][popped.y] = 'o';
//        copy.pop();
//    }
//    printMaze(true);
//}

void printOptimalPath(Point init, Node* currNode) {
    
    // Stack for the optimal path
    stack<Node*> optimalPath;
    
    // Loop that pushes the optimal path to a stack
    // A' + B' is the same as (A x B)' (De Morgan's Law)
    while(currNode->prevNode) {
        //clog << "inLoop currNode->location.x: " << currNode->location.x << " currNode->location.y: " << currNode->location.y << " init.x: " << init.x << " init.y: " << init.y << " prevNode: " << currNode->prevNode->location.x << ", " << currNode->prevNode->location.y << endl;
        optimalPath.push(currNode);
        currNode = currNode->prevNode;
    }
    
    // Push the initial position to stack
    optimalPath.push(currNode);
    
    // Loop that changes the maze to show the optimal path
    while(!optimalPath.empty()) {
        //clog << " exited the in loop that pushes opt path to stack now in emptying stack to maze loop\n";
        //clog << "inLoop currNode->location.x: " << currNode->location.x << " currNode->location.y: " << currNode->location.y << endl;
        currNode = optimalPath.top();
        maze[currNode->location.x][currNode->location.y] = 'o';
        optimalPath.pop();
    }
    
    printMaze(true);
    
}
int main() {
    
    // While loop for popping until I go back to stack pushed position so end pathway is correct one maybe
    // OR we could have the whole path even if a path reached a dead end
    
    // Implement find (maybe with a seperate function) the most efficient path to the cheese
    
    // Add the ability to add any maze (add a checker that checks if the maze is legit (has 4 walls)
    
    Point cheese { 1, 1 };
    
    Point init { 1, 7 };
    
    // Needs to be before findPath because it looks for t spaces to find the optimal path, findPath changes to *
   
    // Print the maze before finding the path to cheeese
    cout << "The initial position of the mouse is { " << init.x << ", " << init.y << " } and the cheese is at { " << cheese.x << ", " << cheese.y << " }";
    printMaze(true);
    
    vector<Node*> possibleSpaces = initFindOptimalPath();
    
    // To check if the cheese has been found or not
    Point foundCheese = findPath(init, cheese);
    bool found;
    if(foundCheese.x == -1 && foundCheese.y == -1) found = false;
    else found = true;

    if (found){
        cout << "Mouse found the cheese!\nCheese is at: " << foundCheese.x << ", " << foundCheese.y << endl;
        
        // Print maze with pathway the mouse took to find the cheese
        cout << "Pathway taken by the mouse to find the cheese:" << endl;
        printMaze(true);
        
        // Prints where the cheese is and where the initial position of the mouse is
        cout << "The initial position of the mouse is { " << init.x << ", " << init.y << " }\n";
        
        // Prints the most optimal path to cheese
        printOptimalPath(init, findOptimalPath(possibleSpaces, init, cheese));
        
        // Print maze with the pathway to the cheese (not necessarily the most optimal)
//        printPathToCheese();
    }
    else{
        cout << "Mouse did not find the cheese" << endl;
        
        // Print maze with pathway the mouse took to try to find the cheese
        cout << "Pathway taken by the mouse to try to find the cheese:" << endl;
        printMaze(true);
    }

    return 0;
}
