/******************************************************************************
* Copyright (c) 2019, Connor Manning (connor@hobu.co)
*
* Entwine -- Point cloud indexing
*
* Entwine is available under the terms of the LGPL2 license. See COPYING
* for specific license text and more information.
*
******************************************************************************/

#pragma once

#include <entwine/third/arbiter/arbiter.hpp>
#include <entwine/types/reprojection.hpp>
#include <entwine/types/source.hpp>
#include <entwine/util/optional.hpp>

namespace entwine
{

json createInfoPipeline(
    json pipeline = json::array({ json::object() }),
    bool deep = false,
    optional<Reprojection> = { });

json extractInfoPipelineFromConfig(json config);

SourceInfo analyzeOne(std::string path, bool deep, json pipelineTemplate);
Source parseOne(std::string path, const arbiter::Arbiter& a = { });

SourceList analyze(
    const StringList& inputs,
    const json& pipelineTemplate,
    bool deep,
    std::string tmp = arbiter::getTempPath(),
    const arbiter::Arbiter& a = { },
    unsigned threads = 8);

SourceList analyze(const json& config);

void serialize(
    const SourceList& sources,
    const arbiter::Endpoint& ep,
    unsigned threads = 8);

} // namespace entwine
