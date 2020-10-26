#include "kMeans.hpp"
#include <iostream>
#include <opencv2/core.hpp>

int main() {
  const std::string &filename{"data/images/imageCompressedCam0_0000000.png"};
  cv::Mat img_sift = bovw::compute_sifts(filename);
  std::cout << "The type of img_sift is " << img_sift.type() << " and size is "
            << img_sift.size << std::endl;

  auto [center2, labels2] = bovw::kMeans(img_sift, 10, 100);
  std::cout << "label size: " << labels2.size << " type: " << labels2.type()
            << std::endl;
  std::cout << "center size: " << labels2.size << " type: " << labels2.type()
            << std::endl;
}