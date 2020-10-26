#include "convert_dataset.hpp"
#include "utils.hpp"

#include <filesystem>
#include <iostream>
#include <opencv2/core.hpp>
#include <string>
#include <vector>

namespace fs = std::filesystem;
using namespace cv;

int main() {
  // Build dictionary
  const std::string &bin_path{"data/bin/"};
  const std::string &bin_extension{".bin"};
  std::vector<std::string> bin_sequence =
      bovw::get_sequence(bin_path, bin_extension, 0, 691);
  std::cout << "Start to read binary files.\n";
  std::vector<cv::Mat> descriptors =
      bovw::serialization::sifts::LoadDataset("data/bin/", bin_sequence, 700);
  std::cout << "Start to create dictionary.\n";
  auto &dictionary = bovw::BowDictionary::GetInstance();
  dictionary.build(10, 1000, descriptors);
  dictionary.serialize_dic("data/dictionary/dic_3_10_1000.bin");
  //   dictionary.deserialize_dic("data/bin/dictionary/dic_100_1000.bin");
  // std::cout << "vocabulary: " << dictionary.vocabulary() << std::endl;
  std::cout << "bestLabels: " << dictionary.bestLabels() << std::endl;

  return 0;
}