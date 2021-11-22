#ifndef ROLLING_ARRAY_H
#define ROLLING_ARRAY_H

template<typename T, typename F>
T comp_non_zero(T a, T b, F f) {
  if (a == T{0})
    return b;
  if (b == T{0})
    return a;
  return f(a, b);
}

template<typename T>
T lesser(T a, T b) { return comp_non_zero(a, b, [](T a, T b) { return a < b ? a : b; }); };
template<typename T>
T bigger(T a, T b) { return comp_non_zero(a, b, [](T a, T b) { return a > b ? a : b; }); };

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
      min = lesser(min, vals[i]);
    }
    return min;
  }

  T get_max() const {
    T max = T{0};
    for (size_t i = 0; i < SLOT; ++i) {
      max = bigger(max, vals[i]);
    }
    return max;
  }

  T get_avg() const {
    T acc = T{0};
    T cnt = T{0};
    for (size_t i = 0; i < SLOT; ++i) {
      if (vals[i] == T{0}) continue;
      acc = acc + vals[i];
      cnt = cnt + 1;
    }
    return cnt == T{0} ? T{0} : T{acc / cnt};
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
    return bigger(arr_max, slot_max_);
  }

  T get_min() const {
    T arr_min = array_day_min_.get_min();
    return lesser(arr_min, slot_min_);
  }

  void add_val(TM_T tm, T val) {
    slot_min_ = lesser(val, slot_min_);
    slot_max_ = bigger(val, slot_max_);
    if (tm - prev_slot_update_ > INTERVAL_SEC * 1000) {
      array_day_min_.insert(slot_min_);
      array_day_max_.insert(slot_max_);
      slot_min_ = slot_max_ = 0;
      prev_slot_update_ = tm;      
    }
  }

  bool empty() const { return slot_min_ == T{0}; }
private:
  constexpr static TM_T SLOT_SEC = DAY_SEC / INTERVAL_SEC;
  rolling_array<T, SLOT_SEC> array_day_min_, array_day_max_;
  TM_T prev_slot_update_ = 0;
  T slot_min_ = T{0};
  T slot_max_ = T{0};
};

#endif