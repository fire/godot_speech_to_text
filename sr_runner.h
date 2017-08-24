#ifndef SR_RUNNER_H
#define SR_RUNNER_H

#include "scene/main/node.h"
#include "core/os/thread.h"

#include "sr_config.h"
#include "sr_queue.h"

// Signal emitted when speech recognition thread has ended
#define SR_RUNNER_END_SIGNAL "speech_recognition_end"

class SRRunner : public Node {
    OBJ_TYPE(SRRunner, Node);

private:
    Thread *recognition;  // Used to run the speech recognition in parallel
    bool is_running;      // If true, speech recognition loop is currently on

    Ref<SRConfig> config;  // Configuration object containing recognition variables
    Ref<SRQueue> queue;    // Queue for storing recognized keywords

    /*
     * Thread wrapper function, calls _recognize() method of its SRRunner
     * argument.
     */
    static void _thread_recognize(void *sr);

    /*
     * Repeatedly listens to keywords from the user's microphone input.
     */
    void _recognize();

protected:
    /*
     * Needed so that GDScript can recognize public methods from this class.
     */
    static void _bind_methods();

public:
    /*
     * Creates a thread to repeatedly listen to keywords. The thread can be stopped
     * with the stop() method. If start() was previously called, the current thread
     * is halted and a new recognition, with the specified arguments, is created.
     *
     * Note: The signal SR_RUNNER_END_SIGNAL is emitted when the thread ends. It
     * contains an SRError::Error argument representing what made it stop, which can
     * be one of the following values:
     * - OK
     * - REC_START_ERR
     * - REC_STOP_ERR
     * - UTT_START_ERR
     * - UTT_RESTART_ERR
     * - AUDIO_READ_ERR
     */
    void start(Ref<SRConfig> config, Ref<SRQueue> queue);

    /*
     * Returns true if the speech recognition thread is active, or false otherwise.
     */
    bool running();

    /*
     * Stops a created thread that is running the run() method. If run() wasn't
     * called previously, this function does nothing.
     */
    void stop();

    /*
     * Initializes speech recognizer variables.
     */
    SRRunner();

    /*
     * Clears memory used by a speech recognizer object.
     */
    ~SRRunner();
};

#endif
