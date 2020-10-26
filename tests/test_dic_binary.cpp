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
  // Compute histograms
  std::vector<bovw::Histogram> vec_hist_from_csv =
      bovw::gen_vechist_from_csv("Histogram.csv");
  // Build dictionary
  const std::string &bin_path{"data/bin/"};
  const std::string &bin_extension{".bin"};
  std::vector<std::string> bin_sequence =
      bovw::get_sequence(bin_path, bin_extension, 0, 9);
  std::cout << "Start to read binary files.\n";
  std::vector<cv::Mat> descriptors =
      bovw::serialization::sifts::LoadDataset("data/bin/", bin_sequence, 12);
  //   std::cout << "descriptors vector size: " << descriptors.size() <<
  //   std::endl;
  std::cout << "Start to create dictionary.\n";
  auto &dictionary = bovw::BowDictionary::GetInstance();
  dictionary.build(100, 100, descriptors);
  dictionary.set_words_ocurrence(vec_hist_from_csv);
  dictionary.serialize_dic("data/bin/dictionary/dic_100.bin");
  // create dictionary from binary
  Mat vocabulary = dictionary.vocabulary();
  Mat bestLabels = dictionary.bestLabels();
  std::vector<int> words_occurrence = dictionary.words_occurrence();
  std::cout << "Create dictionary from binary file.\n";
  dictionary.deserialize_dic("data/bin/dictionary/dic_100.bin");
  // compare class attributes of the current dictionary and previous one
  bool voc = mat_are_equal<float>(vocabulary, dictionary.vocabulary());
  bool bestL = mat_are_equal<int>(bestLabels, dictionary.bestLabels());
  auto it = dictionary.words_occurrence().cbegin();
  bool wordO = true;
  for (const auto &ele : words_occurrence) {
    if (ele != *it++) {
      wordO = false;
    }
  }
  std::cout << "Compare the created dictionary from the original one.\n";
  std::cout << std::boolalpha << "voc: " << voc << "; bestL: " << bestL
            << "; word_occurrence: " << wordO << std::endl;
  return 0;
}