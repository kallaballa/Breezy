#include <signal.h>
#include <getopt.h>
#include <locale.h>
#include <ctype.h>
#include <assert.h>
#include <string>
#include <iostream>
#include <vector>

#include <pulse/pulseaudio.h>
#include "PulseMonitorSource.hpp"

using std::string;
using std::cerr;
using std::cout;

static pa_context *context = NULL;
static pa_mainloop_api *mainloop_api = NULL;

static bool suspend;
static pa_proplist *proplist = NULL;

static int actions = 1;
static std::vector<string> monitorSources;

static void quit(int ret) {
    mainloop_api->quit(mainloop_api, ret);
}

static void context_drain_complete(pa_context *c, void *userdata) {
    pa_context_disconnect(c);
}

static void drain(void) {
    pa_operation *o;

    if (!(o = pa_context_drain(context, context_drain_complete, NULL)))
        pa_context_disconnect(context);
    else
        pa_operation_unref(o);
}

static void complete_action(void) {
    if (!(--actions))
        drain();
}

static string eliminateNull(const char* c) {
	if(c == NULL)
		return "NULL";
    else
    	return string(c);
}

static void get_sink_info_callback(pa_context *c, const pa_sink_info *i, int is_last, void *userdata) {
    if (is_last < 0) {
        std:cerr << "Failed to get sink information: " << pa_strerror(pa_context_errno(c));
        quit(1);
        return;
    }

    if (is_last) {
        complete_action();
        return;
    }

    if(i->monitor_source_name != NULL)
    	monitorSources.push_back(i->monitor_source_name);
}

static void context_state_callback(pa_context *c, void *userdata) {
    switch (pa_context_get_state(c)) {
        case PA_CONTEXT_CONNECTING:
        case PA_CONTEXT_AUTHORIZING:
        case PA_CONTEXT_SETTING_NAME:
            break;

        case PA_CONTEXT_READY:
        	pa_operation_unref(pa_context_get_sink_info_list(c, get_sink_info_callback, NULL));
            break;

        case PA_CONTEXT_TERMINATED:
            quit(0);
            break;

        case PA_CONTEXT_FAILED:
        default:
            cerr << "Connection failure: " << pa_strerror(pa_context_errno(c));
            quit(1);
    }
}

static void exit_signal_callback(pa_mainloop_api *m, pa_signal_event *e, int sig, void *userdata) {
	cerr << "Got SIGINT, exiting.";
    quit(0);
}

std::vector<string> getPulseMonitorSource() {
	monitorSources.clear();
    pa_mainloop *m = NULL;
    int ret = 1, c;
    char *server = NULL, *bn;

    setlocale(LC_ALL, "");


    if (!(m = pa_mainloop_new())) {
        cerr << "pa_mainloop_new() failed.";
        goto quit;
    }

    mainloop_api = pa_mainloop_get_api(m);

    assert(pa_signal_init(mainloop_api) == 0);
    pa_signal_new(SIGINT, exit_signal_callback, NULL);
    pa_signal_new(SIGTERM, exit_signal_callback, NULL);
  //FIXME: do we miss that call?
  //pa_disable_sigpipe();

    if (!(context = pa_context_new_with_proplist(mainloop_api, NULL, proplist))) {
        cerr << "pa_context_new() failed.";
        goto quit;
    }

    pa_context_set_state_callback(context, context_state_callback, NULL);
    if (pa_context_connect(context, server, PA_CONTEXT_NOFLAGS, NULL) < 0) {
    	cerr << "pa_context_connect() failed: " << pa_strerror(pa_context_errno(context));
        goto quit;
    }

    if (pa_mainloop_run(m, &ret) < 0) {
    	cerr << "pa_mainloop_run() failed.";
        goto quit;
    }

quit:
    if (context)
        pa_context_unref(context);

    if (m) {
        pa_signal_done();
        pa_mainloop_free(m);
    }

    pa_xfree(server);

    if (proplist)
        pa_proplist_free(proplist);

    return monitorSources;
}

