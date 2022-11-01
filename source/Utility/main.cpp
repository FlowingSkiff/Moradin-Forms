#include <algorithm>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <fmt/core.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <tinyxml2.h>

static std::shared_ptr<spdlog::logger> logger;

/// @brief Finds all xml files in the specified directory.
/// @param root Directory to search for files.
/// @return
[[nodiscard]] auto find_xml_files(const std::filesystem::path& root)
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

struct attribute
{
  std::string name;
  std::string value;
};

struct element
{
  std::string name;
  std::string text;
  std::vector<element> children;
  std::vector<attribute> attributes;
};

namespace impl
{
void build_element_recurse_impl(tinyxml2::XMLElement* node, element& head)
{
  if (static_cast<bool>(node)) {
    element tmp;
    tmp.name = node->Name();
    if (static_cast<bool>(node->GetText())) {
      tmp.text = node->GetText();
      logger->trace("Has text: {}", tmp.text);
    }
    if (tmp.name != "description") {
      auto* att = node->FirstAttribute();
      while (static_cast<bool>(att)) {
        logger->trace("Found attribute: {} {}", att->Name(), att->Value());
        tmp.attributes.push_back({att->Name(), att->Value()});
        att = att->Next();
      }
      auto* child = node->FirstChildElement();
      while (static_cast<bool>(child)) {
        logger->trace("Found child: {}", child->Name());
        build_element_recurse_impl(child, tmp);
        child = child->NextSiblingElement();
      }
    }
    head.children.push_back(tmp);
  }
}
}  // namespace impl

auto build_element_recurse(tinyxml2::XMLElement* node)
{
  element head;
  head.name = node->Name();
  auto* child = node->FirstChildElement();
  while (static_cast<bool>(child)) {
    impl::build_element_recurse_impl(child, head);
    child = child->NextSiblingElement();
  }
  return head;
}

/// @brief Parses a xml file and finds all element types.
/// @param file file to open and parse
/// @return
[[nodiscard]] auto parse_xml_file(const std::filesystem::path& file)
    -> std::unordered_map<std::string, std::vector<std::string>>
{
  std::unordered_map<std::string, std::vector<std::string>> type_list;
  using tinyxml2::XMLDocument;
  XMLDocument doc;
  if (doc.LoadFile(file.string().c_str()) == tinyxml2::XML_SUCCESS) {
    auto* head = doc.FirstChildElement();
    auto all_elements = build_element_recurse(head);
    // auto* ele = doc.FirstChildElement("elements");
    // auto* child = ele->FirstChildElement("element");
    // if (static_cast<bool>(child)) {
    //   while (static_cast<bool>(child)) {
    //     const auto* txt = child->Attribute("name");
    //     const auto* type = child->Attribute("type");
    //     if (static_cast<bool>(txt) && static_cast<bool>(type)) {
    //       type_list[std::string(type)].push_back(std::string(txt));
    //     } else if (static_cast<bool>(type)) {
    //       logger->warn(
    //           "Could not find text for type {} in {}", type, file.string());
    //     } else if (static_cast<bool>(txt)) {
    //       logger->warn(
    //           "Could not find type for name {} in {}", txt, file.string());
    //     } else {
    //       logger->warn("Could not find type or name in {}", file.string());
    //     }
    //
    //     child = child->NextSiblingElement("element");
    //   }
    // }
  }
  return type_list;
}

/// @brief sets up spdlog for use
void spdlog_setup()
{
  spdlog::set_level(spdlog::level::trace);
  std::vector<spdlog::sink_ptr> sinks;
  sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
  sinks.push_back(
      std::make_shared<spdlog::sinks::basic_file_sink_mt>("log.txt"));
  logger = std::make_shared<spdlog::logger>(
      "Name", std::begin(sinks), std::end(sinks));
  logger->set_level(spdlog::level::trace);
}

auto main(int argc, char* argv[]) noexcept -> int
{
  bool error = false;
  try {
    spdlog_setup();
    if (argc < 2) {
      logger->error("Tracer expects at least one argument");
      error = true;
    } else if (not std::filesystem::exists(
                   std::filesystem::path(std::string(argv[1]))))
    {
      logger->error("Specified directory does not exist.");
      error = true;
    }
    if (not error) {
      auto files = find_xml_files(std::filesystem::path(std::string(argv[1])));
      std::unordered_map<std::string, std::vector<std::string>> type_map;
      for (const auto& path : files) {
        auto tmp = parse_xml_file(path);
        for (auto [type, list] : tmp) {
          auto& curr_list = type_map[type];
          std::move(list.begin(), list.end(), std::back_inserter(curr_list));
        }
      }
      for (auto [type, list] : type_map) {
        logger->trace("Found type {}", type);
      }
    }
  } catch (std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    error = true;
  }
  return static_cast<int>(error);
}