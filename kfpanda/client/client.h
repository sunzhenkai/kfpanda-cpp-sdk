/**
 * @file client.h
 * @brief
 * @author zhenkai.sun
 * @date 2025-05-11 20:38:14
 */
#pragma once
#include <google/protobuf/descriptor.h>

#include <memory>
#include <string>

#include "absl/status/status.h"
#include "brpc/channel.h"
#include "protos/service/kfpanda/kfpanda.pb.h"

namespace kfpanda {
class KfpandaClient {
 public:
  KfpandaClient(const std::string &service, const std::string &server) : service_(service), server_(server) {}

  absl::Status Init();
  kfpanda::KfPandaService_Stub *Stub() const;
  absl::Status Record(google::protobuf::MethodDescriptor *method, google::protobuf::Message *msg,
                      kfpanda::RecordType record_type = kfpanda::RECORD_TYPE_GRPC);
  absl::Status Sample(std::vector<std::string> &result, int count = 1, const std::string service = "");

  inline bool HasError() const { return has_error_; }

 private:
  std::string service_;
  std::string server_;
  brpc::Channel channel_;
  std::shared_ptr<kfpanda::KfPandaService_Stub> stub_;
  bool has_error_{false};
};

inline kfpanda::KfPandaService_Stub *KfpandaClient::Stub() const { return stub_.get(); }
}  // namespace kfpanda
