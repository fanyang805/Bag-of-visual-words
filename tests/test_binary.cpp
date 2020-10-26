#include "convert_dataset.hpp"
#include "utils.hpp"

#include <filesystem>
#include <iostream>
#include <opencv2/core.hpp>
#include <string>
#include <vector>

namespace fs = std::filesystem;

int main() {
  // Create the squence to read img file and generate binary file
  const std::string &img_path{"tests/data/images/"};
  const std::string &extension{".png"};
  std::vector<std::string> img_sequence =
      bovw::get_sequence(img_path, extension, 0, 9);
  // Extract SIFT descripters from .png images and convert descripters to the
  // .bin binary files
  std::vector<std::string> bin_sequence =
      bovw::serialization::sifts::ConvertDataset(img_path, img_sequence, 12);

  // Convert binary files to the vector of SIFT descripters
  std::vector<cv::Mat> vec_des = bovw::serialization::sifts::LoadDataset(
      "tests/data/bin/", bin_sequence, 12);

  // Compare the descripters extracted from the binaries to the original
  // descripters in the .png images.
  int idx = 0;
  for (const std::string &filename : img_sequence) {
    cv::Mat descriptors = bovw::compute_sifts(filename);
    std::cout << std::boolalpha
              << bovw::mat_are_equal<float>(descriptors, vec_des[idx++])
              << std::endl;
  }

  return 0;
}