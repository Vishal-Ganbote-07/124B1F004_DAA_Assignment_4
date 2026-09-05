#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

class Edge {
public:
    int v;
    int wt;

    Edge(int v, int wt) {
        this->v = v;
        this->wt = wt;
    }
};

class Result {
public:
    int distance;
    vector<int> path;

    Result(int distance, vector<int> path) {
        this->distance = distance;
        this->path = path;
    }
};

Result dijkstra(int source,
                vector<vector<Edge>>& graph,
                vector<int>& hospitals) {

    int V = graph.size();

    vector<int> dist(V, INT_MAX);
    vector<int> parent(V, -1);

    priority_queue<
        pair<int, int>,
        vector<pair<int, int>>,
        greater<pair<int, int>>
    > pq;

    dist[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {

        int u = pq.top().second;
        int currentDist = pq.top().first;

        pq.pop();

        if (currentDist > dist[u])
            continue;

        bool isHospital = false;

        for (int h : hospitals) {
            if (u == h) {
                isHospital = true;
                break;
            }
        }

        if (isHospital) {

            vector<int> path;
            int current = u;

            while (current != -1) {
                path.push_back(current);
                current = parent[current];
            }

            reverse(path.begin(), path.end());

            return Result(dist[u], path);
        }

        for (Edge edge : graph[u]) {

            int v = edge.v;
            int wt = edge.wt;

            if (dist[u] != INT_MAX &&
                dist[v] > dist[u] + wt) {

                dist[v] = dist[u] + wt;
                parent[v] = u;

                pq.push({dist[v], v});
            }
        }
    }

    return Result(-1, {});
}

void addEdge(vector<vector<Edge>>& graph,
             int u, int v, int wt) {

    graph[u].push_back(Edge(v, wt));
    graph[v].push_back(Edge(u, wt));
}

void updateEdgeWeight(vector<vector<Edge>>& graph,
                      int u, int v, int newWeight) {

    for (Edge& edge : graph[u]) {
        if (edge.v == v) {
            edge.wt = newWeight;
        }
    }

    for (Edge& edge : graph[v]) {
        if (edge.v == u) {
            edge.wt = newWeight;
        }
    }
}

void printPath(vector<int>& path) {

    for (int i = 0; i < path.size(); i++) {

        cout << path[i];

        if (i != path.size() - 1)
            cout << " -> ";
    }

    cout << endl;
}

int main() {

    int V, E;

    cout << "Enter number of intersections (vertices): ";
    cin >> V;

    cout << "Enter number of roads (edges): ";
    cin >> E;

    vector<vector<Edge>> graph(V);

    cout << "Enter edges (u v w):" << endl;

    for (int i = 0; i < E; i++) {

        int u, v, w;

        cin >> u >> v >> w;

        addEdge(graph, u, v, w);
    }

    int H;

    cout << "Enter number of hospitals: ";
    cin >> H;

    vector<int> hospitals(H);

    cout << "Enter hospital nodes: ";

    for (int i = 0; i < H; i++) {
        cin >> hospitals[i];
    }

    int source;

    cout << "Enter ambulance starting location: ";
    cin >> source;

    Result result = dijkstra(source, graph, hospitals);

    if (result.distance != -1) {

        cout << "\nInitial shortest travel time: "
             << result.distance << " minutes" << endl;

        cout << "Path: ";
        printPath(result.path);

        cout << "Nearest hospital: "
             << result.path.back() << endl;
    }
    else {

        cout << "\nNo hospital reachable." << endl;
    }

    int u, v, newWeight;

    cout << "\nEnter edge to update (u v newWeight): ";
    cin >> u >> v >> newWeight;

    updateEdgeWeight(graph, u, v, newWeight);

    Result newResult = dijkstra(source, graph, hospitals);

    if (newResult.distance != -1) {

        cout << "\nAfter traffic update:" << endl;

        cout << "New shortest travel time: "
             << newResult.distance << " minutes" << endl;

        cout << "New path: ";
        printPath(newResult.path);

        cout << "Nearest hospital: "
             << newResult.path.back() << endl;
    }
    else {

        cout << "\nNo hospital reachable after update." << endl;
    }

    return 0;
}
