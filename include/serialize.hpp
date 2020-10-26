
#ifndef SERIALIZE_HPP_
#define SERIALIZE_HPP_

#include <string>

#include <opencv2/core/mat.hpp>

namespace bovw::serialization {

/**
 * @brief Writes the input cv::Mat(Image, SIFT, etc) to a binary file
 *
 * @param m        The input cv::Mat you want to serialize.
 * @param filename The filename of the file where the cv::Mat will be written.
 */
void Serialize(const cv::Mat &m, const std::string &filename);

/**
 * @brief Reads a binary file representing a cv::Mat type and outputs the
 * contetnt of this file into a new cv::Mat
 *
 * @param filename The binary file you want to read
 * @return cv::Mat The cv::Mat created from the binary file
 */
cv::Mat Deserialize(const std::string &filename);

} // namespace bovw::serialization

#endif // SERIALIZE_HPP_
