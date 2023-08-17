// Copyright (C) 2018-2023 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "openvino/util/file_util.hpp"
#include "functional_test_utils/ov_plugin_cache.hpp"
#include "utils/model.hpp"

namespace ov {
namespace tools {
namespace subgraph_dumper {

void save_model_status_to_file(const std::map<ModelCacheStatus, std::vector<std::string>>& caching_status,
                               const std::string& output_dir) {
    std::string cache_status_path = ov::util::path_join({output_dir, "model_caching_status"});
    if (!ov::util::directory_exists(cache_status_path)) {
        ov::util::create_directory_recursive(cache_status_path);
    }
    for (const auto& status_info : caching_status) {
        std::string output_file_path = ov::util::path_join({ cache_status_path, model_cache_status_to_str[status_info.first] + ov::test::utils::LST_EXTENSION});
        ov::test::utils::vec2File(status_info.second, output_file_path);
    }
}

// { models, { not_read_model }}
std::pair<std::vector<std::string>, std::pair<ModelCacheStatus, std::vector<std::string>>>
find_models(const std::vector<std::string> &dirs, const std::string& regexp) {
    std::vector<std::string> models, full_content;
    for (const auto& dir : dirs) {
        std::vector<std::string> dir_content;
        if (ov::util::directory_exists(dir)) {
            dir_content = ov::test::utils::getFileListByPatternRecursive({dir}, FROTEND_REGEXP);
        } else if (ov::util::file_exists(dir) && std::regex_match(dir, std::regex(".*" + std::string(ov::test::utils::LST_EXTENSION)))) {
            dir_content = ov::test::utils::readListFiles({dir});
        } else {
            std::string msg = "Input directory (" + dir + ") doesn't not exist!";
            throw std::runtime_error(msg);
        }
        if (!dir_content.empty()) {
            full_content.insert(full_content.end(), dir_content.begin(), dir_content.end());
        }
    }
    std::multimap<size_t, std::string> models_sorted_by_size;
    std::vector<std::string> not_read_model;
    auto in_regex = std::regex(regexp);
    for (const auto& model_file : full_content) {
        if (std::regex_match(model_file, in_regex)) {
            try {
                // models.emplace_back(file);
                if (ov::util::file_exists(model_file)) {
                    auto core = ov::test::utils::PluginCache::get().core();
                    auto model_size = core->read_model(model_file)->get_graph_size();
                    models_sorted_by_size.insert({ model_size, model_file});
                } else {
                    continue;
                }
            } catch (std::exception& e) {
                not_read_model.emplace_back(model_file);
                std::cout << "[ ERROR ] Impossible to read model: " << model_file << std::endl << "Exception: " << e.what();
            }
        }
    }
    // sort model by size with reverse
    auto model_cnt = models_sorted_by_size.size();
    models.resize(model_cnt);
    auto it = models_sorted_by_size.rbegin();
    for (size_t i = 0; i < model_cnt; ++i) {
        models[i] = it->second;
        ++it;
    }
    std::cout << "[ INFO ] Total model number is " << models.size() << std::endl;
    return { models, { ModelCacheStatus::NOT_READ, not_read_model } };
}

std::map<ModelCacheStatus, std::vector<std::string>> cache_models(
    std::vector<std::shared_ptr<ICache>>& caches,
    const std::vector<std::string>& models,
    bool extract_body) {
    std::map<ModelCacheStatus, std::vector<std::string>> cache_status = {
        { ModelCacheStatus::SUCCEED, {} },
        { ModelCacheStatus::NOT_FULLY_CACHED, {} },
        { ModelCacheStatus::NOT_READ, {} }
    };
    auto core = ov::test::utils::PluginCache::get().core();

    for (auto& cache : caches) {
        for (const auto& model : models) {
            if (ov::util::file_exists(model)) {
                ModelCacheStatus model_status = ModelCacheStatus::SUCCEED;
                try {
                    std::shared_ptr<ov::Model> function = core->read_model(model);
                    try {
                        cache->update_cache(function, model, extract_body);
                    } catch (std::exception &e) {
                        std::cout << "[ ERROR ] Model processing failed with exception:" << std::endl << e.what() << std::endl;
                        model_status = ModelCacheStatus::NOT_FULLY_CACHED;
                    }
                } catch (std::exception &e) {
                    model_status = ModelCacheStatus::NOT_READ;
                    std::cout << "[ ERROR ] Model reading failed with exception:" << std::endl << e.what() << std::endl;
                }
                cache_status[model_status].push_back(model);
            }
        }
        cache->serialize_cache();
        cache->reset_cache();
    }
    return cache_status;
}

}  // namespace subgraph_dumper
}  // namespace tools
}  // namespace ov