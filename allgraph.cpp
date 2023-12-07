#include <iostream>
#include <cmath>

class illegal_exception : public std::exception {
public:
    const char* what() const throw() {
        return "illegal argument";
    }
};

class Edge {
public:
    int destination;
    double distance;
    double speedLimit;

    Edge() : destination(0), distance(0.0), speedLimit(0.0) {}

    Edge(int dest, double dist, double speed) : destination(dest), distance(dist), speedLimit(speed) {}
};

class Vertex {
public:
    int id;
    Edge* edges;
    int numEdges;

    Vertex() : id(0), edges(nullptr), numEdges(0) {}

    Vertex(int vertexId) : id(vertexId), edges(nullptr), numEdges(0) {}

    ~Vertex() {
        delete[] edges;
    }
};

class resizableArray {
private:
    Vertex* array;
    int size;
    int currentCapacity;

public:
    resizableArray(int initCap);
    ~resizableArray();
    int getSize();
    int getCapacity();
    Vertex& getAt(int indx);
    bool push(Vertex val);
    void resize(int newSize);
};

class Graph {
private:
    resizableArray* vertices = new resizableArray(500000);
    Vertex& getVertexById(int id) {
        for (int i = 0; i < vertices->getSize(); ++i) {
            Vertex& vertex = vertices->getAt(i);
            if (vertex.id == id) {
                return vertex;
            }
        }

        // If the vertex is not found, add it to the graph
        vertices->push(Vertex(id));
        return vertices->getAt(vertices->getSize() - 1);
    }

    bool edgeExists(int from, int to) {
        auto& vertex = getVertexById(from);
        for (int i = 0; i < vertex.numEdges; ++i) {
            if (vertex.edges[i].destination == to) {
                return true;
            }
        }
        return false;
    }

    void addEdge(Vertex& vertexA, Vertex& vertexB, double d, double s) {
        Edge newEdgeToB(vertexB.id, d, s);
        Edge newEdgeToA(vertexA.id, d, s);

        Edge* tempEdgesA = new Edge[vertexA.numEdges + 1];
        for (int i = 0; i < vertexA.numEdges; ++i) {
            tempEdgesA[i] = vertexA.edges[i];
        }

        Edge* tempEdgesB = new Edge[vertexB.numEdges + 1];
        for (int i = 0; i < vertexB.numEdges; ++i) {
            tempEdgesB[i] = vertexB.edges[i];
        }

        tempEdgesA[vertexA.numEdges] = newEdgeToB;
        tempEdgesB[vertexB.numEdges] = newEdgeToA;

        delete[] vertexA.edges;
        delete[] vertexB.edges;

        vertexA.edges = tempEdgesA;
        vertexA.numEdges++;

        vertexB.edges = tempEdgesB;
        vertexB.numEdges++;
    }

public:
    Graph(int initCapacity = 500000) : vertices(new resizableArray(initCapacity)) {}

    void insert(int a, int b, double d, double s) {
        if (a <= 0 || b <= 0 || a > 500000 || b > 500000 || d <= 0 || s <= 0) {
            throw illegal_exception();
        }

        Vertex& vertexA = getVertexById(a);
        Vertex& vertexB = getVertexById(b);

        if (edgeExists(a, b)) {
            // Update existing edge
            for (int i = 0; i < vertexA.numEdges; ++i) {
                if (vertexA.edges[i].destination == b) {
                    vertexA.edges[i].distance = d;
                    vertexA.edges[i].speedLimit = s;
                    break;
                }
            }

            std::cout << "success" << std::endl;
        } else {
            // Add new edge
            addEdge(vertexA, vertexB, d, s);
            std::cout << "success" << std::endl;
        }
    }
    void print(int a);
    void deleteVertex(int a);

    ~Graph(){
        delete vertices;
    }
};

// resizableArray member function definitions
resizableArray::resizableArray(int initCap) : currentCapacity(initCap), size(0) {
    array = new Vertex[initCap];
}

resizableArray::~resizableArray() {
    delete[] array;
}

int resizableArray::getSize() {
    return size;
}

int resizableArray::getCapacity() {
    return currentCapacity;
}

Vertex& resizableArray::getAt(int indx) {
    if (indx < 0 || indx >= size) {
        // You might want to handle this differently, e.g., throw an exception
        std::cerr << "Index out of bounds." << std::endl;
    }
    return array[indx];
}

bool resizableArray::push(Vertex val) {
    if (size == currentCapacity - 1) {
        Vertex* newArray = new Vertex[currentCapacity * 2];
        for (int i = 0; i < size; i++)
            newArray[i] = array[i];
        currentCapacity *= 2;
        delete[] array;
        array = newArray;
    }

    array[size] = val;
    size++;
    return true;
}

void resizableArray::resize(int newSize) {
    if (newSize <= 0) {
        return;
    }

    if (newSize < currentCapacity) {
        if (newSize >= size) {
            Vertex* newArray = new Vertex[newSize];
            for (int i = 0; i < size; i++)
                newArray[i] = array[i];
            currentCapacity = newSize;
            delete[] array;
            array = newArray;
        }
    } else if (newSize > currentCapacity) {
        Vertex* newArray = new Vertex[newSize];
        for (int i = 0; i < size; i++)
            newArray[i] = array[i];
        currentCapacity = newSize;
        delete[] array;
        array = newArray;
    }
}

//Print function
void Graph::print(int a) {
    if (a <= 0 || a > 500000) {
        throw illegal_exception();
    }

    bool found = false;

    // Iterate over the vertices to find the vertex with ID 'a'
    for (int i = 0; i < vertices->getSize(); ++i) {
        Vertex& vertexA = vertices->getAt(i);

        // Check if the current vertex has the ID 'a'
        if (vertexA.id == a) {
            found = true;

            if (vertexA.numEdges == 0) {
                std::cout << std::endl;  // Print a blank line if no adjacent vertices
            } else {
                for (int j = 0; j < vertexA.numEdges; ++j) {
                    std::cout << vertexA.edges[j].destination << " ";
                }
                std::cout << std::endl;
            }

            break;
        }
    }

    // If the vertex is not found
    if (!found) {
        std::cout << "failure" << std::endl;
    }
}


//Delete function
void Graph::deleteVertex(int a) {
    if (a <= 0 || a > 500000) {
        throw illegal_exception();
    }

    bool found = false;
    int indexToDelete = -1;

    // Iterate over the vertices to find the vertex with ID 'a'
    for (int i = 0; i < vertices->getSize(); ++i) {
        Vertex& currentVertex = vertices->getAt(i);

        // Check if the current vertex has the ID 'a'
        if (currentVertex.id == a) {
            found = true;
            indexToDelete = i;

            // Delete the edges array of the current vertex
            delete[] currentVertex.edges;

            break;
        }
    }

    // If the vertex is found
    if (found) {
        // Iterate over all other vertices to remove edges pointing to 'a'
        for (int j = 0; j < vertices->getSize(); ++j) {
            if (j != indexToDelete) {
                Vertex& otherVertex = vertices->getAt(j);

                // Remove edges pointing to 'a'
                for (int k = 0; k < otherVertex.numEdges; ++k) {
                    if (otherVertex.edges[k].destination == a) {
                        // Remove the edge
                        for (int l = k; l < otherVertex.numEdges - 1; ++l) {
                            otherVertex.edges[l] = otherVertex.edges[l + 1];
                        }
                        --otherVertex.numEdges;

                        // Resize the edges array
                        Edge* newEdges = new Edge[otherVertex.numEdges];
                        for (int l = 0; l < otherVertex.numEdges; ++l) {
                            newEdges[l] = otherVertex.edges[l];
                        }
                        delete[] otherVertex.edges;
                        otherVertex.edges = newEdges;
                        break;
                    }
                }
            }
        }

        // Remove the vertex
        for (int j = indexToDelete; j < vertices->getSize() - 1; ++j) {
            vertices->getAt(j) = vertices->getAt(j + 1);
        }

        // Resize the array to remove the last element
        vertices->resize(vertices->getSize() - 1);
        
        std::cout << "success" << std::endl;
    } else {
        // If the vertex is not found
        std::cout << "failure" << std::endl;
    }
}

int main() {
    Graph* graph = new Graph();
    while (true) {
        std::string command;
        std::cin >> command;

        if (command == "INSERT") {
            int a, b;
            double d, s;
            std::cin >> a >> b >> d >> s;

            try {
                graph->insert(a, b, d, s);
            } catch (const illegal_exception& e) {
                std::cout << e.what() << std::endl;
            }

        }

        else if (command == "PRINT"){
            int a;
            std::cin >> a;

            try {
                graph->print(a);
            } catch (const illegal_exception& e) {
                std::cout << e.what() << std::endl;
            }
        }

        else if (command == "DELETE") {
            int a;
            std::cin >> a;

            try {
                graph->deleteVertex(a);
            } catch (const illegal_exception& e) {
                std::cout << e.what() << std::endl;
            }   
        }
        
        else if (command == "END") {
            break;
        }
    }

    delete graph;
    return 0;
}

// following code for testing
/*
CITATION:

FOLLOWING TEST CODE GENERATED BY CHATGPT
*/

/*
int main() {
    Graph graph(100);

    // Test case 1: Insert a valid edge
    try {
        graph->insert(1, 2, 10.0, 5.0);
    } catch (const illegal_exception& e) {
        std::cout << "Test case 1 failed: " << e.what() << std::endl;
    }

    // Test case 2: Insert an edge with invalid vertex
    try {
        graph->insert(0, 2, 10.0, 5.0);
    } catch (const illegal_exception& e) {
        // This should print "illegal argument"
        std::cout << "Test case 2 passed: " << e.what() << std::endl;
    }

    // Test case 3: Insert an edge with invalid distance
    try {
        graph->insert(3, 4, -5.0, 3.0);
    } catch (const illegal_exception& e) {
        // This should print "illegal argument"
        std::cout << "Test case 3 passed: " << e.what() << std::endl;
    }

    // Test case 4: Insert an edge with invalid speed limit
    try {
        graph->insert(5, 6, 20.0, -2.0);
    } catch (const illegal_exception& e) {
        // This should print "illegal argument"
        std::cout << "Test case 4 passed: " << e.what() << std::endl;
    }

    // Add more test cases as needed

    return 0;
}

*/