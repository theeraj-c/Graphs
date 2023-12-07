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
    //Graph* graph = new Graph();
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

    void printGraph() {
        for (int i = 0; i < vertices->getSize(); ++i) {
            Vertex& vertex = vertices->getAt(i);
            std::cout << "Vertex " << vertex.id << " Edges: ";
            for (int j = 0; j < vertex.numEdges; ++j) {
                Edge& edge = vertex.edges[j];
                std::cout << "(" << edge.destination << ", " << edge.distance << ", " << edge.speedLimit << ") ";
            }
            std::cout << std::endl;
        }
    }

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

// Add the following function to your Graph class

void Graph::print(int a) {
    if (a <= 0 || a > 500000) {
        throw illegal_exception();
    }

    Vertex& vertexA = getVertexById(a);

    if (vertexA.numEdges == 0) {
        std::cout << std::endl;  // Print a blank line if no adjacent vertices
        return;
    }

    for (int i = 0; i < vertexA.numEdges; ++i) {
        std::cout << vertexA.edges[i].destination << " ";
    }
    std::cout << std::endl;
}


int main() {
    // Specify the initial capacity of the graph
    Graph* graph = new Graph();

    int a, b;
    double d, s;

    // Test case 1
    a = 1;
    b = 2;
    d = 10.0;
    s = 5.0;
    try {
        graph->insert(a, b, d, s);
    } catch (const illegal_exception& e) {
        std::cout << e.what() << std::endl;
    }

    // Test case 2
    a = 0;
    b = 2;
    d = 10.0;
    s = 5.0;
    try {
        graph->insert(a, b, d, s);
    } catch (const illegal_exception& e) {
        std::cout << e.what() << std::endl;
    }

    // Test case 3
    a = 3;
    b = 4;
    d = -5.0;
    s = 3.0;
    try {
        graph->insert(a, b, d, s);
    } catch (const illegal_exception& e) {
        std::cout << e.what() << std::endl;
    }

    // Test case 4
    a = 5;
    b = 6;
    d = 20.0;
    s = -2.0;
    try {
        graph->insert(a, b, d, s);
    } catch (const illegal_exception& e) {
        std::cout << e.what() << std::endl;
    }

    // Test case 5
    a = 1;
    b = 2;
    d = 15.0;  // Update distance
    s = 6.0;   // Update speed limit
    try {
        graph->insert(a, b, d, s);
    } catch (const illegal_exception& e) {
        std::cout << e.what() << std::endl;
    }

    // Test case 6 (valid new edge)
    a = 1;
    b = 3;
    d = 8.0;
    s = 4.0;
    try {
        graph->insert(a, b, d, s);
    } catch (const illegal_exception& e) {
        std::cout << e.what() << std::endl;
    }

    // Test case 7 (invalid vertex IDs)
    a = -1;
    b = 2;
    d = 10.0;
    s = 5.0;
    try {
        graph->insert(a, b, d, s);
    } catch (const illegal_exception& e) {
        std::cout << e.what() << std::endl;
    }

    // Test case 8 (invalid distance)
    a = 4;
    b = 5;
    d = 0.0;
    s = 3.0;
    try {
        graph->insert(a, b, d, s);
    } catch (const illegal_exception& e) {
        std::cout << e.what() << std::endl;
    }

    // Test case 9 (invalid speed limit)
    a = 6;
    b = 7;
    d = 12.0;
    s = -1.0;
    try {
        graph->insert(a, b, d, s);
    } catch (const illegal_exception& e) {
        std::cout << e.what() << std::endl;
    }

    // Test case 10 (invalid vertex ID > 500000)
    a = 600000;
    b = 2;
    d = 10.0;
    s = 5.0;
    try {
        graph->insert(a, b, d, s);
    } catch (const illegal_exception& e) {
        std::cout << e.what() << std::endl;
    }
    

    // Test case 1
    a = 1;
    std::cout << "PRINT " << a << std::endl;
    try {
        graph->print(a);
    } catch (const illegal_exception& e) {
        std::cout << e.what() << std::endl;
    }

    // Test case 2
    a = 2;
    std::cout << "PRINT " << a << std::endl;
    try {
        graph->print(a);
    } catch (const illegal_exception& e) {
        std::cout << e.what() << std::endl;
    }

    // Test case 3
    a = 1;
    std::cout << "PRINT " << a << std::endl;
    try {
        graph->print(a);
    } catch (const illegal_exception& e) {
        std::cout << e.what() << std::endl;
    }

    // Test case 4
    a = -1;
    std::cout << "PRINT " << a << std::endl;
    try {
        graph->print(a);
    } catch (const illegal_exception& e) {
        std::cout << e.what() << std::endl;
    }

    // Test case 5
    a = 600000;
    std::cout << "PRINT " << a << std::endl;
    try {
        graph->print(a);
    } catch (const illegal_exception& e) {
        std::cout << e.what() << std::endl;
    }

    // Test case 6
    a = 500000;
    std::cout << "PRINT " << a << std::endl;
    try {
        graph->print(a);
    } catch (const illegal_exception& e) {
        std::cout << e.what() << std::endl;
    }

    // Test case 7
    a = 1;
    std::cout << "PRINT " << a << std::endl;
    try {
        graph->print(a);
    } catch (const illegal_exception& e) {
        std::cout << e.what() << std::endl;
    }

    // Print the final state of the graph
    graph->printGraph();

    delete graph;

    return 0;
}
