#include <filesystem>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <string>

#include "serialize.hpp"
#include "utils.hpp"

int main() {
  const std::string &filename{"data/images/imageCompressedCam0_0000000.png"};
  cv::Mat img_mat = cv::imread(filename, cv::IMREAD_GRAYSCALE);
  std::cout << "The type of img_mat is " << img_mat.type() << " and size is "
            << img_mat.size << std::endl;
  namespace fs = std::filesystem;
  if (!fs::exists(filename)) {
    std::cerr << "The file " << filename << " does not exist!\n";
  }
  cv::Mat img_sift = bovw::compute_sifts(filename);
  std::cout << "The type of img_sift is " << img_sift.type() << " and size is "
            << img_sift.size << std::endl;
  const std::string &binfilename{"data/bin/imageCompressedCam0_0000000.bin"};
  cv::Mat img_mat_from_bin = bovw::serialization::Deserialize(binfilename);
  std::cout << "The type of img_mat_from_bin is " << img_mat_from_bin.type()
            << " and size is " << img_mat_from_bin.size << std::endl;
  return 0;
}