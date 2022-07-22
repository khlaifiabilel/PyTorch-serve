
#ifndef TS_CPP_UTILS_MESSAGE_HH_
#define TS_CPP_UTILS_MESSAGE_HH_

#include <folly/dynamic.h>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace torchserve {
  // TODO: expand to support model instance, large model (ref: ModelConfig in config.hh)
  struct LoadModelRequest {
    // /path/to/model/file
    const std::string model_path;	
    const std::string model_name;
    // Existing: -1 if CPU else gpu_id
    // TODO: 
    // - support CPU core
    // - device type combine together
    int gpu_id;		
    // Expected to be null for cpp backend				        
    const std::string handler;	
    // name of wrapper/unwrapper of request data if provided	
    const std::string envelope;	  
    int batch_size;
    // limit pillow image max_image_pixels
    bool limit_max_image_pixels;	

    LoadModelRequest(
      const std::string& model_path,
      const std::string& model_name,
      int gpu_id,
      const std::string& handler,
      const std::string& envelope,
      int batch_size,
      bool limit_max_image_pixels
    ) : model_path{model_path}, model_name{model_name}, gpu_id{gpu_id}, 
    handler{handler}, envelope{envelope}, batch_size{batch_size}, 
    limit_max_image_pixels{limit_max_image_pixels} {}
  };

  struct LoadModelResponse {
    int code;
    int length;
    const std::string buf;

    LoadModelResponse(int code, int length, const std::string buf) : 
    code(code), length(length), buf(buf) {};
  };

  using ParameterValue = std::variant<
    std::string,                           // str_value
    folly::dynamic,                        // json_value
    std::vector<std::byte>                 // bytes_value
  >;             
  struct InferenceRequest {
    struct Header {
      std::string name;
      std::vector<std::byte> value;
    };

    struct Parameter {
      std::string name;
      std::string content_type;
      std::vector<ParameterValue> values;
    };
    std::string request_id;
    std::vector<Header> request_headers;
    std::vector<Parameter> parameters;
  };

  struct InferenceResponse {
    // TODO: definition
    int code;
    int length;
    const std::string buf;
    InferenceResponse() {};
  };
} // namespace torchserve
#endif // TS_CPP_UTILS_MESSAGE_HH_