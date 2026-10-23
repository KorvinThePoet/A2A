#include "datetime.hpp"

namespace DateTime{
// TIME.CPP

time::time(int hour, int minute, int second) {
  if (hour >= 24 || hour < 0) {
    throw std::invalid_argument(
        "time::time: hours must be between 0 and 23");
  }
  if (minute >= 60 || minute < 0) {
    throw std::invalid_argument(
        "time::time: minutes must be between 0 and 59");
  }
  if (second >= 60 || second < 0) {
    throw std::invalid_argument(
        "time::time: seconds must be between 0 and 59");
  }

  total_seconds_ = hour * 60 * 60 + minute * 60 + second;
}

int time::hour() const { return this->total_seconds_ / 3600; }

int time::minute() const {
  return (this->total_seconds_ % 3600) / 60;
}

int time::second() const { return this->total_seconds_ % 60; }

void time::add_seconds(int nb_seconds) {
  if (total_seconds_ + nb_seconds >= 24 * 60 * 60) {
    total_seconds_ = 24 * 60 * 60 - 1;
  } else if (total_seconds_ + nb_seconds < 0) {
    total_seconds_ = 0;
  } else {
    total_seconds_ += nb_seconds;
  }
}

void time::add_hours(int nb_hours) { add_seconds(nb_hours * 3600); }

void time::add_minutes(int nb_minutes) {
  add_seconds(nb_minutes * 60);
}

int time::total_seconds() const { return this->total_seconds_; }
bool operator==(const time& lhs,
                          const time& rhs) {
  return lhs.hour() == rhs.hour() && lhs.minute() == rhs.minute() &&
         lhs.second() == rhs.second();
}
bool operator!=(const time& lhs,
                          const time& rhs) {
  return !(rhs == lhs);
}
bool operator<(const time& lhs, const time& rhs) {
  return lhs.total_seconds() < rhs.total_seconds();
}
bool operator>(const time& lhs, const time& rhs) {
  return !(lhs <= rhs);
}
bool operator<=(const time& lhs,
                          const time& rhs) {
  return lhs < rhs || lhs == rhs;
}
bool operator>=(const time& lhs,
                          const time& rhs) {
  return !(lhs < rhs);
}

timediff time::operator-(const time& rhs) const {
  time t = *this;
  int mn = 1;
  t.total_seconds_ -= rhs.total_seconds_;
  if (t.total_seconds_ < 0) {
    mn = -1;
    t.total_seconds_ *= -1;
  }
  int d = t.total_seconds_ / 86400;
  t.total_seconds_ %= 86400;

  return timediff(mn * d, mn * t.hour(), mn * t.minute(), mn * t.second());
}

std::string to_string(const time& rhs) {
  std::string result = "HH:mm:ss";

  result[0] = static_cast<char>((rhs.hour() / 10) + 48);  // '0' = 48;
  result[1] = static_cast<char>((rhs.hour() % 10) + 48);

  result[3] = static_cast<char>((rhs.minute() / 10) + 48);
  result[4] = static_cast<char>((rhs.minute() % 10) + 48);

  result[6] = static_cast<char>((rhs.second() / 10) + 48);
  result[7] = static_cast<char>((rhs.second() % 10) + 48);

  return result;
}

// ~~~ TIME.CPP ~~~ //

// DATE.CPP

bool date::is_leapyear(int year) {
  return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int date::days_in_month(int year, int month) {
  static const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if (month == 2 && is_leapyear(year)) {
    return 29;
  }
  return days[month - 1];
}
int date::to_days(int year, int month, int day) {
  int total = 0;
  for (int y = 1; y < year; ++y) {
    total += is_leapyear(y) ? 366 : 365;
  }
  for (int m = 1; m < month; ++m) {
    total += days_in_month(year, m);
  }
  total += day - 1;
  return total;
}

void date::from_days(int days, int& year, int& month, int& day) {
  year = 1;
  while (true) {
    int days_in_year = is_leapyear(year) ? 366 : 365;
    if (days < days_in_year) {
      break;
    }
    days -= days_in_year;
    ++year;
  }

  month = 1;
  while (true) {
    int days_in_month = date::days_in_month(year, month);
    if (days < days_in_month) {
      break;
    }
    days -= days_in_month;
    ++month;
  }

  day = days + 1;
}

bool date::is_valid_date(int year, int month, int day) {
  if (year < 1 || month < 1 || month > 12 || day < 1) {
    return false;
  }
  return day <= days_in_month(year, month);
}

date::date(int year, int month, int day)
    : days_(to_days(year, month, day)) {
  if (!is_valid_date(year, month, day)) {
    throw std::invalid_argument("Invalid date");
  }
}

int date::year() const {
  int year;
  int month;
  int day;
  from_days(days_, year, month, day);
  return year;
}
int date::month() const {
  int year;
  int month;
  int day;
  from_days(days_, year, month, day);
  return month;
}
int date::day() const {
  int year;
  int month;
  int day;
  from_days(days_, year, month, day);
  return day;
}
weekday date::weekday() const {
  int month_;
  int year_;
  int day_;
  from_days(days_, year_, month_, day_);
  int a = (14 - month_) / 12;
  int y = year_ + 4800 - a;
  int m = month_ + 12 * a - 3;
  int JDN =
      day_ + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;

  return static_cast<DateTime::weekday>(JDN % 7);
}

bool date::is_leapyear() const { return is_leapyear(this->year()); }
void date::add_days(int nb_days) { days_ += nb_days; }

date date::next() const {
  date new_date = *this;
  new_date.add_days(1);
  return new_date;
}

date date::prev() const {
  date new_date = *this;
  new_date.add_days(-1);
  return new_date;
}

bool operator==(const date& lhs,
                          const date& rhs) {
  return lhs.day() == rhs.day() && lhs.month() == rhs.month() &&
         lhs.year() == rhs.year();
}

bool operator<(const date& lhs, const date& rhs) {
  if (lhs.year() != rhs.year()) {
    return lhs.year() < rhs.year();
  }
  if (lhs.month() != rhs.month()) {
    return lhs.month() < rhs.month();
  }
  return lhs.day() < rhs.day();
}

bool operator>(const date& lhs, const date& rhs) {
  return !(lhs <= rhs);
}
bool operator<=(const date& lhs,
                          const date& rhs) {
  return lhs < rhs || lhs == rhs;
}
bool operator>=(const date& lhs,
                          const date& rhs) {
  return !(lhs < rhs);
}
bool operator!=(const date& lhs,
                          const date& rhs) {
  return !(rhs == lhs);
}

date& date::operator--() {
  this->add_days(-1);
  return *this;
}
date date::operator--(int) {
  date cp = *this;
  this->add_days(-1);
  return cp;
}
date& date::operator++() {
  this->add_days(1);
  return *this;
}
date date::operator++(int) {
  date cp = *this;
  this->add_days(1);
  return cp;
}

std::string to_string(const date& rhs) {
  std::string result = "yyyy.MM.dd";
  int y = rhs.year();
  int t = 1000;
  for (int i = 0; i < 4; ++i) {
    result[i] = static_cast<char>((y / t) + 48);
    y %= t;
    t /= 10;
  }

  result[5] = static_cast<char>(rhs.month() / 10 + 48);
  result[6] = static_cast<char>(rhs.month() % 10 + 48);

  result[8] = static_cast<char>(rhs.day() / 10 + 48);
  result[9] = static_cast<char>(rhs.day() % 10 + 48);

  return result;
}
timediff date::operator-(const date& rhs) const {
  int mn = 1;
  int tot = 0;
  date s = rhs;
  date e = *this;

  if (*this < rhs) {
    mn = -1;
    s = *this;
    e = rhs;
  }

  while (s < e) {
    if (s.year() == e.year() && s.month() == e.month()) {
      tot += e.day() - s.day();
      break;
    }
    tot += days_in_month(s.year(), s.month()) - s.day() + 1;
    s.add_days(days_in_month(s.year(), s.month()) - s.day() + 1);
  }
  return timediff(mn * tot, 0, 0, 0);
}
// ~~~~ DATE.CPP ~~~~ //

// DATETIME.CPP

datetime::datetime(int year, int month, int day, int hour, int minute,
                             int second)
    : date(year, month, day), time(hour, minute, second) {}

datetime::datetime(const date& dt, const time& tm)
    : date(dt), time(tm) {}

void datetime::add_seconds_with_carry(int nb_seconds) {
  int new_total_seconds = total_seconds() + nb_seconds;

  if (new_total_seconds >= 0 && new_total_seconds < 86400) {
    this->total_seconds_ = new_total_seconds;
  } else {
    int days_to_add = new_total_seconds / 86400;
    int remaining_seconds = new_total_seconds % 86400;

    if (remaining_seconds < 0) {
      days_to_add--;
      remaining_seconds += 86400;
    }

    this->total_seconds_ = remaining_seconds;
    add_days(days_to_add);
  }
}

void datetime::add_days(int nb_days) { date::add_days(nb_days); }

void datetime::add_hours(int nb_hours) {
  add_seconds_with_carry(nb_hours * 3600);
}

void datetime::add_minutes(int nb_minutes) {
  add_seconds_with_carry(nb_minutes * 60);
}

void datetime::add_seconds(int nb_seconds) {
  add_seconds_with_carry(nb_seconds);
}

std::string to_string(const datetime& dt,
                                const char* format) {
  std::string result;
  std::string short_months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  std::string months[12] = {"January",   "February", "March",    "April",
                            "May",       "June",     "July",     "August",
                            "September", "October",  "November", "December"};
  std::string day_of_week[7] = {"Monday", "Tuesday",  "Wednesday", "Thursday",
                                "Friday", "Saturday", "Sunday"};
  std::string short_day_of_week[7] = {"Mon", "Tue", "Wed", "Thu",
                                      "Fri", "Sat", "Sun"};
  for (int i = 0; format[i] != '\0'; ++i) {
    if ((format[i] != '%' || format[i + 1] == '\0') ||
        (format[i] == '%' && (format[i + 1] != 'Y' && format[i + 1] != 'y' &&
                              format[i + 1] != 'm' && format[i + 1] != 'd' &&
                              format[i + 1] != 'b' && format[i + 1] != 'B' &&
                              format[i + 1] != 'a' && format[i + 1] != 'A' &&
                              format[i + 1] != 'H' && format[i + 1] != 'M' &&
                              format[i + 1] != 'S'))) {
      result += format[i];
    } else {
      if (format[i + 1] == 'Y') {
        result += std::to_string(dt.year());
      }
      if (format[i + 1] == 'y') {
        result += std::to_string(dt.year() % 100);
      }
      if (format[i + 1] == 'm') {
        if (dt.month() < 10) {
          result += '0';
        }
        result += std::to_string(dt.month());
      }
      if (format[i + 1] == 'd') {
        if (dt.day() < 10) {
          result += '0';
        }
        result += std::to_string(dt.day());
      }
      if (format[i + 1] == 'b') {
        result += short_months[dt.month() - 1];
      }
      if (format[i + 1] == 'B') {
        result += months[dt.month() - 1];
      }
      if (format[i + 1] == 'a') {
        result += short_day_of_week[static_cast<int>(dt.weekday())];
      }
      if (format[i + 1] == 'A') {
        result += day_of_week[static_cast<int>(dt.weekday())];
      }
      if (format[i + 1] == 'H') {
        if (dt.hour() < 10) {
          result += '0';
        }
        result += std::to_string(dt.hour());
      }
      if (format[i + 1] == 'M') {
        if (dt.minute() < 10) {
          result += '0';
        }
        result += std::to_string(dt.minute());
      }
      if (format[i + 1] == 'S') {
        if (dt.second() < 10) {
          result += '0';
        }
        result += std::to_string(dt.second());
      }
      ++i;
    }
  }
  return result;
}

bool operator==(const datetime& lhs,
                          const datetime& other) {
  return lhs.year() == other.year() && lhs.month() == other.month() &&
         lhs.day() == other.day() && lhs.hour() == other.hour() &&
         lhs.minute() == other.minute() && lhs.second() == other.second();
}
bool operator<(const datetime& lhs,
                         const datetime& other) {
  if (lhs.year() != other.year()) {
    return lhs.year() < other.year();
  }
  if (lhs.month() != other.month()) {
    return lhs.month() < other.month();
  }
  if (lhs.day() != other.day()) {
    return lhs.day() < other.day();
  }
  if (lhs.hour() != other.hour()) {
    return lhs.hour() < other.hour();
  }
  if (lhs.minute() != other.minute()) {
    return lhs.minute() < other.minute();
  }
  return lhs.second() < other.second();
}

bool operator>(const datetime& lhs,
                         const datetime& rhs) {
  return !(lhs < rhs) && !(lhs == rhs);
}
bool operator<=(const datetime& lhs,
                          const datetime& rhs) {
  return !(lhs > rhs);
}
bool operator>=(const datetime& lhs,
                          const datetime& rhs) {
  return !(lhs < rhs);
}
bool operator!=(const datetime& lhs,
                          const datetime& rhs) {
  return !(rhs == lhs);
}

datetime datetime::operator-(
    const timediff& rhs) const {
  timediff new_rhs(-rhs.days(), -rhs.hours(), -rhs.minutes(),
                             -rhs.seconds());
  return *this + new_rhs;
}
datetime datetime::operator+(
    const timediff& rhs) const {
  datetime res(*this);

  res.add_seconds(rhs.total_seconds());

  return res;
}
timediff datetime::operator-(
    const datetime& rhs) const {
  int mn_d = 1;
  int mn_t = 1;
  date d1(this->year(), this->month(), this->day());
  date d2(rhs.year(), rhs.month(), rhs.day());
  time t1(this->hour(), this->minute(), this->second());
  time t2(rhs.hour(), rhs.minute(), rhs.second());
  timediff date_diff = d1 - d2;

  timediff time_diff = t1 - t2;
  if (d1 < d2) {
    date_diff = d2 - d1;
    mn_d = -1;
    if (t1 < t2) {
      time_diff = t1 - t2;
      mn_t = 1;
    } else if (t1 > t2) {
      date_diff = date_diff - timediff(1, 0, 0, 0);
      time_diff = t2 - t1;
      mn_t = -1;
      time_diff = timediff(1, 0, 0, 0) + time_diff;
    }
  } else {
    if (t1 < t2) {
      date_diff = date_diff - timediff(1, 0, 0, 0);
      time_diff = timediff(1, 0, 0, 0) + time_diff;
    }
  }
  return timediff(mn_d * date_diff.days(), mn_t * time_diff.hours(),
                            mn_t * time_diff.minutes(),
                            mn_t * time_diff.seconds());
}

// ~~~~ DATETIME.CPP ~~~~~ //

// TIMEDIFF.CPP

timediff::timediff(int days, int hours, int minutes, int seconds) {
  if (!((days >= 0 && hours >= 0 && minutes >= 0 && seconds >= 0) ||
        (days <= 0 && hours <= 0 && minutes <= 0 && seconds <= 0))) {
    throw std::invalid_argument("all arguments must have the same sign");
  }

  if (hours < -23 || hours > 23) {
    throw std::invalid_argument("Hours must be between -23 and 23");
  }
  if (minutes < -59 || minutes > 59) {
    throw std::invalid_argument("Minutes must be between -59 and 59");
  }
  if (seconds < -59 || seconds > 59) {
    throw std::invalid_argument("Seconds must be between -59 and 59");
  }

  total_seconds_ = days * 86400 + hours * 3600 + minutes * 60 + seconds;
}

int timediff::days() const { return total_seconds_ / 86400; }

int timediff::hours() const {
  int remaining = total_seconds_ % 86400;
  return remaining / 3600;
}

int timediff::minutes() const {
  int remaining = total_seconds_ % 3600;
  return remaining / 60;
}

int timediff::seconds() const {
  int remaining = total_seconds_ % 60;
  return remaining;
}

bool operator==(const timediff& lhs, const timediff& rhs) {
  return lhs.days() == rhs.days() && lhs.hours() == rhs.hours() &&
         lhs.minutes() == rhs.minutes() && lhs.seconds() == rhs.seconds();
}
bool operator<(const timediff& lhs, const timediff& rhs) {
  if (lhs.days() != rhs.days()) {
    return lhs.days() < rhs.days();
  }
  return lhs.hours() * 60 * 60 + lhs.minutes() * 60 + lhs.seconds() <
         rhs.hours() * 60 * 60 + rhs.minutes() * 60 + rhs.seconds();
}

bool operator>(const timediff& lhs, const timediff& rhs) {
  return !(lhs < rhs) && !(lhs == rhs);
}
bool operator<=(const timediff& lhs, const timediff& rhs) {
  return !(lhs > rhs);
}
bool operator>=(const timediff& lhs, const timediff& rhs) {
  return !(lhs < rhs);
}
bool operator!=(const timediff& lhs, const timediff& rhs) {
  return !(rhs == lhs);
}

timediff timediff::operator-(
    const timediff& rhs) const {
  int dif = total_seconds_ - rhs.total_seconds_;
  int d = dif / 86400;
  dif %= 86400;
  int h = dif / 3600;
  dif %= 3600;
  int m = dif / 60;
  dif %= 60;
  int s = dif;

  return timediff(d, h, m, s);
}

timediff timediff::operator+(
    const timediff& rhs) const {
  int dif = 24 * 60 * 60 * (days() + rhs.days()) +
            60 * 60 * (hours() + rhs.hours()) +
            60 * (minutes() + rhs.minutes()) + (seconds() + rhs.seconds());
  int d = dif / 86400;
  dif %= 86400;
  int h = dif / 3600;
  dif %= 3600;
  int m = dif / 60;
  dif %= 60;
  int s = dif;
  return timediff(d, h, m, s);
}
// ~~~~ TIMEDIFF.CPP ~~~~~ //

} // namespace DateTime