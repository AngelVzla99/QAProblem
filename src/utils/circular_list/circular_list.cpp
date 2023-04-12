#include "./circular_list.hpp"

// make a circular list
circular_list make_circular_list(int limit) {
  circular_list list;
  list.limit = limit;
  return list;
}

// add a element to the list
void add_element(circular_list *list, pair<int, int> value) {
  list->list.push_back(value);
  if (list->list.size() > list->limit) {
    list->list.erase(list->list.begin());
  }
}

// print the list
void print_circular_list(circular_list *list) {
  for (int i = 0; i < list->list.size(); i++) {
    cout << list->list[i].first << " " << list->list[i].second << endl;
  }
}

// delete a element from the list
void delete_element(circular_list *list, pair<int, int> value) {
  for (int i = 0; i < list->list.size(); i++) {
    if (list->list[i] == value) {
      list->list.erase(list->list.begin() + i);
      break;
    }
  }
}

// add a element to the list in the first position and delete the last element
// if the list is full
void add_filo_element(circular_list *list, pair<int, int> value) {
  list->list.insert(list->list.begin(), value);
  if (list->list.size() > list->limit) {
    list->list.pop_back();
  }
}

// check if a element is in the list
bool is_in_list(circular_list *list, pair<int, int> value) {
  for (int i = 0; i < list->list.size(); i++) {
    if (list->list[i] == value) {
      return true;
    }
  }
  return false;
}
