#include "client.h"

#include <absl/status/status.h>
#include <brpc/controller.h>
#include <protos/service/kfpanda/kfpanda.pb.h>

#include <memory>
#include <string>

#include "fmt/format.h"

namespace kfpanda {
absl::Status KfpandaClient::Init() {
  // Initialize the channel, NULL means using default options.
  brpc::ChannelOptions options;
  options.protocol = "baidu_std";
  options.connection_type = "single";
  options.connect_timeout_ms = 3000;
  options.timeout_ms = 3000 /*milliseconds*/;
  options.max_retry = 0;
  if (channel_.Init(server_.c_str(), "", &options) != 0) {
    has_error_ = true;
    return absl::ErrnoToStatus(400, "absl::string_view message");
  }
  stub_ = std::make_shared<kfpanda::KfPandaService_Stub>(&channel_);
  return absl::OkStatus();
}

absl::Status KfpandaClient::Record(google::protobuf::MethodDescriptor *method, google::protobuf::Message *msg,
                                   kfpanda::RecordType record_type) {
  if (method == nullptr || msg == nullptr) {
    return absl::ErrnoToStatus(400, "[KfpandaClient::Record] method or msg is nullptr");
  }
  if (has_error_ || stub_ == nullptr) {
    return absl::ErrnoToStatus(500, "[KfpandaClient::Record] kfpanda client init failed");
  }
  auto pt = fmt::format("/{}/{}", method->service()->full_name(), method->name());
  kfpanda::RecordRequest n_req;
  kfpanda::RecordResponse n_resp;
  n_req.set_service(service_);
  n_req.set_type(record_type);
  n_req.mutable_uri()->set_path(pt);
  msg->SerializeToString(n_req.mutable_data());

  brpc::Controller cntl;
  stub_->Record(&cntl, &n_req, &n_resp, nullptr);
  if (cntl.Failed()) {
    return absl::ErrnoToStatus(cntl.ErrorCode(), cntl.ErrorText());
  } else {
    return absl::OkStatus();
  }
}

absl::Status KfpandaClient::Sample(std::vector<std::string> &result, int count, const std::string service) {
  brpc::Controller cntl;
  kfpanda::SampleRequest request;
  request.set_service(service.empty() ? service_ : service);
  request.set_count(count);
  kfpanda::SampleResponse response;
  stub_->Sample(&cntl, &request, &response, nullptr);
  if (cntl.Failed()) {
    return absl::ErrnoToStatus(cntl.ErrorCode(), cntl.ErrorText());
  } else if (response.code() == 0) {
    for (auto &data : response.data()) {
      result.emplace_back(data);
    }
    return absl::OkStatus();
  } else {
    return absl::ErrnoToStatus(response.code(), response.message());
  }
}

}  // namespace kfpanda
