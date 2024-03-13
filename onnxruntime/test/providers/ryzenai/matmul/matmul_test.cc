// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "gtest/gtest.h"
#include "test/common/cuda_op_test_utils.h"
#include "test/common/quantization_test_utils.h"
#include "test/providers/provider_test_utils.h"
#include "core/session/inference_session.h"
#include "core/session/onnxruntime_cxx_api.h"
#include "core/session/onnxruntime_session_options_config_keys.h"

#include "core/common/common.h"
#include "core/framework/op_kernel.h"
#include "core/mlas/inc/mlas.h"
#include "core/util/math_cpuonly.h"
#include "core/util/qmath.h"

#include <algorithm>
#include <random>

// in test_main.cc
extern std::unique_ptr<Ort::Env> ort_env;
#define ORT_MODEL_FOLDER ORT_TSTR("testdata/")

namespace onnxruntime {
namespace test {

TEST(MatmulFloatRyzenTest, MatMulRyzen) {
//   OpTester test("MatMul", 20);
//   test.AddInput<float>("T1", {4, 3}, {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0});
//   test.AddInput<float>("T2", {3, 2}, {1.0, 1.0, 1.0, 1.0, 1.0, 1.0});
//   test.AddOutput<float>("T3", {4, 2}, {3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0});

//   // use ryzen EP
//   auto ryzen_ep = []() -> std::vector<std::unique_ptr<IExecutionProvider>> {
//     std::vector<std::unique_ptr<IExecutionProvider>> execution_providers;
//     execution_providers.push_back(DefaultRyzenAIExecutionProvider());
//     return execution_providers;
//   };
  Ort::SessionOptions so;
  onnxruntime::ProviderOptions options;
  // no real options currently but set a value to make sure it's passed through. requires manual validation.
  options["one"] = "two";
  so.AppendExecutionProvider("RYZENAI", options);
  const ORTCHAR_T* ort_model_path = ORT_TSTR("C:\\Users\\yraparti\\onnxrt_work\\onnxruntime\\build\\Windows\\Debug\\Debug\\testdata\\matmul_1.onnx");
  Ort::Session session(*ort_env, ort_model_path, so);

    // dirty hack to access the underlying InferenceSession but don't know a better way.
    const OrtSession* ort_session = session;
    const InferenceSession* s = reinterpret_cast<const InferenceSession*>(ort_session);

    bool have_ryzenai_ep = false;

    for (const auto& provider : s->GetRegisteredProviderTypes()) {
      if (provider == kRyzenAIExecutionProvider) {
        have_ryzenai_ep = true;
        break;
      }
    }

    ASSERT_TRUE(have_ryzenai_ep) << "ryzenai EP was not found in registered providers for session.";

//   auto ep_vec = ryzen_ep();
//     test.Run(so.GetConst().clone(), OpTester::ExpectResult::kExpectSuccess, "", {}, nullptr,
//              &ep_vec, {});
}

} //test

}// onnxruntime