#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

Mat guidedfilter( Mat &I, Mat &p, int r, double eps );
Mat getimage(Mat &a);