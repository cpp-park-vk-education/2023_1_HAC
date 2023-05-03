#pragma once // NO_LINT
#include "exception.h"

namespace market_mentor {

class Error : public MarketMentorException {
 public:
    Error() :
    MarketMentorException("Error") {}
};

} // namespace market_mentor