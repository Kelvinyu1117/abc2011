namespace utils {
template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};

} // namespace utils
