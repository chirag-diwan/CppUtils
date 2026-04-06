template <class... lambdas> struct mix : lambdas... {
  using lambdas::operator()...;
};
template <class... lambdas> mix(lambdas...) -> mix<lambdas...>;
