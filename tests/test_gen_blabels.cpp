#include "bowdictionary.hpp"
#include "convert_dataset.hpp"
#include "histogram.hpp"
#include "serialize.hpp"
#include "utils.hpp"

#include <iostream>
#include <opencv2/core.hpp>
#include <string>
#include <vector>

using namespace cv;

int main() {
  // Build dictionary
  const std::string &bin_path{"data/bin/"};
  const std::string &bin_extension{".bin"};
  std::vector<std::string> bin_sequence =
      bovw::get_sequence(bin_path, bin_extension, 0, 9);
  std::cout << "Start to read binary files.\n";
  std::vector<cv::Mat> descriptors =
      bovw::serialization::sifts::LoadDataset("data/bin/", bin_sequence, 12);
  std::cout << "descriptors vector size: " << descriptors.size() << std::endl;
  std::cout << "Start to create dictionary.\n";
  auto &dictionary = bovw::BowDictionary::GetInstance();
  dictionary.build(100, 100, descriptors);
  std::cout << "Dictionary size: " << dictionary.size() << std::endl;
  std::cout << std::boolalpha << "empty: " << dictionary.empty() << std::endl;
  std::cout << "blabel size: " << dictionary.bestLabels().size << std::endl;
  std::cout << "VOC size: " << dictionary.vocabulary().size << std::endl;
  // Create histograms
  std::vector<Mat> label_for_each =
      bovw::gen_blabels(dictionary.bestLabels(), descriptors);
  std::cout << "bestLabels size: " << label_for_each.size() << std::endl;
  int idx = 0;
  for (const auto &des : descriptors) {
    std::cout << "size of des " << idx++ << ": " << des.size
              << " type: " << des.type() << std::endl;
  }
  idx = 0;
  for (const auto &labels : label_for_each) {
    std::cout << "size of label for img " << idx++ << ": " << labels.size
              << " type: " << labels.type() << std::endl;
  }
  std::vector<bovw::Histogram> vec_hist;
  vec_hist.reserve(11);
  for (const auto &label : label_for_each) {
    bovw::Histogram hist = bovw::Histogram(label, dictionary.size());
    std::cout << hist << std::endl;
    vec_hist.emplace_back(hist);
  }
  std::cout << "Size of vec_hist: " << vec_hist.size() << std::endl;

  for (const auto &hist : vec_hist) {
    hist.WriteToCSV("Histogram.csv");
  }

  std::vector<bovw::Histogram> vec_hist_from_csv =
      bovw::gen_vechist_from_csv("Histogram.csv");
  std::cout << vec_hist_from_csv.size() << std::endl;
  for (auto &hist : vec_hist_from_csv) {
    std::cout << "empty: " << std::boolalpha << hist.empty() << std::endl;
    std::cout << hist << std::endl;
  }

  return 0;
}