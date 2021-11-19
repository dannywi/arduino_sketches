#ifndef ROLLING_ARRAY_H
#define ROLLING_ARRAY_H

template<typename T>
bool less_than(T a, T b) {
  if (a == T{0})
    return b;
  if (b == T{0})
    return a;
  return a < b;
}

template<typename T, size_t SLOT>
struct rolling_array {
  T vals[SLOT];
  size_t idx = 0;
 
  rolling_array() {
    for (size_t i = 0; i < SLOT; ++i)
      vals[i] = T{0};
  }
 
  void insert(T val) {
    vals[idx++] = val;
    if (idx == SLOT)
      idx = 0;
  }

  T get_min() const {
    T min = T{0};
    for (size_t i = 0; i < SLOT; ++i) {
      if (less_than(vals[i], min))
        min = vals[i];
    }
    return min;
  }

  T get_max() const {
    T max = T{0};
    for (size_t i = 0; i < SLOT; ++i) {
      if (less_than(max, vals[i]))
        max = vals[i];
    }
    return max;
  }
};

template<
  typename T,
  typename TM_T = unsigned long,
  TM_T DAY_SEC = 86400,
  TM_T INTERVAL_SEC = 1800
>
class rolling_minmax {
public:
  T get_max() const {
    T arr_max = array_day_max_.get_max();
    if (less_than(arr_max, slot_max_))
      return slot_max_;
    return arr_max;
  }

  T get_min() const {
    T arr_min = array_day_min_.get_min();
    if (less_than(slot_min_, arr_min))
      return slot_min_;
    return arr_min;
  }

  void add_val(TM_T tm, T val) {
    if (tm - prev_slot_update_ > tm * 1000) {
      array_day_min_.insert(slot_min_);
      array_day_max_.insert(slot_max_);
      slot_min_ = slot_max_ = 0;
      prev_slot_update_ = tm;      
    } else {
      if (less_than(val, slot_min_))
        slot_min_ = val;
      if (less_than(slot_max_, val))
        slot_max_ = val;
    }
  }

private:
  constexpr static TM_T SLOT_SEC = 86400 / INTERVAL_SEC;
  rolling_array<T, SLOT_SEC> array_day_min_, array_day_max_;
  TM_T prev_slot_update_ = 0;
  T slot_min_ = T{0};
  T slot_max_ = T{0};
};

#endif