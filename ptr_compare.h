#ifndef PTR_COMPARE_H

template <typename T> T *ptr(T &obj) {
  return &obj;
} // turn reference into pointer!

template <typename T> T *ptr(T *obj) {
  return obj;
} // obj is already pointer, return it!

template <typename T> std::unique_ptr<T> ptr(std::unique_ptr<T>& obj) {
  return obj;
} // obj is already pointer, return it!

template <typename T> std::shared_ptr<T> ptr(std::shared_ptr<T>& obj) {
  return obj;
} // obj is already pointer, return it!

template <typename T> const std::unique_ptr<T> ptr(const std::unique_ptr<T>& obj) {
  return obj;
} // obj is already pointer, return it!

template <typename T> const std::shared_ptr<T> ptr(const std::shared_ptr<T>& obj) {
  return obj;
} // obj is already pointer, return it!

template <typename T> bool peq(T &a, T &b) { return (*ptr(a) == *ptr(b)); }
template <typename T> bool pcmp(T &a, T &b) { return (*ptr(a) < *ptr(b)); }

#endif // PTR_COMPARE_H