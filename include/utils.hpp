#ifndef UTILS_HPP_
#define UTILS_HPP_
#include "histogram.hpp"

#include <algorithm>
#include <opencv2/core.hpp>
#include <string>
#include <vector>

namespace bovw {

template <typename Tp>
bool inline mat_are_equal(const cv::Mat &m1, const cv::Mat &m2) {
  return std::equal(m1.begin<Tp>(), m1.end<Tp>(), m2.begin<Tp>());
}

const cv::Mat compute_sifts(const std::string &);

const std::string gen_img_name(const std::string &img_path,
                               const std::string &extension, int img_id);
const std::vector<std::string> gen_img_name(const std::string &img_path,
                                            const std::string &extension,
                                            const std::vector<int> &img_ids);

const std::vector<std::string> get_sequence(const std::string &img_path,
                                            const std::string &extension,
                                            int min_num, int max_num);

float cosine_dis(const cv::Mat &mat1, const cv::Mat &mat2);
float cosine_dis(const std::vector<float> &vec1,
                 const std::vector<float> &vec2);

float euclidean_dis_sq(const cv::Mat &mat1, const cv::Mat &mat2);
float euclidean_dis_sq(const std::vector<float> &vec1,
                       const std::vector<float> &vec2);

cv::Mat Vec_MatToMat(const std::vector<cv::Mat> &);

std::vector<cv::Mat> gen_blabels(const cv::Mat &bestLabels,
                                 const std::vector<cv::Mat> &descriptors);

std::vector<bovw::Histogram> gen_vechist_from_csv(const std::string &filename,
                                                  bool reweight = false);

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &vec) {
  std::cout << "[";
  for (auto voc_it = vec.cbegin(); voc_it < vec.cend(); ++voc_it) {
    os << *voc_it;
    if (voc_it < vec.cend() - 1) {
      os << ", ";
    } else {
      os << "]";
    }
  }
  return os;
}

void show_results(const Histogram &query_img,
                  const std::vector<Histogram> &vec_hist, int results_num,
                  const std::string html_name = "show_results.html");

} // namespace bovw

#endif // UTILS_HPP_
