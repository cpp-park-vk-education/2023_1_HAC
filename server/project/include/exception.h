#pragma once // NOLINT

#include <stdexcept>
#include <string>

namespace market_mentor {

class MarketMentorException: public std::runtime_error {
 public:
  explicit MarketMentorException(const std::string& msg)
      : std::runtime_error(msg) {} 
};

} // namespace market_mentor