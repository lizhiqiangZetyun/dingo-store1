// Copyright (c) 2023 dingodb.com, Inc. All Rights Reserved
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef DINGODB_CLIENT_HELPER_H_
#define DINGODB_CLIENT_HELPER_H_

#include <map>
#include <memory>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "braft/util.h"
#include "brpc/channel.h"
#include "brpc/controller.h"
#include "butil/strings/string_split.h"
#include "butil/strings/stringprintf.h"
#include "common/logging.h"
#include "proto/meta.pb.h"
#include "proto/store.pb.h"

namespace client {

const char kAlphabet[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                          's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

const char kAlphabetV2[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                            'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y'};

class Helper {
 public:
  static int GetRandInt() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::mt19937::result_type> distrib(1, 1000000000);
    return distrib(gen);
  }

  // rand string
  static std::string GenRandomString(int len) {
    std::string result;
    int alphabet_len = sizeof(kAlphabet);

    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> distrib(1, 1000000000);
    for (int i = 0; i < len; ++i) {
      result.append(1, kAlphabet[distrib(rng) % alphabet_len]);
    }

    return result;
  }

  static std::string GenRandomStringV2(int len) {
    std::string result;
    int alphabet_len = sizeof(kAlphabetV2);

    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> distrib(1, 1000000000);
    for (int i = 0; i < len; ++i) {
      result.append(1, kAlphabetV2[distrib(rng) % alphabet_len]);
    }

    return result;
  }

  static std::vector<std::string> GenKeys(int nums) {
    std::vector<std::string> vec;
    vec.reserve(nums);
    for (int i = 0; i < nums; ++i) {
      vec.push_back(GenRandomString(4));
    }

    return vec;
  }

  static std::map<std::string, std::string> GenDataset(int n) {
    std::map<std::string, std::string> dataset;

    for (int i = 0; i < n; ++i) {
      std::string key = GenRandomStringV2(32);
      dataset[key] = GenRandomString(256);
    }

    return dataset;
  }

  static std::vector<butil::EndPoint> StrToEndpoints(const std::string& str) {
    std::vector<std::string> addrs;
    butil::SplitString(str, ',', &addrs);

    std::vector<butil::EndPoint> endpoints;
    for (const auto& addr : addrs) {
      butil::EndPoint endpoint;
      if (butil::hostname2endpoint(addr.c_str(), &endpoint) != 0 && str2endpoint(addr.c_str(), &endpoint) != 0) {
        continue;
      }

      endpoints.push_back(endpoint);
    }

    return endpoints;
  }
};

}  // namespace client

#endif  // DINGODB_CLIENT_HELPER_H_