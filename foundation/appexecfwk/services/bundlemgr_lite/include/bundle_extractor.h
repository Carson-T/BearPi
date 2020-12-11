/*
 * Copyright (c) 2020 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef OHOS_BUNDLE_EXTRACTOR_H
#define OHOS_BUNDLE_EXTRACTOR_H

#include <cstdint>
#include <sstream>
#include <string>

namespace OHOS {
class BundleExtractor {
public:
    static uint8_t ExtractHapProfile(const std::string &hapFile, std::ostringstream &profileStream);
private:
    BundleExtractor() = default;
    ~BundleExtractor() = default;
};
} // namespace OHOS
#endif // OHOS_BUNDLE_EXTRACTOR_H
