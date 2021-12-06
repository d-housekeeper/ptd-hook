
#include "android-utils.h"
#include "json.hpp"
#include "responses/start-quest-response.h"
#include "string-utils.h"
#include <android/log.h>
#include <fstream>
#include <sstream>

using json = nlohmann::json;

static std::string loadResponsePmFromFile(const std::string &apiName);
static std::string wrapResponsePm(const std::string &responsePm);

std::string loadResponse(const std::string &apiName, const std::string &requestPM) {
  json requestJSON = json::parse(requestPM, nullptr, true);
  if (requestJSON.is_discarded()) {
    __android_log_print(ANDROID_LOG_ERROR, androidLogTag, "Failed to parse %s request", apiName.c_str());
    return "";
  }

  if (apiName == "StartQuest") {
    return wrapResponsePm(getStartQuestResponse(requestJSON));
  }

  std::string responsePm = loadResponsePmFromFile(apiName);
  return wrapResponsePm(responsePm);
}

static std::string loadResponsePmFromFile(const std::string &apiName) {
  std::string fileName = string_format("%s/Responses/%s.json", getExternalFilesDir(), apiName.c_str());

  std::ifstream t(fileName);
  if (t.fail()) {
    __android_log_print(ANDROID_LOG_ERROR, androidLogTag, "Failed to open response file at %s", fileName.c_str());
    return "";
  } else {
    __android_log_print(ANDROID_LOG_DEBUG, androidLogTag, "Loading response file from %s", fileName.c_str());
  }

  std::stringstream buffer;
  if (!(buffer << t.rdbuf())) {
    __android_log_print(ANDROID_LOG_ERROR, androidLogTag, "Failed to read response file from %s", fileName.c_str());
    return "";
  }

  return buffer.str();
}

static std::string wrapResponsePm(const std::string &responsePm) {
  json responseJson = {{"pm", responsePm}};

  return responseJson.dump();
}
