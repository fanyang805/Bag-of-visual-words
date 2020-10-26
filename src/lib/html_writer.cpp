// #include <fstream>
#include "html_writer.hpp"
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <string>
namespace fs = std::filesystem;
namespace bovw::image_browser {

// void OpenDocument() {
//   std::ifstream readhtml{"out_app.html"};
//   if (!readhtml) {
//     std::cerr << "Canot open the html file!\n";
//   }
//   while (readhtml) {
//     std::string readline;
//     std::getline(readhtml, readline);
//     ofs << readline << std::endl;
//   }
// }
// void OpenDocument() { ofs << "<!DOCTYPE html>\n<html>"; }

// void CloseDocument() { ofs << "</html>\n"; }

void AddCSSStyle(const std::string &stylesheet, std::ofstream &ofs) {
  ofs << "<head>\n"
      << "<link rel=\"stylesheet\" type=\"text/css\" href=\"" << stylesheet
      << "\"/>\n"
      << "</head>\n";
}

void AddTitle(const std::string &title, std::ofstream &ofs) {
  ofs << "<title>" << title << "</title>\n";
}

void OpenBody(std::ofstream &ofs) { ofs << "<body>\n"; }

void CloseBody(std::ofstream &ofs) { ofs << "</body>\n"; }

void OpenRow(std::ofstream &ofs) { ofs << "<div class=\" row \">\n"; }

void CloseRow(std::ofstream &ofs) { ofs << " </div>\n"; }

void AddImage(const std::string &img_path, float score, std::ofstream &ofs,
              bool highlight) {
  const std::string img_extension = fs::path(img_path).extension();
  if (img_extension != ".png" && img_extension != ".jpg" &&
      img_extension != "") {
    std::cerr << "Image file format error. Please use .png or .jpg.\n";
  }

  if (highlight) {
    ofs << "<div class=\"column\" style = \"border: 5px solid green;\">\n";
  } else {
    ofs << "<div class = \"column\">\n";
  }
  ofs << "<h2>" << fs::path(img_path).stem().string() << "</h2>\n";
  ofs << "<img src=\"" << img_path << "\" />\n";
  ofs << "<p>score = ";
  ofs << std::setprecision(2) << std::fixed << score << "</p>\n";
  ofs << "</div>\n";
}
} // namespace bovw::image_browser