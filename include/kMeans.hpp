#ifndef KMEANS_HPP_
#define KMEANS_HPP_
#include <opencv2/core.hpp>
#include <tuple>
#include <utils.hpp>

namespace bovw {
/**
* @brief
* 1. Given cluster centroids i initialized in some way,
* 2. For iteration t=1..T:
*
1. Compute the distance from each point x to each cluster centroid ,
*
2. Assign each point to the centroid it is closest to,
*
3. Recompute each centroid as the mean of all points assigned to it,
*
* @param descriptors
The input SIFT descriptors to cluster.
* @param num_clusters
The size of the dictionary, ie, number of visual words.
* @param max_iterations Maximum number of iterations before convergence.
* @return cv::Mat
One unique Matrix representing all the $k$-means(stacked).
*/

std::tuple<cv::Mat, cv::Mat> kMeans(const std::vector<cv::Mat> &descriptors,
                                    int num_clusters, int max_iter);

std::tuple<cv::Mat, cv::Mat> kMeans(const cv::Mat &mat_data, int num_clusters,
                                    int max_iter);

} // namespace bovw
#endif