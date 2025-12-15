#include "Global.h"

using namespace app;

void fiber_loop()
{
    while (true)
    {
        util::do_timed("test_message", 500, []{
            call_once([]{
                 DBG(cmd::detail::colorize("Colored message Example", Color(0xffc993)));
            });

            DBG("This is sent every 0.5 seconds !");
        });

        util::do_timed("util_example", 5000, [&] {
            std::string hello = util::convert("Hello", eTextCase::Small);
            std::string world = util::convert("World", eTextCase::Caps);
            std::string full_string = util::combine_strings(hello, world, ",", std::hex, 4911914);
            std::string forward_until = util::get_string_until(full_string, ",");
            std::string backward_until = util::get_string_until(full_string, ",", eTextDirection::Backward);

            DBG("{}", full_string);
            DBG("forward_until: {}", forward_until);
            DBG("backward_until: {}", backward_until);
        });

        Fiber::get()->yield();
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR pCmdLine, int nCmdShow)
{
    Fiber::ensure_thread_is_a_fiber();

    cmd::create(APP);

    cmd::sub_header(std::format("{} {}",
        cmd::detail::colorize("App:", Color(0xffffff)),
        cmd::detail::colorize("Example", Color(0x6c98ff))
    ));

    cmd::sub_header(std::format("{} {}",
        cmd::detail::colorize("Unload Key:", Color(0xffffff)),
        cmd::detail::colorize("F12", Color(0x6c98ff))
    ));

    cmd::raw_send("________________________________________________\n");
    
    fiber_manager::add_fiber("fiber_test", &fiber_loop);

    while (true)
    {
        fiber_manager::tick();

        if (input::is_key_pressed(VK_F12))
        {
            break;
        }

        std::this_thread::sleep_for(0ms);
    }

    MessageBoxA(NULL, "Press OK to exit.", "Close Application", MB_OK);

    cmd::destroy();

    return 0;
}
