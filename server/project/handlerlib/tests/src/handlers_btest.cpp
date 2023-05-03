#include "utils.h"
#include "gmock/gmock.h"
#include "handlers.h"
#include "handler_exception.h"
#include "dbcontroller.hpp"

TEST(HandlersTests, NullInHandler) {
    
    class MockDBController : public IDataBaseController {
        MOCK_METHOD0();  
    };
    
    
    handlers::PredictHandler()

    EXPECT_THROW();
}