/**
 * @file client.h
 * @brief
 * @author zhenkai.sun
 * @date 2025-05-11 20:38:14
 */
#pragma once
#include <memory>
#include <string>

#include "absl/status/status.h"
#include "brpc/channel.h"
#include "protos/service/kfpanda/kfpanda.pb.h"

namespace kfpanda {
class KfpandaClient {
 public:
  explicit KfpandaClient(const std::string &server) : server_(server) {}
  absl::Status Init();
  kfpanda::KfPandaService_Stub *Stub() const;
  bool HasError() const { return has_error_; }

 private:
  std::string server_;
  brpc::Channel channel_;
  std::shared_ptr<kfpanda::KfPandaService_Stub> stub_;
  bool has_error_{false};
};

inline kfpanda::KfPandaService_Stub *KfpandaClient::Stub() const { return stub_.get(); }
}  // namespace kfpanda
