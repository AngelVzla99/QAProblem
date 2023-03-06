#ifndef CIRCULAR_LIST_HPP
#define CIRCULAR_LIST_HPP

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>

typedef long long ll;

using namespace std;

typedef struct circular_list {
  vector<pair<int, int>> list;
  int limit;
} circular_list;

circular_list make_circular_list(int limit);
void add_element(circular_list *list, pair<int, int> value);
void print_circular_list(circular_list *list);
void delete_element(circular_list *list, pair<int, int> value);
void add_filo_element(circular_list *list, pair<int, int> value);
bool is_in_list(circular_list *list, pair<int, int> value);

template <typename T>
class circular_list_t {
  vector<T> list;
  int limit;

 public:
  circular_list_t(int limit) { this->limit = limit; }
  void add_element(T value) {
    list.push_back(value);
    if (list.size() > limit) {
      list.erase(list.begin());
    }
  }
  void print_circular_list() {
    for (int i = 0; i < list.size(); i++) {
      cout << list[i] << endl;
    }
  }
  void delete_element(T value) {
    for (int i = 0; i < list.size(); i++) {
      if (list[i] == value) {
        list.erase(list.begin() + i);
        break;
      }
    }
  }
  void add_filo_element(T value) {
    list.insert(list.begin(), value);
    if (list.size() > limit) {
      list.pop_back();
    }
  }
  bool is_in_list(T value) {
    for (int i = 0; i < list.size(); i++) {
      if (list[i] == value) {
        return true;
      }
    }
    return false;
  }
  T get_element(int index) { return list[index]; }
};

#endif  // CIRCULAR_LIST_HPP