// Copyright (C) 2023 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "ngraph_functions/builders.hpp"
#include "ngraph_functions/utils/ngraph_helpers.hpp"
#include "test_utils/cpu_test_utils.hpp"
#include "shared_test_classes/base/layer_test_utils.hpp"
#include "shared_test_classes/base/ov_subgraph.hpp"

using namespace ngraph;
namespace SubgraphTestsDefinitions {
class ConvDWConv : virtual public ov::test::SubgraphBaseTest {
protected:
    void SetUp() override {
        targetDevice = ov::test::utils::DEVICE_CPU;
        const auto precision = ov::element::f32;
        ov::test::InputShape input_shape{{}, {{1, 32, 112, 112}}};
        init_input_shapes({input_shape});


        auto params = ngraph::builder::makeDynamicParams(precision, inputDynamicShapes);
        auto conv_weights = ngraph::builder::makeConstant(precision, std::vector<size_t>{32, 32, 1, 1}, std::vector<float>{}, true);
        auto conv = ngraph::builder::makeConvolution(params[0],
                                                     conv_weights,
                                                     precision,
                                                     std::vector<size_t>{1, 1},
                                                     std::vector<size_t>{1, 1},
                                                     ov::CoordinateDiff{0, 0},
                                                     ov::CoordinateDiff{0, 0},
                                                     std::vector<size_t>{1, 1},
                                                     ngraph::op::PadType::EXPLICIT,
                                                     32,
                                                     true);

        auto dw_conv_weights = ngraph::builder::makeConstant(precision, std::vector<size_t>{32, 1, 1, 3, 3}, std::vector<float>{}, true);
        auto dw_conv = ngraph::builder::makeGroupConvolution(conv,
                                                             dw_conv_weights,
                                                             precision,
                                                             std::vector<size_t>{1, 1},
                                                             ov::CoordinateDiff{1, 1},
                                                             ov::CoordinateDiff{1, 1},
                                                             std::vector<size_t>{1, 1},
                                                             ngraph::op::PadType::EXPLICIT);
        auto bias_const = ngraph::builder::makeConstant(precision, {1, 32 , 1, 1}, std::vector<float>{}, true);
        auto bias = std::make_shared<ov::opset10::Add>(dw_conv, bias_const);
        function = std::make_shared<ov::Model>(bias, params, "ConvDWConv");
    }
};

TEST_F(ConvDWConv, smoke_CompareWithRefs) {
    run();
}

} // namespace SubgraphTestsDefinitions
