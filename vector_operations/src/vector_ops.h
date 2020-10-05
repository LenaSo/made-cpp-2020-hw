#pragma once
#include <cmath>  // fabs
#include <iostream>
#include <vector>

namespace task {
std::vector<double> operator+(const std::vector<double>& a,
                              const std::vector<double>& b) {
  std::vector<double> out(a.size());
  for (int i = 0; i < a.size(); i++) out[i] = a[i] + b[i];
  return out;
}

std::vector<double> operator-(const std::vector<double>& a,
                              const std::vector<double>& b) {
  std::vector<double> out(a.size());
  for (int i = 0; i < a.size(); i++) out[i] = a[i] - b[i];
  return out;
}

std::vector<double> operator+(const std::vector<double>& inp) {
  std::vector<double> out = inp;
  return out;
}

std::vector<double> operator-(const std::vector<double>& inp) {
  std::vector<double> out(inp.size());
  for (int i = 0; i < inp.size(); i++) out[i] = -inp[i];
  return out;
}

double operator*(const std::vector<double>& a, const std::vector<double>& b) {
  double out = 0;
  for (int i = 0; i < a.size(); i++) out += a[i] * b[i];
  return out;
}

std::vector<double> operator%(const std::vector<double>& a,
                              const std::vector<double>& b) {
  std::vector<double> out(3);
  out[0] = a[2] * b[1] - a[1] * b[2];
  out[1] = a[0] * b[2] - a[2] * b[0];
  out[2] = a[1] * b[0] - a[0] * b[1];
  return out;
}

bool operator||(const std::vector<double>& a, const std::vector<double>& b) {
  double ab2 = a * b;
  ab2 *= ab2;
  double a2b2 = (a * a) * (b * b);

  double eps = 1e-7;
  return (fabs(ab2 - a2b2) < eps * ab2);
}

bool operator&&(const std::vector<double>& a, const std::vector<double>& b) {
  return (a * b > 0 && (a || b));
}

std::istream& operator>>(std::istream& in, std::vector<double>& v) {
  size_t size;
  in >> size;
  v.resize(size);
  for (int i = 0; i < size; i++) in >> v[i];
  return in;
}

std::ostream& operator<<(std::ostream& out, const std::vector<double>& v) {
  for (int i = 0; i < v.size(); i++)
    out << v[i] << ((i < v.size() - 1) ? ' ' : '\n');
  return out;
}

void reverse(std::vector<double>& v) { std::reverse(v.begin(), v.end()); }

std::vector<int> operator|(const std::vector<int>& a,
                           const std::vector<int>& b) {
  std::vector<int> out(a.size());
  for (int i = 0; i < a.size(); i++) out[i] = a[i] | b[i];
  return out;
}

std::vector<int> operator&(const std::vector<int>& a,
                           const std::vector<int>& b) {
  std::vector<int> out(a.size());
  for (int i = 0; i < a.size(); i++) out[i] = a[i] & b[i];
  return out;
}

}  // namespace task
