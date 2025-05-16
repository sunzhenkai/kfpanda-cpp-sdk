/**
 * @file debug_client.h
 * @brief
 * @author zhenkai.sun
 * @date 2025-05-11 20:44:09
 */
#pragma once
#include <memory>
#include <string>

#include "absl/status/status.h"
#include "brpc/channel.h"
#include "protos/service/kfpanda/kfpanda.pb.h"

namespace kfpanda {
class KfPandaDebugClient {
 public:
  explicit KfPandaDebugClient(const std::string &server) : server_(server) {}
  absl::Status Init();
  kfpanda::KfPandaDebugService_Stub *Stub() const;
  bool HasError() const { return has_error_; }

 private:
  std::string server_;
  brpc::Channel channel_;
  std::shared_ptr<kfpanda::KfPandaDebugService_Stub> stub_;
  bool has_error_{false};
};

inline absl::Status KfPandaDebugClient::Init() {
  // Initialize the channel, NULL means using default options.
  brpc::ChannelOptions options;
  options.protocol = "baidu_std";
  options.connection_type = "single";
  options.timeout_ms = 1000 /*milliseconds*/;
  options.max_retry = 0;
  if (channel_.Init(server_.c_str(), "", &options) != 0) {
    has_error_ = true;
    return absl::ErrnoToStatus(400, "cannot connect to server");
  }
  stub_ = std::make_shared<kfpanda::KfPandaDebugService_Stub>(&channel_);
  return absl::OkStatus();
}

inline kfpanda::KfPandaDebugService_Stub *KfPandaDebugClient::Stub() const { return stub_.get(); }
}  // namespace kfpanda
