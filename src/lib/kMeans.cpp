#include "kMeans.hpp"
#include "utils.hpp"
#include <algorithm>
#include <iostream>
#include <tuple>

using namespace cv;

namespace bovw {

std::tuple<cv::Mat, cv::Mat> kMeans(const std::vector<cv::Mat> &descriptors,
                                    int num_clusters, int max_iter) {
  Mat mat_data = Vec_MatToMat(descriptors);
  if (mat_data.empty()) {
    std::cerr << "The matrix is empty!\n";
    return std::make_tuple(Mat(), Mat());
  }
  Mat bestLabels = Mat(mat_data.rows, 1, mat_data.type());
  Mat centers = Mat(num_clusters, mat_data.cols, mat_data.type());
  kmeans(
      mat_data, num_clusters, bestLabels,
      cv::TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, max_iter, 1.0),
      3, KMEANS_PP_CENTERS, centers);
  return std::make_tuple(centers, bestLabels);
}

std::tuple<cv::Mat, cv::Mat> kMeans(const Mat &mat_data, int num_clusters,
                                    int max_iter) {
  max_iter = 0;

  if (mat_data.empty()) {
    std::cerr << "The matrix is empty!\n";
    return std::make_tuple(Mat(), Mat());
  }
  cv::Mat bestLabels = Mat(mat_data.rows, 1, mat_data.type());
  Mat centers = Mat(num_clusters, mat_data.cols, mat_data.type());
  // kmeans(
  //     mat_data, num_clusters, bestLabels,
  //     cv::TermCriteria(TermCriteria::EPS + TermCriteria::COUNT,
  //     max_iter, 10.0), 1, KMEANS_PP_CENTERS, centers);
  kmeans(mat_data, num_clusters, bestLabels,
         cv::TermCriteria(TermCriteria::EPS, max_iter, 10.0), 3,
         KMEANS_PP_CENTERS, centers);
  // std::cout << "size: " << bestLabels.size << " type: " << bestLabels.type()
  //           << std::endl;
  // std::cout << "bestLabels size: " << bestLabels.size << std::endl;
  return std::make_tuple(centers, bestLabels);
}

} // namespace bovw
