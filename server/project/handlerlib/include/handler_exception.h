#pragma once // NO_LINT
#include "exception.h"

namespace market_mentor {

class NullInHTTPError : public MarketMentorException {
 public:
    NullInHTTPError() :
        MarketMentorException("Passed nullptr in HTTPHandle") {}
};

} // namespace market_mentor