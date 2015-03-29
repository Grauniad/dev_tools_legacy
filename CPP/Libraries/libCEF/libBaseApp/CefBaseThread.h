/*
 * CefBaseThread.h
 *
 *  Created on: 28 Mar 2015
 *      Author: lhumphreys
 */

#ifndef CEFBASETHREAD_H_
#define CEFBASETHREAD_H_

#include <thread>
#include <future>

#include <functional>
#include <memory>

#include "include/cef_task.h"

#include "include/base/cef_bind.h"
#include "include/wrapper/cef_closure_task.h"

/**
 * Utility class for performing a specific task in a work thread.
 * Implementations should derive from this abstract class and implement the
 * DoWork function.
 *
 * Work is done in a detached thread. If the operations requires work to be done
 * on a CEF main thread, typically the java-script renderer thread, it can be
 * done synchronously via GetResultFromCEFThread.
 *
 * The DoInNewThread is a useful wrapper to simply dispatch a function:
 *      DoInNewThread(
 *          []() {
 *              cout << "Hello from the thread: "
 *                   << std::this_thread::get_id() << endl;
 *
 *              cout << "The render thread has id "
 *                   << CefBaseThread::GetResultFromRender
 *                          <decltype(std::this_thread::get_id())> (
 *                              [] () {
 *                                  return std::this_thread::get_id();
 *                              }
 *                          )
 *                   << endl;
 *          }
 *
 */
class CefBaseThread {
public:
    virtual ~CefBaseThread() { }

    /*
     * Synchronously start this thread's work in the current thread.
     */
    void Run() {
        DoWork();
    }

    /*
     * Create a new detached thread, and use it to do this instance's work.
     */
    void RunInNewThread() {
        std::thread([=]() { this->Run();}).detach();
    }

    template<class T, class TASK>
    static T GetResultFromCEFThread(cef_thread_id_t tid, TASK&& task) {
        CefRefPtr<Work<T,TASK>> work =
            new CefBaseThread::Work<T,TASK>(std::move(task));

        CefPostTask(tid,base::Bind(&Work<T,TASK>::Execute,work));

        return work->Result();
    }

    /**
     * Execute a task in the render process, and wait for the result...
     */
    template<class T, class TASK>
    static T GetResultFromRender(TASK&& task) {
        return GetResultFromCEFThread<T,TASK>(TID_RENDERER,std::move(task));
    }

protected:
    /***
     * Derived classes should implement this to do the work of the thread
     */
    virtual void DoWork() = 0;

    template<class RET_TYPE, class Function>
    class Work
    {
    public:
        Work(Function&& f): func(f) { }

        void Execute() { result.set_value(func()); }

        RET_TYPE Result() { return result.get_future().get(); }

    private:
        Function func;
        std::promise<RET_TYPE> result;
        IMPLEMENT_REFCOUNTING(Work);
    };

};

/**
 * A specialisation of CefBaseThread for dispatching an object which behaves
 * like std::function
 */
template<class F>
class CefBaseWorkerThread: public CefBaseThread {
public:
    void DoWork() {
        function();
    }

    static void StartInThread(F&& function) {
        std::shared_ptr<CefBaseWorkerThread<F>> worker(
            new CefBaseWorkerThread(std::move(function)));

        std::thread([=]() { worker->Run();}).detach();
    }

private:
    CefBaseWorkerThread(F&& function): function(function) { }

    CefBaseWorkerThread(const CefBaseWorkerThread& rhs) = delete;

    F function;
};

/**
 * Finally provide a vanilla function wrapper to CefBaseWorkerThread, to help gcc with
 * the template type deduction.
 */
template <class F>
void DoInNewThread(F function) {
    CefBaseWorkerThread<F>::StartInThread(std::move(function));
}
#endif /* CEFBASETHREAD_H_ */
