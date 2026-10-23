#pragma once
#include <iostream>
#include <string>
// TODO перенести операции сравнения вне класса
namespace DateTime {

enum class weekday {
  monday,
  tuesday,
  wednesday,
  thursday,
  friday,
  saturday,
  sunday
};
class timediff;
class time {
 protected:
  int total_seconds_;

 public:
  time(int hour, int minute, int second);

  int total_seconds() const;

  int hour() const;
  int minute() const;
  int second() const;

  virtual void add_hours(int nb_hours);
  virtual void add_minutes(int nb_minutes);
  virtual void add_seconds(int nb_seconds);

  timediff operator-(const time& rhs) const;
};

bool operator==(const time&, const time&);
bool operator!=(const time&, const time&);
bool operator<(const time&, const time&);
bool operator>(const time&, const time&);
bool operator<=(const time&, const time&);
bool operator>=(const time&, const time&);

class date {
 private:
  static int to_days(int, int, int);
  static int days_in_month(int, int);
  static bool is_valid_date(int, int, int);
  static void from_days(int days, int& year, int& month, int& day);

 protected:
  int days_;

 public:
  static bool is_leapyear(int);

  date(int year, int month, int day);  // ctor

  int year() const;
  int month() const;
  int day() const;
  DateTime::weekday weekday() const;

  bool is_leapyear() const;

  virtual void add_days(int nb_days);

  date next() const;
  date prev() const;

  date operator--(int);
  date& operator--();

  date operator++(int);
  date& operator++();

  timediff operator-(const date& rhs) const;
};

bool operator==(const date&, const date&);
bool operator!=(const date&, const date&);
bool operator<(const date&, const date&);
bool operator>(const date&, const date&);
bool operator<=(const date&, const date&);
bool operator>=(const date&, const date&);

class datetime : public date, public time {
 private:
  void normalize();
  void add_seconds_with_carry(int);

 public:
  datetime(int year, int month, int day, int hour = 0, int minute = 0,
           int second = 0);
  datetime(const date& dt, const time& tm);

  void add_days(int nb_days) override;
  void add_hours(int nb_hours) override;
  void add_minutes(int nb_minutes) override;
  void add_seconds(int nb_seconds) override;

  datetime operator-(const timediff&) const;
  datetime operator+(const timediff&) const;
  timediff operator-(const datetime&) const;
};

bool operator==(const datetime&, const datetime&);
bool operator<(const datetime&, const datetime&);
bool operator!=(const datetime&, const datetime&);
bool operator<=(const datetime&, const datetime&);
bool operator>=(const datetime&, const datetime&);
bool operator>(const datetime&, const datetime&);

class timediff {
 private:
  int total_seconds_;

 public:
  timediff(int days, int hours, int minutes, int seconds);  // ctor
  // getters
  int days() const;
  int hours() const;
  int minutes() const;
  int seconds() const;
  int total_seconds() const { return total_seconds_; }
  int total_minutes() const { return total_seconds_ / 60; }
  int total_hours() const { return total_seconds_ / 3600; }

  timediff operator-(const timediff& rhs) const;
  timediff operator+(const timediff& rhs) const;
};

bool operator==(const timediff&, const timediff&);
bool operator<(const timediff&, const timediff&);
bool operator>(const timediff&, const timediff&);
bool operator!=(const timediff&, const timediff&);
bool operator<=(const timediff&, const timediff&);
bool operator>=(const timediff&, const timediff&);

std::string to_string(const time& rhs);
std::string to_string(const date& rhs);
std::string to_string(const datetime&,
                      const char* format = "%Y.%m.%d %H:%M:%S");
}  // namespace DateTime
