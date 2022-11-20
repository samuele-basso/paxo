#pragma once
#include "Pipe.h"
#include "PipelineState.h"
#include "core/ISystem.h"

namespace editor
{

    class pipeline : public rt::isystem
    {
    public:
        pipeline(rt::root& root);

        void process();

        virtual nlohmann::json serialize() override;
        virtual void deserialize(nlohmann::json& dt) override;
        static const char* serialize_key() { return "system_pipeline"; }

        draw_pipe draw_pipe;
#ifndef PX_STANDALONE
        msaa_pipe msaa_pipe;
#else
        msaa_pipe_default msaa_pipe;
#endif

        pipeline_state pipeline_state;
    };

}