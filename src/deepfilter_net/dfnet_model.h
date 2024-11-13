// Copyright (C) 2024 Intel Corporation
// SPDX-License-Identifier: GPL-3.0-only
#pragma once
#ifdef USE_DEEPFILTERNET
#include <optional>
#include <openvino/openvino.hpp>
#include "multiframe.h"

namespace melo {
      namespace ov_deepfilternet {
         enum class ModelSelection
         {
            DEEPFILTERNET2,
            DEEPFILTERNET3,
         };

         class Mask;
         class DFNetModel
         {
         public:

            DFNetModel(
               std::unique_ptr<ov::Core>& core,
               std::string model_folder,
               std::string device,
               ModelSelection model_selection,
               const ov::AnyMap& nf_ov_cfg,
               torch::Tensor erb_widths,
               int64_t lookahead = 2, int64_t nb_df = 96);

            torch::Tensor
               forward(torch::Tensor spec, torch::Tensor feat_erb, torch::Tensor feat_spec, bool post_filter=false);

            int64_t num_static_hops()
            {
               return _num_hops;
            };

            [[maybe_unused]] inline void get_profiling_info(ov::InferRequest& _infer_request) {
               std::vector<ov::ProfilingInfo> perfs_count_list = _infer_request.get_profiling_info();
               perfs_count_list.erase(std::remove_if(perfs_count_list.begin(),perfs_count_list.end(),
                       [](ov::ProfilingInfo info){return info.status == ov::ProfilingInfo::Status::NOT_RUN;}), perfs_count_list.end());
               std::sort(perfs_count_list.begin(),perfs_count_list.end(),[&](ov::ProfilingInfo x, ov::ProfilingInfo y){return x.real_time>y.real_time;});
               std::cout << std::endl;
               for (const auto& x : perfs_count_list) {
                   if (x.status == ov::ProfilingInfo::Status::NOT_RUN) continue;

                   std::cout << x.node_name << ',' << x.node_type << ',' << (int)(x.status) << ',' << x.exec_type << ',' << x.cpu_time << ',' << x.real_time << std::endl;
               }
               std::cout << std::endl;
            }
         private:

            torch::Tensor
               forward_df3(torch::Tensor spec, torch::Tensor feat_erb, torch::Tensor feat_spec, bool post_filter);

            [[maybe_unused]] torch::Tensor
               forward_df2(torch::Tensor spec, torch::Tensor feat_erb, torch::Tensor feat_spec);

            ov::CompiledModel _model_request_enc;
            ov::CompiledModel _model_request_erb_dec;
            ov::CompiledModel _model_request_df_dec;
            ov::InferRequest _infer_request_enc;
            ov::InferRequest _infer_request_erb_dec;
            ov::InferRequest _infer_request_df_dec;

            std::shared_ptr< torch::nn::ConstantPad3d > _pad_spec;
            std::shared_ptr< torch::nn::ConstantPad2d > _pad_feat;
            std::shared_ptr< Mask > _mask;

            int64_t _nb_df;
            DF _df;

            int64_t _num_hops;

            bool _bDF3;
         };
      }
}
#endif // USE_DEEPFILTERNET



