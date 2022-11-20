#include "Pipeline.h"

#include "event/Definitions.h"
#include "event/Event.h"

namespace editor
{

    pipeline::pipeline(rt::root& root)
        : isystem(root), draw_pipe(root, pipeline_state), msaa_pipe(root, pipeline_state)
    {
        rt::gfx::texture_internal_format fb = rt::gfx::texture_internal_format::rgba8;
        pipeline_state.target.init(&fb, 1, 1920, 1080, true, 16);
#ifndef PX_STANDALONE
        pipeline_state.target_msaa.init(&fb, 1, 1920, 1080);
#endif

        rt::evnt::evnt::subscribe(rt::evnt::rt_event_type::viewport_resize_event, rt::evnt::evnt_callback(
            [&](rt::evnt::rt_event* e) {
                rt::evnt::viewport_resize_event* vr = (rt::evnt::viewport_resize_event*)e;
                this->pipeline_state.target.gen_attachments(vr->width, vr->height);
#ifndef PX_STANDALONE
                this->pipeline_state.target_msaa.gen_attachments(vr->width, vr->height);
#endif
            }
        ));


        rt::evnt::evnt::subscribe(rt::evnt::rt_event_type::window_resize_event, rt::evnt::evnt_callback(
            [&](rt::evnt::rt_event* e) {
                rt::evnt::window_resize_event* vr = (rt::evnt::window_resize_event*)e;
                rt::evnt::viewport_resize_event evt(vr->window, vr->width, vr->height);
                rt::evnt::evnt::fire(rt::evnt::rt_event_type::viewport_resize_event, &evt);
            }
        ));

        pipeline_state.viewport_size = { 1920.0f, 1080.0f };
    }

    void pipeline::process()
    {
        draw_pipe.process();
        msaa_pipe.process();
    }

    nlohmann::json pipeline::serialize()
    {
        return {
            pipeline_state.active_camera
        };
    }

    void pipeline::deserialize(nlohmann::json& dt)
    {
        pipeline_state.active_camera = dt[0];
    }

}