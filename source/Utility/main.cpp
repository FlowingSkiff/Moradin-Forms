#include <algorithm>
#include <filesystem>
#include <iterator>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <fmt/core.h>
#include <spdlog/spdlog.h>
#include <tinyxml2.h>

auto findXmlFiles(std::filesystem::path root)
    -> std::vector<std::filesystem::path>
{
  std::vector<std::filesystem::path> all_xml_files;
  for (const auto& path : std::filesystem::recursive_directory_iterator(root)) {
    if (path.is_regular_file() && path.path().extension().string() == ".xml") {
      all_xml_files.push_back(path);
    }
  }
  return all_xml_files;
}

auto parseXmlFile(std::filesystem::path file)
    -> std::unordered_map<std::string, std::vector<std::string>>
{
  std::unordered_map<std::string, std::vector<std::string>> type_list;
  using namespace tinyxml2;
  XMLDocument doc;
  if (doc.LoadFile(file.string().c_str()) == XML_SUCCESS) {
    auto ele = doc.FirstChildElement("elements");
    if (auto ch = ele->FirstChildElement("element")) {
      while (ch) {
        const auto* txt = ch->Attribute("name");
        const auto* type = ch->Attribute("type");
        if (txt && type) {
          type_list[std::string(type)].push_back(std::string(txt));
        } else if (type) {
          spdlog::warn(
              "Could not find text for type {} in {}", type, file.string());
        } else if (txt) {
          spdlog::warn(
              "Could not find type for name {} in {}", txt, file.string());
        } else {
          spdlog::warn("Could not find type or name in {}", file.string());
        }

        ch = ch->NextSiblingElement("element");
      }
    }
  }
  return type_list;
}

auto main(int argc, char* argv[]) -> int
{
  if (argc < 2) {
    spdlog::error("Tracer expects at least one argument");
    return 1;
  } else if (not std::filesystem::exists(
                 std::filesystem::path(std::string(argv[1]))))
  {
    spdlog::error("Specified directory does not exist.");
    return 1;
  }
  spdlog::set_level(spdlog::level::trace);
  auto files = findXmlFiles(std::filesystem::path(std::string(argv[1])));
  std::unordered_map<std::string, std::vector<std::string>> type_map;
  for (const auto& path : files) {
    auto tmp = parseXmlFile(path);
    for (auto [type, list] : tmp) {
      auto& curr_list = type_map[type];
      std::move(list.begin(), list.end(), std::back_inserter(curr_list));
    }
  }
  for (auto [type, list] : type_map) {
    spdlog::trace("Found type {}", type);
  }
  return 0;
}