#include <iostream>
#include <string>
#include <vector>

#include "bowdictionary.hpp"
#include "convert_dataset.hpp"
#include "histogram.hpp"
#include "image_browser.hpp"
#include "serialize.hpp"
#include "utils.hpp"

using namespace bovw;
using namespace cv;

int main() {
  // Compute histograms
  std::vector<bovw::Histogram> vec_hist_from_csv =
      bovw::gen_vechist_from_csv("Reweighted_histogram.csv", true);
  std::cout << vec_hist_from_csv.size() << std::endl;

  show_results(vec_hist_from_csv[9], vec_hist_from_csv, 6);
  return 0;
}