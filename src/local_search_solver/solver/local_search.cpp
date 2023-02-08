#include "local_search.hpp"
#include <set>

vector<int> generate_random_positions(int N) {
    // set of numbers
    set<int> used;
    vector<int> positions;
    for (int i=0;i<N;i++) {
        int pos = rand()%N;
        while (used.find(pos) != used.end()) {
            pos = rand()%N;
        }
        used.insert(pos);
        positions.push_back(pos);
    }

    return positions;
}

vector<int> find_best_neighbour(QAP instance_qap, vector<int> positions) {
    vector<int> best_neighbour = positions;
    int best_cost = get_cost(instance_qap, positions);
    for (int i=0;i<instance_qap.N;i++) {
        for (int j=i+1;j<instance_qap.N;j++) {
            vector<int> neighbour = positions;
            int aux = neighbour[i];
            neighbour[i] = neighbour[j];
            neighbour[j] = aux;
            int cost = get_cost(instance_qap, neighbour);
            if (cost < best_cost) {
                best_cost = cost;
                best_neighbour = neighbour;
            }
        }
    }
    return best_neighbour;
}

QAP_solution local_search_solution(QAP instance_qap) {
    printf("Local search solution\n");

    cout << "\nInstance\n\n";
    cout << QAP_to_string(instance_qap) << '\n';

    vector<int> old_positions, new_positions;

    new_positions = generate_random_positions(instance_qap.N);

    cout << "Initial positions\n";
    for (int i=0;i<instance_qap.N;i++) {
        cout << new_positions[i] << " ";
    }
    cout << '\n';

    do {
        old_positions = new_positions;
        new_positions = find_best_neighbour(instance_qap, old_positions);
    } while (new_positions != old_positions);

    QAP_solution solution;
    solution.cost = get_cost(instance_qap, new_positions);
    solution.positions = new_positions;
    return solution;
}


QAP_solution start_local_search_solution(vector<int> start, QAP instance_qap){
    vector<int> old_positions, new_positions;
    new_positions = start;

    do {
        old_positions = new_positions;
        new_positions = find_best_neighbour(instance_qap, old_positions);
    } while (new_positions != old_positions);

    QAP_solution solution;
    solution.cost = get_cost(instance_qap, new_positions);
    solution.positions = new_positions;
    return solution;
}