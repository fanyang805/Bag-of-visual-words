#include "convert_dataset.hpp"
#include "utils.hpp"

#include <filesystem>
#include <iostream>
#include <opencv2/core.hpp>
#include <string>
#include <vector>

namespace fs = std::filesystem;

int main() {
  const std::string &bin_path{"data/bin/"};
  const std::string &bin_extension{".bin"};
  std::vector<std::string> bin_sequence =
      bovw::get_sequence(bin_path, bin_extension, 0, 691);

  std::cout << "Start to read binary files.\n";
  std::vector<cv::Mat> vec_des =
      bovw::serialization::sifts::LoadDataset("data/bin/", bin_sequence, 700);

  std::cout << "Start to compare SIFT descripters from original .png files and "
               ".bin files\n";
  int idx = 0;
  const std::string &img_path{"data/images/"};
  const std::string &extension{".png"};
  std::vector<std::string> img_sequence =
      bovw::get_sequence(img_path, extension, 0, 691);
  for (const std::string &filename : img_sequence) {
    cv::Mat descriptors = bovw::compute_sifts(filename);
    std::cout << "Are desicripters of " << fs::path(filename).stem()
              << " same in .png and .bin files? ";
    std::cout << std::boolalpha
              << bovw::mat_are_equal<float>(descriptors, vec_des[idx++])
              << std::endl;
  }
}