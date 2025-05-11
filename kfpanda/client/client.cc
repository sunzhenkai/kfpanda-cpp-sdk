#include "client.h"

#include <memory>

namespace kfpanda {
absl::Status KfpandaClient::Init() {
  // Initialize the channel, NULL means using default options.
  brpc::ChannelOptions options;
  options.protocol = "baidu_std";
  options.connection_type = "single";
  options.timeout_ms = 1000 /*milliseconds*/;
  options.max_retry = 0;
  if (channel_.Init(server_.c_str(), "", &options) != 0) {
    has_error_ = true;
    return absl::ErrnoToStatus(400, "absl::string_view message");
  }
  stub_ = std::make_shared<kfpanda::KfPandaService_Stub>(&channel_);
  return absl::OkStatus();
}
}  // namespace kfpanda
