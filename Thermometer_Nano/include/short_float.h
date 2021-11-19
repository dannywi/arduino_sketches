#ifndef SHORT_FLOAT_H
#define SHORT_FLOAT_H

template<int PREC>
class short_float {
public:
  short_float() = default;
  short_float(float v) : v_(v * mult()) {
    // todo: check short max, and maybe do round up/down
  }
  float val() const { return static_cast<float>(v_) / mult(); }

  operator float() const { return val(); }
  bool operator<(short_float<PREC> oth) const { return v_ < oth.v_; }
  bool operator<=(short_float<PREC> oth) const { return v_ <= oth.v_; }
  bool operator==(short_float<PREC> oth) const { return v_ == oth.v_; }
  bool operator>(short_float<PREC> oth) const { return v_ > oth.v_; }
  bool operator>=(short_float<PREC> oth) const { return v_ >= oth.v_; }

private:
  short mult() const { return pow(10, PREC); }
  short v_ = 0;
};

#endif