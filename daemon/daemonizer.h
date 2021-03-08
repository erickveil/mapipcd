/**
 * daemonizer.h
 * Erick Veil
 * 2021-03-07
 * Copyight 2021 Erick Veil
 */
#ifndef DAEMONIZER_H
#define DAEMONIZER_H

#include "staticlogger.h"

#include <csignal>
#include <functional>

#include <QCoreApplication>
#include <QObject>
#include <QTimer>

/**
 * @brief The Daemonizer class
 * Simple way to turn your program into a Daemon.
 * Handles basic kill signals for New Unix Daemons.
 * Provides public Attributes to assign callbacks for alternate signal handling.
 * Just get an instance of this singleton and call init.
 * Must call QCoreApplication a(argc, argv) first, so the timer works.
 * Must be instantiated and init run before QCoreApplication::exec() is called.
 * Requires C++17 for inline static members.
 */
class Daemonizer : public QObject
{
    Q_OBJECT

    /**
     * @brief _instance
     * inline static members only available in > C++17
     */
    inline static Daemonizer *_instance = nullptr;

    inline static bool _isSigInt = false;
    inline static bool _isSigTerm = false;
    inline static bool _isSigHup = false;

    QTimer _signalMonitor;

    bool _isInit = false;

public:

    /**
     * @brief SigIntCallback
     * Public Attribute
     * Ctrl-c signal.
     * "Weakest kill"
     * "Stop what you're doing right now and wait for further user input."
     * Non-interactive programs, such as daemons, treat it like SIGTERM.
     */
    std::function<void ()> SigIntCallback;

    /**
     * @brief SigTermCallback
     * Public Attribute
     * "Normal Kill"
     * Exit the program cleanly.
     * Save its state, free resources, clean up children and temp files.
     * Finish up critical stuff before exiting.
     */
    std::function<void ()> SigTermCallback;

    /**
     * @brief SigHupCallback
     * Public Attribute
     * Traditionally means that the remote terminal that was running the
     * process got disconnected; An involontary SIGTERM.
     * In daemons, this is a signal to reload the config file.

     */
    std::function<void ()> SigHupCallback;

    /**
     * @brief Instance
     * Get an instance of this singleton.
     * @return
     */
    static Daemonizer* Instance();

    /**
     * @brief init
     * Initialize the singleton.
     * Call this first in main.
     * Sets up callbacks and signal handling.
     */
    void init();

private:
    Daemonizer();
    Daemonizer(Daemonizer const&) = delete;
    void operator=(Daemonizer const&)= delete;

    static void _signalHandler(int signal);

    void _sigintHandler();
    void _sigtermHandler();
    void _sighupHandler();

signals:

public slots:

    void _eventCheckSignalFlags();
};

#endif // DAEMONIZER_H
