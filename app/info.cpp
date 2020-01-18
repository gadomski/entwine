/******************************************************************************
* Copyright (c) 2019, Connor Manning (connor@hobu.co)
*
* Entwine -- Point cloud indexing
*
* Entwine is available under the terms of the LGPL2 license. See COPYING
* for specific license text and more information.
*
******************************************************************************/

#include "info.hpp"

#include <entwine/types/metadata.hpp>
#include <entwine/types/srs.hpp>
#include <entwine/util/fs.hpp>
#include <entwine/util/info.hpp>
#include <entwine/util/time.hpp>

namespace entwine
{
namespace app
{

void Info::addArgs()
{
    m_ap.setUsage("entwine info <path(s)> (<options>)");

    addInput(
            "File paths or directory entries.  For a recursive directory "
            "search, the notation is 'directory/**'\n"
            "Example: --input path.laz, --input data-directory/",
            true);

    addOutput(
            "If provided, detailed per-file information will be written "
            "to this directory in JSON format\n"
            "Example: --output my-output/");

    addTmp();
    addDeep();
    addReprojection();
    addSimpleThreads();
    addConfig();
    addArbiter();
}

void Info::run()
{
    const std::string output(m_json.value("output", ""));

    std::cout << "Analyzing" << std::endl;
    const auto start = now();
    const auto sources = analyze(m_json);
    const auto reduced = manifest::reduce(sources);
    std::cout << "Analyzed in " << formatTime(since(start)) << "s." <<
        std::endl;

    if (output.size())
    {
        std::cout << "Serializing to " << output << std::endl;

        const arbiter::Arbiter a(m_json.value("arbiter", json()).dump());
        const unsigned int threads(m_json.value("threads", 8));

        if (a.isLocal(output)) arbiter::mkdirp(output);

        const auto endpoint(a.getEndpoint(output));

        const auto start = now();
        serialize(sources, endpoint, threads);

        std::cout << "Serialized in " << formatTime(since(start)) << "s." <<
            std::endl;
    }

    // std::cout << "Sources: " << json(sources).dump(2) << std::endl;
    std::cout << "Info: " << json(reduced).dump(2) << std::endl;
}

} // namespace app
} // namespace entwine
