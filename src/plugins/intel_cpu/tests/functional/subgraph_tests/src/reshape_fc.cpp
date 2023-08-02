// Copyright (C) 2021-2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "shared_test_classes/base/ov_subgraph.hpp"
#include <common_test_utils/ov_tensor_utils.hpp>
#include "ngraph_functions/builders.hpp"
#include "test_utils/fusing_test_utils.hpp"

using namespace CPUTestUtils;
using namespace ov::test;

namespace CPULayerTestsDefinitions {

using ReshapeFcSpecParams = std::tuple<std::vector<InputShape>, // input shapes
                                       std::vector<int>,        // reshape data
                                       ElementType>;            // precision

using ReshapeFcParams = std::tuple<ReshapeFcSpecParams,
                                   fusingSpecificParams,
                                   CPUSpecificParams>;

class ReshapeFcCPUTest : public testing::WithParamInterface<ReshapeFcParams>, virtual public SubgraphBaseTest, public CpuTestWithFusing {
public:
    static std::string getTestCaseName(testing::TestParamInfo<ReshapeFcParams> obj) {
        std::vector<InputShape> shapes;
        std::vector<int> data;
        ElementType prc;

        ReshapeFcSpecParams specParams;
        fusingSpecificParams fusingParams;
        CPUSpecificParams cpuParams;

        std::tie(specParams, fusingParams, cpuParams) = obj.param;
        std::tie(shapes, data, prc) = specParams;

        std::ostringstream result;

        result << "IS=";
        for (const auto& shape : shapes) {
            result << ov::test::utils::partialShape2str({shape.first}) << "_";
        }
        result << "TS=";
        for (const auto& shape : shapes) {
            result << "(";
            if (!shape.second.empty()) {
                auto itr = shape.second.begin();
                do {
                    result << ov::test::utils::vec2str(*itr);
                } while (++itr != shape.second.end() && result << "_");
            }
            result << ")_";
        }
        result << "DATA=" << "[" << ov::test::utils::vec2str(data) << "]_";
        result << "PRC=" << prc << "_";

        result << CpuTestWithFusing::getTestCaseName(fusingParams);
        result << CPUTestsBase::getTestCaseName(cpuParams);

        return result.str();
    }

protected:
    void SetUp() override {
        targetDevice = ov::test::utils::DEVICE_CPU;

        std::vector<InputShape> shapes;
        std::vector<int> data;
        ElementType prc;

        ReshapeFcSpecParams specParams;
        fusingSpecificParams fusingParams;
        CPUSpecificParams cpuParams;

        std::tie(specParams, fusingParams, cpuParams) = this->GetParam();
        std::tie(shapes, data, prc) = specParams;
        std::tie(inFmts, outFmts, priority, selectedType) = cpuParams;
        std::tie(postOpMgrPtr, fusedOps) = fusingParams;

        selectedType = makeSelectedTypeStr(selectedType, prc);

        init_input_shapes(shapes);

        auto params = ngraph::builder::makeDynamicParams(prc, {inputDynamicShapes.front()});
        auto reshapeData = ngraph::builder::makeConstant(ElementType::i32, {data.size()}, data);
        auto reshape = std::make_shared<ngraph::opset1::Reshape>(params[0], reshapeData, true);

        auto weight = ngraph::builder::makeDynamicInputLayer(prc, ngraph::helpers::InputLayerType::CONSTANT, inputDynamicShapes.back());
        auto matMul = ngraph::builder::makeMatMul(reshape, weight, false, false);

        function = makeNgraphFunction(prc, params, matMul, "ReshapeFcModel");
    }
};

TEST_P(ReshapeFcCPUTest, CompareWithRefs) {
   run();
   CheckPluginRelatedResults(compiledModel, "FullyConnected");
}

const std::vector<ReshapeFcSpecParams> reshFcParams = {
    ReshapeFcSpecParams{
        {
            {{{1, 10}, 160}, {{1, 160}, {1, 160}, {5, 160}, {2, 160}}},
            {{32, 3}, {{32, 3}, {32, 3}, {32, 3}, {32, 3}}}
        },
        std::vector<int>{-1, 5, 32},
        ElementType::f32
    }
};

static std::vector<fusingSpecificParams> filterFusingParams(const std::vector<fusingSpecificParams>& orig) {
#ifdef OV_CPU_WITH_MLAS
    return {emptyFusingSpec, fusingBias};
#else
    return orig;
#endif
}

std::vector<fusingSpecificParams> fusingParamsSet {
        emptyFusingSpec,
        fusingBias,
        fusingMultiplyPerChannel
};

#ifdef OV_CPU_WITH_MLAS
const auto gemmParam = CPUSpecificParams{{}, {}, {"gemm_mlas"}, "gemm_mlas"};
#else
const auto gemmParam = CPUSpecificParams{{}, {}, {"jit_gemm"}, "jit_gemm"};
#endif
const auto params = ::testing::Combine(
    ::testing::ValuesIn(reshFcParams),
    ::testing::ValuesIn(filterFusingParams(fusingParamsSet)),
    ::testing::Values(gemmParam));

INSTANTIATE_TEST_SUITE_P(smoke_ReshapeFc, ReshapeFcCPUTest, params, ReshapeFcCPUTest::getTestCaseName);

} // namespace CPULayerTestsDefinitions
