#include <iostream>
#include <string>
#include <vector>

#include "bowdictionary.hpp"
#include "convert_dataset.hpp"
#include "histogram.hpp"
#include "serialize.hpp"
#include "utils.hpp"

using namespace bovw;
using namespace cv;

int main() {
  // Create dictionary from binary
  const std::string dictionary_bin{"data/dictionary/dic_3_10_1000.bin"};
  std::vector<std::string> bin_sequence =
      bovw::get_sequence("data/bin/", ".bin", 0, 691);
  auto &dictionary = bovw::BowDictionary::GetInstance();
  dictionary.deserialize_dic(dictionary_bin);
  // Create histograms
  std::vector<cv::Mat> descriptors =
      bovw::serialization::sifts::LoadDataset("data/bin/", bin_sequence, 12);
  std::vector<Mat> label_for_each =
      bovw::gen_blabels(dictionary.bestLabels(), descriptors);

  std::vector<bovw::Histogram> vec_hist;
  vec_hist.reserve(700);
  std::vector<std::string> img_sequence =
      bovw::get_sequence("data/images/", ".png", 0, 691);
  auto img_it = img_sequence.cbegin();
  for (const auto &label : label_for_each) {
    std::cout << "Creating histogram for " << *img_it++ << std::endl;
    bovw::Histogram hist = bovw::Histogram(label, dictionary.size());
    // std::cout << hist << std::endl;
    vec_hist.emplace_back(hist);
  }
  // write .csv of histograms
  img_it = img_sequence.cbegin();
  for (const auto &hist : vec_hist) {
    std::cout << "Write .csv of histogram for " << *img_it++ << std::endl;
    hist.WriteToCSV("data/histograms/Histogram.csv");
  }
  // compute the words_occurrence of dictionary
  dictionary.set_words_ocurrence(vec_hist);
  dictionary.serialize_dic("data/dictionary/dic_full_3_10_1000.bin");
  dictionary.deserialize_dic("data/dictionary/dic_full_3_10_1000.bin");
  std::cout << dictionary.words_occurrence() << std::endl;
  // write .csv of reweighted histograms
  const std::string filename_reweighted{
      "data/histograms/Reweighted_histogram.csv"};
  img_it = img_sequence.cbegin();
  for (auto &hist : vec_hist) {
    hist.set_img_num(vec_hist.size());
    hist.reweight_hist(dictionary);
    std::cout << "Write .csv of reweighted histogram for " << *img_it++
              << std::endl;
    hist.WriteToCSV(filename_reweighted, true);
  }

  return 0;
}