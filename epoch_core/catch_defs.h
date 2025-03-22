#pragma once
//
// Created by dewe on 12/1/22.
//
#include <catch.hpp>


#define DYNAMIC_SECTION2(flag) \
    DYNAMIC_SECTION((flag ? std::string(#flag).append(" = true") : std::string(#flag).append(" = false")))

#define FOR_EACH_FLAG(var) for (bool var : { true, false })


namespace epoch_core {
template <bool ordered = true, class ContainerTruth,
          template <class T> class Container, class T>
inline void VECTOR_EQUALS(ContainerTruth const &trueContainer,
                          Container<T> const &container) {
  if constexpr (ordered)
    REQUIRE_THAT(trueContainer, Catch::Matchers::Equals(container));
  else
    REQUIRE_THAT(trueContainer, Catch::Matchers::UnorderedEquals(container));
}

template <bool ordered = true, class ContainerTruth,
          template <class T> class Container, class T>
inline void VECTOR_EQUALS_CAST(ContainerTruth const &trueContainer,
                               Container<T> const &container) {
  auto casted = std::vector<T>(trueContainer.begin(), trueContainer.end());
  if constexpr (ordered)
    REQUIRE_THAT(casted, Catch::Matchers::Equals(container));
  else
    REQUIRE_THAT(casted, Catch::Matchers::UnorderedEquals(container));
}

template <bool ordered = true, class ContainerTruth, class T>
inline void VECTOR_EQUALS_(ContainerTruth const &trueContainer,
                           std::initializer_list<T> const &container) {
  auto expected = std::vector(container);
  if constexpr (ordered)
    REQUIRE_THAT(trueContainer, Catch::Matchers::Equals(expected));
  else
    REQUIRE_THAT(trueContainer, Catch::Matchers::UnorderedEquals(expected));
}

template <bool ordered = true, class ContainerTruth, class T>
inline void VECTOR_EQUALS_CAST_(ContainerTruth const &trueContainer,
                                std::initializer_list<T> const &container) {
  auto casted = std::vector<T>(trueContainer.begin(), trueContainer.end());
  auto expected = std::vector(container);
  if constexpr (ordered)
    REQUIRE_THAT(casted, Catch::Matchers::Equals(expected));
  else
    REQUIRE_THAT(casted, Catch::Matchers::UnorderedEquals(expected));
}

template <bool ordered = true, class ContainerTruth, class T>
inline void REQUIRE_CONTAINS(ContainerTruth const &trueContainer,
                             std::initializer_list<T> const &container) {
  auto casted = std::vector<T>(trueContainer.begin(), trueContainer.end());
  auto expected = std::vector(container);
  if constexpr (ordered)
    REQUIRE_THAT(casted, Catch::Matchers::Equals(expected));
  else
    REQUIRE_THAT(casted, Catch::Matchers::UnorderedEquals(expected));
}

template <typename Range>
struct ContainsKeyMatcher : public Catch::Matchers::MatcherGenericBase {
  explicit ContainsKeyMatcher(Range const &container) : range(container) {}

  bool match(Range::key_type const &in) const { return range.contains(in); }

  [[nodiscard]] std::string describe() const override {
    std::ostringstream ss;
    ss << " in " << range << "\n";
    return ss.str();
  }

private:
  Range const &range;
};
}