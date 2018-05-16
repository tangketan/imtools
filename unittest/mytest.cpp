#include <opencv2/ts/ts.hpp>
#include "FileUtility.h"

class MyTestClass : public cvtest::BaseTest
{
public:
    MyTestClass(): mat(cv::Scalar::all(1)){}

protected:
    cv::Mat mat;

    void run(int) {
      ts->set_failed_test_info(cvtest::TS::OK);

      //check that all values are between 1 and 1 (and not Nan)
      if (tkt::isdir(".")==0)
          ts->set_failed_test_info(cvtest::TS::FAIL_INVALID_TEST_DATA);
      if (tkt::isdir("mytest.exe")==1)
          ts->set_failed_test_info(cvtest::TS::FAIL_INVALID_TEST_DATA);
  }
};

TEST(MyTestSuite, ATestThatPasses) {
  MyTestClass myTestClass;
  myTestClass.safe_run();
}
