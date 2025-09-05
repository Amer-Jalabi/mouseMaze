// Mouse Maze Project

#include <iostream> // For input and output
#include <stack> // For stack
#include <limits> // For infinity
#include <queue> // For queues
#include <unordered_set> // For unordered_set

# define f 'f' // Barriers
# define t 't' // Allowed spaces
# define SIZE_X 9 // Vertical size
# define SIZE_Y 9 // Horizontal space

using namespace std;

// The mouse maze, t is for allowed spaces and f is for barriers
char maze[SIZE_X][SIZE_Y] = { f, f, f, f, f, f, f, f, f,\
                              f, t, f, t, f, t, t, t, f,\
                              f, t, f, t, f, t, f, t, f,\
                              f, t, f, t, f, t, t, t, f,\
                              f, t, t, t, t, t, f, t, f,\
                              f, t, f, t, t, f, f, t, f,\
                              f, t, t, f, t, t, f, t, f,\
                              f, t, t, t, f, t, f, f, f,\
                              f, f, f, t, t, t, f, f, f };

// Struct for each point (position) in the maze
struct Point {
    int x, y;
};

// Struct for nodes in the graph of possible spaces the mouse can visit
struct Node {
    struct Point location;
    float shortestDist = numeric_limits<float>::infinity();
    struct Node* prevNode = nullptr; // Previous node in shortest path from original node
    bool isVisited = false;
    unordered_set<Node*> neighbours;
};

// Stack for spaces with multiple possible pathways
stack<Point> multipath;

// Stack for the pathway to the current position of the mouse
stack<Point> currentPathway;

// Function that prints the maze, either with the numberings on the sides or without
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

// This function finds the path to the cheese (checks in this order: down, right, left, up)
Point findPath(Point currLoc, Point cheese) {
    
    // Variables: x for current x position of mouse, y for current y position of mouse, and possibleMoves for keeping count of how many possible spaces the mouse can go to in the next move
    int x = currLoc.x, y = currLoc.y, possibleMoves = 0;
    
    // Variable for keeping track of if the mouse moved yet or not
    bool moved = false;
    
    // Array for the position the mouse will move to in the next move
    int toMoveTo[2] = { -1, -1 };

    // If current location is the cheese then return true
    if (cheese.x == x && cheese.y == y) {
        // currentPathway.push({ x, y }); // Uncomment if you do not want to show the symbol c when priting pathway
        maze[x][y] = 'c';
        return currLoc;
    }

    if (maze[x + 1][y] == t) { // Check down: 0
        
        // Add 1 to possibleMoves
        possibleMoves++;
        
        // Update to move to position
        toMoveTo[0] = x + 1; // X
        toMoveTo[1] = y; // Y
        moved = true;
    }
    if (maze[x][y + 1] == t) { // Check right: 1
        
        // Add 1 to possibleMoves
        possibleMoves++;
        
        // If mouse did not move yet, update toMoveTo
        if (!moved) {
            // Update to move to position
            toMoveTo[0] = x; // X
            toMoveTo[1] = y + 1; // Y
            moved = true;
        }
    }
    if (maze[x][y - 1] == t) { // Check left: 2
        
        // Add 1 to possibleMoves
        possibleMoves++;
        
        // If mouse did not move yet, update toMoveTo
        if (!moved) {
            // Update to move to position
            toMoveTo[0] = x; // X
            toMoveTo[1] = y - 1; // Y
            moved = true;
        }
    }
    if (maze[x - 1][y] == t) { // Check up: 3
        
        // Add 1 to possibleMoves
        possibleMoves++;
        
        // If mouse did not move yet, update toMoveTo
        if (!moved) {
            // Update to move to position
            toMoveTo[0] = x - 1; // X
            toMoveTo[1] = y; // Y
            moved = true;
        }
    }
    
    // Make current location *
    maze[x][y] = '*';
    currentPathway.push({ x, y });

    // If there is a multipath
    if (possibleMoves >= 2) {
        
        // Add current location to second stack
        multipath.push({ x, y });
        
        // Move to toMoveTo
        currLoc.x = toMoveTo[0];
        currLoc.y = toMoveTo[1];
    }
    else if (possibleMoves == 0) { // Reached a dead end
        
        // If cheese is not found at all, return -1, -1
        if(multipath.empty()) return Point { -1, -1 };
        
        // Assign popped the top of multipath stack
        Point popped = multipath.top();

        // Go back to the last multipath
        currLoc.x = popped.x;
        currLoc.y = popped.y;
        
        // Pop currentPathway all the way back to the multipath position
        do {
            popped = currentPathway.top();
            currentPathway.pop();
        } while(popped.x != currLoc.x || popped.y != currLoc.y);

        // Pop the last multipath (if there was more than 2 pathes in that multipath it will be pushed back in)
        multipath.pop();
    }
    else {
        
        // Move to toMoveTo
        currLoc.x = toMoveTo[0];
        currLoc.y = toMoveTo[1];
    }

    // Recurse to the next location
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
                
                // Create a new Node for the possible space the mouse can go to
                Node* n = new Node;
                n->location.x = i;
                n->location.y = j;
                
                // Push the new node to the allNodes vector
                allNodes.push_back(n);
            }
        }
    }
    
    // Two loops to go through each node and check if any of the other nodes are its neighbours and add them
    for(Node* n1 : allNodes) {
        for(Node* n2 : allNodes) {
            
            // If n2 is a neighbour of n1, add each as a neighbour of the other (if not already done so)
            // A'B'(C^D) + C'D'(A^B) (true iff the total distance of n2 from n1 is exactly 1 (either in x or y direction))
            int dx = abs(n1->location.x - n2->location.x);
            int dy = abs(n1->location.y - n2->location.y);
            
            if(dx + dy == 1) {
                
                // Its neighbours not n1 or n2, neighbours is a vector of type Node*
                n1->neighbours.insert(n2);
                n2->neighbours.insert(n1);
                
            }
        }
    }
    
    return allNodes; // return the vector (of type Node*) containing all the possible spaces the mouse can visit
}

// Function that finds the most optimal path to the cheese by using BFS
Node* findOptimalPath(vector<Node*> allNodes, Point init, Point cheese) {
    
    // Breadth-First Search
     
    //  (1) Mark current node as visited, (2) For all its neighbours, if they are not visited add it to queue, and if the distance there is less than current distance, update the distance and prevNode to current node, (3) Pop from the front of the queue to current node
     
    // There are many loops I am aware, but time complexity does not matter when adding up complexities (n + n + ... = n)
    
    // Loop to look for the node of the initial position
     Node* currNode = nullptr;
    for(Node* n : allNodes)
     if(n->location.x == init.x && n->location.y == init.y) {
         currNode = n;
         currNode->shortestDist = 0;
         break;
     }
     
     // Queue for BFS
    queue<Node*> bfsQueue;
    
    // Push currNode to bfsQueue
    bfsQueue.push(currNode);
    
    // Variable to exit the while loop when the cheese is found
    bool exit = false;
     
    // Loop that keeps on going until the queue is empty, or it finds the cheese
    while(!bfsQueue.empty() && !exit) {
         
        // Assign front of the queue to currNode and pop from queue
        currNode = bfsQueue.front();
        bfsQueue.pop();
         
        // Loop that goes through the neighbours of currNode and adds to queue the unvisited ones
        for(Node* nn : currNode->neighbours) {
            
            // If that neighbouring node has not been visited, push it to the bfs queue
            if(!nn->isVisited) {
                bfsQueue.push(nn);
            }
             
            // If the distance to that neighbouring node is less that its current shortestDist then update shortestDist and prevNode
            if(currNode->shortestDist + 1 < nn->shortestDist) {
                nn->shortestDist = currNode->shortestDist + 1;
                nn->prevNode = currNode;
            }
             
            // If cheese is found break from both loops
            if(nn->location.x == cheese.x && nn->location.y == cheese.y) {
                exit = true;
                break;
            }
             
         }
    
     }
     
     return currNode; // Return the node before the cheese
}

// Function that adds the optimal path to the maze and prints the maze
void printOptimalPath(Point init, Node* currNode) {
    
    // Stack for the optimal path
    stack<Node*> optimalPath;
    
    // Loop that pushes the optimal path to a stack
    // A' + B' is the same as (A x B)' (De Morgan's Law)
    while(currNode->prevNode) {
        optimalPath.push(currNode);
        currNode = currNode->prevNode;
    }
    
    // Push the initial position to stack
    optimalPath.push(currNode);
    
    // Loop that changes the maze to show the optimal path
    while(!optimalPath.empty()) {
        currNode = optimalPath.top();
        maze[currNode->location.x][currNode->location.y] = 'o';
        optimalPath.pop();
    }
    
    printMaze(true);
    
}
    
int main() {
    
    // Position of cheese
    Point cheese { 1, 1 };
    
    // Position of the mouse initially
    Point init { 1, 7 };
   
    // Print the maze before finding the path to cheeese
    cout << "The initial position of the mouse is { " << init.x << ", " << init.y << " } and the cheese is at { " << cheese.x << ", " << cheese.y << " }";
    printMaze(true);
    
    vector<Node*> possibleSpaces = initFindOptimalPath();
    
    // To check if the cheese has been found or not
    Point foundCheese = findPath(init, cheese);
    
    // Variable to save the result of findPath (if the mouse found the cheese or not)
    bool found;
    
    // If foundCheese was -1, -1 then it was not found, else it was found
    if(foundCheese.x == -1 && foundCheese.y == -1) found = false;
    else found = true;

    if (found) {
        cout << "Mouse found the cheese!\nCheese is at: " << foundCheese.x << ", " << foundCheese.y << endl;
        
        // Print maze with pathway the mouse took to find the cheese
        cout << "Pathway taken by the mouse to find the cheese:" << endl;
        printMaze(true);
        
        // Prints where the cheese is and where the initial position of the mouse is
        cout << "The initial position of the mouse is { " << init.x << ", " << init.y << " }\n";
        
        // Prints the most optimal path to cheese
        printOptimalPath(init, findOptimalPath(possibleSpaces, init, cheese));
        
    }
    else {
        cout << "Mouse did not find the cheese" << endl;
        
        // Print maze with pathway the mouse took to try to find the cheese
        cout << "Pathway taken by the mouse to try to find the cheese:" << endl;
        printMaze(true);
    }

    return 0;
}
